

# File qflocalfilesimageprovider.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qflocalfilesimageprovider.cpp**](qflocalfilesimageprovider_8cpp.md)

[Go to the documentation of this file](qflocalfilesimageprovider_8cpp.md)


```C++
/***************************************************************************
  qflocalfilesimageprovider.cpp - QfLocalFilesImageProvider

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

#include "qflocalfilesimageprovider.h"

#include <QIcon>
#include <qgsgdalutils.h>

#include <gdal.h>

QfLocalFilesImageProvider::QfLocalFilesImageProvider()
  : QQuickImageProvider( Pixmap )
{
}

QImage QfLocalFilesImageProvider::requestImage( const QString &id, QSize *size, const QSize &requestedSize )
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
  QImage image;
  bool hasColorTable = false;
  if ( bands == 4 )
  {
    image = QImage( outputSize, QImage::Format_RGBA8888 );
  }
  else if ( bands == 3 )
  {
    image = QImage( outputSize, QImage::Format_RGB888 );
  }
  else
  {
    if ( bands == 1 )
    {
      hasColorTable = GDALGetRasterColorTable( GDALGetRasterBand( dataset.get(), 1 ) );
    }

    if ( hasColorTable )
    {
      image = QImage( outputSize, QImage::Format_RGBA8888 );
    }
    else
    {
      image = QImage( outputSize, QImage::Format_Grayscale8 );
    }
  }

  if ( image.isNull() )
  {
    return defaultIcon.pixmap( QSize( requestedSize ) ).toImage();
  }

  if ( hasColorTable )
  {
    std::vector<GByte> rasterData( static_cast<unsigned long>( outputSize.width() ) * outputSize.height() );
    CPLErr err = GDALRasterIO( GDALGetRasterBand( dataset.get(), 1 ),
                               GF_Read, 0, 0, cols, rows,
                               rasterData.data(),
                               outputSize.width(), outputSize.height(),
                               GDT_Byte, 0, 0 );
    if ( err != CE_None )
    {
      return defaultIcon.pixmap( QSize( requestedSize ) ).toImage();
    }
    GDALColorTableH colorTable = GDALGetRasterColorTable( GDALGetRasterBand( dataset.get(), 1 ) );
    for ( int y = 0; y < outputSize.height(); ++y )
    {
      // cppcheck-suppress cstyleCast
      QRgb *lineScan = ( QRgb * ) image.scanLine( y );
      const int rowOffset = y * outputSize.width();
      for ( int x = 0; x < outputSize.width(); ++x )
      {
        GByte index = rasterData[rowOffset + x];
        const GDALColorEntry *entry = GDALGetColorEntry( colorTable, index );
        if ( entry )
        {
          lineScan[x] = qRgba( entry->c3, entry->c2, entry->c1, entry->c4 );
        }
        else
        {
          lineScan[x] = qRgba( 0, 0, 0, 255 );
        }
      }
    }
  }
  else
  {
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
  }
  return image;
}
```


