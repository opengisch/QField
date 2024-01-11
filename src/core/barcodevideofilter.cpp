/***************************************************************************
 barcodevideofilter.cpp - BarcodeVideoFilter

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

#include "barcodevideofilter.h"

class BarcodeVideoFilterRunnable : public QVideoFilterRunnable
{
  public:
    explicit BarcodeVideoFilterRunnable( BarcodeVideoFilter *filter = nullptr )
      : mFilter( filter )
    {
    }

    QVideoFrame run( QVideoFrame *input,
                     const QVideoSurfaceFormat &surfaceFormat,
                     QVideoFilterRunnable::RunFlags flags ) override
    {
      Q_UNUSED( surfaceFormat );
      Q_UNUSED( flags );

      if ( !mFilter->isDecoding() )
      {
        mFilter->decodeVideoFrame( input );
      }

      return *input;
    }

  private:
    BarcodeVideoFilter *mFilter;
};

BarcodeVideoFilter::BarcodeVideoFilter()
{
}

void BarcodeVideoFilter::setDecoder( BarcodeDecoder *decoder )
{
  if ( mDecoder == decoder )
  {
    return;
  }

  mDecoder = decoder;

  emit decoderChanged();
}

void BarcodeVideoFilter::decodeVideoFrame( QVideoFrame *input )
{
  if ( !mDecoder || isDecoding() )
  {
    return;
  }

  QImage image = input->image();
  if ( !image.isNull() && image.format() != QImage::Format_ARGB32 )
  {
    image = image.convertToFormat( QImage::Format_ARGB32 );
  }

  mDecodingThread = new BarcodeDecoderThread( mDecoder, image );
  connect( mDecodingThread, &QThread::finished, this, [=] {
    mDecodingThread->deleteLater();
    mDecodingThread = nullptr;
  } );
  mDecodingThread->start();
}

QVideoFilterRunnable *BarcodeVideoFilter::createFilterRunnable()
{
  return new BarcodeVideoFilterRunnable( this );
}
