#include "qgsgpkgflusher.h"
#include "qgsmessagelog.h"
#include "qgsvectorlayer.h"

#include <qgsproject.h>
#include <sqlite3.h>
#include <QObject>


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
      QString filePath = dataSourceUri.left( dataSourceUri.indexOf( '|' ) );
      QFileInfo fi( filePath );
      if ( fi.isFile() )
      {
        connect( vl, &QgsVectorLayer::committedFeaturesAdded,
        [this, filePath]() { emit requestFlush( filePath ); } );
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
