/***************************************************************************
 qfandroidviewstatus.cpp

 ---------------------
 begin                : 29.02.2020
 copyright            : (C) 2020 by david
 email                : david at opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfandroidviewstatus.h"
#include "qfplatformutilities.h"

QfAndroidViewStatus::QfAndroidViewStatus( QObject *parent )
  : QfViewStatus( parent )
{
  connect( QfPlatformUtilities::instance(), &QfPlatformUtilities::resourceReceived, this, &QfAndroidViewStatus::handleResourceOpened );
  connect( QfPlatformUtilities::instance(), &QfPlatformUtilities::resourceCanceled, this, &QfAndroidViewStatus::handleResourceCanceled );
}

void QfAndroidViewStatus::handleResourceOpened( const QString &path )
{
  disconnect( QfPlatformUtilities::instance(), &QfPlatformUtilities::resourceReceived, this, &QfAndroidViewStatus::handleResourceOpened );
  disconnect( QfPlatformUtilities::instance(), &QfPlatformUtilities::resourceCanceled, this, &QfAndroidViewStatus::handleResourceCanceled );

  emit finished();
}

void QfAndroidViewStatus::handleResourceCanceled( const QString &message )
{
  disconnect( QfPlatformUtilities::instance(), &QfPlatformUtilities::resourceReceived, this, &QfAndroidViewStatus::handleResourceOpened );
  disconnect( QfPlatformUtilities::instance(), &QfPlatformUtilities::resourceCanceled, this, &QfAndroidViewStatus::handleResourceCanceled );

  emit statusReceived( message );
  emit finished();
}
