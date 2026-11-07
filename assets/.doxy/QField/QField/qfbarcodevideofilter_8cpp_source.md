

# File qfbarcodevideofilter.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfbarcodevideofilter.cpp**](qfbarcodevideofilter_8cpp.md)

[Go to the documentation of this file](qfbarcodevideofilter_8cpp.md)


```C++
/***************************************************************************
 qfbarcodevideofilter.cpp - QfBarcodeVideoFilter

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

#include "qfbarcodevideofilter.h"

class QfBarcodeVideoFilterRunnable : public QVideoFilterRunnable
{
  public:
    explicit QfBarcodeVideoFilterRunnable( QfBarcodeVideoFilter *filter = nullptr )
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
    QfBarcodeVideoFilter *mFilter;
};

QfBarcodeVideoFilter::QfBarcodeVideoFilter()
{
}

void QfBarcodeVideoFilter::setDecoder( QfBarcodeDecoder *decoder )
{
  if ( mDecoder == decoder )
  {
    return;
  }

  mDecoder = decoder;

  emit decoderChanged();
}

void QfBarcodeVideoFilter::decodeVideoFrame( QVideoFrame *input )
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

  mDecodingThread = new QfBarcodeDecoderThread( mDecoder, image );
  connect( mDecodingThread, &QThread::finished, this, [=] {
    mDecodingThread->deleteLater();
    mDecodingThread = nullptr;
  } );
  mDecodingThread->start();
}

QVideoFilterRunnable *QfBarcodeVideoFilter::createFilterRunnable()
{
  return new QfBarcodeVideoFilterRunnable( this );
}
```


