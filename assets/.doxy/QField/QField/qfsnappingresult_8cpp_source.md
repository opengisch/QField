

# File qfsnappingresult.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfsnappingresult.cpp**](qfsnappingresult_8cpp.md)

[Go to the documentation of this file](qfsnappingresult_8cpp.md)


```C++
/***************************************************************************
  qfsnappingresult.cpp - QfSnappingResult

 ---------------------
 begin                : 8.10.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "qfsnappingresult.h"

QfSnappingResult::Type matchTypeToSnappingResultType( QgsPointLocator::Type type )
{
  return static_cast<QfSnappingResult::Type>( type );
}

QfSnappingResult::QfSnappingResult()
  : mType( Invalid )
  , mDist( 0 )
  , mPoint()
  , mLayer( nullptr )
  , mFid( 0 )
  , mVertexIndex( 0 )
{}

QfSnappingResult::QfSnappingResult( QfSnappingResult::Type t, QgsVectorLayer *vl, QgsFeatureId fid, double dist, const QgsPoint &pt, int vertexIndex, const QgsPoint *edgePoints )
  : mType( t )
  , mDist( dist )
  , mPoint( pt )
  , mLayer( vl )
  , mFid( fid )
  , mVertexIndex( vertexIndex )
{
  if ( edgePoints )
  {
    mEdgePoints[0] = edgePoints[0];
    mEdgePoints[1] = edgePoints[1];
  }
}

QfSnappingResult::QfSnappingResult( const QgsPointLocator::Match &match )
  : mType( matchTypeToSnappingResultType( match.type() ) )
  , mDist( match.distance() )
  , mPoint( match.point() )
  , mLayer( match.layer() )
  , mFid( match.featureId() )
  , mVertexIndex( match.vertexIndex() )
{
}

bool QfSnappingResult::operator==( const QfSnappingResult &other ) const
{
  return mType == other.mType && mDist == other.mDist && mPoint == other.mPoint && mLayer == other.mLayer && mFid == other.mFid && mVertexIndex == other.mVertexIndex;
}

QfSnappingResult::Type QfSnappingResult::type() const
{
  return mType;
}

bool QfSnappingResult::isValid() const
{
  return mType != Invalid;
}

bool QfSnappingResult::hasVertex() const
{
  return mType == Vertex;
}

bool QfSnappingResult::hasEdge() const
{
  return mType == Edge;
}

bool QfSnappingResult::hasArea() const
{
  return mType == Area;
}

double QfSnappingResult::distance() const
{
  return mDist;
}

QgsPoint QfSnappingResult::point() const
{
  return mPoint;
}

void QfSnappingResult::setPoint( const QgsPoint point )
{
  mPoint = point;
}

int QfSnappingResult::vertexIndex() const
{
  return mVertexIndex;
}

QgsVectorLayer *QfSnappingResult::layer() const
{
  return mLayer.data();
}

QgsFeatureId QfSnappingResult::featureId() const
{
  return mFid;
}

void QfSnappingResult::edgePoints( QgsPoint &pt1, QgsPoint &pt2 ) const
{
  pt1 = mEdgePoints[0];
  pt2 = mEdgePoints[1];
}
```


