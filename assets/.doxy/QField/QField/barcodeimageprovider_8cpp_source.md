

# File barcodeimageprovider.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**barcodeimageprovider.cpp**](barcodeimageprovider_8cpp.md)

[Go to the documentation of this file](barcodeimageprovider_8cpp.md)


```C++
/***************************************************************************
  barcodeimageprovider.cpp - BarcodeImageProvider

 ---------------------
 begin                : 26.04.2025
 copyright            : (C) 2025 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "barcodeimageprovider.h"

#include <QFileInfo>
#include <QUrlQuery>

#include <ZXing/BarcodeFormat.h>
#include <ZXing/BitMatrix.h>
#include <ZXing/MultiFormatWriter.h>

BarcodeImageProvider::BarcodeImageProvider()
  : QQuickImageProvider( Image )
{
}

QImage BarcodeImageProvider::requestImage( const QString &id, QSize *size, const QSize &requestedSize )
{
  // the id is passed on as an encoded URL string which needs decoding
  const QUrlQuery urlQuery = QUrlQuery( QUrl( id.toUtf8() ) );
  const QString text = urlQuery.queryItemValue( QStringLiteral( "text" ), QUrl::FullyDecoded );
  const QColor foregroundColor = QColor( urlQuery.queryItemValue( QStringLiteral( "color" ) ) );

  if ( text.isEmpty() )
  {
    return QImage();
  }

  const int width = requestedSize.width() > 0 ? requestedSize.width() : DEFAULT_BARCODE_SIZE;
  const int height = requestedSize.height() > 0 ? requestedSize.height() : DEFAULT_BARCODE_SIZE;

  ZXing::MultiFormatWriter writer( ZXing::BarcodeFormat::QRCode );
  ZXing::BitMatrix matrix = writer.encode( text.toStdString(), width, height );
  ZXing::Matrix<uint8_t> bitmap = ZXing::ToMatrix<uint8_t>( matrix );

  QImage barcodeImage( bitmap.data(), bitmap.width(), bitmap.height(), bitmap.width(), QImage::Format::Format_Grayscale8 );
  barcodeImage.convertTo( QImage::Format::Format_ARGB32 );

  const QRgb white = qRgba( 255, 255, 255, 255 );
  const QRgb black = qRgba( 0, 0, 0, 255 );

  const QRgb transparent = qRgba( 255, 255, 255, 0 );
  const QRgb color = foregroundColor.isValid() ? foregroundColor.rgba() : qRgba( 0, 0, 0, 255 );

  for ( int y = 0; y < barcodeImage.height(); ++y )
  {
    QRgb *pixel = reinterpret_cast<QRgb *>( barcodeImage.scanLine( y ) );
    for ( int x = 0; x < barcodeImage.width(); ++x, ++pixel )
    {
      if ( *pixel == white )
      {
        *pixel = transparent;
      }
      else if ( *pixel == black )
      {
        *pixel = color;
      }
    }
  }

  return barcodeImage.copy();
}
```


