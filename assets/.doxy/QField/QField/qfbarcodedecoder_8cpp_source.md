

# File qfbarcodedecoder.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfbarcodedecoder.cpp**](qfbarcodedecoder_8cpp.md)

[Go to the documentation of this file](qfbarcodedecoder_8cpp.md)


```C++
/***************************************************************************
 qfbarcodedecoder.cpp - QfBarcodeDecoder

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

#include "qfbarcodedecoder.h"

#include <QImageReader>
#include <QVideoFrame>

#include <ZXing/ReadBarcode.h>

class QfBarcodeDecoderThread : public QThread
{
  public:
    explicit QfBarcodeDecoderThread( QfBarcodeDecoder *decoder, const QImage &image )
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

    QfBarcodeDecoder *mDecoder = nullptr;
    QImage mImage;
};

QfBarcodeDecoder::QfBarcodeDecoder( QObject *parent )
  : QObject( parent )
{
}

void QfBarcodeDecoder::clearDecodedString()
{
  if ( mDecodedString.isEmpty() )
  {
    return;
  }

  mDecodedString.clear();

  emit decodedStringChanged();
}

bool QfBarcodeDecoder::decodeImage( const QImage &image )
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
    options.setMinLineCount( 5 );
    options.setMaxNumberOfSymbols( 1 );

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
      ZXing::Position position = result.position();
      const int width = std::abs( position.bottomRight().x - position.bottomLeft().x );
      const int height = std::abs( position.bottomLeft().y - position.topLeft().y );
      if ( width > 10 && height > 10 )
      {
        mDecodedString = resultText;
        emit decodedStringChanged();
        return true;
      }
    }
  }

  return false;
}

bool QfBarcodeDecoder::decodeImageFile( const QString &path )
{
  if ( mDecodingThread )
  {
    mDecodingThread->wait();
  }

  QImageReader reader( path );
  reader.setDecideFormatFromContent( true );

  QImage image = reader.read();
  if ( image.isNull() )
  {
    return false;
  }

  if ( image.format() != QImage::Format_ARGB32 )
  {
    image = image.convertToFormat( QImage::Format_ARGB32 );
  }

  clearDecodedString();

  return decodeImage( image );
}

QVideoSink *QfBarcodeDecoder::videoSink() const
{
  return mVideoSink.get();
}

void QfBarcodeDecoder::setVideoSink( QVideoSink *sink )
{
  if ( mVideoSink == sink )
    return;

  if ( mVideoSink )
    disconnect( mVideoSink );

  mVideoSink = sink;
  connect( mVideoSink, &QVideoSink::videoFrameChanged, this, &QfBarcodeDecoder::decodeVideoFrame );

  emit videoSinkChanged();
}

void QfBarcodeDecoder::decodeVideoFrame( const QVideoFrame &frame )
{
  if ( mDecodingThread || !frame.isValid() )
    return;

  QImage image = frame.toImage();
  if ( !image.isNull() && image.format() != QImage::Format_ARGB32 )
  {
    image = image.convertToFormat( QImage::Format_ARGB32 );
  }

  mDecodingThread = new QfBarcodeDecoderThread( this, image );
  connect( mDecodingThread, &QThread::finished, this, [this] {
    mDecodingThread->deleteLater();
    mDecodingThread = nullptr;
  } );
  mDecodingThread->start();
}
```


