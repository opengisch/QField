

# File qgsgpkgflusher.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgsgpkgflusher.cpp**](qgsgpkgflusher_8cpp.md)

[Go to the documentation of this file](qgsgpkgflusher_8cpp.md)


```C++
/***************************************************************************
                          qgsgpkgflusher.cpp
                             -------------------
  begin                : Oct 2019
  copyright            : (C) 2019 by Matthias Kuhn
  email                : matthias@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "qgsgpkgflusher.h"

#include <QObject>
#include <QRegularExpression>
#include <QTimer>
#include <qgsmessagelog.h>
#include <qgsproject.h>
#include <qgsprovidermetadata.h>
#include <qgsproviderregistry.h>
#include <qgsvectorlayer.h>

#include <sqlite3.h>

class Flusher : public QObject
{
    Q_OBJECT

  public slots:
    void scheduleFlush( const QString &filename );

    void flush( const QString &filename );

    void stop( const QString &fileName );

    void start( const QString &fileName );

    bool isStopped( const QString &fileName ) const;

  private:
    QMutex mMutex;
    QMap<QString, QTimer *> mScheduledFlushes;
    QMap<QString, bool> mStoppedFlushes;
};

QgsGpkgFlusher::QgsGpkgFlusher( QgsProject *project )
  : QObject()
{
  connect( project, &QgsProject::layersAdded, this, &QgsGpkgFlusher::onLayersAdded );
  mFlusher = new Flusher();
  mFlusher->moveToThread( &mFlusherThread );
  connect( this, &QgsGpkgFlusher::requestFlush, mFlusher, &Flusher::scheduleFlush );
  mFlusherThread.start();
}

QgsGpkgFlusher::~QgsGpkgFlusher()
{
  mFlusherThread.quit();
  mFlusherThread.wait();
}

void QgsGpkgFlusher::onLayersAdded( const QList<QgsMapLayer *> &layers )
{
  for ( QgsMapLayer *layer : layers )
  {
    QgsVectorLayer *vl = dynamic_cast<QgsVectorLayer *>( layer );
    if ( vl && vl->dataProvider() )
    {
      QString dataSourceUri = vl->dataProvider()->dataSourceUri();
      QgsProviderMetadata *metadata = QgsProviderRegistry::instance()->providerMetadata( vl->dataProvider()->name() );
      const QString filePath = metadata->decodeUri( dataSourceUri ).value( QStringLiteral( "path" ) ).toString();
      if ( !filePath.endsWith( QStringLiteral( ".gpkg" ), Qt::CaseInsensitive ) && !filePath.endsWith( QStringLiteral( ".sqlite" ), Qt::CaseInsensitive ) )
      {
        continue;
      }

      const QFileInfo fi( filePath );
      if ( fi.isFile() )
      {
        connect( vl, &QgsVectorLayer::editingStopped, [this, filePath]() { emit requestFlush( filePath ); } );
      }
    }
  }
}

void QgsGpkgFlusher::stop( const QString &fileName )
{
  mFlusher->stop( fileName );
}

void QgsGpkgFlusher::start( const QString &fileName )
{
  mFlusher->start( fileName );
}

bool QgsGpkgFlusher::isStopped( const QString &fileName ) const
{
  return mFlusher->isStopped( fileName );
}

void Flusher::scheduleFlush( const QString &filename )
{
  if ( mStoppedFlushes.value( filename, false ) )
    return;

  if ( mScheduledFlushes.contains( filename ) )
  {
    mScheduledFlushes.value( filename )->start( 500 );
  }
  else
  {
    QTimer *timer = new QTimer();
    connect( timer, &QTimer::timeout, this, [this, filename]() { flush( filename ); } );
    timer->setSingleShot( true );
    mScheduledFlushes.insert( filename, timer );
    timer->start( 500 );
  }
}

void Flusher::flush( const QString &filename )
{
  if ( mStoppedFlushes.value( filename, false ) )
    return;

  QMutexLocker locker( &mMutex );

  sqlite3_database_unique_ptr db;
  int status = db.open_v2( filename, SQLITE_OPEN_READWRITE, nullptr );
  if ( status != SQLITE_OK )
  {
    QgsMessageLog::logMessage( QObject::tr( "There was an error opening the database <b>%1</b>: %2" ).arg( filename, db.errorMessage() ) );
    if ( !QFileInfo::exists( filename ) )
      return;
  }

  QString error;
  db.exec( "PRAGMA wal_checkpoint;", error );

  if ( error.isEmpty() )
  {
    delete mScheduledFlushes[filename];
    mScheduledFlushes.remove( filename );
  }
  else
  {
    QgsMessageLog::logMessage( QObject::tr( "Could not flush database %1 (%3) " ).arg( filename, error ) );
    mScheduledFlushes[filename]->start( 500 );
  }

  locker.unlock();
}

void Flusher::stop( const QString &fileName )
{
  if ( !mScheduledFlushes.contains( fileName ) )
    return;

  mScheduledFlushes.value( fileName )->stop();
  mScheduledFlushes.remove( fileName );

  flush( fileName );

  mStoppedFlushes.insert( fileName, true );
}

void Flusher::start( const QString &fileName )
{
  mStoppedFlushes.remove( fileName );
}

bool Flusher::isStopped( const QString &fileName ) const
{
  return mStoppedFlushes.value( fileName, false );
}

#include "qgsgpkgflusher.moc"
```


