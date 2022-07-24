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

class BarcodeVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

    Q_PROPERTY( BarcodeDecoder *decoder READ decoder WRITE setDecoder NOTIFY decoderChanged );

  public:
    BarcodeVideoFilter();

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
