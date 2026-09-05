

# File qfprojectsimageprovider.h

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfprojectsimageprovider.h**](qfprojectsimageprovider_8h.md)

[Go to the documentation of this file](qfprojectsimageprovider_8h.md)


```C++
/***************************************************************************
  qfprojectsimageprovider.h - QfProjectsImageProvider

 ---------------------
 begin                : 15.07.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFPROJECTSIMAGEPROVIDER_H
#define QFPROJECTSIMAGEPROVIDER_H

#include <QQuickImageProvider>

class QfProjectsImageProvider : public QQuickImageProvider
{
  public:
    explicit QfProjectsImageProvider();

    QQmlImageProviderBase::ImageType imageType() const override { return QQmlImageProviderBase::Image; }
    QImage requestImage( const QString &id, QSize *size, const QSize &requestedSize ) override;
};

#endif // QFPROJECTSIMAGEPROVIDER_H
```


