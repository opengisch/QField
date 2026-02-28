

# File maplayermodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**maplayermodel.cpp**](maplayermodel_8cpp.md)

[Go to the documentation of this file](maplayermodel_8cpp.md)


```C++
/***************************************************************************
  maplayermodel.cpp

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

#include "maplayermodel.h"


MapLayerBaseModel::MapLayerBaseModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

void MapLayerBaseModel::resetModel()
{
  beginResetModel();

  mLayers.clear();
  if ( mEnabled && mProject )
  {
    addLayers( mProject->mapLayers().values() );
  }

  endResetModel();
}

void MapLayerBaseModel::setEnabled( bool enabled )
{
  if ( mEnabled == enabled )
  {
    return;
  }

  mEnabled = enabled;
  emit enabledChanged();

  if ( mProject )
  {
    if ( mEnabled )
    {
      connect( mProject, &QgsProject::layersAdded, this, &MapLayerBaseModel::addLayers );
      connect( mProject, static_cast<void ( QgsProject::* )( const QStringList & )>( &QgsProject::layersWillBeRemoved ), this, &MapLayerBaseModel::removeLayers );
    }
    else
    {
      disconnect( mProject, &QgsProject::layersAdded, this, &MapLayerBaseModel::addLayers );
      disconnect( mProject, static_cast<void ( QgsProject::* )( const QStringList & )>( &QgsProject::layersWillBeRemoved ), this, &MapLayerBaseModel::removeLayers );
    }
  }

  resetModel();
}

void MapLayerBaseModel::setProject( QgsProject *project )
{
  if ( mProject == project )
  {
    return;
  }

  if ( mEnabled && mProject )
  {
    disconnect( mProject, &QgsProject::layersAdded, this, &MapLayerBaseModel::addLayers );
    disconnect( mProject, static_cast<void ( QgsProject::* )( const QStringList & )>( &QgsProject::layersWillBeRemoved ), this, &MapLayerBaseModel::removeLayers );
  }

  mProject = project;
  emit projectChanged();

  if ( mEnabled && mProject )
  {
    connect( mProject, &QgsProject::layersAdded, this, &MapLayerBaseModel::addLayers );
    connect( mProject, static_cast<void ( QgsProject::* )( const QStringList & )>( &QgsProject::layersWillBeRemoved ), this, &MapLayerBaseModel::removeLayers );
  }

  resetModel();
}

void MapLayerBaseModel::setTrackingModel( TrackingModel *trackingModel )
{
  if ( mTrackingModel == trackingModel )
  {
    return;
  }

  mTrackingModel = trackingModel;
  emit trackingModelChanged();
}

void MapLayerBaseModel::removeLayers( const QStringList &layerIds )
{
  for ( const QString &layerId : layerIds )
  {
    QModelIndex startIndex = index( 0, 0 );
    QModelIndexList list = match( startIndex, MapLayerModel::IdRole, layerId );
    if ( !list.isEmpty() )
    {
      QModelIndex index = list[0];
      beginRemoveRows( QModelIndex(), index.row(), index.row() );
      mLayers.removeAt( index.row() );
      endRemoveRows();
    }
  }
}

void MapLayerBaseModel::addLayers( const QList<QgsMapLayer *> &layers )
{
  if ( !layers.isEmpty() )
  {
    beginInsertRows( QModelIndex(), mLayers.size(), mLayers.size() + layers.size() - 1 );
    for ( QgsMapLayer *layer : layers ) // cppcheck-suppress constVariablePointer
    {
      mLayers.append( layer );
    }
    endInsertRows();
  }
}

int MapLayerBaseModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return mLayers.size();
}

QVariant MapLayerBaseModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() < 0 || index.row() >= mLayers.size() )
  {
    return QVariant();
  }

  QgsMapLayer *layer = mLayers.value( index.row() );

  switch ( role )
  {
    case Qt::DisplayRole:
    case MapLayerModel::NameRole:
    {
      return layer ? layer->name() : QString();
    }

    case MapLayerModel::IdRole:
    {
      return layer ? layer->id() : QString();
    }

    case MapLayerModel::LayerRole:
    {
      return QVariant::fromValue<QgsMapLayer *>( layer );
    }

    case MapLayerModel::LayerTypeRole:
    {
      if ( layer )
      {
        return QVariant::fromValue<Qgis::LayerType>( layer->type() );
      }
      return QVariant();
    }

    case MapLayerModel::GeometryTypeRole:
    {
      if ( QgsVectorLayer *vlayer = dynamic_cast<QgsVectorLayer *>( layer ) )
      {
        return QVariant::fromValue<Qgis::GeometryType>( vlayer->geometryType() );
      }
      return QVariant();
    }

    default:
      break;
  }

  return QVariant();
}

QHash<int, QByteArray> MapLayerBaseModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

  roles[MapLayerModel::IdRole] = "Id";
  roles[MapLayerModel::NameRole] = "Name";
  roles[MapLayerModel::LayerTypeRole] = "LayerType";
  roles[MapLayerModel::GeometryTypeRole] = "GeometryType";
  roles[MapLayerModel::LayerRole] = "LayerPointer";

  return roles;
}


MapLayerModel::MapLayerModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mModel( new MapLayerBaseModel( this ) )
{
  setSourceModel( mModel );
  setDynamicSortFilter( true );
  setSortLocaleAware( true );
  setFilterCaseSensitivity( Qt::CaseInsensitive );
  sort( 0 );

  connect( mModel, &MapLayerBaseModel::enabledChanged, this, &MapLayerModel::enabledChanged );
  connect( mModel, &MapLayerBaseModel::projectChanged, this, &MapLayerModel::projectChanged );
  connect( mModel, &MapLayerBaseModel::trackingModelChanged, this, &MapLayerModel::trackingModelChanged );
}

bool MapLayerModel::enabled() const
{
  return mModel->enabled();
}

void MapLayerModel::setEnabled( bool enabled )
{
  mModel->setEnabled( enabled );
}

QgsProject *MapLayerModel::project() const
{
  return mModel->project();
}

void MapLayerModel::setProject( QgsProject *project )
{
  mModel->setProject( project );
}

TrackingModel *MapLayerModel::trackingModel() const
{
  return mModel->trackingModel();
}

void MapLayerModel::setTrackingModel( TrackingModel *trackingModel )
{
  mModel->setTrackingModel( trackingModel );

  if ( mRequiresTrackingAvailability && enabled() )
  {
    invalidateFilter();
  }
}

void MapLayerModel::setFilters( Qgis::LayerFilters filters )
{
  if ( mFilters == filters )
  {
    return;
  }

  mFilters = filters;
  emit filtersChanged();

  invalidateFilter();
}

void MapLayerModel::setRequiresTrackingAvailability( bool requiresTrackingAvailability )
{
  if ( mRequiresTrackingAvailability == requiresTrackingAvailability )
  {
    return;
  }

  mRequiresTrackingAvailability = requiresTrackingAvailability;
  emit requiresTrackingAvailabilityChanged();

  invalidateFilter();
}

int MapLayerModel::findLayer( QgsMapLayer *layer ) const
{
  if ( layer )
  {
    QModelIndex startIndex = index( 0, 0 );
    QModelIndexList list = match( startIndex, MapLayerModel::IdRole, layer->id() );
    if ( !list.isEmpty() )
    {
      QModelIndex index = list[0];
      return index.row();
    }
  }

  return -1;
}

QVariantMap MapLayerModel::get( int row ) const
{
  QVariantMap data;
  const QModelIndex idx = index( row, 0 );
  if ( !idx.isValid() )
  {
    return data;
  }

  const QHash<int, QByteArray> roles = roleNames();
  QHashIterator<int, QByteArray> it( roles );
  while ( it.hasNext() )
  {
    it.next();
    data[it.value()] = idx.data( it.key() );
  }

  return data;
}

bool MapLayerModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  const QModelIndex sourceIndex = mModel->index( sourceRow, 0, sourceParent );
  if ( sourceIndex.isValid() )
  {
    QgsMapLayer *layer = mModel->data( sourceIndex, MapLayerModel::LayerRole ).value<QgsMapLayer *>();
    return layerMatchesFilters( layer );
  }
  return false;
}

bool MapLayerModel::layerMatchesFilters( QgsMapLayer *layer ) const
{
  if ( !layer )
  {
    return false;
  }

  if ( mRequiresTrackingAvailability )
  {
    QgsVectorLayer *vlayer = dynamic_cast<QgsVectorLayer *>( layer );
    if ( !vlayer )
    {
      return false;
    }

    if ( vlayer->geometryType() == Qgis::GeometryType::Null || vlayer->geometryType() == Qgis::GeometryType::Unknown )
    {
      return false;
    }

    if ( !mModel->trackingModel() || mModel->trackingModel()->layerInActiveTracking( vlayer ) )
    {
      return false;
    }
  }

  if ( mFilters.testFlag( Qgis::LayerFilter::WritableLayer ) && layer->readOnly() )
  {
    return false;
  }

  if ( mFilters.testFlag( Qgis::LayerFilter::All ) )
  {
    return true;
  }

  if ( ( mFilters.testFlag( Qgis::LayerFilter::RasterLayer ) && layer->type() == Qgis::LayerType::Raster ) || ( mFilters.testFlag( Qgis::LayerFilter::VectorLayer ) && layer->type() == Qgis::LayerType::Vector ) || ( mFilters.testFlag( Qgis::LayerFilter::MeshLayer ) && layer->type() == Qgis::LayerType::Mesh ) || ( mFilters.testFlag( Qgis::LayerFilter::VectorTileLayer ) && layer->type() == Qgis::LayerType::VectorTile ) || ( mFilters.testFlag( Qgis::LayerFilter::PointCloudLayer ) && layer->type() == Qgis::LayerType::PointCloud ) || ( mFilters.testFlag( Qgis::LayerFilter::AnnotationLayer ) && layer->type() == Qgis::LayerType::Annotation ) || ( mFilters.testFlag( Qgis::LayerFilter::TiledSceneLayer ) && layer->type() == Qgis::LayerType::TiledScene ) || ( mFilters.testFlag( Qgis::LayerFilter::PluginLayer ) && layer->type() == Qgis::LayerType::Plugin ) )
  {
    return true;
  }

  // geometry type
  const bool detectGeometry = mFilters.testFlag( Qgis::LayerFilter::NoGeometry ) || mFilters.testFlag( Qgis::LayerFilter::PointLayer ) || mFilters.testFlag( Qgis::LayerFilter::LineLayer ) || mFilters.testFlag( Qgis::LayerFilter::PolygonLayer );
  if ( detectGeometry && layer->type() == Qgis::LayerType::Vector )
  {
    if ( const QgsVectorLayer *vl = qobject_cast<const QgsVectorLayer *>( layer ) )
    {
      if ( mFilters.testFlag( Qgis::LayerFilter::HasGeometry ) && vl->isSpatial() )
      {
        return true;
      }
      if ( mFilters.testFlag( Qgis::LayerFilter::NoGeometry ) && vl->geometryType() == Qgis::GeometryType::Null )
      {
        return true;
      }
      if ( mFilters.testFlag( Qgis::LayerFilter::PointLayer ) && vl->geometryType() == Qgis::GeometryType::Point )
      {
        return true;
      }
      if ( mFilters.testFlag( Qgis::LayerFilter::LineLayer ) && vl->geometryType() == Qgis::GeometryType::Line )
      {
        return true;
      }
      if ( mFilters.testFlag( Qgis::LayerFilter::PolygonLayer ) && vl->geometryType() == Qgis::GeometryType::Polygon )
      {
        return true;
      }
    }
  }

  return false;
}

bool MapLayerModel::lessThan( const QModelIndex &left, const QModelIndex &right ) const
{
  const QString leftStr = mModel->data( left, MapLayerModel::NameRole ).toString();
  const QString rightStr = mModel->data( right, MapLayerModel::NameRole ).toString();
  return QString::localeAwareCompare( leftStr, rightStr ) < 0;
}
```


