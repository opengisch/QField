/***************************************************************************
  iospicturesource.h - IosPictureSource

 ---------------------
 begin                : 5.7.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef IOSPICTURESOURCE_H
#define IOSPICTURESOURCE_H

#include "picturesource.h"


class IosPictureSource : public PictureSource
{
    Q_OBJECT

public:
    explicit IosPictureSource( QObject *parent = nullptr, const QString &prefix = QString(), const QString &pictureFilePath = QString() );

    QString picturePath() {
        return mPicturePath;
    }

signals:
    void pictureReceived( const QString &path );

public slots:
    void takePicture();

private:
    QQuickItem *mParent = nullptr;
    QString mPicturePath;
    class CameraDelegateContainer;
    CameraDelegateContainer *mDelegate = nullptr;
};


#endif // IOSPICTURESOURCE_H
