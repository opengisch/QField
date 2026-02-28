

# File iosresourcesource.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**platforms**](dir_4ed093858358730befc336a35ce794a4.md) **>** [**ios**](dir_493ad750a5d458d3fc370128c0d017ff.md) **>** [**iosresourcesource.h**](iosresourcesource_8h.md)

[Go to the documentation of this file](iosresourcesource_8h.md)


```C++
/***************************************************************************
  iospicturesource.h - IosPictureSource

  begin                : September 2021
  copyright            : (C) 2020 by Denis Rouzaud
  email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef IOSRESOURCESOURCE_H
#define IOSRESOURCESOURCE_H

#include "resourcesource.h"


class IosResourceSource : public ResourceSource
{
    Q_OBJECT

  public:
    explicit IosResourceSource( const QString &prefix = QString(), const QString &resourceFilePath = QString(), QObject *parent = nullptr );

    QString resourceFilePath() const { return mResourceFilePath; }
    QString prefixPath() const { return mPrefixPath; }

  signals:
    void resourceReceived( const QString &path );

  public slots:
    void takePicture();
    void takeVideo();
    void pickGalleryPicture();
    void pickGalleryVideo();

  private:
    QQuickItem *mParent = nullptr;
    QString mPrefixPath;
    QString mResourceFilePath;
    class CameraDelegateContainer;
    CameraDelegateContainer *mDelegate = nullptr;
};


#endif // IOSRESOURCESOURCE_H
```


