

# File qfgeometrywrapper.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfgeometrywrapper.cpp**](qfgeometrywrapper_8cpp.md)

[Go to the documentation of this file](qfgeometrywrapper_8cpp.md)


```C++
/***************************************************************************
  qfgeometrywrapper.cpp - QfGeometryWrapper

 ---------------------
 begin                : 18.08.2018
 copyright            : (C) 2018 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfgeometrywrapper.h"

QVariantList QfGeometryWrapper::pointList() const
{
  QVariantList pointList;
  if ( mQgsGeometry.type() != Qgis::GeometryType::Point )
    return pointList;

  QgsVertexIterator vertexIterator = mQgsGeometry.vertices();
  while ( vertexIterator.hasNext() )
  {
    const QgsPoint &pt = vertexIterator.next();
    pointList.append( QVariant::fromValue<QgsPoint>( pt ) );
  }

  return pointList;
}

void QfGeometryWrapper::clear()
{
  setQgsGeometry( QgsGeometry() );
  setCrs( QgsCoordinateReferenceSystem() );
}

QgsGeometry QfGeometryWrapper::qgsGeometry() const
{
  return mQgsGeometry;
}

void QfGeometryWrapper::setQgsGeometry( const QgsGeometry &qgsGeometry )
{
  if ( qgsGeometry.constGet() == mQgsGeometry.constGet() )
    return;

  mQgsGeometry = qgsGeometry;
  emit qgsGeometryChanged();
}

QgsCoordinateReferenceSystem QfGeometryWrapper::crs() const
{
  return mCrs;
}

void QfGeometryWrapper::setCrs( const QgsCoordinateReferenceSystem &crs )
{
  if ( mCrs == crs )
    return;

  mCrs = crs;
  emit crsChanged();
}
```


