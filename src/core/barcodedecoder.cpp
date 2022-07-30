/***************************************************************************
 barcodedecoder.cpp - BarcodeDecoder

 ---------------------
 begin                : 22.07.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "barcodedecoder.h"

#include <ZXing/ReadBarcode.h>

BarcodeDecoder::BarcodeDecoder( QObject *parent )
  : QObject( parent )
{
}

void BarcodeDecoder::clearDecodedString()
{
  if ( mDecodedString.isEmpty() )
  {
    return;
  }

  mDecodedString.clear();

  emit decodedStringChanged();
}

void BarcodeDecoder::decodeImage( const QImage &image )
{
  auto imageFormatFromQImage = []( const QImage &img ) {
    switch ( img.format() )
    {
      case QImage::Format_ARGB32:
      case QImage::Format_RGB32:
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        return ZXing::ImageFormat::BGRX;
#else
        return ImageFormat::XRGB;
#endif
      case QImage::Format_RGB888:
        return ZXing::ImageFormat::RGB;
      case QImage::Format_RGBX8888:
      case QImage::Format_RGBA8888:
        return ZXing::ImageFormat::RGBX;
      case QImage::Format_Grayscale8:
        return ZXing::ImageFormat::Lum;
      default:
        return ZXing::ImageFormat::None;
    }
  };

  ZXing::ImageFormat imageFormat = imageFormatFromQImage( image );
  if ( imageFormat != ZXing::ImageFormat::None )
  {
    ZXing::ImageView imageView( image.bits(), image.width(), image.height(), imageFormat, static_cast<int>( image.bytesPerLine() ) );

    ZXing::DecodeHints hints;
    hints.setFormats( ZXing::BarcodeFormat::Any );
    hints.setTryRotate( true );

    ZXing::Result result = ZXing::ReadBarcode( imageView, hints );
    const std::wstring text = result.text();
    QString resultText = QString::fromWCharArray( text.c_str() );
    if ( !resultText.isEmpty() && mDecodedString != resultText )
    {
      mDecodedString = resultText;
      emit decodedStringChanged();
    }
  }

  return;
}
