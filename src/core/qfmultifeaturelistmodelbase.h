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

#ifndef QFMULTIFEATURELISTMODELBASE_H
#define QFMULTIFEATURELISTMODELBASE_H

#include "qfidentifytool.h"

#include <QAbstractItemModel>
#include <qgis.h>
#include <qgsconditionalstyle.h>
#include <qgsfeaturerequest.h>

/**
 * \ingroup core
 */
class QfMultiFeatureListModelBase : public QAbstractItemModel
{
    Q_OBJECT

  public:
    explicit QfMultiFeatureListModelBase( QObject *parent = nullptr );

    //! \copydoc QfMultiFeatureListModel::setFeatures
    void setFeatures( const QMap<QgsVectorLayer *, QgsFeatureRequest> &requests );

    //! \copydoc QfMultiFeatureListModel::appendFeatures
    void appendFeatures( const QList<QfIdentifyTool::IdentifyResult> &results );

    //! \copydoc QfMultiFeatureListModel::clear
    void clear( const bool keepSelected = false );

    //! \copydoc QfMultiFeatureListModel::clearSelection
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

    //! \copydoc QfMultiFeatureListModel::count
    int count() const;

    //! \copydoc QfMultiFeatureListModel::selectedCount
    int selectedCount() const;

    //! \copydoc QfMultiFeatureListModel::canEditAttributesSelection
    bool canEditAttributesSelection() const;

    //! \copydoc QfMultiFeatureListModel::canMergeSelection
    bool canMergeSelection() const;

    //! \copydoc QfMultiFeatureListModel::canDeleteSelection
    bool canDeleteSelection() const;

    //! \copydoc QfMultiFeatureListModel::canDuplicateSelection
    bool canDuplicateSelection() const;

    //! \copydoc QfMultiFeatureListModel::canMoveSelection
    bool canMoveSelection() const;

    //! \copydoc QfMultiFeatureListModel::canRotateSelection
    bool canRotateSelection() const;

    //! \copydoc QfMultiFeatureListModel::canProcessSelection
    bool canProcessSelection() const;

    //! \copydoc QfMultiFeatureListModel::mergeSelection
    bool mergeSelection();

    /**
     * Deletes a feature from a vector layer
     *
     * \param layer The layer from which a feature will be removed
     * \param fid The id of the feature to remove
     * \param selectionAction
     */
    bool deleteFeature( QgsVectorLayer *layer, QgsFeatureId fid, bool selectionAction = false );

    //! \copydoc QfMultiFeatureListModel::deleteSelection
    bool deleteSelection();

    //! \copydoc QfMultiFeatureListModel::duplicateFeature
    bool duplicateFeature( QgsVectorLayer *layer, const QgsFeature &feature );

    //! \copydoc QfMultiFeatureListModel::duplicateSelection
    bool duplicateSelection();

    //! \copydoc QfMultiFeatureListModel::moveSelection
    bool moveSelection( const double x, const double y, const QgsPoint &destinationPoint );

    //! \copydoc QfMultiFeatureListModel::rotateSelection
    bool rotateSelection( const double angle );

    //! \copydoc QfMultiFeatureListModel::toggleSelectedItem
    void toggleSelectedItem( int item );

    //! \copydoc QfMultiFeatureListModel::selectedFeatures
    QList<QgsFeature> selectedFeatures() const;

    //! \copydoc QfMultiFeatureListModel::selectedLayer
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
    bool updateConditionalStylingDetails( QgsVectorLayer *vectorLayer, const QgsFeature &feature, QgsExpressionContext &expressionContext );

    inline QPair<QgsMapLayer *, QgsFeature> *toFeature( const QModelIndex &index ) const
    {
      return static_cast<QPair<QgsMapLayer *, QgsFeature> *>( index.internalPointer() );
    }

    QList<QPair<QgsMapLayer *, QgsFeature>> mFeatures;
    QList<QPair<QgsMapLayer *, QgsFeature>> mSelectedFeatures;

    QMap<QString, QgsConditionalStyle> mFeaturesConditionalStyle;

    QMap<QString, QgsVectorLayer *> mRepresentationalLayers;
};

#endif // QFMULTIFEATURELISTMODELBASE_H
