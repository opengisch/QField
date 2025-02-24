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

#include <qgscurvepolygon.h>
#include <qgslogger.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

RubberbandModel::RubberbandModel( QObject *parent )
  : QObject( parent )
  , mLayer( nullptr )
{
  mCompoundCurve.addVertex( QgsPoint() );
}

int RubberbandModel::vertexCount() const
{
  return mCompoundCurve.vertexCount( 0, 0 );
}

bool RubberbandModel::isEmpty() const
{
  return mCompoundCurve.isEmpty();
}

QVector<QgsPoint> RubberbandModel::vertices() const
{
  QgsVertexIterator vertexIterator = mCompoundCurve.curveToLine()->vertices();


  // Create an empty QVector<QgsPoint>
  QVector<QgsPoint> points;

  // Iterate over the vertices and add them to the vector
  while ( vertexIterator.hasNext() )
  {
    points.append( vertexIterator.next() );
  }
  return points;
}

QVector<QgsPoint> RubberbandModel::verticesCopy( bool skipCurrentPoint ) const
{
  QVector<QgsPoint> points;
  for ( const QgsPoint &pt : vertices() )
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

  for ( const QgsPoint &pt : vertices() )
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

  for ( const QgsPoint &pt : vertices() )
  {
    sequence.append( ct.transform( pt.x(), pt.y() ) );
  }

  return sequence;
}

void RubberbandModel::setVertex( int index, QgsPoint coordinate )
{
  QgsVertexId id = QgsVertexId( 0, 0, index );
  if ( mCompoundCurve.vertexAt( id ) != coordinate )
  {
    mCompoundCurve.moveVertex( id, coordinate );
    emit vertexChanged( index );
  }
}

void RubberbandModel::insertVertices( int index, int count )
{
  for ( int i = 0; i < count; ++i )
  {
    QgsVertexId id = QgsVertexId( 0, 0, index );
    mCompoundCurve.insertVertex( id, currentCoordinate() );
  }

  emit verticesInserted( index, count );
  emit vertexCountChanged();
}

