

# File qfmarkupitem.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfmarkupitem.cpp**](qfmarkupitem_8cpp.md)

[Go to the documentation of this file](qfmarkupitem_8cpp.md)


```C++
/***************************************************************************
 qfmarkupitem.cpp - QfMarkupItem

 ---------------------
 begin                : 12.09.2026
 copyright            : (C) 2026 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfmarkupitem.h"


QfMarkupItem::QfMarkupItem( const QString &label, const QString &description, const QgsGeometry &geometry, const QColor &color )
  : mUuid( QUuid::createUuid().toString( QUuid::WithoutBraces ) )
  , mLabel( label )
  , mDescription( description )
  , mGeometry( geometry )
  , mColor( color )
{
}

QfMarkupItem::QfMarkupItem( const QfMarkupItem &other )
  : mUuid( other.mUuid )
  , mLabel( other.mLabel )
  , mDescription( other.mDescription )
  , mGeometry( other.mGeometry )
  , mColor( other.mColor )
{
}
```


