/***************************************************************************
  barcodeimageprovider.h - BarcodeImageProvider

 ---------------------
 begin                : 26.04.2025
 copyright            : (C) 2025 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef BARCODEIMAGEPROVIDER_H
#define BARCODEIMAGEPROVIDER_H

#include <QQuickImageProvider>

#define DEFAULT_BARCODE_SIZE 255

/**
 * \ingroup core
 */
class BarcodeImageProvider : public QQuickImageProvider
{
  public:
    explicit BarcodeImageProvider();

    QQmlImageProviderBase::ImageType imageType() const override { return QQmlImageProviderBase::Image; }
    QImage requestImage( const QString &id, QSize *size, const QSize &requestedSize ) override;
};

#endif // BARCODEIMAGEPROVIDER_H
