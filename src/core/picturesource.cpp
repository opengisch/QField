/***************************************************************************
  picturesource.cpp - PictureSource

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

#include "picturesource.h"

#include <QTimer>

PictureSource::PictureSource( QObject *parent, const QString &prefix, const QString &pictureFilePath )
  : QObject( parent )
  , mPrefix( prefix )
  , mPictureFilePath( pictureFilePath )
{
  // prevent emit signal if the pictureFilePath is empty ( e.g. when AndroidPictureSource )
  if ( mPictureFilePath.isEmpty() )
    return;

  if ( mPictureFilePath.startsWith( mPrefix ) )
    mPictureFilePath = mPictureFilePath.remove( mPrefix );

  QTimer::singleShot( 0, this, [=]() {
    emit pictureReceived( mPictureFilePath );
  } );
}

PictureSource::~PictureSource()
{
}
