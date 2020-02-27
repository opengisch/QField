#include "fileutils.h"
#include <QMimeDatabase>
#include <QDebug>
#include <QFileInfo>
#include <QFileIconProvider>

FileUtils::FileUtils( QObject *parent )
  : QObject( parent )
{

}

QString FileUtils::mimeTypeName( const QString filePath )
{
  QMimeDatabase db;
  QMimeType mimeType = db.mimeTypeForFile( filePath );
  return mimeType.name();
}
