

# File qfbarcodeimageprovider.h

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfbarcodeimageprovider.h**](qfbarcodeimageprovider_8h.md)

[Go to the documentation of this file](qfbarcodeimageprovider_8h.md)


```C++
/***************************************************************************
  qfbarcodeimageprovider.h - QfBarcodeImageProvider

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
#ifndef QFBARCODEIMAGEPROVIDER_H
#define QFBARCODEIMAGEPROVIDER_H

#include <QQuickImageProvider>

#define DEFAULT_BARCODE_SIZE 255

class QfBarcodeImageProvider : public QQuickImageProvider
{
  public:
    explicit QfBarcodeImageProvider();

    QQmlImageProviderBase::ImageType imageType() const override { return QQmlImageProviderBase::Image; }
    QImage requestImage( const QString &id, QSize *size, const QSize &requestedSize ) override;
};

#endif // QFBARCODEIMAGEPROVIDER_H
```


