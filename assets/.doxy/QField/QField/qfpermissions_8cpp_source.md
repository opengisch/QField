

# File qfpermissions.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfpermissions.cpp**](qfpermissions_8cpp.md)

[Go to the documentation of this file](qfpermissions_8cpp.md)


```C++
/***************************************************************************
 qfpermissions.cpp - QfCameraPermission, QfMicrophonePermission

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

#include "qfpermissions.h"
#include "qfplatformutilities.h"

#include <QGuiApplication>

QfCameraPermission::QfCameraPermission( QObject *parent )
  : QObject( parent )
{
}

Qt::PermissionStatus QfCameraPermission::status()
{
  if ( !mStatusChecked )
  {
    mStatus = QfPlatformUtilities::instance()->checkCameraPermission();
    mStatusChecked = true;
  }

  return mStatus;
}

void QfCameraPermission::request()
{
  QfPlatformUtilities::instance()->requestCameraPermission( [this]( Qt::PermissionStatus status ) {
    if ( mStatus != status )
    {
      mStatus = status;
      mStatusChecked = true;
      emit statusChanged();
    }
  } );
}

QfMicrophonePermission::QfMicrophonePermission( QObject *parent )
  : QObject( parent )
{
}

Qt::PermissionStatus QfMicrophonePermission::status()
{
  if ( !mStatusChecked )
  {
    mStatus = QfPlatformUtilities::instance()->checkMicrophonePermission();
    mStatusChecked = true;
  }

  return mStatus;
}

void QfMicrophonePermission::request()
{
  QfPlatformUtilities::instance()->requestMicrophonePermission( [this]( Qt::PermissionStatus status ) {
    if ( mStatus != status )
    {
      mStatus = status;
      mStatusChecked = true;
      emit statusChanged();
    }
  } );
}
```


