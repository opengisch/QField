

# File qflocalfilesimageprovider.h

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qflocalfilesimageprovider.h**](qflocalfilesimageprovider_8h.md)

[Go to the documentation of this file](qflocalfilesimageprovider_8h.md)


```C++
/***************************************************************************
  qflocalfilesimageprovider.h - QfLocalFilesImageProvider

 ---------------------
 begin                : 18.05.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFLOCALFILESIMAGEPROVIDER_H
#define QFLOCALFILESIMAGEPROVIDER_H

#include <QQuickImageProvider>

class QfLocalFilesImageProvider : public QQuickImageProvider
{
  public:
    explicit QfLocalFilesImageProvider();

    QQmlImageProviderBase::ImageType imageType() const override { return QQmlImageProviderBase::Image; }
    QImage requestImage( const QString &id, QSize *size, const QSize &requestedSize ) override;
};

#endif // QFLOCALFILESIMAGEPROVIDER_H
```


