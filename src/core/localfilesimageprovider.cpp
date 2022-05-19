/***************************************************************************
  localfilesimageprovider.cpp - LocalFilesImageProvider

 ---------------------
 begin                : 18.05.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "localfilesimageprovider.h"

LocalFilesImageProvider::LocalFilesImageProvider()
  : QQuickImageProvider( Pixmap )
{
}

QPixmap LocalFilesImageProvider::requestPixmap( const QString &id, QSize *size, const QSize &requestedSize )
{
  // the id is passed on as an encoded URL string which needs decoding
  const QString path = QUrl::fromPercentEncoding( id.toUtf8() );
  QImage image( path );
  image = image.scaledToWidth( requestedSize.width() );
  size->setWidth( image.width() );
  size->setHeight( image.height() );

  return QPixmap::fromImage( image );
}
