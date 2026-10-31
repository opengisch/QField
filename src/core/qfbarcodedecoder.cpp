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
#include <QTransform>
#include <QVideoFrame>

#include <ZXing/ReadBarcode.h>
#include <algorithm>

namespace
{
  ZXing::ImageFormat zxingImageFormat( QImage::Format format )
  {
    switch ( format )
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
  }

  QList<QfVideoBarcodeModel::Barcode> readBarcodes( const QImage &image )
  {
    QList<QfVideoBarcodeModel::Barcode> barcodes;

    const ZXing::ImageFormat imageFormat = zxingImageFormat( image.format() );
    if ( imageFormat == ZXing::ImageFormat::None )
    {
      return barcodes;
    }

    auto addBarcode = [&image, &barcodes]( const QString &text, const ZXing::Position &position ) {
      const ZXing::Position box = ZXing::BoundingBox( position );
      const QRectF bounds( QPointF( box.topLeft().x, box.topLeft().y ), QPointF( box.bottomRight().x, box.bottomRight().y ) );
      if ( !text.isEmpty() && bounds.width() > 10 && bounds.height() > 10 )
      {
        barcodes << QfVideoBarcodeModel::Barcode( text, QTransform::fromScale( 1.0 / image.width(), 1.0 / image.height() ).mapRect( bounds ) );
      }
    };

    ZXing::ImageView imageView( image.bits(), image.width(), image.height(), imageFormat, static_cast<int>( image.bytesPerLine() ) );

#if ZXing_VERSION_MAJOR >= 2
    ZXing::ReaderOptions options;
    options.setFormats( ZXing::BarcodeFormat::Any );
    options.setTryRotate( true );
    options.setMinLineCount( 5 );
    options.setMaxNumberOfSymbols( 8 );

    for ( const ZXing::Barcode &result : ZXing::ReadBarcodes( imageView, options ) )
    {
      addBarcode( QString::fromStdString( result.text() ), result.position() );
    }
#else
    ZXing::DecodeHints hints;
    hints.setFormats( ZXing::BarcodeFormat::Any );
    hints.setTryRotate( true );

    const ZXing::Result result = ZXing::ReadBarcode( imageView, hints );
    addBarcode( QString::fromStdWString( result.text() ), result.position() );
#endif

    return barcodes;
  }
} // namespace

class QfBarcodeDecoderThread : public QThread
{
  public:
    explicit QfBarcodeDecoderThread( const QImage &image, const QTransform &displayTransform )
      : QThread()
      , mImage( image )
      , mDisplayTransform( displayTransform )
    {
    }

    QList<QfVideoBarcodeModel::Barcode> barcodes() const { return mBarcodes; }

  private:
    void run() override
    {
      mBarcodes = readBarcodes( mImage );
      for ( QfVideoBarcodeModel::Barcode &barcode : mBarcodes )
      {
        barcode.bounds = mDisplayTransform.mapRect( barcode.bounds );
      }
    }

    QImage mImage;
    QTransform mDisplayTransform;
    QList<QfVideoBarcodeModel::Barcode> mBarcodes;
};

QfBarcodeDecoder::QfBarcodeDecoder( QObject *parent )
  : QObject( parent )
{
}

void QfBarcodeDecoder::setDecodedString( const QString &decodedString )
{
  if ( mDecodedString == decodedString )
  {
    return;
  }

  mDecodedString = decodedString;

  emit decodedStringChanged();
}

void QfBarcodeDecoder::clearDecodedString()
{
  setDecodedString( QString() );
}

bool QfBarcodeDecoder::decodeImage( const QImage &image )
{
  const QList<QfVideoBarcodeModel::Barcode> barcodes = readBarcodes( image );
  if ( barcodes.isEmpty() )
  {
    return false;
  }

  const auto largestBarcode = std::max_element( barcodes.begin(), barcodes.end(), []( const QfVideoBarcodeModel::Barcode &a, const QfVideoBarcodeModel::Barcode &b ) -> bool {
    return a.bounds.width() * a.bounds.height() < b.bounds.width() * b.bounds.height();
  } );
  setDecodedString( largestBarcode->text );
  return true;
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

  QTransform displayTransform;
  displayTransform.translate( 0.5, 0.5 );
  if ( frame.mirrored() )
  {
    displayTransform.scale( -1, 1 );
  }
  displayTransform.rotate( static_cast<int>( frame.rotation() ) );
  displayTransform.translate( -0.5, -0.5 );

  mDecodingThread = new QfBarcodeDecoderThread( image, displayTransform );
  connect( mDecodingThread, &QThread::finished, this, [this] {
    const int previousBarcodeCount = mVideoBarcodes->rowCount();
    mVideoBarcodes->updateBarcodes( mDecodingThread->barcodes() );
    const int barcodeCount = mVideoBarcodes->rowCount();
    if ( barcodeCount == 1 )
    {
      setDecodedString( mVideoBarcodes->data( mVideoBarcodes->index( 0 ), QfVideoBarcodeModel::BarcodeText ).toString() );
    }
    else if ( barcodeCount > 1 && previousBarcodeCount <= 1 )
    {
      // more codes came into view, drop the automatic pick
      clearDecodedString();
    }

    mDecodingThread->deleteLater();
    mDecodingThread = nullptr;
  } );
  mDecodingThread->start();
}
