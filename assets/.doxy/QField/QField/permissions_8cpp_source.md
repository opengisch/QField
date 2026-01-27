

# File permissions.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**permissions.cpp**](permissions_8cpp.md)

[Go to the documentation of this file](permissions_8cpp.md)


```C++
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
{
}

Qt::PermissionStatus CameraPermission::status()
{
  if ( !mStatusChecked )
  {
    mStatus = PlatformUtilities::instance()->checkCameraPermission();
    mStatusChecked = true;
  }

  return mStatus;
}

void CameraPermission::request()
{
  PlatformUtilities::instance()->requestCameraPermission( [this]( Qt::PermissionStatus status ) {
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
{
}

Qt::PermissionStatus MicrophonePermission::status()
{
  if ( !mStatusChecked )
  {
    mStatus = PlatformUtilities::instance()->checkMicrophonePermission();
    mStatusChecked = true;
  }

  return mStatus;
}

void MicrophonePermission::request()
{
  PlatformUtilities::instance()->requestMicrophonePermission( [this]( Qt::PermissionStatus status ) {
    if ( mStatus != status )
    {
      mStatus = status;
      mStatusChecked = true;
      emit statusChanged();
    }
  } );
}
```


