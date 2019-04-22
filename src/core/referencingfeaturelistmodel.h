/***************************************************************************
  referencingfeaturelistmodel.h - ReferencingFeatureListModel

 ---------------------
 begin                : 1.3.2019
 copyright            : (C) 2019 by David Signer
 email                : david@opengis.ch
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
#include "qgsvectorlayer.h"
#include "attributeformmodel.h"

class QgsVectorLayer;
class AttributeFormModel;

class ReferencingFeatureListModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY( QgsFeature feature WRITE setFeature READ feature NOTIFY featureChanged )
    Q_PROPERTY( QgsRelation relation WRITE setRelation READ relation NOTIFY relationChanged )
    Q_PROPERTY( QgsRelation nmRelation WRITE setNmRelation READ nmRelation NOTIFY nmRelationChanged )
    Q_PROPERTY( bool parentPrimariesAvailable WRITE setParentPrimariesAvailable READ parentPrimariesAvailable NOTIFY parentPrimariesAvailableChanged )

  public:
    explicit ReferencingFeatureListModel( QObject *parent = nullptr );

    enum ReferencedFeatureListRoles
    {
      DisplayString = Qt::UserRole,
      ReferencingFeature,
      NmReferencedFeature
    };

    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;
    QModelIndex parent( const QModelIndex &index ) const override;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

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
     * The relation connectiong the parent feature with the children in this model
     * \param relation
     * \see relation
     */
    void setRelation( const QgsRelation &relation );

    /**
     * The relation connectiong the parent feature with the children in this model
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

    //! Reloads the model
    Q_INVOKABLE void reload();
    //! Deletes a feature regarding the referencing layer and the feature id \param referencingFeatureId of the selected child
    Q_INVOKABLE void deleteFeature( QgsFeatureId referencingFeatureId );

  signals:
    void attributeFormModelChanged();
    void featureChanged();
    void relationChanged();
    void nmRelationChanged();
    void parentPrimariesAvailableChanged();

  private:
    struct Entry
    {
      Entry( const QString &displayString, const QgsFeatureId &referencingFeatureId )
        : displayString( displayString )
        , referencingFeatureId( referencingFeatureId )
      {}

      Entry() = default;

      QString displayString;
      QgsFeatureId referencingFeatureId;
    };

    QList<Entry> mEntries;

    QgsFeature mFeature;
    QgsRelation mRelation;
    QgsRelation mNmRelation;
    bool mParentPrimariesAvailable = false;

    //! Checks if the parent pk(s) is not null
    bool checkParentPrimaries();
};

#endif // REFERENCINGFEATURELISTMODEL_H
