

# File localfilesimageprovider.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**localfilesimageprovider.h**](localfilesimageprovider_8h.md)

[Go to the documentation of this file](localfilesimageprovider_8h.md)


```C++
/***************************************************************************
  localfilesimageprovider.h - LocalFilesImageProvider

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
#ifndef LOCALFILESIMAGEPROVIDER_H
#define LOCALFILESIMAGEPROVIDER_H

#include <QQuickImageProvider>

class LocalFilesImageProvider : public QQuickImageProvider
{
  public:
    explicit LocalFilesImageProvider();

    QQmlImageProviderBase::ImageType imageType() const override { return QQmlImageProviderBase::Image; }
    QImage requestImage( const QString &id, QSize *size, const QSize &requestedSize ) override;
};

#endif // LOCALFILESIMAGEPROVIDER_H
```


