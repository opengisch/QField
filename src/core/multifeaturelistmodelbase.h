/***************************************************************************
                            featurelistmodelbase.cpp
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MULTIFEATURELISTMODELBASE_H
#define MULTIFEATURELISTMODELBASE_H

#include "identifytool.h"

#include <QAbstractItemModel>
#include <qgsfeaturerequest.h>

class MultiFeatureListModelBase : public QAbstractItemModel
{
    Q_OBJECT

  public:
    explicit MultiFeatureListModelBase( QObject *parent = nullptr );

    /**
     * Resets the model to contain features found from a list of \a requests.
     */
    void setFeatures( const QMap<QgsVectorLayer *, QgsFeatureRequest> requests );

    /**
     * Appends features from a list of \a results.
     */
    void appendFeatures( const QList<IdentifyTool::IdentifyResult> &results );

    /**
     * Resets the model to either an empty feature list or one that contains only the selected features.
     * \param keepSelected if set to TRUE, selected features will be kept when resetting the model.
     */
    void clear( const bool keepSelected = false );

    /**
     * Empties the list of selected features.
     */
    void clearSelection();

    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;
    QModelIndex parent( const QModelIndex &child ) const override;
    int rowCount( const QModelIndex &parent ) const override;
    int columnCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    /**
     * Removes a defined number of rows starting from a given position. The parent index is not
     * used as we have a list only.
     *
     * @param row   The first row to remove
     * @param count The numbe rof rows to remove
     * @param parent Can savely be omitted as it is unused and defaults to an invalid index
     */
    virtual bool removeRows( int row, int count, const QModelIndex &parent ) override;

    /**
     * Returns the number of features in the model.
     */
    int count() const;

    /**
     * Returns the number of selected features in the model.
     */
    int selectedCount() const;

    //! Returns TRUE if the selected features can have their attributes value changed
    bool canEditAttributesSelection();

    //! Returns TRUE if the selected features can be merged
    bool canMergeSelection();

    //! Returns TRUE if the selected features can be deleted
    bool canDeleteSelection();

    //! Returns TRUE if the selected features can be duplicated onto their associated layer
    bool canDuplicateSelection();

    //! Returns TRUE if the selected features' geometry can be moved
    bool canMoveSelection();

    /**
     * Merges selected features by updating the first seleted feature's geometry
     * to a combination (i.e. union) of geometries of all selected features.
     *
     * All but the first feature will then be removed from the vector layer containing
     * the selected features.
     */
    bool mergeSelection();

    /**
     * Deletes a feature from a vector layer
     *
     * \param layer The layer from which a feature will be removed
     * \param fid The id of the feature to remove
     */
    bool deleteFeature( QgsVectorLayer *layer, QgsFeatureId fid, bool selectionAction = false );

    //! Deletes selected features
    bool deleteSelection();

    /**
     * Duplicates a feature on a given layer
     *
     * \param layer The layer within which the feature will be duplicated
     * \param feature The feature to be duplicated
     */
    bool duplicateFeature( QgsVectorLayer *layer, const QgsFeature &feature );

    //! Duplicates selected features onto their associated layer
    bool duplicateSelection();

    //! Moves selected features along a given \a vector.
    bool moveSelection( const double x, const double y );

    /**
     * Toggles the selection state of a given item.
     * \param item the item's row number
     */
    void toggleSelectedItem( int item );

    /**
     * Returns the list of currently selected features.
     * \note the current implementation only allow for selected features from a single layer
     */
    QList<QgsFeature> selectedFeatures() const;

    /**
     * Returns the layer containing the list of currently selected features.
     */
    QgsVectorLayer *selectedLayer() const;

  signals:

    void countChanged();

    void selectedCountChanged();

  private slots:

    void layerDeleted( QObject *object );

    void featureDeleted( QgsFeatureId fid );

    void attributeValueChanged( QgsFeatureId fid, int idx, const QVariant &value );

    void geometryChanged( QgsFeatureId fid, const QgsGeometry &geometry );

  private:
    inline QPair<QgsVectorLayer *, QgsFeature> *toFeature( const QModelIndex &index ) const
    {
      return static_cast<QPair<QgsVectorLayer *, QgsFeature> *>( index.internalPointer() );
    }

    QList<QPair<QgsVectorLayer *, QgsFeature>> mFeatures;
    QList<QPair<QgsVectorLayer *, QgsFeature>> mSelectedFeatures;
};

#endif // MULTIFEATURELISTMODELBASE_H
