/***************************************************************************
 barcodedecoder.h - BarcodeDecoder

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

#ifndef BARCODEDECODER_H
#define BARCODEDECODER_H

#include <QImage>
#include <QObject>
#include <QThread>

class BarcodeDecoder : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString decodedString READ decodedString WRITE setDecodedString NOTIFY decodedStringChanged )

  public:
    explicit BarcodeDecoder( QObject *parent = nullptr );

    QString decodedString() const { return mDecodedString; }
    void setDecodedString( const QString &decodedString );

    void decodeImage( const QImage &image );

  signals:
    void decodedStringChanged();

  private:
    QImage mImage;
    QString mDecodedString;
};

class BarcodeDecoderThread : public QThread
{
    Q_OBJECT

  public:
    explicit BarcodeDecoderThread( BarcodeDecoder *decoder, const QImage &image );

  private:
    void run() override;

    BarcodeDecoder *mDecoder = nullptr;
    QImage mImage;
};

#endif // BARCODEDECODER_H
