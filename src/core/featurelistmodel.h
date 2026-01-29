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

#include "appexpressioncontextscopesgenerator.h"
#include "featureexpressionvaluesgatherer.h"

#include <QAbstractItemModel>
#include <QTimer>
#include <qgsfeature.h>
#include <qgsstringutils.h>

#include <stringutils.h>

class QgsVectorLayer;

/**
 * Provides access to a list of features from a layer.
 * For each feature, the display expression is exposed as DisplayRole
 * and a keyField as KeyFieldRole for a unique identifier.
 * If a displayValueField is set it replaces the display expression of the layer.
 * \ingroup core
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
     * The grouping key field
     */
    Q_PROPERTY( QString groupField READ groupField WRITE setGroupField NOTIFY groupFieldChanged )

    /**
     * Set to TRUE if the group name will be displayed in the list
     */
    Q_PROPERTY( bool displayGroupName READ displayGroupName WRITE setDisplayGroupName NOTIFY displayGroupNameChanged )

    /**
     * Set to TRUE if features should be ordered by value
     */
    Q_PROPERTY( bool orderByValue READ orderByValue WRITE setOrderByValue NOTIFY orderByValueChanged )

    /**
     * Set to TRUE if null values are allowed in the list
     */
    Q_PROPERTY( bool addNull READ addNull WRITE setAddNull NOTIFY addNullChanged )

    /**
     * Expression to filter features with. Empty string if no filter is applied.
     */
    Q_PROPERTY( QString filterExpression READ filterExpression WRITE setFilterExpression NOTIFY filterExpressionChanged )

    /**
     * The current form feature, used to evaluate expressions such as `current_value('attr1')`
     */
    Q_PROPERTY( QgsFeature currentFormFeature READ currentFormFeature WRITE setCurrentFormFeature NOTIFY currentFormFeatureChanged )

    /**
     * The application expression context scope generator used when filtering by expression
     */
    Q_PROPERTY( AppExpressionContextScopesGenerator *appExpressionContextScopesGenerator READ appExpressionContextScopesGenerator WRITE setAppExpressionContextScopesGenerator NOTIFY appExpressionContextScopesGeneratorChanged )

  public:
    enum FeatureListRoles
    {
      KeyFieldRole = Qt::UserRole + 1,
      DisplayStringRole,
      GroupFieldRole,
      FeatureIdRole,
    };

    Q_ENUM( FeatureListRoles )

    explicit FeatureListModel( QObject *parent = nullptr );
    ~FeatureListModel();

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

    /**
     * Returns the first feature matching the key  \a id.
     */
    Q_INVOKABLE QgsFeature getFeatureById( QgsFeatureId id ) const;

    virtual QHash<int, QByteArray> roleNames() const override;

    QgsVectorLayer *currentLayer() const;
    void setCurrentLayer( QgsVectorLayer *currentLayer );

    QString keyField() const;
    void setKeyField( const QString &keyField );

    QString displayValueField() const;
    void setDisplayValueField( const QString &displayValueField );

    QString groupField() const;
    void setGroupField( const QString &groupField );

    bool displayGroupName() const;
    void setDisplayGroupName( bool displayGroupName );

    /**
     * Get the row for a given key value.
     */
    Q_INVOKABLE int findKey( const QVariant &key ) const;

    /**
     * Get rows for a given filter string used to match display values.
     */
    Q_INVOKABLE QList<int> findDisplayValueMatches( const QString &filter ) const;

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
     * The current form feature, used to evaluate expressions such as `current_value('attr1')`
     */
    QgsFeature currentFormFeature() const;

    /**
     * Sets the current form feature, used to evaluate expressions such as `current_value('attr1')`
     */
    void setCurrentFormFeature( const QgsFeature &feature );

    /**
     * Returns the application expression context scope generator used when filtering by expression
     */
    AppExpressionContextScopesGenerator *appExpressionContextScopesGenerator() const;

    /**
     * Sets the application expression context scope generator used when filtering by expression
     */
    void setAppExpressionContextScopesGenerator( AppExpressionContextScopesGenerator *generator );

  signals:
    void currentLayerChanged();
    void keyFieldChanged();
    void displayValueFieldChanged();
    void groupFieldChanged();
    void displayGroupNameChanged();
    void orderByValueChanged();
    void addNullChanged();
    void filterExpressionChanged();
    void currentFormFeatureChanged();
    void appExpressionContextScopesGeneratorChanged();

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
        Entry( const QString &displayString, const QVariant &key, const QVariant &group, const QgsFeatureId &fid )
          : displayString( displayString )
          , key( key )
          , group( group )
          , fid( fid )
          , fuzzyScore( 0 )
        {}

        Entry() = default;

        QString displayString;
        QVariant key;
        QVariant group;
        QgsFeatureId fid;
        double fuzzyScore = 0.0;
    };
    Entry entryFromRow( int row );

    /**
       * Triggers a reload of the values from the layer.
       * To avoid having the (expensive) reload operation happening for
       * every property change, it will only execute this after a very short delay.
       * This allows changing multiple properties at once and have a single reload
       * in the end.
       */
    void reloadLayer();

    void cleanupGatherer();

    QPointer<QgsVectorLayer> mCurrentLayer;

    FeatureExpressionValuesGatherer *mGatherer = nullptr;

    QList<Entry> mEntries;
    QString mKeyField;
    QString mDisplayValueField;
    QString mGroupField;
    bool mDisplayGroupName = false;
    bool mOrderByValue = false;
    bool mAddNull = false;
    QString mFilterExpression;
    QgsFeature mCurrentFormFeature;
    QPointer<AppExpressionContextScopesGenerator> mAppExpressionContextScopesGenerator;

    QTimer mReloadTimer;

    friend class FeatureCheckListModel;
    friend class FeatureCheckListModelBase;
};

#endif // FEATURELISTMODEL_H