void RubberbandModel::removeVertices( int index, int count )
{
  if ( vertexCount() <= 1 )
    return;

  for ( int i = 0; i < count; ++i )
  {
    QgsVertexId id = QgsVertexId( 0, 0, index );
    mCompoundCurve.deleteVertex( id );
  }

  if ( vertexCount() == 0 )
  {
    mCompoundCurve.addVertex( QgsPoint() );
  }

  if ( mCurrentCoordinateIndex >= vertexCount() )
  {
    setCurrentCoordinateIndex( vertexCount() - 1 );
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
  QgsVertexId id = QgsVertexId( 0, 0, mCurrentCoordinateIndex );
  QgsPoint currentPt = mCompoundCurve.vertexAt( id );
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
  QgsVertexId id = QgsVertexId( 0, 0, mCurrentCoordinateIndex );
  return mCompoundCurve.vertexAt( id );
}

QgsPoint RubberbandModel::firstCoordinate() const
{
  if ( mCompoundCurve.isEmpty() )
    return QgsPoint();

  QgsVertexId id = QgsVertexId( 0, 0, 0 );
  return mCompoundCurve.vertexAt( id );
}

QgsPoint RubberbandModel::lastCoordinate() const
{
  if ( mCompoundCurve.isEmpty() )
    return QgsPoint();
  QgsVertexId id = QgsVertexId( 0, 0, mCurrentCoordinateIndex > 0 ? mCurrentCoordinateIndex - 1 : 0 );
  return mCompoundCurve.vertexAt( id );
}

QgsPoint RubberbandModel::penultimateCoordinate() const
{
  if ( mCompoundCurve.vertexCount( 0, 0 ) < 3 )
    return QgsPoint();
  QgsVertexId id = QgsVertexId( 0, 0, mCurrentCoordinateIndex > 1 ? mCurrentCoordinateIndex - 2 : 0 );
  return mCompoundCurve.vertexAt( id );
}

void RubberbandModel::setCurrentCoordinate( const QgsPoint &currentCoordinate )
{
  // play safe, but try to find out
  if ( mCompoundCurve.isEmpty() )
    return;

  QgsVertexId id = QgsVertexId( 0, 0, mCurrentCoordinateIndex );
  if ( mCompoundCurve.vertexAt( id ) == currentCoordinate )
    return;

  if ( mFrozen )
    return;

  if ( mDuringCurveDrawing == false )
  {
    mCompoundCurve.moveVertex( id, currentCoordinate );
  }
  else
  {
    QgsCircularString *curve = new QgsCircularString( mLastStartCurvePoint, mLastMiddleCurvePoint, currentCoordinate );
    if ( mCompoundCurve.nCurves() != 0 )
    {
      const QgsCurve *lastSegment = mCompoundCurve.curveAt( mCompoundCurve.nCurves() - 1 );
      if ( lastSegment->hasCurvedSegments() == true )
      {
        mCompoundCurve.removeCurve( mCompoundCurve.nCurves() - 1 );
      }
      else
      {
        if ( lastSegment->vertexCount( 0, 0 ) > 2 )
        {
          removeVertices( mCurrentCoordinateIndex - 1, 1 );
          setCurrentCoordinateIndex( mCurrentCoordinateIndex + 1 );
        }
        else
        {
          mCompoundCurve.removeCurve( mCompoundCurve.nCurves() - 1 );
        }
      }
    }
    mCompoundCurve.addCurve( curve, true );
  }

  if ( !mLayer || QgsWkbTypes::hasM( mLayer->wkbType() ) )
  {
    if ( !std::isnan( mMeasureValue ) )
    {
      if ( QgsWkbTypes::hasM( mCompoundCurve.vertexAt( id ).wkbType() ) )
      {
        mCompoundCurve.vertexAt( id ).setM( mMeasureValue );
      }
      else
      {
        mCompoundCurve.vertexAt( id ).addMValue( mMeasureValue );
      }
    }
    else
    {
      mCompoundCurve.vertexAt( id ).dropMValue();
    }
  }
  mCurrentCoordinate = currentCoordinate;
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
  if ( mCompoundCurve.vertexCount( 0, 0 ) > 1 && currentCoordinate() == penultimateCoordinate() )
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
  if ( QSettings().value( "/QField/Digitizing/CurveEdition", true ).toBool() && LayerUtils::isCurvedGeometry( mLayer ) == true )
  {
    mDuringCurveDrawing = !mDuringCurveDrawing;
    if ( mDuringCurveDrawing == false )
    {
      mCompoundCurve.addVertex( mCurrentCoordinate );
      setCurrentCoordinateIndex( vertexCount() - 1 );
      emit vertexCountChanged();
    }
  }
}

void RubberbandModel::addCurve()
{
  mDuringCurveDrawing = false;
  QgsCircularString *curve = new QgsCircularString( mLastStartCurvePoint, mLastMiddleCurvePoint, mCurrentCoordinate );
  if ( mCompoundCurve.nCurves() != 0 )
  {
    mCompoundCurve.removeCurve( mCompoundCurve.nCurves() - 1 );
  }

  mCompoundCurve.addCurve( curve, true );
  mCompoundCurve.addVertex( mCurrentCoordinate );
  setCurrentCoordinateIndex( vertexCount() - 1 );
  emit vertexCountChanged();
}

void RubberbandModel::addMiddlePointCurve()
{
  mDuringCurveDrawing = true;
  mLastMiddleCurvePoint = currentCoordinate();
  mLastStartCurvePoint = lastCoordinate();
  setCurrentCoordinateIndex( mCurrentCoordinateIndex + 1 );
}

void RubberbandModel::removeCurve()
{
}

void RubberbandModel::reset()
{
  mCompoundCurve.clear();
  mDuringCurveDrawing = false;
  mCompoundCurve.addVertex( mCurrentCoordinate );
  emit verticesRemoved( 0, vertexCount() - 1 );
  setCurrentCoordinateIndex( 0 );
  emit vertexCountChanged();
  mFrozen = false;
  emit frozenChanged();
}

void RubberbandModel::setDataFromGeometry( QgsGeometry geometry, const QgsCoordinateReferenceSystem &crs )
{
  if ( geometry.type() != mGeometryType )
    return;

  QgsCoordinateTransform ct( crs, mCrs, QgsProject::instance()->transformContext() );
  geometry.transform( ct );

  mCompoundCurve.clear();
  const QgsAbstractGeometry *abstractGeom = geometry.constGet();
  if ( !abstractGeom )
    return;

  if ( LayerUtils::isCurvedGeometry( mLayer ) == false )
  {
    QgsVertexId vertexId;
    QgsPoint pt;
    while ( abstractGeom->nextVertex( vertexId, pt ) )
    {
      if ( vertexId.part > 1 || vertexId.ring > 0 )
      {
        break;
      }
      mCompoundCurve.addVertex( pt );
    }
  }
  else
  {
    if ( geometry.type() == Qgis::GeometryType::Polygon )
    {
      QgsCurvePolygon *curve = qgsgeometry_cast<QgsCurvePolygon *>( abstractGeom->clone() );
      mCompoundCurve = *qgsgeometry_cast<QgsCompoundCurve *>( curve->exteriorRing() );
    }
    else
    {
      mCompoundCurve = *qgsgeometry_cast<QgsCompoundCurve *>( abstractGeom->clone() );
    }
  }

  // for polygons, remove the last vertex which is a duplicate of the first vertex
  if ( geometry.type() == Qgis::GeometryType::Polygon )
  {
    mCompoundCurve.removeDuplicateNodes();
  }

  // insert the last point twice so the resutling rubberband's current coordinate property being modified (by e.g.
  // the GNSS position) will not replace the last vertex from the passed geometry
  mCompoundCurve.addVertex( mCompoundCurve.endPoint() );

  mCurrentCoordinateIndex = mCompoundCurve.vertexCount( 0, 0 ) - 1;

  emit verticesInserted( 0, mCompoundCurve.vertexCount( 0, 0 ) );
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
