/***************************************************************************
  featurelistmodel.h - FeatureListModel

 ---------------------
 begin                : 1.2.2017
 copyright            : (C) 2017 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef FEATURELISTMODEL_H
#define FEATURELISTMODEL_H

#include <stringutils.h>

#include <QAbstractItemModel>
#include <QTimer>
#include <QThread>
#include <QMutex>

#include "qgsapplication.h"
#include "qgslogger.h"
#include "qgsvectorlayer.h"
#include "qgsvectorlayerfeatureiterator.h"
#include <qgsfeature.h>
#include <qgsstringutils.h>

class QgsVectorLayer;

/**
 * \class QgsFieldExpressionValuesGatherer
 * Gathers features with substring matching on an expression.
 *
 * \since QGIS 3.14
 */
class FeatureExpressionValuesGatherer: public QThread
{
    Q_OBJECT

  public:

    /**
       * Constructor
       * \param layer the vector layer
       * \param displayExpression if empty, the display expression is taken from the layer definition
       * \param request the request to perform
       * \param identifierFields an optional list of fields name to be save in a variant list for an easier reuse
       */
    FeatureExpressionValuesGatherer( QgsVectorLayer *layer,
                                        const QString &displayExpression = QString(),
                                        const QgsFeatureRequest &request = QgsFeatureRequest(),
                                        const QStringList &identifierFields = QStringList() )
      : mSource( new QgsVectorLayerFeatureSource( layer ) )
      , mDisplayExpression( displayExpression.isEmpty() ? layer->displayExpression() : displayExpression )
      , mRequest( request )
      , mIdentifierFields( identifierFields )
    {
    }

    struct Entry
    {
      Entry() = default;

      Entry( const QVariantList &_identifierFields, const QString &_value, const QgsFeature &_feature )
        : identifierFields( _identifierFields )
        , featureId( _feature.isValid() ? _feature.id() : FID_NULL )
        , value( _value )
        , feature( _feature )
      {}

      Entry( const QgsFeatureId &_featureId, const QString &_value, const QgsVectorLayer *layer )
        : featureId( _featureId )
        , value( _value )
        , feature( QgsFeature( layer ? layer->fields() : QgsFields() ) )
      {}

      QVariantList identifierFields;
      QgsFeatureId featureId;
      QString value;
      QgsFeature feature;

      bool operator()( const Entry &lhs, const Entry &rhs ) const;
    };

    static Entry nullEntry( QgsVectorLayer *layer )
    {
      return Entry( QVariantList(), QgsApplication::nullRepresentation(), QgsFeature( layer->fields() ) );
    }

    void run() override
    {
      mWasCanceled = false;

      QgsFeatureIterator iterator = mSource->getFeatures( mRequest );

      mDisplayExpression.prepare( &mExpressionContext );

      QgsFeature feature;
      QList<int> attributeIndexes;
      for ( const QString &fieldName : qgis::as_const( mIdentifierFields ) )
        attributeIndexes << mSource->fields().indexOf( fieldName );

      while ( iterator.nextFeature( feature ) )
      {
        mExpressionContext.setFeature( feature );
        QVariantList attributes;
        for ( const int idx : attributeIndexes )
          attributes << feature.attribute( idx );

        const QString expressionValue = mDisplayExpression.evaluate( &mExpressionContext ).toString();

        mEntries.append( Entry( attributes, expressionValue, feature ) );

        QMutexLocker locker( &mCancelMutex );
        if ( mWasCanceled )
          return;
      }
    }

    //! Informs the gatherer to immediately stop collecting values
    void stop()
    {
      QMutexLocker locker( &mCancelMutex );
      mWasCanceled = true;
    }

    //! Returns TRUE if collection was canceled before completion
    bool wasCanceled() const
    {
      QMutexLocker locker( &mCancelMutex );
      return mWasCanceled;
    }

    QVector<Entry> entries() const
    {
      return mEntries;
    }

    QgsFeatureRequest request() const
    {
      return mRequest;
    }

    /**
     * Internal data, use for whatever you want.
     */
    QVariant data() const
    {
      return mData;
    }

    /**
     * Internal data, use for whatever you want.
     */
    void setData( const QVariant &data )
    {
      mData = data;
    }

  protected:
    QVector<Entry> mEntries;

  private:
    std::unique_ptr<QgsVectorLayerFeatureSource> mSource;
    QgsExpression mDisplayExpression;
    QgsExpressionContext mExpressionContext;
    QgsFeatureRequest mRequest;
    bool mWasCanceled = false;
    mutable QMutex mCancelMutex;
    QStringList mIdentifierFields;
    QVariant mData;
};

/**
 * Provides access to a list of features from a layer.
 * For each feature, the display expression is exposed as DisplayRole
 * and a keyField as KeyFieldRole for a unique identifier.
 * If a displayValueField is set it replaces the display expression of the layer.
 */
class FeatureListModel : public QAbstractItemModel
{
    Q_OBJECT

    /**
       * The vector layer to list
       */
    Q_PROPERTY( QgsVectorLayer *currentLayer READ currentLayer WRITE setCurrentLayer NOTIFY currentLayerChanged )
    /**
       * The primary key field
       */
    Q_PROPERTY( QString keyField READ keyField WRITE setKeyField NOTIFY keyFieldChanged )
    /**
       * The display value field
       */
    Q_PROPERTY( QString displayValueField READ displayValueField WRITE setDisplayValueField NOTIFY displayValueFieldChanged )

