

# File qfmaplayermodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfmaplayermodel.cpp**](qfmaplayermodel_8cpp.md)

[Go to the documentation of this file](qfmaplayermodel_8cpp.md)


```C++
/***************************************************************************
  qfmaplayermodel.cpp

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

#include "qflayerutils.h"
#include "qfmaplayermodel.h"


QfMapLayerBaseModel::QfMapLayerBaseModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

void QfMapLayerBaseModel::resetModel()
{
  beginResetModel();

  mLayers.clear();
  if ( mEnabled && mProject )
  {
    addLayers( mProject->mapLayers().values() );
  }

  endResetModel();
}

void QfMapLayerBaseModel::setEnabled( bool enabled )
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
      connect( mProject, &QgsProject::layersAdded, this, &QfMapLayerBaseModel::addLayers );
      connect( mProject, static_cast<void ( QgsProject::* )( const QStringList & )>( &QgsProject::layersWillBeRemoved ), this, &QfMapLayerBaseModel::removeLayers );
    }
    else
    {
      disconnect( mProject, &QgsProject::layersAdded, this, &QfMapLayerBaseModel::addLayers );
      disconnect( mProject, static_cast<void ( QgsProject::* )( const QStringList & )>( &QgsProject::layersWillBeRemoved ), this, &QfMapLayerBaseModel::removeLayers );
    }
  }

  resetModel();
}

void QfMapLayerBaseModel::setProject( QgsProject *project )
{
  if ( mProject == project )
  {
    return;
  }

  if ( mEnabled && mProject )
  {
    disconnect( mProject, &QgsProject::layersAdded, this, &QfMapLayerBaseModel::addLayers );
    disconnect( mProject, static_cast<void ( QgsProject::* )( const QStringList & )>( &QgsProject::layersWillBeRemoved ), this, &QfMapLayerBaseModel::removeLayers );
  }

  mProject = project;
  emit projectChanged();

  if ( mEnabled && mProject )
  {
    connect( mProject, &QgsProject::layersAdded, this, &QfMapLayerBaseModel::addLayers );
    connect( mProject, static_cast<void ( QgsProject::* )( const QStringList & )>( &QgsProject::layersWillBeRemoved ), this, &QfMapLayerBaseModel::removeLayers );
  }

  resetModel();
}

void QfMapLayerBaseModel::setTrackingModel( QfTrackingModel *trackingModel )
{
  if ( mTrackingModel == trackingModel )
  {
    return;
  }

  mTrackingModel = trackingModel;
  emit trackingModelChanged();
}

void QfMapLayerBaseModel::removeLayers( const QStringList &layerIds )
{
  for ( const QString &layerId : layerIds )
  {
    QModelIndex startIndex = index( 0, 0 );
    QModelIndexList list = match( startIndex, QfMapLayerModel::IdRole, layerId );
    if ( !list.isEmpty() )
    {
      QModelIndex index = list[0];
      beginRemoveRows( QModelIndex(), index.row(), index.row() );
      mLayers.removeAt( index.row() );
      endRemoveRows();
    }
  }
}

void QfMapLayerBaseModel::addLayers( const QList<QgsMapLayer *> &layers )
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

int QfMapLayerBaseModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return mLayers.size();
}

QVariant QfMapLayerBaseModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() < 0 || index.row() >= mLayers.size() )
  {
    return QVariant();
  }

  QgsMapLayer *layer = mLayers.value( index.row() );

  switch ( role )
  {
    case Qt::DisplayRole:
    case QfMapLayerModel::NameRole:
    {
      return layer ? layer->name() : QString();
    }

    case QfMapLayerModel::IdRole:
    {
      return layer ? layer->id() : QString();
    }

    case QfMapLayerModel::LayerRole:
    {
      return QVariant::fromValue<QgsMapLayer *>( layer );
    }

    case QfMapLayerModel::LayerTypeRole:
    {
      if ( layer )
      {
        return QVariant::fromValue<Qgis::LayerType>( layer->type() );
      }
      return QVariant();
    }

    case QfMapLayerModel::GeometryTypeRole:
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

QHash<int, QByteArray> QfMapLayerBaseModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

  roles[QfMapLayerModel::IdRole] = "Id";
  roles[QfMapLayerModel::NameRole] = "Name";
  roles[QfMapLayerModel::LayerTypeRole] = "LayerType";
  roles[QfMapLayerModel::GeometryTypeRole] = "GeometryType";
  roles[QfMapLayerModel::LayerRole] = "LayerPointer";

  return roles;
}


QfMapLayerModel::QfMapLayerModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mModel( new QfMapLayerBaseModel( this ) )
{
  setSourceModel( mModel );
  setDynamicSortFilter( true );
  setSortLocaleAware( true );
  setFilterCaseSensitivity( Qt::CaseInsensitive );
  sort( 0 );

  connect( mModel, &QfMapLayerBaseModel::enabledChanged, this, &QfMapLayerModel::enabledChanged );
  connect( mModel, &QfMapLayerBaseModel::projectChanged, this, &QfMapLayerModel::projectChanged );
  connect( mModel, &QfMapLayerBaseModel::trackingModelChanged, this, &QfMapLayerModel::trackingModelChanged );
}

bool QfMapLayerModel::enabled() const
{
  return mModel->enabled();
}

void QfMapLayerModel::setEnabled( bool enabled )
{
  mModel->setEnabled( enabled );
}

QgsProject *QfMapLayerModel::project() const
{
  return mModel->project();
}

void QfMapLayerModel::setProject( QgsProject *project )
{
  mModel->setProject( project );
}

QfTrackingModel *QfMapLayerModel::trackingModel() const
{
  return mModel->trackingModel();
}

void QfMapLayerModel::setTrackingModel( QfTrackingModel *trackingModel )
{
  mModel->setTrackingModel( trackingModel );

  if ( mRequiresTrackingAvailability && enabled() )
  {
    beginFilterChange();
    endFilterChange( QSortFilterProxyModel::Direction::Rows );
  }
}

void QfMapLayerModel::setFilters( Qgis::LayerFilters filters )
{
  if ( mFilters == filters )
  {
    return;
  }

  beginFilterChange();
  mFilters = filters;
  endFilterChange( QSortFilterProxyModel::Direction::Rows );

  emit filtersChanged();
}

void QfMapLayerModel::setRequiresTrackingAvailability( bool requiresTrackingAvailability )
{
  if ( mRequiresTrackingAvailability == requiresTrackingAvailability )
  {
    return;
  }

  beginFilterChange();
  mRequiresTrackingAvailability = requiresTrackingAvailability;
  endFilterChange( QSortFilterProxyModel::Direction::Rows );

  emit requiresTrackingAvailabilityChanged();
}

int QfMapLayerModel::findLayer( QgsMapLayer *layer ) const
{
  if ( layer )
  {
    QModelIndex startIndex = index( 0, 0 );
    QModelIndexList list = match( startIndex, QfMapLayerModel::IdRole, layer->id() );
    if ( !list.isEmpty() )
    {
      QModelIndex index = list[0];
      return index.row();
    }
  }

  return -1;
}

int QfMapLayerModel::findLayerName( const QString &name ) const
{
  if ( !name.isEmpty() )
  {
    QModelIndex startIndex = index( 0, 0 );
    QModelIndexList list = match( startIndex, QfMapLayerModel::NameRole, name );
    if ( !list.isEmpty() )
    {
      QModelIndex index = list[0];
      return index.row();
    }
  }

  return -1;
}

QVariantMap QfMapLayerModel::get( int row ) const
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

bool QfMapLayerModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  const QModelIndex sourceIndex = mModel->index( sourceRow, 0, sourceParent );
  if ( sourceIndex.isValid() )
  {
    QgsMapLayer *layer = mModel->data( sourceIndex, QfMapLayerModel::LayerRole ).value<QgsMapLayer *>();
    return layerMatchesFilters( layer );
  }
  return false;
}

bool QfMapLayerModel::layerMatchesFilters( QgsMapLayer *layer ) const
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

    if ( vlayer->readOnly() || QfLayerUtils::isFeatureAdditionLocked( vlayer ) )
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

bool QfMapLayerModel::lessThan( const QModelIndex &left, const QModelIndex &right ) const
{
  const QString leftStr = mModel->data( left, QfMapLayerModel::NameRole ).toString();
  const QString rightStr = mModel->data( right, QfMapLayerModel::NameRole ).toString();
  return QString::localeAwareCompare( leftStr, rightStr ) < 0;
}
```


