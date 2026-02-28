

# File geometry.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**geometry.cpp**](geometry_8cpp.md)

[Go to the documentation of this file](geometry_8cpp.md)


```C++
/***************************************************************************
    geometry.cpp
    ---------------------
    begin                : March 2020
    copyright            : (C) 2020 by David Signer
    email                : david at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "geometry.h"

#include <qgslinestring.h>
#include <qgspoint.h>
#include <qgspolygon.h>
#include <qgsvectorlayer.h>

Geometry::Geometry( QObject *parent )
  : QObject( parent )
{
}

QgsGeometry Geometry::asQgsGeometry() const
{
  QgsAbstractGeometry *geom = nullptr;

  if ( !mVectorLayer )
    return QgsGeometry();

  switch ( mVectorLayer->geometryType() )
  {
    case Qgis::GeometryType::Point:
    {
      geom = new QgsPoint( mRubberbandModel->currentPoint( mVectorLayer->crs(), mVectorLayer->wkbType() ) );
      break;
    }
    case Qgis::GeometryType::Line:
    {
      QgsLineString *line = new QgsLineString();
      line->setPoints( mRubberbandModel->pointSequence( mVectorLayer->crs(), mVectorLayer->wkbType() ) );
      geom = line;
      break;
    }
    case Qgis::GeometryType::Polygon:
    {
      QgsPolygon *polygon = new QgsPolygon();
      QgsLineString *ring = new QgsLineString();
      ring->setPoints( mRubberbandModel->pointSequence( mVectorLayer->crs(), mVectorLayer->wkbType(), true ) );
      polygon->setExteriorRing( ring );
      geom = polygon;
      break;
    }

    case Qgis::GeometryType::Unknown:
      break;
    case Qgis::GeometryType::Null:
      break;
  }

  // QgsCoordinateTransform ct( mRubberbandModel->crs(), mVectorLayer->crs() );
  // return ct.transform( QgsGeometry( geom ) );
  return QgsGeometry( geom );
}

RubberbandModel *Geometry::rubberbandModel() const
{
  return mRubberbandModel;
}

void Geometry::setRubberbandModel( RubberbandModel *rubberbandModel )
{
  if ( mRubberbandModel == rubberbandModel )
    return;

  mRubberbandModel = rubberbandModel;

  emit rubberbandModelChanged();
}

void Geometry::applyRubberband()
{
  // TODO: Will need to be implemented for multipart features or polygons with holes.
}

void Geometry::updateRubberband( const QgsGeometry &geometry )
{
  if ( !mRubberbandModel )
    return;

  if ( !geometry.isEmpty() )
  {
    if ( mVectorLayer )
    {
      mRubberbandModel->setDataFromGeometry( geometry, mVectorLayer->crs() );
    }
    else
    {
      mRubberbandModel->setDataFromGeometry( geometry );
    }
  }
  else
  {
    mRubberbandModel->reset( false );
  }
}

QgsVectorLayer *Geometry::vectorLayer() const
{
  return mVectorLayer.data();
}

void Geometry::setVectorLayer( QgsVectorLayer *vectorLayer )
{
  if ( mVectorLayer == vectorLayer )
    return;

  mVectorLayer = vectorLayer;
  emit vectorLayerChanged();
}
```


