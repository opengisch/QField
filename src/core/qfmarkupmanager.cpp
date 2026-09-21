/***************************************************************************
 qfmarkupmanager.cpp - QfMarkupManager

 ---------------------
 begin                : 12.09.2026
 copyright            : (C) 2026 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qffileutils.h"
#include "qfmarkupmanager.h"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>


QfMarkupManager::QfMarkupManager( QObject *parent )
  : QObject( parent )
{
}

QStringList QfMarkupManager::collectionNames() const
{
  QStringList names;
  for ( const QfMarkupCollection *collection : mCollections )
  {
    names << collection->name();
  }
  return names;
}

QString QfMarkupManager::collectionUuid( QfMarkupCollection *collection ) const
{
  return mCollections.key( collection );
}

QfMarkupCollection *QfMarkupManager::collection( const QString &uuid ) const
{
  return mCollections.contains( uuid ) ? mCollections[uuid] : nullptr;
}

void QfMarkupManager::setHiddenCollectionUuids( const QStringList &hiddenCollectionUuids )
{
  if ( mHiddenCollectionUuids == hiddenCollectionUuids )
  {
    return;
  }

  mHiddenCollectionUuids = hiddenCollectionUuids;
  emit hiddenCollectionUuidsChanged();

  if ( !mCollections.isEmpty() )
  {
    emit visibleCollectionsChanged();
  }
}

QList<QfMarkupCollection *> QfMarkupManager::visibleCollections() const
{
  QList<QfMarkupCollection *> visibleCollections;
  for ( QfMarkupCollection *collection : mCollections ) // cppcheck-suppress constVariablePointer
  {
    if ( !mHiddenCollectionUuids.contains( collection->uuid() ) )
    {
      visibleCollections << collection;
    }
  }
  return visibleCollections;
}

bool QfMarkupManager::hasItems() const
{
  return std::any_of( mCollections.begin(), mCollections.end(), []( const QfMarkupCollection *collection ) { return collection->count() > 0; } );
}

void QfMarkupManager::reset( const QString &path, const QString &prefix )
{
  emit collectionsWillBeRemoved( mCollections.keys() );

  qDeleteAll( mCollections );
  mCollections.clear();

  emit collectionsChanged();
  emit visibleCollectionsChanged();
  emit hasItemsChanged();

  const QFileInfo fi( path );
  mPath = fi.absoluteFilePath();
  mPrefix = prefix;

  QDirIterator collectionGeoJsonPaths( path, QStringList() << QStringLiteral( "%1*.geojson" ).arg( prefix ), QDir::Files );
  while ( collectionGeoJsonPaths.hasNext() )
  {
    collectionGeoJsonPaths.next();
    QfMarkupCollection *collection = new QfMarkupCollection();
    if ( collection->readGeoJson( collectionGeoJsonPaths.filePath() ) )
    {
      if ( collection->name().isEmpty() )
      {
        collection->setName( tr( "Unnamed collection" ) );
      }
      insertCollection( collection, collectionGeoJsonPaths.filePath() );
    }
  }

  if ( mCollections.isEmpty() )
  {
    // A default markup collection is added
    QfMarkupCollection *collection = new QfMarkupCollection( tr( "Default collection" ) );
    insertCollection( collection );
  }

  emit collectionsAdded( mCollections.keys() );

  emit collectionsChanged();
  emit visibleCollectionsChanged();
  emit hasItemsChanged();
}

void QfMarkupManager::insertCollection( QfMarkupCollection *collection, const QString &collectionPath )
{
  if ( mCollections.contains( collection->name() ) )
  {
    int i = 1;
    QString uniqueName = QStringLiteral( "%1 - %2" ).arg( collection->name(), QString::number( i ) );
    while ( mCollections.contains( uniqueName ) )
    {
      uniqueName = QStringLiteral( "%1 - %2" ).arg( collection->name(), QString::number( ++i ) );
    }
    collection->setName( uniqueName );
  }

  connect( collection, &QfMarkupCollection::itemsChanged, this, &QfMarkupManager::processCollectionItemsChanged );
  connect( collection, &QfMarkupCollection::countChanged, this, &QfMarkupManager::hasItemsChanged );

  mCollections.insert( collection->uuid(), collection );


  QString geoJsonPath = collectionPath;
  if ( geoJsonPath.isEmpty() )
  {
    geoJsonPath = QStringLiteral( "%1/%2markup-%3.geojson" ).arg( mPath, !mPrefix.isEmpty() ? QStringLiteral( "%1-" ).arg( mPrefix ) : QString(), QDateTime::currentDateTime().toString( QStringLiteral( "yyyyMMddHHmmss" ) ) );
  }

  mCollectionPaths.insert( collection->uuid(), geoJsonPath );
}

void QfMarkupManager::processCollectionItemsChanged()
{
  QfMarkupCollection *collection = qobject_cast<QfMarkupCollection *>( sender() );
  if ( !collection )
  {
    return;
  }

  emit collectionItemsChanged( collection->uuid() );

  if ( mCollectionPaths.contains( collection->uuid() ) )
  {
    const QString geoJsonPath = mCollectionPaths[collection->uuid()];
    if ( !collection->writeGeoJson( geoJsonPath ) )
    {
      qInfo() << QStringLiteral( "Error: Markup collection '%1' could not be written on disk" ).arg( collection->name() );
    }
  }
}
