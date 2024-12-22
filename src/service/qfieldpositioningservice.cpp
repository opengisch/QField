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

#include <QJniObject>
#include <QLocale>
#include <QQmlEngine>
#include <QtCore/private/qandroidextras_p.h>


QFieldPositioningService::QFieldPositioningService( int &argc, char **argv )
  : QAndroidService( argc, argv )
{
  qDebug() << "XXX!!!XXX";
  mPositioningSource = new PositioningSource( this );
  mHost.setHostUrl( QUrl( QStringLiteral( "localabstract:replica" ) ) );
  mHost.enableRemoting( mPositioningSource, "PositioningSource" );

  mNotificationTimer.setInterval( 2500 );
  mNotificationTimer.setSingleShot( false );

  connect( mPositioningSource, &PositioningSource::activeChanged, this, [=] {
    if ( mPositioningSource->active() )
    {
      mNotificationTimer.start();
    }
    else
    {
      mNotificationTimer.stop();
    }
  } );

  connect( &mNotificationTimer, &QTimer::timeout, this, [=] {
    const GnssPositionInformation pos = mPositioningSource->positionInformation();
    QJniObject message = QJniObject::fromString( QStringLiteral( "Latitude %1 | Longitude %2 | Altitude %3" ).arg( QLocale::system().toString( pos.latitude(), 'f', 7 ), QLocale::system().toString( pos.longitude(), 'f', 7 ), QLocale::system().toString( pos.elevation(), 'f', 3 ) ) );
    QJniObject::callStaticMethod<void>( "ch/opengis/" APP_PACKAGE_NAME "/QFieldPositioningService",
                                        "sendNotification",
                                        message.object<jstring>() );
  } );
}

QFieldPositioningService::~QFieldPositioningService()
{
}
