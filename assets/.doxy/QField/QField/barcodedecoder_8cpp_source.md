

# File barcodedecoder.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**barcodedecoder.cpp**](barcodedecoder_8cpp.md)

[Go to the documentation of this file](barcodedecoder_8cpp.md)


```C++
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

#include <QVideoFrame>

#include <ZXing/ReadBarcode.h>

class BarcodeDecoderThread : public QThread
{
  public:
    explicit BarcodeDecoderThread( BarcodeDecoder *decoder, const QImage &image )
      : QThread()
      , mDecoder( decoder )
      , mImage( image )
    {
    }

  private:
    void run() override
    {
      if ( mDecoder )
      {
        mDecoder->decodeImage( mImage );
      }
    }

    BarcodeDecoder *mDecoder = nullptr;
    QImage mImage;
};

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
        return ZXing::ImageFormat::BGRA;
#else
        return ZXing::ImageFormat::ARGB;
#endif
      case QImage::Format_RGB888:
        return ZXing::ImageFormat::RGB;
      case QImage::Format_RGBX8888:
      case QImage::Format_RGBA8888:
        return ZXing::ImageFormat::RGBA;
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

#if ZXing_VERSION_MAJOR >= 2
    ZXing::ReaderOptions options;
    options.setFormats( ZXing::BarcodeFormat::Any );
    options.setTryRotate( true );

    ZXing::Result result = ZXing::ReadBarcode( imageView, options );
    const std::string text = result.text();
    QString resultText = QString::fromStdString( text.c_str() );
#else
    ZXing::DecodeHints hints;
    hints.setFormats( ZXing::BarcodeFormat::Any );
    hints.setTryRotate( true );

    ZXing::Result result = ZXing::ReadBarcode( imageView, hints );
    const std::wstring text = result.text();
    QString resultText = QString::fromWCharArray( text.c_str() );
#endif
    if ( !resultText.isEmpty() && mDecodedString != resultText )
    {
      mDecodedString = resultText;
      emit decodedStringChanged();
    }
  }

  return;
}

QVideoSink *BarcodeDecoder::videoSink() const
{
  return mVideoSink.get();
}

void BarcodeDecoder::setVideoSink( QVideoSink *sink )
{
  if ( mVideoSink == sink )
    return;

  if ( mVideoSink )
    disconnect( mVideoSink );

  mVideoSink = sink;
  connect( mVideoSink, &QVideoSink::videoFrameChanged, this, &BarcodeDecoder::decodeVideoFrame );

  emit videoSinkChanged();
}

void BarcodeDecoder::decodeVideoFrame( const QVideoFrame &frame )
{
  if ( mDecodingThread || !frame.isValid() )
    return;

  QImage image = frame.toImage();
  if ( !image.isNull() && image.format() != QImage::Format_ARGB32 )
  {
    image = image.convertToFormat( QImage::Format_ARGB32 );
  }

  mDecodingThread = new BarcodeDecoderThread( this, image );
  connect( mDecodingThread, &QThread::finished, this, [this] {
    mDecodingThread->deleteLater();
    mDecodingThread = nullptr;
  } );
  mDecodingThread->start();
}
```


