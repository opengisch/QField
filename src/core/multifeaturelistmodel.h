/***************************************************************************
                            featurelistmodel.cpp
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

#ifndef MULTIFEATURELISTMODEL_H
#define MULTIFEATURELISTMODEL_H

#include <QAbstractItemModel>

#include <qgsfeaturerequest.h>

#include "identifytool.h"
#include "multifeaturelistmodelbase.h"

class MultiFeatureListModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( int count READ count NOTIFY countChanged )
    Q_PROPERTY( QList<QgsFeature> selectedFeatures READ selectedFeatures NOTIFY selectedCountChanged )
    Q_PROPERTY( int selectedCount READ selectedCount NOTIFY selectedCountChanged )
    Q_PROPERTY( bool canEditAttributesSelection READ canEditAttributesSelection NOTIFY selectedCountChanged  )
    Q_PROPERTY( bool canMergeSelection READ canMergeSelection NOTIFY selectedCountChanged  )
    Q_PROPERTY( bool canDeleteSelection READ canDeleteSelection NOTIFY selectedCountChanged  )

  public:
    enum FeatureListRoles
    {
      FeatureIdRole = Qt::UserRole + 1,
      FeatureSelectedRole,
      FeatureRole,
      LayerNameRole,
      LayerRole,
      GeometryRole,
      CrsRole,
      DeleteFeatureRole,
      EditGeometryRole
    };

    explicit MultiFeatureListModel( QObject *parent = nullptr );

    /**
     * Resets the model to contain features found from a list of \a requests.
     */
    void setFeatures( const QMap<QgsVectorLayer *, QgsFeatureRequest> requests );

    /**
     * Resets the model to contain features found from a \a vl.
     */
    Q_INVOKABLE void setFeatures( QgsVectorLayer *vl );

    /**
     * Appends features from a list of \a results.
     */
    void appendFeatures( const QList<IdentifyTool::IdentifyResult> &results );

    /**
     * Resets the model to either an empty feature list or one that contains only the selected features.
     * \param keepSelected if set to TRUE, selected features will be kept when resetting the model.
     */
    Q_INVOKABLE void clear( const bool keepSelected = false );

    /**
     * Empties the list of selected features.
     */
    Q_INVOKABLE void clearSelection();

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

    /**
     * Merges selected features by updating the first seleted feature's geometry
     * to a combination (i.e. union) of geometries of all selected features.
     *
     * All but the first feature will then be removed from the vector layer containing
     * the selected features.
     */
    Q_INVOKABLE bool mergeSelection();

    /**
     * Deletes a feature from a vector layer
     *
     * \param layer The layer from which a feature will be removed
     * \param fid The id of the feature to remove
     */
    Q_INVOKABLE bool deleteFeature( QgsVectorLayer *layer, QgsFeatureId fid );

    //! Deletes selected features
    Q_INVOKABLE bool deleteSelection();

    /**
     * Toggles the selection state of a given item.
     * \param item the item's row number
     */
    void toggleSelectedItem( int item );

    /**
     * Returns the list of currently selected features.
     */
    QList<QgsFeature> selectedFeatures();

  signals:

    void countChanged();

    void selectedCountChanged();

  protected:

    virtual bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

    void adjustFilterToSelectedCount();

  private:

    MultiFeatureListModelBase *mSourceModel = nullptr;

    QgsVectorLayer *mFilterLayer = nullptr;
};

#endif // MULTIFEATURELISTMODEL_H
