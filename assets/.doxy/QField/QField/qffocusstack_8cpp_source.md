

# File qffocusstack.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qffocusstack.cpp**](qffocusstack_8cpp.md)

[Go to the documentation of this file](qffocusstack_8cpp.md)


```C++
/***************************************************************************
  qffocusstack.cpp

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
#include "qffocusstack.h"

QfFocusStack::QfFocusStack( QObject *parent )
  : QObject( parent )
{
}

void QfFocusStack::addFocusTaker( QObject *object )
{
  const QVariant visible = object->property( "visible" );
  const QVariant opened = object->property( "opened" );
  if ( opened.isValid() )
  {
    connect( object, SIGNAL( opened() ), this, SLOT( popupOpened() ) );
    connect( object, SIGNAL( closed() ), this, SLOT( popupClosed() ) );
  }
  else if ( visible.isValid() )
  {
    connect( object, SIGNAL( visibleChanged() ), this, SLOT( visibleChanged() ) );
    if ( visible.toBool() )
    {
      mStackList.append( object );
    }
  }
}

void QfFocusStack::popupOpened()
{
  setFocused( sender() );
}

void QfFocusStack::popupClosed()
{
  setUnfocused( sender() );
}

void QfFocusStack::visibleChanged()
{
  const QVariant visible = sender()->property( "visible" );
  if ( visible.toBool() )
  {
    setFocused( sender() );
  }
  else
  {
    setUnfocused( sender() );
  }
}

void QfFocusStack::setFocused( QObject *object )
{
  mStackList.removeAll( object );
  mStackList.append( object );
  QMetaObject::invokeMethod( object, "forceActiveFocus", Qt::DirectConnection );
}

void QfFocusStack::setUnfocused( QObject *object )
{
  const QVariant visible = object->property( "visible" );
  const QVariant opened = object->property( "opened" );
  if ( opened.isValid() )
  {
    if ( !opened.toBool() )
    {
      mStackList.removeAll( object );
      mStackList.removeIf( []( const QPointer<QObject> &pointer ) { return pointer.isNull(); } );

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
      mStackList.removeIf( []( const QPointer<QObject> &pointer ) { return pointer.isNull(); } );

      if ( !mStackList.isEmpty() )
      {
        QMetaObject::invokeMethod( mStackList.last(), "forceActiveFocus", Qt::DirectConnection );
      }
    }
  }
}

void QfFocusStack::forceActiveFocusOnLastTaker()
{
  mStackList.removeIf( []( const QPointer<QObject> &pointer ) { return pointer.isNull(); } );
  if ( mStackList.isEmpty() )
  {
    return;
  }

  QMetaObject::invokeMethod( mStackList.last(), "forceActiveFocus", Qt::DirectConnection );
}
```


