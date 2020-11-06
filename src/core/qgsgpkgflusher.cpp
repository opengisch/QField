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
#include <qgsmessagelog.h>
#include <qgsvectorlayer.h>

#include <qgsproject.h>
#include <sqlite3.h>
#include <QObject>

class Flusher : public QObject
{
    Q_OBJECT

  public slots:
    /**
     * Schedules a new flush for the given \a filename after 500ms.
     * If a new flush is scheduled for the same file before the actual flush is performed, the timer is reset to wait another 500ms.
     */
    void scheduleFlush( const QString &filename );

    /**
     * Flushes the contents of the given \a filename.
     */
    void flush( const QString &filename );

    /**
     * Immediately performs a flush for a given \a fileName and returns. If the flusher is stopped, flush for that \a fileName would be ignored.
     */
    void stop( const QString &fileName );

    /**
     * Reenables scheduling flushes for a given \a fileName.
     */
    void start( const QString &fileName );

    /**
     * Returns whether the flusher is stopped for a given \a fileName.
     */
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

      QString filePath;
      if ( dataSourceUri.contains( QStringLiteral( ".sqlite" ), Qt::CaseInsensitive ) )
      {
        //sqlite source
        QRegularExpression re( ".*dbname='(?<filepath>[^']*).*" );
        QRegularExpressionMatch match = re.match( dataSourceUri );
        if ( match.hasMatch() )
          filePath = match.captured( QStringLiteral( "filepath" ) );
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
  if ( ! mScheduledFlushes.contains( fileName ) )
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
