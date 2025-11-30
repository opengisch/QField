

# File qfieldpositioningservice.cpp

[**File List**](files.md) **>** [**service**](dir_4f691f982165d1317925cee58b4b4f56.md) **>** [**qfieldpositioningservice.cpp**](qfieldpositioningservice_8cpp.md)

[Go to the documentation of this file](qfieldpositioningservice_8cpp.md)


```C++
/***************************************************************************
  qFieldpositioningservice.cpp - QFieldPositioningService

 ---------------------
 begin                : 21.12.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "positioningsource.h"
#include "qfield_android.h"
#include "qfieldpositioningservice.h"

#include <QFile>
#include <QJniObject>
#include <QLocale>
#include <QQmlEngine>
#include <QtCore/private/qandroidextras_p.h>


QFieldPositioningService::QFieldPositioningService( int &argc, char **argv )
  : QAndroidService( argc, argv )
{
  qRegisterMetaType<GnssPositionInformation>( "GnssPositionInformation" );

  mPositioningSource = new PositioningSource( this );
  mHost.setHostUrl( QUrl( QStringLiteral( "localabstract:" APP_PACKAGE_NAME "replica" ) ) );
  mHost.enableRemoting( mPositioningSource, "PositioningSource" );

  mNotificationTimer.setInterval( 1000 );
  mNotificationTimer.setSingleShot( false );
  connect( &mNotificationTimer, &QTimer::timeout, this, &QFieldPositioningService::triggerShowNotification );

  connect( mPositioningSource, &PositioningSource::positionInformationChanged, this, [=] {
    if ( !mPositioningSource->backgroundMode() && QFile::exists( PositioningSource::backgroundFilePath ) )
    {
      mPositioningSource->setBackgroundMode( true );
    }
  } );

  connect( mPositioningSource, &PositioningSource::backgroundModeChanged, this, [=] {
    if ( mPositioningSource->active() )
    {
      if ( mPositioningSource->backgroundMode() )
      {
        triggerShowNotification();
        mNotificationTimer.start();
      }
      else
      {
        mNotificationTimer.stop();
        triggerReturnNotification();
      }
    }
  } );

  connect( mPositioningSource, &PositioningSource::activeChanged, this, [=] {
    if ( mPositioningSource->active() )
    {
      if ( mPositioningSource->backgroundMode() )
      {
        triggerShowNotification();
        mNotificationTimer.start();
      }
      else
      {
        triggerReturnNotification();
      }
    }
    else
    {
      mNotificationTimer.stop();
      triggerStopNotification();
    }
  } );
}

void QFieldPositioningService::triggerShowNotification()
{
  const GnssPositionInformation pos = mPositioningSource->positionInformation();
  QJniObject message = QJniObject::fromString( tr( "Latitude %1 | Longitude %2 | Altitude %3 m | Speed %4 m/s | Direction %5°" ).arg( QLocale::system().toString( pos.latitude(), 'f', 7 ), QLocale::system().toString( pos.longitude(), 'f', 7 ), QLocale::system().toString( pos.elevation(), 'f', 3 ), QLocale::system().toString( pos.speed(), 'f', 1 ), QLocale::system().toString( pos.direction(), 'f', 1 ) ) );
  QJniObject::callStaticMethod<void>( "ch/opengis/" APP_PACKAGE_NAME "/QFieldPositioningService",
                                      "triggerShowNotification",
                                      message.object<jstring>(),
                                      true );
}

void QFieldPositioningService::triggerReturnNotification()
{
  QJniObject message = QJniObject::fromString( tr( "Positioning service running" ) );
  QJniObject::callStaticMethod<void>( "ch/opengis/" APP_PACKAGE_NAME "/QFieldPositioningService",
                                      "triggerShowNotification",
                                      message.object<jstring>(),
                                      false );
}

void QFieldPositioningService::triggerStopNotification()
{
  QJniObject message = QJniObject::fromString( tr( "Positioning service stopped" ) );
  QJniObject::callStaticMethod<void>( "ch/opengis/" APP_PACKAGE_NAME "/QFieldPositioningService",
                                      "triggerShowNotification",
                                      message.object<jstring>(),
                                      false );
}

QFieldPositioningService::~QFieldPositioningService()
{
}
```


