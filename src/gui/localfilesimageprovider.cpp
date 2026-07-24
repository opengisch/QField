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

#include <QIcon>
#include <qgsgdalutils.h>

#include <gdal.h>

LocalFilesImageProvider::LocalFilesImageProvider()
  : QQuickImageProvider( Pixmap )
{
}

QImage LocalFilesImageProvider::requestImage( const QString &id, QSize *size, const QSize &requestedSize )
{
  static const QIcon defaultIcon( QStringLiteral( ":/themes/qfield/nodpi/ic_file_green_48dp.svg" ) );

  // the id is passed on as an encoded URL string which needs decoding
  QString path = QUrl::fromPercentEncoding( id.toUtf8() );
  if ( path.toLower().endsWith( QStringLiteral( ".zip" ) ) )
    path = QStringLiteral( "/vsizip/%1" ).arg( path );

  const gdal::dataset_unique_ptr dataset( GDALOpen( path.toLocal8Bit().data(), GA_ReadOnly ) );
  if ( !dataset )
    return defaultIcon.pixmap( QSize( requestedSize ) ).toImage();

  const int cols = GDALGetRasterXSize( dataset.get() );
  const int rows = GDALGetRasterYSize( dataset.get() );
  int bands = std::min( 4, GDALGetRasterCount( dataset.get() ) );
  if ( bands == 2 )
  {
    // For 2-band raster, go for a 1-band grayscale representation
    bands = 1;
  }

  const QSize outputSize( requestedSize.width(), rows * requestedSize.width() / cols );
  QImage image( outputSize, bands == 4 ? QImage::Format_RGBA8888 : bands == 3 ? QImage::Format_RGB888
                                                                              : QImage::Format_Grayscale8 );
  if ( image.isNull() )
    return defaultIcon.pixmap( QSize( requestedSize ) ).toImage();

  GByte *firstPixel = reinterpret_cast<GByte *>( image.bits() );
  for ( int i = 0; i < bands; i++ )
  {
    CPLErr err = GDALRasterIOEx( GDALGetRasterBand( dataset.get(), i + 1 ),
                                 GF_Read, 0, 0, cols, rows,
                                 firstPixel + ( i ),
                                 outputSize.width(), outputSize.height(),
                                 GDT_Byte, bands, image.bytesPerLine(), nullptr );
    if ( err != CE_None )
    {
      return defaultIcon.pixmap( QSize( requestedSize ) ).toImage();
    }
  }
  return image;
}
