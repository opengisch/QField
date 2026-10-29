/***************************************************************************
 qfbarcodedecoder.h - QfBarcodeDecoder

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

#ifndef QFBARCODEDECODER_H
#define QFBARCODEDECODER_H

#include "qfbarcodemodel.h"

#include <QImage>
#include <QObject>
#include <QPointer>
#include <QThread>
#include <QVideoSink>

class QfBarcodeDecoderThread;

/**
 * \ingroup core
 */
class QfBarcodeDecoder : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QVideoSink *videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged )
    Q_PROPERTY( QString decodedString READ decodedString WRITE setDecodedString NOTIFY decodedStringChanged )
    Q_PROPERTY( QfBarcodeModel *barcodeModel READ barcodeModel CONSTANT )

  public:
    explicit QfBarcodeDecoder( QObject *parent = nullptr );

    /**
     * Returns the last barcode decoded string.
     */
    QString decodedString() const { return mDecodedString; }

    /**
     * Sets the decoded string, used to pick one of several barcodes seen in the video feed.
     * When a single barcode is in view, it is picked automatically.
     */
    void setDecodedString( const QString &decodedString );

    /**
     * Returns the barcodes seen in the latest video frames.
     */
    QfBarcodeModel *barcodeModel() const { return mBarcodeModel; }

    /**
     * Clears the decoded string value and sets it to an empty string.
     */
    Q_INVOKABLE void clearDecodedString();

    /**
     * Scans a provided \a image for barcodes and if present sets the decoded string value.
     * Returns TRUE when a barcode was decoded.
     */
    bool decodeImage( const QImage &image );

    /**
     * Scans an image found at a given \a path for barcodes and if present sets the decoded
     * string value. Returns TRUE when a barcode was decoded.
     */
    Q_INVOKABLE bool decodeImageFile( const QString &path );

    QVideoSink *videoSink() const;
    void setVideoSink( QVideoSink *sink );

  public slots:
    void decodeVideoFrame( const QVideoFrame &frame );

  signals:
    void decodedStringChanged();
    void videoSinkChanged();

  private:
    QString mDecodedString;
    QfBarcodeModel *mBarcodeModel = new QfBarcodeModel( this );
    QfBarcodeDecoderThread *mDecodingThread = nullptr;
    QPointer<QVideoSink> mVideoSink;
};

#endif // QFBARCODEDECODER_H
