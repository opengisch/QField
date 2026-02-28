

# File rubberbandmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**rubberbandmodel.cpp**](rubberbandmodel_8cpp.md)

[Go to the documentation of this file](rubberbandmodel_8cpp.md)


```C++
/***************************************************************************
  rubberbandmodel.cpp - RubberbandModel

 ---------------------
 begin                : 10.6.2016
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
#include "rubberbandmodel.h"
#include "snappingutils.h"

#include <qgslogger.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

RubberbandModel::RubberbandModel( QObject *parent )
  : QObject( parent )
  , mLayer( nullptr )
{
  mPointList.insert( 0, QgsPoint() );
}

int RubberbandModel::vertexCount() const
{
  return static_cast<int>( mPointList.size() );
}

bool RubberbandModel::isEmpty() const
{
  return mPointList.isEmpty();
}

QVector<QgsPoint> RubberbandModel::vertices() const
{
  return mPointList;
}

QVector<QgsPoint> RubberbandModel::verticesCopy( bool skipCurrentPoint ) const
{
  QVector<QgsPoint> points;
  for ( const QgsPoint &pt : mPointList )
  {
    points << QgsPoint( pt );
  }
  if ( skipCurrentPoint )
    points.remove( mCurrentCoordinateIndex );

  return points;
}

QgsPointSequence RubberbandModel::pointSequence( const QgsCoordinateReferenceSystem &crs, Qgis::WkbType wkbType, bool closeLine ) const
{
  QgsPointSequence sequence;
  QgsCoordinateTransform ct( mCrs, crs, QgsProject::instance()->transformContext() );

  for ( const QgsPoint &pt : mPointList )
  {
    //crs transformation of XY
    QgsPointXY p1 = ct.transform( pt.x(), pt.y() );

    //get point containing ZM if existing
    QgsPoint p2 = SnappingUtils::newPoint( pt, wkbType );
    p2.setX( p1.x() );
    p2.setY( p1.y() );

    //overwrite z and m values if already existent in the point
    if ( QgsWkbTypes::hasM( wkbType ) )
    {
      p2.addMValue( QgsWkbTypes::hasM( pt.wkbType() ) ? pt.m() : 0 );
    }

    if ( QgsWkbTypes::hasZ( wkbType ) )
    {
      p2.addZValue( QgsWkbTypes::hasZ( pt.wkbType() ) ? pt.z() : 0 );
    }

    sequence.append( p2 );
  }

  if ( closeLine && sequence.count() > 1 )
  {
    sequence.append( sequence.at( 0 ) );
  }
  return sequence;
}

QVector<QgsPointXY> RubberbandModel::flatPointSequence( const QgsCoordinateReferenceSystem &crs ) const
{
  QVector<QgsPointXY> sequence;

  QgsCoordinateTransform ct( mCrs, crs, QgsProject::instance()->transformContext() );

  for ( const QgsPoint &pt : mPointList )
  {
    sequence.append( ct.transform( pt.x(), pt.y() ) );
  }

  return sequence;
}

QgsPoint RubberbandModel::vertexAt( int index, const QgsCoordinateReferenceSystem &crs ) const
{
  if ( index >= mPointList.size() )
    return QgsPoint();

  QgsPoint point = mPointList.at( index );
  if ( crs.isValid() && mCrs != crs )
  {
    QgsCoordinateTransform ct( mCrs, crs, QgsProject::instance()->transformContext() );
    point.transform( ct );
  }

  return point;
}

void RubberbandModel::setVertex( int index, QgsPoint coordinate )
{
  if ( mPointList.at( index ) != coordinate )
  {
    mPointList.replace( index, coordinate );
    emit vertexChanged( index );
  }
}

void RubberbandModel::insertVertices( int index, int count )
{
  for ( int i = 0; i < count; ++i )
  {
    mPointList.insert( index, currentCoordinate() );
  }

  emit verticesInserted( index, count );
  emit vertexCountChanged();
}

void RubberbandModel::removeVertices( int index, int count, bool keepLast )
{
  if ( mPointList.size() <= ( keepLast ? 1 : 0 ) )
    return;

  mPointList.remove( index, count );

  if ( mCurrentCoordinateIndex >= mPointList.size() )
  {
    setCurrentCoordinateIndex( static_cast<int>( mPointList.size() - 1 ) );
  }

  emit verticesRemoved( index, count );
  emit vertexCountChanged();
}

int RubberbandModel::currentCoordinateIndex() const
{
  return mCurrentCoordinateIndex;
}

void RubberbandModel::setCurrentCoordinateIndex( int currentCoordinateIndex )
{
  if ( currentCoordinateIndex < 0 )
    currentCoordinateIndex = 0;

  if ( currentCoordinateIndex == mCurrentCoordinateIndex )
    return;

  mCurrentCoordinateIndex = currentCoordinateIndex;
  emit currentCoordinateIndexChanged();
  emit currentCoordinateChanged();
}

QgsPoint RubberbandModel::currentPoint( const QgsCoordinateReferenceSystem &crs, Qgis::WkbType wkbType ) const
{
  QgsCoordinateTransform ct( mCrs, crs, QgsProject::instance()->transformContext() );

  QgsPoint currentPt = mPointList.at( mCurrentCoordinateIndex );
  double x = currentPt.x();
  double y = currentPt.y();
  double z = QgsWkbTypes::hasZ( currentPt.wkbType() ) ? currentPt.z() : 0;
  double m = QgsWkbTypes::hasM( currentPt.wkbType() ) ? currentPt.m() : 0;

  try
  {
    ct.transformInPlace( x, y, z );
  }
  catch ( const QgsCsException &exp )
  {
    Q_UNUSED( exp )
    QgsDebugMsgLevel( exp.what(), 2 );
  }
  catch ( ... )
  {
    // catch any other errors
    QgsDebugMsgLevel( "Transform exception caught - possibly because of missing gsb file.", 2 );
  }

  QgsPoint resultPt( x, y );
  if ( QgsWkbTypes::hasZ( wkbType ) )
    resultPt.addZValue( z );
  if ( QgsWkbTypes::hasM( wkbType ) )
    resultPt.addMValue( m );

  return resultPt;
}

QgsPoint RubberbandModel::currentCoordinate() const
{
  return mPointList.value( mCurrentCoordinateIndex );
}

QgsPoint RubberbandModel::firstCoordinate() const
{
  if ( mPointList.isEmpty() )
    return QgsPoint();

  return mPointList.at( 0 );
}

QgsPoint RubberbandModel::lastCoordinate() const
{
  if ( mPointList.isEmpty() )
    return QgsPoint();

  return mPointList.at( mCurrentCoordinateIndex > 0 ? mCurrentCoordinateIndex - 1 : 0 );
}

QgsPoint RubberbandModel::penultimateCoordinate() const
{
  if ( mPointList.size() < 3 )
    return QgsPoint();

  return mPointList.at( mCurrentCoordinateIndex > 1 ? mCurrentCoordinateIndex - 2 : 0 );
}

void RubberbandModel::setCurrentCoordinate( const QgsPoint &currentCoordinate )
{
  if ( mPointList.count() == 0 )
  {
    mPointList << QgsPoint();
  }

  if ( mPointList.at( mCurrentCoordinateIndex ) == currentCoordinate )
  {
    return;
  }

  if ( mFrozen )
  {
    return;
  }

  mPointList.replace( mCurrentCoordinateIndex, currentCoordinate );

  if ( !mLayer || QgsWkbTypes::hasM( mLayer->wkbType() ) )
  {
    if ( !std::isnan( mMeasureValue ) )
    {
      if ( QgsWkbTypes::hasM( mPointList[mCurrentCoordinateIndex].wkbType() ) )
      {
        mPointList[mCurrentCoordinateIndex].setM( mMeasureValue );
      }
      else
      {
        mPointList[mCurrentCoordinateIndex].addMValue( mMeasureValue );
      }
    }
    else
    {
      mPointList[mCurrentCoordinateIndex].dropMValue();
    }
  }

  emit currentCoordinateChanged();
  emit vertexChanged( mCurrentCoordinateIndex );
}

QDateTime RubberbandModel::currentPositionTimestamp() const
{
  return mCurrentPositionTimestamp;
}

void RubberbandModel::setCurrentPositionTimestamp( const QDateTime &currentPositionTimestamp )
{
  mCurrentPositionTimestamp = currentPositionTimestamp;
}

double RubberbandModel::measureValue() const
{
  return mMeasureValue;
}

void RubberbandModel::setMeasureValue( const double measureValue )
{
  if ( mMeasureValue == measureValue )
    return;

  mMeasureValue = measureValue;

  emit measureValueChanged();

  if ( !mLayer || QgsWkbTypes::hasM( mLayer->wkbType() ) )
  {
    QgsPoint currentPoint = currentCoordinate();
    currentPoint.dropMValue(); // Reset the M value to insure new value is added below
    setCurrentCoordinate( currentPoint );
  }
}

void RubberbandModel::addVertex()
{
  // Avoid double vertices accidentally
  if ( mPointList.size() > 1 && *( mPointList.end() - 1 ) == *( mPointList.end() - 2 ) )
  {
    return;
  }

  insertVertices( mCurrentCoordinateIndex + 1, 1 );
  setCurrentCoordinateIndex( mCurrentCoordinateIndex + 1 );
}

void RubberbandModel::addVertexFromPoint( const QgsPoint &point )
{
  setCurrentCoordinate( point );
  addVertex();
}

void RubberbandModel::removeVertex()
{
  setCurrentCoordinateIndex( mCurrentCoordinateIndex - 1 );
  removeVertices( mCurrentCoordinateIndex + 1, 1 );
}

void RubberbandModel::reset( bool keepLast )
{
  removeVertices( 0, static_cast<int>( mPointList.size() ) - ( keepLast ? 1 : 0 ), keepLast );

  mFrozen = false;
  emit frozenChanged();
}

void RubberbandModel::setDataFromGeometry( QgsGeometry geometry, const QgsCoordinateReferenceSystem &crs )
{
  if ( geometry.type() != mGeometryType )
    return;

  QgsCoordinateTransform ct( crs, mCrs, QgsProject::instance()->transformContext() );
  geometry.transform( ct );

  mPointList.clear();
  const QgsAbstractGeometry *abstractGeom = geometry.constGet();
  if ( !abstractGeom )
    return;

  QgsVertexId vertexId;
  QgsPoint pt;
  while ( abstractGeom->nextVertex( vertexId, pt ) )
  {
    if ( vertexId.part > 1 || vertexId.ring > 0 )
    {
      break;
    }
    mPointList << pt;
  }

  // for polygons, remove the last vertex which is a duplicate of the first vertex
  if ( geometry.type() == Qgis::GeometryType::Polygon )
  {
    mPointList.removeLast();
  }

  // insert the last point twice so the resutling rubberband's current coordinate property being modified (by e.g.
  // the GNSS position) will not replace the last vertex from the passed geometry
  mPointList << mPointList.last();

  mCurrentCoordinateIndex = static_cast<int>( mPointList.size() ) - 1;

  emit verticesInserted( 0, static_cast<int>( mPointList.size() ) );
  emit vertexCountChanged();
}

Qgis::GeometryType RubberbandModel::geometryType() const
{
  return mGeometryType;
}

void RubberbandModel::setGeometryType( const Qgis::GeometryType &geometryType )
{
  if ( mGeometryType == geometryType )
    return;

  mGeometryType = geometryType;
  emit geometryTypeChanged();
}

QgsCoordinateReferenceSystem RubberbandModel::crs() const
{
  return mCrs;
}

void RubberbandModel::setCrs( const QgsCoordinateReferenceSystem &crs )
{
  if ( crs == mCrs )
    return;

  mCrs = crs;
  emit crsChanged();
}

QgsVectorLayer *RubberbandModel::vectorLayer() const
{
  return mLayer.data();
}

void RubberbandModel::setVectorLayer( QgsVectorLayer *vectorLayer )
{
  if ( vectorLayer == mLayer )
    return;

  mLayer = vectorLayer;

  if ( mLayer )
    setGeometryType( mLayer->geometryType() );

  emit vectorLayerChanged();
}

bool RubberbandModel::frozen() const
{
  return mFrozen;
}

void RubberbandModel::setFrozen( const bool &frozen )
{
  if ( mFrozen == frozen )
    return;

  mFrozen = frozen;

  emit frozenChanged();
}
```


