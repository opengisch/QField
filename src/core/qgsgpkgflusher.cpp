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
#include <qgsmessagelog.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

#include <sqlite3.h>

class Flusher : public QObject
{
  Q_OBJECT

public slots:
  void scheduleFlush( const QString &filename );

  void flush( const QString &filename );

private:
  QMap<QString, QTimer *> mScheduledFlushes;
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

void QgsGpkgFlusher::onLayersAdded( const QList<QgsMapLayer *> layers )
{
  for ( QgsMapLayer *layer : layers )
  {
    QgsVectorLayer *vl = dynamic_cast<QgsVectorLayer *>( layer );
    if ( vl && vl->dataProvider() )
    {
      QString dataSourceUri = vl->dataProvider()->dataSourceUri();

      QString filePath;
      if ( dataSourceUri.contains( QStringLiteral( ".sqlite" ), Qt::CaseInsensitive ) )
      {
        //sqlite source
        QRegExp rx( ".*dbname='([^']*).*" );
        rx.indexIn( dataSourceUri );
        filePath = rx.capturedTexts()[1];
      }
      else if ( dataSourceUri.contains( QStringLiteral( ".gpkg" ), Qt::CaseInsensitive ) )
      {
        //gpkg source
        filePath = dataSourceUri.left( dataSourceUri.indexOf( '|' ) );
      }
      else
      {
        //other source (e.g. postgres or shape)
        continue;
      }
      QFileInfo fi( filePath );
      if ( fi.isFile() )
      {
        connect( vl, &QgsVectorLayer::editingStopped, [this, filePath]() { emit requestFlush( filePath ); } );
      }
    }
  }
}

void Flusher::scheduleFlush( const QString &filename )
{
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
  sqlite3_database_unique_ptr db;
  int status = db.open_v2( filename, SQLITE_OPEN_READWRITE, nullptr );
  if ( status != SQLITE_OK )
  {
    QgsMessageLog::logMessage( QObject::tr( "There was an error opening the database <b>%1</b>: %2" ).arg( filename, db.errorMessage() ) );
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
}

#include "qgsgpkgflusher.moc"
