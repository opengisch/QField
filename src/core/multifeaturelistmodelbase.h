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
#include <qgis.h>
#include <qgsfeaturerequest.h>

/**
 * \ingroup core
 */
class MultiFeatureListModelBase : public QAbstractItemModel
{
    Q_OBJECT

  public:
    explicit MultiFeatureListModelBase( QObject *parent = nullptr );

    //! \copydoc MultiFeatureListModel::setFeatures
    void setFeatures( const QMap<QgsVectorLayer *, QgsFeatureRequest> &requests );

    //! \copydoc MultiFeatureListModel::appendFeatures
    void appendFeatures( const QList<IdentifyTool::IdentifyResult> &results );

    //! \copydoc MultiFeatureListModel::clear
    void clear( const bool keepSelected = false );

    //! \copydoc MultiFeatureListModel::clearSelection
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

    //! \copydoc MultiFeatureListModel::count
    int count() const;

    //! \copydoc MultiFeatureListModel::selectedCount
    int selectedCount() const;

    //! \copydoc MultiFeatureListModel::canEditAttributesSelection
    bool canEditAttributesSelection() const;

    //! \copydoc MultiFeatureListModel::canMergeSelection
    bool canMergeSelection() const;

    //! \copydoc MultiFeatureListModel::canDeleteSelection
    bool canDeleteSelection() const;

    //! \copydoc MultiFeatureListModel::canDuplicateSelection
    bool canDuplicateSelection() const;

    //! \copydoc MultiFeatureListModel::canMoveSelection
    bool canMoveSelection() const;

    //! \copydoc MultiFeatureListModel::canRotateSelection
    bool canRotateSelection() const;

    //! \copydoc MultiFeatureListModel::canProcessSelection
    bool canProcessSelection() const;

    //! \copydoc MultiFeatureListModel::mergeSelection
    bool mergeSelection();

    /**
     * Deletes a feature from a vector layer
     *
     * \param layer The layer from which a feature will be removed
     * \param fid The id of the feature to remove
     * \param selectionAction
     */
    bool deleteFeature( QgsVectorLayer *layer, QgsFeatureId fid, bool selectionAction = false );

    //! \copydoc MultiFeatureListModel::deleteSelection
    bool deleteSelection();

    //! \copydoc MultiFeatureListModel::duplicateFeature
    bool duplicateFeature( QgsVectorLayer *layer, const QgsFeature &feature );

    //! \copydoc MultiFeatureListModel::duplicateSelection
    bool duplicateSelection();

    //! \copydoc MultiFeatureListModel::moveSelection
    bool moveSelection( const double x, const double y );

    //! \copydoc MultiFeatureListModel::rotateSelection
    bool rotateSelection( const double angle );

    //! \copydoc MultiFeatureListModel::toggleSelectedItem
    void toggleSelectedItem( int item );

    //! \copydoc MultiFeatureListModel::selectedFeatures
    QList<QgsFeature> selectedFeatures() const;

    //! \copydoc MultiFeatureListModel::selectedLayer
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
    inline QPair<QgsMapLayer *, QgsFeature> *toFeature( const QModelIndex &index ) const
    {
      return static_cast<QPair<QgsMapLayer *, QgsFeature> *>( index.internalPointer() );
    }

    QList<QPair<QgsMapLayer *, QgsFeature>> mFeatures;
    QList<QPair<QgsMapLayer *, QgsFeature>> mSelectedFeatures;

    QMap<QString, QgsVectorLayer *> mRepresentationalLayers;
};

#endif // MULTIFEATURELISTMODELBASE_H
