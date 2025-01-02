/***************************************************************************
  focusstack.cpp

 ---------------------
 begin                : 11.4.2018
 copyright            : (C) 2018 by david
 email                : david at opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "focusstack.h"

void FocusStack::addFocusTaker( QObject *object )
{
  QVariant visible = object->property( "visible" );
  QVariant opened = object->property( "opened" );
  if ( opened.isValid() )
  {
    connect( object, SIGNAL( opened() ), this, SLOT( popupOpened() ) );
    connect( object, SIGNAL( closed() ), this, SLOT( popupClosed() ) );
  }
  else if ( visible.isValid() )
  {
    connect( object, SIGNAL( activeFocusChanged( bool ) ), this, SLOT( itemFocusChanged( bool ) ) );
  }
}

void FocusStack::popupOpened()
{
  setFocused( sender() );
}

void FocusStack::popupClosed()
{
  setUnfocused( sender() );
}

void FocusStack::itemFocusChanged( bool itemActiveFocus )
{
  if ( itemActiveFocus )
  {
    setFocused( sender() );
  }
  else
  {
    setUnfocused( sender() );
  }
}

void FocusStack::setFocused( QObject *object )
{
  mStackList.removeAll( object );
  mStackList.append( object );
}

void FocusStack::setUnfocused( QObject *object )
{
  QVariant visible = object->property( "visible" );
  QVariant opened = object->property( "opened" );
  if ( opened.isValid() )
  {
    if ( !opened.toBool() )
    {
      mStackList.removeAll( object );
      if ( !mStackList.isEmpty() )
      {
        QMetaObject::invokeMethod( mStackList.last(), "forceActiveFocus", Qt::DirectConnection );
      }
    }
  }
  else if ( visible.isValid() )
  {
    if ( !visible.toBool() )
    {
      mStackList.removeAll( object );
      if ( !mStackList.isEmpty() )
      {
        QMetaObject::invokeMethod( mStackList.last(), "forceActiveFocus", Qt::DirectConnection );
      }
    }
  }
}

void FocusStack::forceActiveFocusOnLastTaker() const
{
  if ( mStackList.isEmpty() )
    return;

  QMetaObject::invokeMethod( mStackList.last(), "forceActiveFocus", Qt::DirectConnection );
}
