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
#include <QThread>
#include <qgsvectorlayer.h>
#include <qgsvectorlayerfeatureiterator.h>

//used for gatherer
#include "qfield_core_export.h"


class QgsVectorLayer;
class FeatureGatherer;
class OrderedRelationModel;

/**
 * \ingroup core
 */
class QFIELD_CORE_EXPORT ReferencingFeatureListModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY( QgsFeature feature WRITE setFeature READ feature NOTIFY featureChanged )
    Q_PROPERTY( QgsRelation relation WRITE setRelation READ relation NOTIFY relationChanged )
    Q_PROPERTY( QString currentRelationId WRITE setCurrentRelationId READ currentRelationId NOTIFY relationChanged )
    Q_PROPERTY( QString currentNmRelationId WRITE setCurrentNmRelationId READ currentNmRelationId NOTIFY nmRelationChanged )
    Q_PROPERTY( QgsRelation nmRelation WRITE setNmRelation READ nmRelation NOTIFY nmRelationChanged )
    Q_PROPERTY( bool parentPrimariesAvailable WRITE setParentPrimariesAvailable READ parentPrimariesAvailable NOTIFY parentPrimariesAvailableChanged )
    Q_PROPERTY( bool isLoading READ isLoading NOTIFY isLoadingChanged )

  public:
    explicit ReferencingFeatureListModel( QObject *parent = nullptr );

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

    //! Returns the id of the relation connecting the parent feature with the children in this model
    QString currentRelationId() const;

    //! Sets the relation connecting the parent feature with the children in this model
    void setCurrentRelationId( const QString &relationId );

    //! On many-to-many relations returns the second relation id connecting the children in the association table to their other parent
    QString currentNmRelationId() const;

    //! On many-to-many relations sets the second relation connecting the children in the association table to their other parent
    void setCurrentNmRelationId( const QString &nmRelationId );

    /**
     * The parent feature for which this model contains the children
     * \param feature
     * \see feature
     */
    void setFeature( const QgsFeature &feature );

    /**
     * The parent feature for which this model contains the children
     * \return the parent feature
     * \see setFeature
     */
    QgsFeature feature() const;

    /**
     * The relation connecting the parent feature with the children in this model
     * \param relation
     * \see relation
     */
    void setRelation( const QgsRelation &relation );

    /**
     * The relation connecting the parent feature with the children in this model
     * \return relation
     * \see setRelation
     */
    QgsRelation relation() const;

    /**
     * On many-to-many relations this is the second relation connecting the children in the associationtable to their other parent
     * \param relation The associated relation
     * \see nmRelation
     */
    void setNmRelation( const QgsRelation &relation );

    /**
     * On many-to-many relations this is the second relation connecting the children in the associationtable to their other parent
     * \return associated relation
     * \see setNmRelation
     */
    QgsRelation nmRelation() const;

    /**
     * The status if the pk of the parent feature (this feature) are valid (not null)
     * \param parentPrimariesAvailable The status if the parent pks are available
     * \see parentPrimariesAvailable
     */
    void setParentPrimariesAvailable( const bool parentPrimariesAvailable );

    /**
     * The status if the pk of the parent feature (this feature) are valid (not null)
     * It's needed to check on opening a form to add a new child
     * \return parentPrimariesAvailable The status if the parent pks are available
     * \see setParentPrimariesAvailable
     */
    bool parentPrimariesAvailable() const;

    /**
     * Reloads the model by starting the reload functionality in the gatherer (seperate thread)
     * Sets the property parentPrimariesAvailable
     */
    Q_INVOKABLE void reload();

    /**
     * Deletes a feature regarding the referencing layer and the feature id of the selected child
     * \param referencingFeatureId id of the selected child
     */
    Q_INVOKABLE bool deleteFeature( QgsFeatureId referencingFeatureId );

    /**
     * Returns the row number for a given feature id
     * \param featureId the feature id
     */
    Q_INVOKABLE int getFeatureIdRow( QgsFeatureId featureId );

    /**
     * Indicator if the model is currently performing any feature iteration in the background.
     */
    bool isLoading() const;

  signals:
    void attributeFormModelChanged();
    void featureChanged();
    void relationChanged();
    void nmRelationChanged();
    void parentPrimariesAvailableChanged();
    void isLoadingChanged();
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

    //! Checks if the parent pk(s) is not null
    bool checkParentPrimaries();
    virtual bool beforeDeleteFeature( QgsVectorLayer *referencingLayer, QgsFeatureId referencingFeatureId );
    virtual void sortEntries();

    friend class FeatureGatherer;
    friend class OrderedRelationModel;
    friend class TestReferencingFeatureListModel;
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

        mEntries.append( ReferencingFeatureListModel::Entry( displayString, childFeature, nmDisplayString, nmFeature ) );

        //cppcheck-suppress knownConditionTrueFalse
        if ( mWasCanceled )
          return;
      }

      emit collectedValues();
    }

    //! Informs the gatherer to immediately stop collecting values
    void stop()
    {
      mWasCanceled = true;
    }

    //! \returns true if collection was canceled before completion
    bool wasCanceled() const { return mWasCanceled; }

    //! \returns the list of entries
    QList<ReferencingFeatureListModel::Entry> entries() const { return mEntries; }

  signals:

    /**
     * Emitted when values have been collected
     * \param values list of unique matching string values
     */
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

    QList<ReferencingFeatureListModel::Entry> mEntries;

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
