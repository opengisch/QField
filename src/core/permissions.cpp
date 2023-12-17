/***************************************************************************
 permissions.cpp - CameraPermission, MicrophonePermission

 ---------------------
 begin                : 01.12.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "permissions.h"
#include "platformutilities.h"

#include <QGuiApplication>

CameraPermission::CameraPermission( QObject *parent )
  : QObject( parent )
  , mStatus( PlatformUtilities::instance()->checkCameraPermission() )
{
}

Qt::PermissionStatus CameraPermission::status()
{
  return mStatus;
}

void CameraPermission::request()
{
  PlatformUtilities::instance()->requestCameraPermission( [=]( Qt::PermissionStatus status ) {
    if ( mStatus != status )
    {
      mStatus = status;
      mStatusChecked = true;
      emit statusChanged();
    }
  } );
}

MicrophonePermission::MicrophonePermission( QObject *parent )
  : QObject( parent )
  , mStatus( PlatformUtilities::instance()->checkCameraPermission() )
{
}

Qt::PermissionStatus MicrophonePermission::status()
{
  return mStatus;
}

void MicrophonePermission::request()
{
  PlatformUtilities::instance()->requestMicrophonePermission( [=]( Qt::PermissionStatus status ) {
    if ( mStatus != status )
    {
      mStatus = status;
      mStatusChecked = true;
      emit statusChanged();
    }
  } );
}
