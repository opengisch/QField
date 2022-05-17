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
#ifndef IOSPICTURESOURCE_H
#define IOSPICTURESOURCE_H

#include "picturesource.h"


class IosPictureSource : public PictureSource
{
    Q_OBJECT

  public:
    explicit IosPictureSource( QObject *parent = nullptr, const QString &prefix = QString(), const QString &pictureFilePath = QString() );

    QString pictureFilePath() const { return mPictureFilePath; }
    QString prefixPath() const { return mPrefixPath; }

  signals:
    void pictureReceived( const QString &path );

  public slots:
    void takePicture();
    void pickGalleryPicture();

  private:
    QQuickItem *mParent = nullptr;
    QString mPrefixPath;
    QString mPictureFilePath;
    class CameraDelegateContainer;
    CameraDelegateContainer *mDelegate = nullptr;
};


#endif // IOSPICTURESOURCE_H
