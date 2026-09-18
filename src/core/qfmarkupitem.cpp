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
  : mLabel( label )
  , mDescription( description )
  , mGeometry( geometry )
  , mColor( color )
{
}
