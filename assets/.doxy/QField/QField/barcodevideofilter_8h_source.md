

# File barcodevideofilter.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**barcodevideofilter.h**](barcodevideofilter_8h.md)

[Go to the documentation of this file](barcodevideofilter_8h.md)


```C++
/***************************************************************************
 barcodevideofilter.h - BarcodeVideoFilter

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

#ifndef BARCODEVIDEOFILTER_H
#define BARCODEVIDEOFILTER_H

#include "barcodedecoder.h"

#include <QAbstractVideoFilter>
#include <QObject>
#include <QThread>

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

class BarcodeVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

    Q_PROPERTY( BarcodeDecoder *decoder READ decoder WRITE setDecoder NOTIFY decoderChanged );

  public:
    explicit BarcodeVideoFilter();

    BarcodeDecoder *decoder() const { return mDecoder; }

    void setDecoder( BarcodeDecoder *decoder );

    void decodeVideoFrame( QVideoFrame *input );

    bool isDecoding() const { return mDecodingThread; }

    QVideoFilterRunnable *createFilterRunnable() override;

  signals:
    void decoderChanged();

  private:
    BarcodeDecoder *mDecoder = nullptr;
    QThread *mDecodingThread = nullptr;
};

#endif // BARCODEVIDEOFILTER_H
```