    /**
      * Whether the features should be ordered by value
      */
    Q_PROPERTY( bool orderByValue READ orderByValue WRITE setOrderByValue NOTIFY orderByValueChanged )

    /**
      * Whether a null values is allowed in the list
      */
    Q_PROPERTY( bool addNull READ addNull WRITE setAddNull NOTIFY addNullChanged )

    /**
      * Expression to filter features with. Empty string if no filter is applied.
      */
    Q_PROPERTY( QString filterExpression READ filterExpression WRITE setFilterExpression NOTIFY filterExpressionChanged )

    /**
     * Search term to filter features with. Empty string if no search is applied.
     */
    Q_PROPERTY( QString searchTerm READ searchTerm WRITE setSearchTerm NOTIFY searchTermChanged )

    /**
      * The current form feature, used to evaluate expressions such as `current_value('attr1')`
      **/
    Q_PROPERTY( QgsFeature currentFormFeature READ currentFormFeature WRITE setCurrentFormFeature NOTIFY currentFormFeatureChanged )

  public:
    enum FeatureListRoles
    {
      KeyFieldRole = Qt::UserRole + 1,
      DisplayStringRole,
    };

    Q_ENUM( FeatureListRoles )

    explicit FeatureListModel( QObject *parent = nullptr );

    virtual QModelIndex index( int row, int column, const QModelIndex &parent ) const override;
    virtual QModelIndex parent( const QModelIndex &child ) const override;
    virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    virtual int columnCount( const QModelIndex &parent ) const override;
    virtual QVariant data( const QModelIndex &index, int role ) const override;

    Q_INVOKABLE QVariant dataFromRowIndex( int row, int role ) { return data( index( row, 0, QModelIndex() ), role ); }

    /**
     * Returns the first feature matching the key  \a value.
     */
    Q_INVOKABLE QgsFeature getFeatureFromKeyValue( const QVariant &value ) const;

    virtual QHash<int, QByteArray> roleNames() const override;

    QgsVectorLayer *currentLayer() const;
    void setCurrentLayer( QgsVectorLayer *currentLayer );

    QString keyField() const;
    void setKeyField( const QString &keyField );

    QString displayValueField() const;
    void setDisplayValueField( const QString &displayValueField );

    /**
       * Get the row for a given key value.
       */
    Q_INVOKABLE int findKey( const QVariant &key ) const;

    /**
       * Orders all the values alphabethically by their displayString.
       */
    bool orderByValue() const;

    /**
       * Orders all the values alphabethically by their displayString.
       */
    void setOrderByValue( bool orderByValue );

    /**
       * Add a NULL value as the first entry.
       */
    bool addNull() const;

    /**
       * Add a NULL value as the first entry.
       */
    void setAddNull( bool addNull );

    /**
     * Expression to filter features with. Empty string if no filter is applied.
     */
    QString filterExpression() const;

    /**
     * Sets an expression to filter features with. Empty string if no filter is applied.
     */
    void setFilterExpression( const QString &filterExpression );

    /**
     * Search term to filter features with. Empty string if no search is applied.
     */
    QString searchTerm() const;

    /**
     * Sets a search term to filter features with. Empty string if no search is applied.
     */
    void setSearchTerm( const QString &searchTerm );

    /**
     * The current form feature, used to evaluate expressions such as `current_value('attr1')`
     */
    QgsFeature currentFormFeature() const;

    /**
     * Sets the current form feature, used to evaluate expressions such as `current_value('attr1')`
     */
    void setCurrentFormFeature( const QgsFeature &feature );

  signals:
    void currentLayerChanged();
    void keyFieldChanged();
    void displayValueFieldChanged();
    void orderByValueChanged();
    void addNullChanged();
    void filterExpressionChanged();
    void searchTermChanged();
    void currentFormFeatureChanged();

  private slots:
    void onFeatureAdded();
    void onAttributeValueChanged( QgsFeatureId fid, int idx, const QVariant &value );
    void onFeatureDeleted();

    void gatherFeatureList();

    /**
       * Reloads a layer. This will normally be triggered
       * by \see reloadLayer and should not be called directly.
       */
    void processFeatureList();

  private:
    struct Entry
    {
      Entry( const QString &displayString, const QVariant &key, const QgsFeatureId &fid )
        : displayString( displayString )
        , key( key )
        , fid( fid )
        , fuzzyScore( 0 )
      {}

      Entry() = default;

      void calcFuzzyScore( const QString &searchTerm )
      {
        fuzzyScore = StringUtils::fuzzyMatch( displayString, searchTerm ) ? 0.5 : 0;
        fuzzyScore += QgsStringUtils::fuzzyScore( displayString, searchTerm ) * 0.5;
      }

      QString displayString;
      QVariant key;
      QgsFeatureId fid;
      double fuzzyScore;
    };

    /**
       * Triggers a reload of the values from the layer.
       * To avoid having the (expensive) reload operation happening for
       * every property change, it will only execute this after a very short delay.
       * This allows changing multiple properties at once and have a single reload
       * in the end.
       */
    void reloadLayer();

    QgsVectorLayer *mCurrentLayer = nullptr;

    FeatureExpressionValuesGatherer *mGatherer = nullptr;

    QList<Entry> mEntries;
    QString mKeyField;
    QString mDisplayValueField;
    bool mOrderByValue = false;
    bool mAddNull = false;
    QString mFilterExpression;
    QString mSearchTerm;
    QgsFeature mCurrentFormFeature;

    QTimer mReloadTimer;
};

#endif // FEATURELISTMODEL_H
