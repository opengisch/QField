/***************************************************************************
  focusstack.cpp - %{Cpp:License:ClassName}

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

#include <QDebug>

FocusStack::FocusStack( QObject* parent )
  : QObject( parent ),
  QList<QQuickItem*>()
{

}

void FocusStack::addFocusTaker( QQuickItem *item )
{
  qDebug() << "Add focus taker item " << item->objectName();
  connect( item, &QQuickItem::activeFocusChanged, this, &FocusStack::itemFocusChanged );
}

void FocusStack::itemFocusChanged( bool itemActiveFocus )
{
  qDebug() << "Item " << sender()->objectName() << " changed focus to " << itemActiveFocus;

  if( itemActiveFocus )
  {
    setFocused( qobject_cast<QQuickItem*>( sender() ) );
  }
  else
  {
    setUnfocused( qobject_cast<QQuickItem*>( sender() ) );
  }
}

void FocusStack::setFocused( QQuickItem* item  )
{
  qDebug() << "Set item focused: " << item->objectName();

  removeAll( item );
  qDebug() << "Size of focusstack after remove: " << size();
  append( item );
  qDebug() << "Size of focusstack after append: " << size();
}

void FocusStack::setUnfocused( QQuickItem* item  )
{
  qDebug() << "Unset item focused: " << item->objectName();

  if( !item->isVisible() )
  {

    qDebug() << "Size of stacklist before remove: " << size();
    removeAll( item );
    qDebug() << "Size of stacklist after remove: " << size();
    if( !isEmpty() )
    {
      last()->forceActiveFocus();
      qDebug() << "Forced focus to: " << last()->objectName();
    }
  }
  else
  {
    qDebug() << "Size of focusstack after doing nothing: " << size();
  }

}
