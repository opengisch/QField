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
  mTimer.setInterval( 20000 );
  connect( &mTimer, &QTimer::timeout, this, &ScreenDimmer::timeout );
}

void ScreenDimmer::setActive( bool active )
{
  mActive = active;
  if ( !mActive )
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
  if ( mActive )
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
  if ( event->type() == QEvent::KeyPress || event->type() == QEvent::MouseButtonPress || event->type() == QEvent::TouchBegin || event->type() == QEvent::InputMethod )
  {
    if ( mActive && !mSuspend )
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
