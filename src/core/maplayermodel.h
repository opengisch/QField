/***************************************************************************
  maplayermodel.h

 ---------------------
 begin                : 04.01.2026
 copyright            : (C) 2026 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef MAPLAYERMODEL_H
#define MAPLAYERMODEL_H

#include "trackingmodel.h"

#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <qgsproject.h>

/**
 * \brief A project map layers base model.
 * \ingroup core
 */
class MapLayerBaseModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    explicit MapLayerBaseModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;

    //! \copydoc MapLayerModel::enabled
    bool enabled() const { return mEnabled; }
    //! \copydoc MapLayerModel::enabled
    void setEnabled( bool enabled );

    //! \copydoc MapLayerModel::project
    QgsProject *project() const { return mProject; }
    //! \copydoc MapLayerModel::project
    void setProject( QgsProject *project );

    //! \copydoc MapLayerModel::trackingModel
    TrackingModel *trackingModel() const { return mTrackingModel; }
    //! \copydoc MapLayerModel::trackingModel
    void setTrackingModel( TrackingModel *trackingModel );

  signals:
    void enabledChanged();
    void projectChanged();
    void trackingModelChanged();

  protected slots:
    void removeLayers( const QStringList &layerIds );
    void addLayers( const QList<QgsMapLayer *> &layers );

  private:
    void resetModel();

    bool mEnabled = true;
    QList<QgsMapLayer *> mLayers;
    QgsProject *mProject = nullptr;
    TrackingModel *mTrackingModel = nullptr;
};


/**
 * \brief A project map layers model within filtering capabilities.
 * \ingroup core
 */
class MapLayerModel : public QSortFilterProxyModel
{
    Q_OBJECT

    /**
     * The enabled state of the model.
     * \note When disabled, any changes to the project's map layers registry will be ignored
     */
    Q_PROPERTY( bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged )

    //! The project from which map layers will be taken from.
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )

    //! The tracking model against which  map layers' tracking availability will be checked against.
    Q_PROPERTY( TrackingModel *trackingModel READ trackingModel WRITE setTrackingModel NOTIFY trackingModelChanged )

    //! The map layer filters.
    Q_PROPERTY( Qgis::LayerFilters filters READ filters WRITE setFilters NOTIFY filtersChanged )

    //! The tracking availability requirement filter.
    Q_PROPERTY( bool requiresTrackingAvailability READ requiresTrackingAvailability WRITE setRequiresTrackingAvailability NOTIFY requiresTrackingAvailabilityChanged )

  public:
    enum Role
    {
      IdRole = Qt::UserRole + 1,
      NameRole,
      LayerRole,
      LayerTypeRole,
      GeometryTypeRole,
    };
    Q_ENUM( Role )

    //! The constructor
    explicit MapLayerModel( QObject *parent = nullptr );

    //! \copydoc MapLayerModel::enabled
    bool enabled() const;
    //! \copydoc MapLayerModel::enabled
    void setEnabled( bool enabled );

    //! \copydoc MapLayerModel::project
    QgsProject *project() const;
    //! \copydoc MapLayerModel::project
    void setProject( QgsProject *project );

    //! \copydoc MapLayerModel::trackingModel
    TrackingModel *trackingModel() const;
    //! \copydoc MapLayerModel::trackingModel
    void setTrackingModel( TrackingModel *trackingModel );

    //! \copydoc MapLayerModel::filters
    Qgis::LayerFilters filters() const { return mFilters; }
    //! \copydoc MapLayerModel::filters
    void setFilters( Qgis::LayerFilters filters );

    //! \copydoc MapLayerModel::requiresTrackingAvailability
    bool requiresTrackingAvailability() const { return mRequiresTrackingAvailability; }
    //! \copydoc MapLayerModel::requiresTrackingAvailability
    void setRequiresTrackingAvailability( bool requiresTrackingAvailability );

    /**
     * Returns the model row for given map \a layer.
     * \note If the map layer is not found, -1 will be returned
     */
    Q_INVOKABLE int findLayer( QgsMapLayer *layer );

    /**
     * Returns a model data map for a given \a row.
     */
    Q_INVOKABLE QVariantMap get( int row ) const;

    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;
    bool lessThan( const QModelIndex &left, const QModelIndex &right ) const override;

  signals:
    void enabledChanged();
    void projectChanged();
    void trackingModelChanged();
    void filtersChanged();
    void requiresTrackingAvailabilityChanged();

  private:
    bool layerMatchesFilters( QgsMapLayer *layer ) const;

    MapLayerBaseModel *mModel = nullptr;
    Qgis::LayerFilters mFilters = Qgis::LayerFilter::All;
    bool mRequiresTrackingAvailability = false;
};

#endif // MAPLAYERMODEL_H
