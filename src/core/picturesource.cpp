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

#include <QFileDialog>

PictureSource::PictureSource( QObject *parent, const QString &prefix, const QString &pictureFilePath )
  : QObject( parent )
  , mPrefix( prefix )
  , mPictureFilePath( pictureFilePath )
{

}

void PictureSource::init()
{
  QString path { QFileDialog::getOpenFileName( nullptr, tr( "Select Media File" ), mPrefix, tr( "JPEG images (*.jpg *.jpeg)" ) ) };

  if ( path.startsWith( mPrefix ) )
    path = path.remove( mPrefix );

  emit pictureReceived( path );
}

PictureSource::~PictureSource()
{

}
