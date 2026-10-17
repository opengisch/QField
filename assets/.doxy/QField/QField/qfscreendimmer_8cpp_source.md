

# File qfscreendimmer.cpp

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qfscreendimmer.cpp**](qfscreendimmer_8cpp.md)

[Go to the documentation of this file](qfscreendimmer_8cpp.md)


```C++
/***************************************************************************
              qfscreendimmer.cpp
               ----------------------------------------------------
              date                 : 26.06.2021
              copyright            : (C) 2021 by Mathieu Pellerin
              email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfplatformutilities.h"
#include "qfscreendimmer.h"

#include <QEvent>

QfScreenDimmer::QfScreenDimmer( QgsApplication *app )
  : QObject( app )
{
  app->installEventFilter( this );
  connect( app, &QGuiApplication::applicationStateChanged, this, [this, app]() { setSuspend( app->applicationState() != Qt::ApplicationActive ); } );

  mTimer.setSingleShot( true );
  mTimer.setInterval( mTimeoutSeconds * 1000 );
  connect( &mTimer, &QTimer::timeout, this, &QfScreenDimmer::timeout );
}

void QfScreenDimmer::setTimeout( int timeoutSeconds )
{
  if ( mTimeoutSeconds == timeoutSeconds )
    return;

  mTimeoutSeconds = timeoutSeconds;

  if ( mTimeoutSeconds > 0 )
  {
    mTimer.setInterval( mTimeoutSeconds * 1000 );
  }
  else
  {
    mTimer.stop();
    if ( mDimmed )
    {
      QfPlatformUtilities::instance()->restoreBrightness();
      mDimmed = false;
    }
  }
}

void QfScreenDimmer::setSuspend( bool suspend )
{
  mSuspend = suspend;
  if ( mTimeoutSeconds > 0 )
  {
    if ( mSuspend )
    {
      mTimer.stop();
      if ( mDimmed )
      {
        QfPlatformUtilities::instance()->restoreBrightness();
        mDimmed = false;
      }
    }
    else
    {
      mTimer.start();
    }
  }
}

bool QfScreenDimmer::eventFilter( QObject *obj, QEvent *event )
{
  const QEvent::Type type = event->type();
  if ( type == QEvent::KeyPress || type == QEvent::MouseMove || type == QEvent::MouseButtonPress || type == QEvent::TabletMove || type == QEvent::TabletPress || type == QEvent::TouchBegin || type == QEvent::TouchUpdate || type == QEvent::InputMethod || type == QEvent::Wheel )
  {
    if ( mTimeoutSeconds > 0 && !mSuspend )
      mTimer.start();

    if ( mDimmed )
    {
      QfPlatformUtilities::instance()->restoreBrightness();
      mDimmed = false;
      return true;
    }
  }

  return QObject::eventFilter( obj, event );
}

void QfScreenDimmer::timeout()
{
  QfPlatformUtilities::instance()->dimBrightness();
  mDimmed = true;
}
```


