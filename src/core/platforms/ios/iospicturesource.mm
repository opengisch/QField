/***************************************************************************
  iospicturesource.cpp - IosPictureSource

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

#include "iospicturesource.h"
#include "qgsapplication.h"
#include "qgsmessagelog.h"

#include <QDir>
#include <QFile>


IosPictureSource::IosPictureSource( const QString &prefix )
  : PictureSource( nullptr, prefix )
  , mPrefix( prefix )
{
}

void IosPictureSource::handleActivityResult( int receiverRequestCode, int resultCode )
{
  if ( receiverRequestCode == 171 )
  {
    emit pictureReceived( "" );
  }
  else
  {
    emit pictureReceived( QString() );
  }
}
