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

    Q_PROPERTY( QString decodedString READ decodedString NOTIFY decodedStringChanged )

  public:
    explicit BarcodeDecoder( QObject *parent = nullptr );

    /**
     * Returns the last barcode decoded string.
     */
    QString decodedString() const { return mDecodedString; }

    /**
     * Clears the decoded string value and sets it to an empty string.
     */
    Q_INVOKABLE void clearDecodedString();

    /**
     * Scans a provided \a image for barcodes and if present sets the decoded string value.
     */
    void decodeImage( const QImage &image );

  signals:
    void decodedStringChanged();

  private:
    QImage mImage;
    QString mDecodedString;
};

#endif // BARCODEDECODER_H
