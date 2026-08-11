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

/**
 * \ingroup core
 */
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

    /**
     * Returns the barcode decoder to be used when analyzing video frames.
     */
    QfBarcodeDecoder *decoder() const { return mDecoder; }

    /**
     * Sets the \a decoder to be used when analyzing video frames.
     */
    void setDecoder( QfBarcodeDecoder *decoder );

    /**
     * Handle the decoding of a video frame barcodes. The frame will be analyzed for the presence of
     * a barcode, and if so the decoder will capture the decoded string.
     */
    void decodeVideoFrame( QVideoFrame *input );

    /**
     * Returns whether a video frame decoding is happening, which will dictate whether incoming video frames
     * will be skipped or not.
     */
    bool isDecoding() const { return mDecodingThread; }

    QVideoFilterRunnable *createFilterRunnable() override;

  signals:
    void decoderChanged();

  private:
    QfBarcodeDecoder *mDecoder = nullptr;
    QThread *mDecodingThread = nullptr;
};

#endif // QFBARCODEVIDEOFILTER_H
