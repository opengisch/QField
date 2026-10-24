

# File qfmaplayermodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfmaplayermodel.h**](qfmaplayermodel_8h.md)

[Go to the documentation of this file](qfmaplayermodel_8h.md)


```C++
/***************************************************************************
  qfmaplayermodel.h

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
#ifndef QFMAPLAYERMODEL_H
#define QFMAPLAYERMODEL_H

#include "qftrackingmodel.h"

#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <qgsproject.h>

class QfMapLayerBaseModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    explicit QfMapLayerBaseModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool enabled() const { return mEnabled; }
    void setEnabled( bool enabled );

    QgsProject *project() const { return mProject; }
    void setProject( QgsProject *project );

    QfTrackingModel *trackingModel() const { return mTrackingModel; }
    void setTrackingModel( QfTrackingModel *trackingModel );

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
    QfTrackingModel *mTrackingModel = nullptr;
};


class QfMapLayerModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged )

    
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )

    Q_PROPERTY( QfTrackingModel *trackingModel READ trackingModel WRITE setTrackingModel NOTIFY trackingModelChanged )

    Q_PROPERTY( Qgis::LayerFilters filters READ filters WRITE setFilters NOTIFY filtersChanged )

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

    
    explicit QfMapLayerModel( QObject *parent = nullptr );

    bool enabled() const;
    void setEnabled( bool enabled );

    QgsProject *project() const;
    void setProject( QgsProject *project );

    QfTrackingModel *trackingModel() const;
    void setTrackingModel( QfTrackingModel *trackingModel );

    Qgis::LayerFilters filters() const { return mFilters; }
    void setFilters( Qgis::LayerFilters filters );

    bool requiresTrackingAvailability() const { return mRequiresTrackingAvailability; }
    void setRequiresTrackingAvailability( bool requiresTrackingAvailability );

    Q_INVOKABLE int findLayer( QgsMapLayer *layer ) const;

    Q_INVOKABLE int findLayerName( const QString &name ) const;

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

    QfMapLayerBaseModel *mModel = nullptr;
    Qgis::LayerFilters mFilters = Qgis::LayerFilter::All;
    bool mRequiresTrackingAvailability = false;
};

#endif // QFMAPLAYERMODEL_H
```


