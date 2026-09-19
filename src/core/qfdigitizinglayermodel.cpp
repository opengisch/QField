/***************************************************************************
  qfdigitizinglayermodel.cpp

 ---------------------
 begin                : 18.09.2026
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

#include "qfdigitizinglayermodel.h"
#include "qflayerutils.h"


QfDigitizingLayerBaseModel::QfDigitizingLayerBaseModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

void QfDigitizingLayerBaseModel::resetModel()
{
  beginResetModel();
  mLayers.clear();

  if ( mEnabled )
  {
    if ( mProject )
    {
      addLayers( mProject->mapLayers().values() );
    }

    if ( mMarkupManager )
    {
      addCollections( mMarkupManager->collectionUuids() );
    }
  }

  endResetModel();
}

void QfDigitizingLayerBaseModel::setEnabled( bool enabled )
{
  if ( mEnabled == enabled )
  {
    return;
  }

  mEnabled = enabled;
  emit enabledChanged();

  if ( mEnabled )
  {
    if ( mProject )
    {
      connect( mProject, &QgsProject::layersAdded, this, &QfDigitizingLayerBaseModel::addLayers );
      connect( mProject, static_cast<void ( QgsProject::* )( const QStringList & )>( &QgsProject::layersWillBeRemoved ), this, &QfDigitizingLayerBaseModel::removeLayers );
    }

    if ( mMarkupManager )
    {
      connect( mMarkupManager, &QfMarkupManager::collectionsAdded, this, &QfDigitizingLayerBaseModel::addCollections );
      connect( mMarkupManager, &QfMarkupManager::collectionsWillBeRemoved, this, &QfDigitizingLayerBaseModel::removeCollections );
    }
  }
  else
  {
    if ( mProject )
    {
      disconnect( mProject, &QgsProject::layersAdded, this, &QfDigitizingLayerBaseModel::addLayers );
      disconnect( mProject, static_cast<void ( QgsProject::* )( const QStringList & )>( &QgsProject::layersWillBeRemoved ), this, &QfDigitizingLayerBaseModel::removeLayers );
    }

    if ( mMarkupManager )
    {
      disconnect( mMarkupManager, &QfMarkupManager::collectionsAdded, this, &QfDigitizingLayerBaseModel::addCollections );
      disconnect( mMarkupManager, &QfMarkupManager::collectionsWillBeRemoved, this, &QfDigitizingLayerBaseModel::removeCollections );
    }
  }

  resetModel();
}

void QfDigitizingLayerBaseModel::setProject( QgsProject *project )
{
  if ( mProject == project )
  {
    return;
  }

  if ( mEnabled && mProject )
  {
    disconnect( mProject, &QgsProject::layersAdded, this, &QfDigitizingLayerBaseModel::addLayers );
    disconnect( mProject, static_cast<void ( QgsProject::* )( const QStringList & )>( &QgsProject::layersWillBeRemoved ), this, &QfDigitizingLayerBaseModel::removeLayers );
  }

  mProject = project;
  emit projectChanged();

  if ( mEnabled && mProject )
  {
    connect( mProject, &QgsProject::layersAdded, this, &QfDigitizingLayerBaseModel::addLayers );
    connect( mProject, static_cast<void ( QgsProject::* )( const QStringList & )>( &QgsProject::layersWillBeRemoved ), this, &QfDigitizingLayerBaseModel::removeLayers );
  }

  resetModel();
}

void QfDigitizingLayerBaseModel::setMarkupManager( QfMarkupManager *markupManager )
{
  if ( mMarkupManager == markupManager )
  {
    return;
  }

  if ( mEnabled && mMarkupManager )
  {
    disconnect( mMarkupManager, &QfMarkupManager::collectionsAdded, this, &QfDigitizingLayerBaseModel::addCollections );
    disconnect( mMarkupManager, &QfMarkupManager::collectionsWillBeRemoved, this, &QfDigitizingLayerBaseModel::removeCollections );
  }

  mMarkupManager = markupManager;
  emit markupManagerChanged();

  if ( mEnabled && mMarkupManager )
  {
    connect( mMarkupManager, &QfMarkupManager::collectionsAdded, this, &QfDigitizingLayerBaseModel::addCollections );
    connect( mMarkupManager, &QfMarkupManager::collectionsWillBeRemoved, this, &QfDigitizingLayerBaseModel::removeCollections );
  }

  resetModel();
}

void QfDigitizingLayerBaseModel::removeLayers( const QStringList &layerIds )
{
  if ( mLayers.isEmpty() || layerIds.isEmpty() )
  {
    return;
  }

  for ( const QString &layerId : layerIds )
  {
    auto match = std::find_if( mLayers.begin(), mLayers.end(), [&layerId]( const DigitizingLayer &l ) {
      return l.type == QfDigitizingLayerModel::MapLayer && l.mapLayer->id() == layerId;
    } );
    if ( match != mLayers.end() )
    {
      const int idx = std::distance( mLayers.begin(), match );
      beginRemoveRows( QModelIndex(), idx, idx );
      mLayers.removeAt( idx );
      endRemoveRows();
    }
  }
}

void QfDigitizingLayerBaseModel::addLayers( const QList<QgsMapLayer *> &layers )
{
  QList<QgsMapLayer *> mapLayers;
  for ( QgsMapLayer *layer : layers ) // cppcheck-suppress constVariablePointer
  {
    if ( !layer || layer->flags().testFlag( QgsMapLayer::Private ) )
    {
      continue;
    }

    QgsVectorLayer *vlayer = dynamic_cast<QgsVectorLayer *>( layer );
    if ( !vlayer || vlayer->readOnly() )
    {
      continue;
    }

    if ( layer->customProperty( QStringLiteral( "QFieldSync/is_feature_addition_locked" ), false ).toBool() && !layer->customProperty( QStringLiteral( "QFieldSync/is_feature_addition_locked_expression_active" ), false ).toBool() )
    {
      continue;
    }

    mapLayers << layer;
  }

  if ( mapLayers.isEmpty() )
  {
    return;
  }

  beginInsertRows( QModelIndex(), mLayers.size(), mLayers.size() + mapLayers.size() - 1 );
  for ( QgsMapLayer *mapLayer : mapLayers ) // cppcheck-suppress constVariablePointer
  {
    DigitizingLayer layer;
    layer.name = mapLayer->name();
    layer.type = QfDigitizingLayerModel::MapLayer;
    layer.mapLayer = mapLayer;
    mLayers.append( layer );
  }
  endInsertRows();
}

void QfDigitizingLayerBaseModel::removeCollections( const QStringList &collectionUuids )
{
  if ( mLayers.isEmpty() || collectionUuids.isEmpty() )
  {
    return;
  }

  for ( const QString &collectionUuid : collectionUuids )
  {
    auto match = std::find_if( mLayers.begin(), mLayers.end(), [this, &collectionUuid]( const DigitizingLayer &l ) {
      return l.type == QfDigitizingLayerModel::MarkupCollection && mMarkupManager->collection( collectionUuid );
    } );
    if ( match != mLayers.end() )
    {
      const int idx = std::distance( mLayers.begin(), match );
      beginRemoveRows( QModelIndex(), idx, idx );
      mLayers.removeAt( idx );
      endRemoveRows();
    }
  }
}

void QfDigitizingLayerBaseModel::addCollections( const QStringList &collectionUuids )
{
  QList<QfMarkupCollection *> collections;
  for ( const QString &collectionUuid : collectionUuids )
  {
    QfMarkupCollection *collection = mMarkupManager->collection( collectionUuid ); // cppcheck-suppress constVariablePointer
    if ( !collection )
    {
      continue;
    }

    collections << collection;
  }

  if ( collections.isEmpty() )
  {
    return;
  }

  beginInsertRows( QModelIndex(), mLayers.size(), mLayers.size() + collections.size() - 1 );
  for ( QfMarkupCollection *collection : collections ) // cppcheck-suppress constVariablePointer
  {
    DigitizingLayer layer;
    layer.name = collection->name();
    layer.type = QfDigitizingLayerModel::MarkupCollection;
    layer.markupCollection = collection;
    mLayers.append( layer );
  }
  endInsertRows();
}

int QfDigitizingLayerBaseModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return mLayers.size();
}

QVariant QfDigitizingLayerBaseModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() < 0 || index.row() >= mLayers.size() )
  {
    return QVariant();
  }

  const DigitizingLayer &layer = mLayers[index.row()];

  switch ( role )
  {
    case Qt::DisplayRole:
    case QfDigitizingLayerModel::NameRole:
    {
      return layer.name;
    }

    case QfDigitizingLayerModel::IdRole:
    {
      switch ( layer.type )
      {
        case QfDigitizingLayerModel::MapLayer:
        {
          if ( layer.mapLayer )
          {
            return layer.mapLayer->id();
          }
        }

        case QfDigitizingLayerModel::MarkupCollection:
        {
          if ( layer.markupCollection && mMarkupManager )
          {
            return mMarkupManager->collectionUuid( layer.markupCollection );
          }
        }
      }

      return QString();
    }

    case QfDigitizingLayerModel::LayerRole:
    {
      switch ( layer.type )
      {
        case QfDigitizingLayerModel::MapLayer:
        {
          return QVariant::fromValue<QgsMapLayer *>( layer.mapLayer );
        }

        case QfDigitizingLayerModel::MarkupCollection:
        {
          return QVariant::fromValue<QfMarkupCollection *>( layer.markupCollection );
        }
      }

      return QVariant();
    }

    case QfDigitizingLayerModel::LayerTypeRole:
    {
      return QVariant::fromValue<QfDigitizingLayerModel::LayerType>( layer.type );
    }

    case QfDigitizingLayerModel::GeometryTypeRole:
    {
      if ( QgsVectorLayer *vlayer = dynamic_cast<QgsVectorLayer *>( layer.mapLayer ) )
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

QHash<int, QByteArray> QfDigitizingLayerBaseModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

  roles[QfDigitizingLayerModel::IdRole] = "Id";
  roles[QfDigitizingLayerModel::LayerTypeRole] = "LayerType";
  roles[QfDigitizingLayerModel::NameRole] = "Name";
  roles[QfDigitizingLayerModel::LayerRole] = "LayerPointer";
  roles[QfDigitizingLayerModel::GeometryTypeRole] = "GeometryType";

  return roles;
}

int QfDigitizingLayerBaseModel::findLayer( QgsMapLayer *layer ) const
{
  if ( layer )
  {
    auto match = std::find_if( mLayers.begin(), mLayers.end(), [layer]( const DigitizingLayer &l ) {
      return l.type == QfDigitizingLayerModel::MapLayer && l.mapLayer == layer;
    } );
    if ( match != mLayers.end() )
    {
      return std::distance( mLayers.begin(), match );
    }
  }

  return -1;
}

int QfDigitizingLayerBaseModel::findLayerName( const QString &name ) const
{
  if ( !name.isEmpty() )
  {
    auto match = std::find_if( mLayers.begin(), mLayers.end(), [&name]( const DigitizingLayer &l ) {
      return l.type == QfDigitizingLayerModel::MapLayer && l.name == name;
    } );
    if ( match != mLayers.end() )
    {
      return std::distance( mLayers.begin(), match );
    }
  }

  return -1;
}

int QfDigitizingLayerBaseModel::findCollection( QfMarkupCollection *collection ) const
{
  if ( collection )
  {
    auto match = std::find_if( mLayers.begin(), mLayers.end(), [collection]( const DigitizingLayer &l ) {
      return l.type == QfDigitizingLayerModel::MarkupCollection && l.markupCollection == collection;
    } );
    if ( match != mLayers.end() )
    {
      return std::distance( mLayers.begin(), match );
    }
  }

  return -1;
}

int QfDigitizingLayerBaseModel::findCollectionName( const QString &name ) const
{
  if ( !name.isEmpty() )
  {
    auto match = std::find_if( mLayers.begin(), mLayers.end(), [&name]( const DigitizingLayer &l ) {
      return l.type == QfDigitizingLayerModel::MarkupCollection && l.name == name;
    } );
    if ( match != mLayers.end() )
    {
      return std::distance( mLayers.begin(), match );
    }
  }

  return -1;
}


QfDigitizingLayerModel::QfDigitizingLayerModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mModel( new QfDigitizingLayerBaseModel( this ) )
{
  setSourceModel( mModel );
  setDynamicSortFilter( true );
  setSortLocaleAware( true );
  sort( 0 );

  connect( mModel, &QfDigitizingLayerBaseModel::enabledChanged, this, &QfDigitizingLayerModel::enabledChanged );
  connect( mModel, &QfDigitizingLayerBaseModel::projectChanged, this, &QfDigitizingLayerModel::projectChanged );
  connect( mModel, &QfDigitizingLayerBaseModel::markupManagerChanged, this, &QfDigitizingLayerModel::markupManagerChanged );
}

bool QfDigitizingLayerModel::enabled() const
{
  return mModel->enabled();
}

void QfDigitizingLayerModel::setEnabled( bool enabled )
{
  mModel->setEnabled( enabled );
}

QgsProject *QfDigitizingLayerModel::project() const
{
  return mModel->project();
}

void QfDigitizingLayerModel::setProject( QgsProject *project )
{
  mModel->setProject( project );
}

QfMarkupManager *QfDigitizingLayerModel::markupManager() const
{
  return mModel->markupManager();
}

void QfDigitizingLayerModel::setMarkupManager( QfMarkupManager *markupManager )
{
  mModel->setMarkupManager( markupManager );
}

int QfDigitizingLayerModel::findLayer( QgsMapLayer *layer ) const
{
  const int sourceRow = mModel->findLayer( layer );
  if ( sourceRow > -1 )
  {
    const QModelIndex sourceIndex = mModel->index( sourceRow, 0 );
    return mapFromSource( sourceIndex ).row();
  }
  return -1;
}

int QfDigitizingLayerModel::findLayerName( const QString &name ) const
{
  const int sourceRow = mModel->findLayerName( name );
  if ( sourceRow > -1 )
  {
    const QModelIndex sourceIndex = mModel->index( sourceRow, 0 );
    return mapFromSource( sourceIndex ).row();
  }
  return -1;
}

int QfDigitizingLayerModel::findCollection( QfMarkupCollection *collection ) const
{
  const int sourceRow = mModel->findCollection( collection );
  if ( sourceRow > -1 )
  {
    const QModelIndex sourceIndex = mModel->index( sourceRow, 0 );
    return mapFromSource( sourceIndex ).row();
  }
  return -1;
}

int QfDigitizingLayerModel::findCollectionName( const QString &name ) const
{
  const int sourceRow = mModel->findCollectionName( name );
  if ( sourceRow > -1 )
  {
    const QModelIndex sourceIndex = mModel->index( sourceRow, 0 );
    return mapFromSource( sourceIndex ).row();
  }
  return -1;
}

QVariantMap QfDigitizingLayerModel::get( int row ) const
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

bool QfDigitizingLayerModel::lessThan( const QModelIndex &left, const QModelIndex &right ) const
{
  const int leftLayerType = mModel->data( left, QfDigitizingLayerModel::LayerTypeRole ).toInt();
  const int rightLayerType = mModel->data( right, QfDigitizingLayerModel::LayerTypeRole ).toInt();
  if ( leftLayerType != rightLayerType )
  {
    return leftLayerType > rightLayerType;
  }

  const QString leftStr = mModel->data( left, QfDigitizingLayerModel::NameRole ).toString();
  const QString rightStr = mModel->data( right, QfDigitizingLayerModel::NameRole ).toString();
  return QString::localeAwareCompare( leftStr, rightStr ) < 0;
}
