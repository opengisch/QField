/***************************************************************************
              screendimmer.h
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

#include "platformutilities.h"
#include "screendimmer.h"

#include <QEvent>

ScreenDimmer::ScreenDimmer( QgsApplication *app )
  : QObject( app )
{
  app->installEventFilter( this );
  connect( app, &QGuiApplication::applicationStateChanged, this, [=]() { setSuspend( app->applicationState() != Qt::ApplicationActive ); } );

  mTimer.setSingleShot( true );
  mTimer.setInterval( mTimeoutSeconds * 1000 );
  connect( &mTimer, &QTimer::timeout, this, &ScreenDimmer::timeout );
}

void ScreenDimmer::setTimeout( int timeoutSeconds )
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
      PlatformUtilities::instance()->restoreBrightness();
      mDimmed = false;
    }
  }
}

void ScreenDimmer::setSuspend( bool suspend )
{
  mSuspend = suspend;
  if ( mTimeoutSeconds > 0 )
  {
    if ( mSuspend )
    {
      mTimer.stop();
      if ( mDimmed )
      {
        PlatformUtilities::instance()->restoreBrightness();
        mDimmed = false;
      }
    }
    else
    {
      mTimer.start();
    }
  }
}

bool ScreenDimmer::eventFilter( QObject *obj, QEvent *event )
{
  const QEvent::Type type = event->type();
  if ( type == QEvent::KeyPress || type == QEvent::MouseMove || type == QEvent::MouseButtonPress || type == QEvent::TabletMove || type == QEvent::TabletPress || type == QEvent::TouchBegin || type == QEvent::TouchUpdate || type == QEvent::InputMethod || type == QEvent::Wheel )
  {
    if ( mTimeoutSeconds > 0 && !mSuspend )
      mTimer.start();

    if ( mDimmed )
    {
      PlatformUtilities::instance()->restoreBrightness();
      mDimmed = false;
      return true;
    }
  }

  return QObject::eventFilter( obj, event );
}

void ScreenDimmer::timeout()
{
  PlatformUtilities::instance()->dimBrightness();
  mDimmed = true;
}
