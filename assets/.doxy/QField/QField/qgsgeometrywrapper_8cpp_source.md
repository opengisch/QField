

# File qgsgeometrywrapper.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgsgeometrywrapper.cpp**](qgsgeometrywrapper_8cpp.md)

[Go to the documentation of this file](qgsgeometrywrapper_8cpp.md)


```C++
/***************************************************************************
  qgsgeometrywrapper.cpp - QgsGeometryWrapper

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

#include "qgsgeometrywrapper.h"

QVariantList QgsGeometryWrapper::pointList() const
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

void QgsGeometryWrapper::clear()
{
  setQgsGeometry( QgsGeometry() );
  setCrs( QgsCoordinateReferenceSystem() );
}

QgsGeometry QgsGeometryWrapper::qgsGeometry() const
{
  return mQgsGeometry;
}

void QgsGeometryWrapper::setQgsGeometry( const QgsGeometry &qgsGeometry )
{
  if ( qgsGeometry.constGet() == mQgsGeometry.constGet() )
    return;

  mQgsGeometry = qgsGeometry;
  emit qgsGeometryChanged();
}

QgsCoordinateReferenceSystem QgsGeometryWrapper::crs() const
{
  return mCrs;
}

void QgsGeometryWrapper::setCrs( const QgsCoordinateReferenceSystem &crs )
{
  if ( mCrs == crs )
    return;

  mCrs = crs;
  emit crsChanged();
}
```


