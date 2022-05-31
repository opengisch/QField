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

#include <gdal.h>

LocalFilesImageProvider::LocalFilesImageProvider()
  : QQuickImageProvider( Pixmap )
{
}

QPixmap LocalFilesImageProvider::requestPixmap( const QString &id, QSize *size, const QSize &requestedSize )
{
  // the id is passed on as an encoded URL string which needs decoding
  const QString path = QUrl::fromPercentEncoding( id.toUtf8() );

  GDALDatasetH dataset = GDALOpen( path.toLocal8Bit().data(), GA_ReadOnly );
  const int rows = GDALGetRasterYSize( dataset );
  const int cols = GDALGetRasterXSize( dataset );
  const int outputCols = requestedSize.width();
  const int outputRows = rows * requestedSize.width() / cols;

  int bands = std::min( 4, GDALGetRasterCount( dataset ) );
  if ( bands == 2 )
  {
    // For 2-band raster, go for a 1-band grayscale representation
    bands = 1;
  }

  std::vector<std::vector<uchar>> bandData( bands );
  for ( auto &data : bandData )
  {
    data.resize( size_t( outputRows * outputCols ) );
  }

  for ( int i = 1; i <= bands; ++i )
  {
    ( void ) GDALRasterIO( GDALGetRasterBand( dataset, i ),
                           GF_Read, 0, 0, cols, rows,
                           bandData[size_t( i - 1 )].data(),
                           outputCols, outputRows, GDT_Byte, 0, 0 );
  }
  GDALClose( dataset );

  std::vector<uchar> outputImage( size_t( bands * outputRows * outputCols ) );
  for ( size_t i = 0, j = 0; i < outputImage.size(); i += bands, j += 1 )
  {
    for ( int k = 0; k < bands; k++ )
    {
      outputImage[i + k] = bandData[k][j];
    }
  }

  QImage image( outputImage.data(), outputCols, outputRows, bands == 4 ? QImage::Format_RGBA8888 : bands == 3 ? QImage::Format_RGB888
                                                                                                              : QImage::Format_Grayscale8 );
  return QPixmap::fromImage( image );
}
