

# File referencingfeaturelistmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**referencingfeaturelistmodel.h**](referencingfeaturelistmodel_8h.md)

[Go to the documentation of this file](referencingfeaturelistmodel_8h.md)


```C++
/***************************************************************************
  referencingfeaturelistmodel.h - ReferencingFeatureListModel

 ---------------------
 begin                : 1.3.2019
 copyright            : (C) 2019 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef REFERENCINGFEATURELISTMODEL_H
#define REFERENCINGFEATURELISTMODEL_H

#include <QAbstractItemModel>
#include <QPair>
#include <QSortFilterProxyModel>
#include <QThread>
#include <qgsvectorlayer.h>
#include <qgsvectorlayerfeatureiterator.h>

//used for gatherer
#include "qfield_core_export.h"


class QgsVectorLayer;
class FeatureGatherer;
class OrderedRelationModel;

class QFIELD_CORE_EXPORT ReferencingFeatureListModelBase : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY( bool isLoading READ isLoading NOTIFY isLoadingChanged )
    Q_PROPERTY( QgsFeature feature WRITE setFeature READ feature NOTIFY featureChanged )
    Q_PROPERTY( QgsRelation relation WRITE setRelation READ relation NOTIFY relationChanged )
    Q_PROPERTY( QString currentRelationId WRITE setCurrentRelationId READ currentRelationId NOTIFY relationChanged )
    Q_PROPERTY( QString currentNmRelationId WRITE setCurrentNmRelationId READ currentNmRelationId NOTIFY nmRelationChanged )
    Q_PROPERTY( QgsRelation nmRelation WRITE setNmRelation READ nmRelation NOTIFY nmRelationChanged )
    Q_PROPERTY( bool parentPrimariesAvailable WRITE setParentPrimariesAvailable READ parentPrimariesAvailable NOTIFY parentPrimariesAvailableChanged )

  public:
    explicit ReferencingFeatureListModelBase( QObject *parent = nullptr );

    enum ReferencedFeatureListRoles
    {
      DisplayString = Qt::UserRole,
      ReferencingFeature,
      NmReferencedFeature,
      NmDisplayString
    };

    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;
    QModelIndex parent( const QModelIndex &index ) const override;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    QString currentRelationId() const;

    void setCurrentRelationId( const QString &relationId );

    QString currentNmRelationId() const;

    void setCurrentNmRelationId( const QString &nmRelationId );

    void setFeature( const QgsFeature &feature );

    QgsFeature feature() const;

    void setRelation( const QgsRelation &relation );

    QgsRelation relation() const;

    void setNmRelation( const QgsRelation &relation );

    QgsRelation nmRelation() const;

    void setParentPrimariesAvailable( const bool parentPrimariesAvailable );

    bool parentPrimariesAvailable() const;

    Q_INVOKABLE void reload();

    Q_INVOKABLE bool deleteFeature( QgsFeatureId referencingFeatureId );

    Q_INVOKABLE int getFeatureIdRow( QgsFeatureId featureId );

    bool isLoading() const;

  signals:
    void attributeFormModelChanged();
    void featureChanged();
    void relationChanged();
    void nmRelationChanged();
    void parentPrimariesAvailableChanged();
    void isLoadingChanged();
    void beforeModelUpdated();
    void modelUpdated();

  private slots:
    void updateModel();
    void gathererThreadFinished();

  private:
    struct Entry
    {
        Entry( const QString &displayString, const QgsFeature &referencingFeature, const QString &nmDisplayString = QString(), const QgsFeature &nmReferencedFeature = QgsFeature() )
          : displayString( displayString )
          , referencingFeature( referencingFeature )
          , nmDisplayString( nmDisplayString )
          , nmReferencedFeature( nmReferencedFeature )
        {}

        Entry() = default;

        QString displayString;
        QgsFeature referencingFeature;
        QString nmDisplayString;
        QgsFeature nmReferencedFeature;
    };

    QList<Entry> mEntries;
    QgsFeature mFeature;
    QgsRelation mRelation;
    QgsRelation mNmRelation;
    bool mParentPrimariesAvailable = false;

    FeatureGatherer *mGatherer = nullptr;

    bool checkParentPrimaries();
    virtual bool beforeDeleteFeature( QgsVectorLayer *referencingLayer, QgsFeatureId referencingFeatureId );

    friend class FeatureGatherer;
    friend class OrderedRelationModel;
    friend class TestReferencingFeatureListModel;
};

class ReferencingFeatureListModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( bool isLoading READ isLoading NOTIFY isLoadingChanged )
    Q_PROPERTY( QgsFeature feature WRITE setFeature READ feature NOTIFY featureChanged )
    Q_PROPERTY( QgsRelation relation WRITE setRelation READ relation NOTIFY relationChanged )
    Q_PROPERTY( QString currentRelationId WRITE setCurrentRelationId READ currentRelationId NOTIFY relationChanged )
    Q_PROPERTY( QString currentNmRelationId WRITE setCurrentNmRelationId READ currentNmRelationId NOTIFY nmRelationChanged )
    Q_PROPERTY( QgsRelation nmRelation WRITE setNmRelation READ nmRelation NOTIFY nmRelationChanged )
    Q_PROPERTY( bool parentPrimariesAvailable WRITE setParentPrimariesAvailable READ parentPrimariesAvailable NOTIFY parentPrimariesAvailableChanged )
    Q_PROPERTY( Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged )

  public:
    explicit ReferencingFeatureListModel( QObject *parent = nullptr );

    QString currentRelationId() const;

    void setCurrentRelationId( const QString &relationId );

    QString currentNmRelationId() const;

    void setCurrentNmRelationId( const QString &nmRelationId );

    void setFeature( const QgsFeature &feature );

    QgsFeature feature() const;

    void setRelation( const QgsRelation &relation );

    QgsRelation relation() const;

    void setNmRelation( const QgsRelation &relation );

    QgsRelation nmRelation() const;

    void setParentPrimariesAvailable( const bool parentPrimariesAvailable );

    bool parentPrimariesAvailable() const;

    Q_INVOKABLE void reload();

    Q_INVOKABLE bool deleteFeature( QgsFeatureId referencingFeatureId );

    Q_INVOKABLE int getFeatureIdRow( QgsFeatureId featureId );

    bool isLoading() const;

    Qt::SortOrder sortOrder() const;

    void setSortOrder( Qt::SortOrder sortOrder );

  signals:
    void attributeFormModelChanged();
    void featureChanged();
    void relationChanged();
    void nmRelationChanged();
    void parentPrimariesAvailableChanged();
    void isLoadingChanged();
    void beforeModelUpdated();
    void modelUpdated();
    void sortOrderChanged();

  private:
    ReferencingFeatureListModelBase *mSourceModel = nullptr;
    Qt::SortOrder mSortOrder = Qt::AscendingOrder;
};

class FeatureGatherer : public QThread
{
    Q_OBJECT

  public:
    FeatureGatherer( QgsFeature feature, QgsRelation &relation, const QgsRelation &nmRelation = QgsRelation() )
    {
      mReferencingSource.reset( new QgsVectorLayerFeatureSource( relation.referencingLayer() ) );

      const bool featureIsNew = std::numeric_limits<QgsFeatureId>::min() == feature.id();
      if ( featureIsNew )
      {
        const auto fieldPairs = relation.fieldPairs();
        for ( QgsRelation::FieldPair fieldPair : fieldPairs )
        {
          {
            if ( relation.referencedLayer() && relation.referencedLayer()->dataProvider() )
            {
              if ( !relation.referencedLayer()->dataProvider()->defaultValueClause( feature.fieldNameIndex( fieldPair.second ) ).isEmpty() )
              {
                feature.setAttribute( fieldPair.second, QVariant() );
              }
            }
          }
        }
      }
      mRequest = relation.getRelatedFeaturesRequest( feature );

      mContext = relation.referencingLayer()->createExpressionContext();
      mDisplayExpression = relation.referencingLayer()->displayExpression();
      if ( nmRelation.isValid() )
      {
        mNmReferencingFields = nmRelation.referencingLayer()->fields();
        mNmReferencedSource.reset( new QgsVectorLayerFeatureSource( nmRelation.referencedLayer() ) );
        mNmFieldPairs = nmRelation.fieldPairs();
        mNmContext = nmRelation.referencedLayer()->createExpressionContext();
        mNmDisplayExpression = nmRelation.referencedLayer()->displayExpression();
      }
    }

    void run() override
    {
      mWasCanceled = false;


      QgsFeatureIterator relatedFeaturesIt = mReferencingSource->getFeatures( mRequest );
      QgsExpression expression( mDisplayExpression );

      QgsFeature childFeature;
      QString displayString;
      while ( relatedFeaturesIt.nextFeature( childFeature ) )
      {
        mContext.setFeature( childFeature );
        displayString = expression.evaluate( &mContext ).toString();

        QgsFeature nmFeature;
        QString nmDisplayString;
        if ( !mNmDisplayExpression.isEmpty() )
        {
          QgsExpression nmExpression( mNmDisplayExpression );
          ( void ) mNmReferencedSource->getFeatures( getNmReferencedFeatureRequest( childFeature.attributes() ) ).nextFeature( nmFeature );
          mNmContext.setFeature( nmFeature );
          nmDisplayString = nmExpression.evaluate( &mNmContext ).toString();
        }

        mEntries.append( ReferencingFeatureListModelBase::Entry( displayString, childFeature, nmDisplayString, nmFeature ) );

        //cppcheck-suppress knownConditionTrueFalse
        if ( mWasCanceled )
          return;
      }

      emit collectedValues();
    }

    void stop()
    {
      mWasCanceled = true;
    }

    bool wasCanceled() const { return mWasCanceled; }

    QList<ReferencingFeatureListModelBase::Entry> entries() const { return mEntries; }

  signals:

    void collectedValues();

  private:
    QgsFeatureRequest getNmReferencedFeatureRequest( const QgsAttributes &attributes )
    {
      // Taken from QgsRelation::getReferencedFeatureRequest, needed to safely run off the main thread
      QStringList conditions;

      for ( const QgsRelation::FieldPair &pair : std::as_const( mNmFieldPairs ) )
      {
        int referencedIdx = mNmReferencedSource->fields().lookupField( pair.referencedField() );
        int referencingIdx = mNmReferencingFields.lookupField( pair.referencingField() );
        if ( referencedIdx >= 0 )
        {
          QMetaType::Type fieldType = mNmReferencedSource->fields().at( referencedIdx ).type();
          conditions << QgsExpression::createFieldEqualityExpression( pair.referencedField(), attributes.at( referencingIdx ), fieldType );
        }
        else
        {
          conditions << QgsExpression::createFieldEqualityExpression( pair.referencedField(), attributes.at( referencingIdx ) );
        }
      }

      QgsFeatureRequest request;
      request.setFilterExpression( conditions.join( QLatin1String( " AND " ) ) );
      return request;
    }

    QList<ReferencingFeatureListModelBase::Entry> mEntries;

    std::unique_ptr<QgsVectorLayerFeatureSource> mReferencingSource;
    QgsFeatureRequest mRequest;
    QgsExpressionContext mContext;
    QString mDisplayExpression;

    QgsFields mNmReferencingFields;
    std::unique_ptr<QgsVectorLayerFeatureSource> mNmReferencedSource;
    QList<QgsRelation::FieldPair> mNmFieldPairs;
    QgsExpressionContext mNmContext;
    QString mNmDisplayExpression;

    bool mWasCanceled = false;
};

#endif // REFERENCINGFEATURELISTMODEL_H
```


