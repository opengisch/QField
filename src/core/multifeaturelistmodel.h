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
     * @brief setFeatures
     * @param requests
     */
    void setFeatures( const QMap<QgsVectorLayer *, QgsFeatureRequest> requests );

    void appendFeatures( const QList<IdentifyTool::IdentifyResult> &results );

    Q_INVOKABLE void clear( const bool keepSelected = false );

    Q_INVOKABLE void clearSelection();

    int count() const;

    int selectedCount() const;

    void checkSelectedCount();

    //! Returns TRUE if the selected features can have their attributes value changed
    bool canEditAttributesSelection();

    //! Returns TRUE if the selected features can be deleted
    bool canDeleteSelection();

    /**
     * Deletes a feature from a vector layer
     *
     * \param layer The layer from which a feature will be removed
     * \param fid The id of the feature to remove
     */
    Q_INVOKABLE bool deleteFeature( QgsVectorLayer *layer, QgsFeatureId fid );

    //! Deletes selected features
    Q_INVOKABLE bool deleteSelection();

    void toggleSelectedItem( int item );

    QList<QgsFeature> selectedFeatures();

  signals:

    void countChanged();

    void selectedCountChanged();

  protected:

    virtual bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

  private:

    MultiFeatureListModelBase *mSourceModel = nullptr;

    QgsVectorLayer *mFilterLayer = nullptr;
};

#endif // MULTIFEATURELISTMODEL_H
