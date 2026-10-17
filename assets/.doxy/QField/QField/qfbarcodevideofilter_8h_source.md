

# File qfbarcodevideofilter.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfbarcodevideofilter.h**](qfbarcodevideofilter_8h.md)

[Go to the documentation of this file](qfbarcodevideofilter_8h.md)


```C++
/***************************************************************************
 qfbarcodevideofilter.h - QfBarcodeVideoFilter

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

#ifndef QFBARCODEVIDEOFILTER_H
#define QFBARCODEVIDEOFILTER_H

#include "qfbarcodedecoder.h"

#include <QAbstractVideoFilter>
#include <QObject>
#include <QThread>

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

class QfBarcodeVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

    Q_PROPERTY( QfBarcodeDecoder *decoder READ decoder WRITE setDecoder NOTIFY decoderChanged );

  public:
    explicit QfBarcodeVideoFilter();

    QfBarcodeDecoder *decoder() const { return mDecoder; }

    void setDecoder( QfBarcodeDecoder *decoder );

    void decodeVideoFrame( QVideoFrame *input );

    bool isDecoding() const { return mDecodingThread; }

    QVideoFilterRunnable *createFilterRunnable() override;

  signals:
    void decoderChanged();

  private:
    QfBarcodeDecoder *mDecoder = nullptr;
    QThread *mDecodingThread = nullptr;
};

#endif // QFBARCODEVIDEOFILTER_H
```


