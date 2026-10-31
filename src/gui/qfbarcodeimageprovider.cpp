/***************************************************************************
  qfbarcodeimageprovider.cpp - QfBarcodeImageProvider

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

#include "qfbarcodeimageprovider.h"

#include <QImageReader>
#include <QPainter>
#include <QUrlQuery>
#include <qgssymbollayerutils.h>

#include <ZXing/BarcodeFormat.h>
#include <ZXing/BitMatrix.h>
#include <ZXing/MultiFormatWriter.h>
#include <cmath>

namespace
{
  constexpr double LOGO_RATIO = 0.24;
} // namespace

QfBarcodeImageProvider::QfBarcodeImageProvider()
  : QQuickImageProvider( Image )
{
}

QImage QfBarcodeImageProvider::requestImage( const QString &id, QSize *size, const QSize &requestedSize )
{
  // the id is passed on as an encoded URL string which needs decoding
  const QUrlQuery urlQuery = QUrlQuery( QUrl( id.toUtf8() ) );
  const QString text = urlQuery.queryItemValue( QStringLiteral( "text" ), QUrl::FullyDecoded );
  const QColor foregroundColor = QgsSymbolLayerUtils::decodeColor( urlQuery.queryItemValue( QStringLiteral( "color" ) ) );

  if ( text.isEmpty() )
  {
    return QImage();
  }

  const int width = requestedSize.width() > 0 ? requestedSize.width() : DEFAULT_BARCODE_SIZE;
  const int height = requestedSize.height() > 0 ? requestedSize.height() : DEFAULT_BARCODE_SIZE;
  const QColor color = foregroundColor.isValid() ? foregroundColor : QColor( Qt::black );

  ZXing::MultiFormatWriter writer( ZXing::BarcodeFormat::QRCode );
  writer.setEccLevel( 8 );
  writer.setMargin( 0 );
  const ZXing::BitMatrix matrix = writer.encode( text.toStdString(), 0, 0 );
  const int matrixWidth = matrix.width();

  int logoModuleCount = static_cast<int>( std::round( matrixWidth * LOGO_RATIO ) );
  if ( logoModuleCount % 2 == 0 )
  {
    logoModuleCount++;
  }
  const QRect logoModules( ( matrixWidth - logoModuleCount ) / 2, ( matrixWidth - logoModuleCount ) / 2, logoModuleCount, logoModuleCount );

  const QRgb pixelColor = color.rgba();
  QImage modules( matrixWidth, matrixWidth, QImage::Format_ARGB32 );
  modules.fill( Qt::transparent );
  for ( int y = 0; y < matrixWidth; ++y )
  {
    QRgb *pixel = reinterpret_cast<QRgb *>( modules.scanLine( y ) );
    for ( int x = 0; x < matrixWidth; ++x, ++pixel )
    {
      if ( matrix.get( x, y ) && !logoModules.contains( x, y ) )
      {
        *pixel = pixelColor;
      }
    }
  }

  QImage barcodeImage = modules.scaled( width, height, Qt::KeepAspectRatio, Qt::FastTransformation );

  const qreal matrixCellWidth = barcodeImage.width() / static_cast<qreal>( matrixWidth );
  const qreal logoPadding = 0.75 * matrixCellWidth;
  const QRectF logoRect = QRectF( QPointF( logoModules.topLeft() ) * matrixCellWidth, QSizeF( logoModules.size() ) * matrixCellWidth ).adjusted( logoPadding, logoPadding, -logoPadding, -logoPadding );
  QImageReader logoReader( QStringLiteral( ":/images/qfield_logo.svg" ) );
  logoReader.setScaledSize( logoRect.size().toSize() );
  QImage logo = logoReader.read();
  if ( !logo.isNull() )
  {
    logo = logo.convertToFormat( QImage::Format_ARGB32_Premultiplied );
    QPainter logoPainter( &logo );
    logoPainter.setCompositionMode( QPainter::CompositionMode_SourceIn );
    logoPainter.fillRect( logo.rect(), color );
    logoPainter.end();

    QPainter painter( &barcodeImage );
    painter.drawImage( logoRect.topLeft(), logo );
  }

  return barcodeImage;
}
