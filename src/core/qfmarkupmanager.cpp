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

void QfMarkupManager::reset( const QString &path, const QString &prefix )
{
  qDeleteAll( mCollections );
  mCollections.clear();

  const QFileInfo fi( path );
  mPath = fi.absoluteFilePath();
  mPrefix = prefix;

  QDirIterator collectionGeoJsonPaths( path, QStringList() << QStringLiteral( "%1*.geojson" ).arg( prefix ), QDir::Files );
  while ( collectionGeoJsonPaths.hasNext() )
  {
    QfMarkupCollection *collection = new QfMarkupCollection();
    if ( collection->readGeoJson( collectionGeoJsonPaths.filePath() ) )
    {
      if ( collection->name().isEmpty() )
      {
        collection->setName( tr( "Unnamed collecton" ) );
      }

      insertCollection( collection );
    }
  }

  if ( mCollections.isEmpty() )
  {
    // A default markup collection is added
    QfMarkupCollection *collection = new QfMarkupCollection( tr( "Default collection" ) );
    insertCollection( collection );
  }

  emit collectionsChanged();
}

void QfMarkupManager::insertCollection( QfMarkupCollection *collection )
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

  connect( collection, &QfMarkupCollection::itemsChanged, this, &QfMarkupManager::collectionItemsChanged );

  mCollections.insert( collection->name(), collection );
}

void QfMarkupManager::collectionItemsChanged()
{
  QfMarkupCollection *collection = qobject_cast<QfMarkupCollection *>( sender() );
  if ( !collection )
  {
    return;
  }

  QString filename = collection->name();
  if ( !mPrefix.isEmpty() )
  {
    filename.prepend( QStringLiteral( "%1-" ).arg( mPrefix ) );
  }
  filename = QfFileUtils::sanitizeFilePathPart( filename );

  collection->writeGeoJson( QStringLiteral( "%1%2%3.geojson" ).arg( mPath, QDir::separator(), filename ) );
}
