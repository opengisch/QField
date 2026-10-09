

# File qf3dgeometry.cpp

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**qf3dgeometry.cpp**](qf3dgeometry_8cpp.md)

[Go to the documentation of this file](qf3dgeometry_8cpp.md)


```C++
/***************************************************************************
  qf3dgeometry.cpp - Qf3DGeometry

 ---------------------
 begin                : 20.4.2026
 copyright            : (C) 2026 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qf3dgeometry.h"
#include "qf3dgeometryutils.h"

#include <qgsgeometrycollection.h>
#include <qgslinestring.h>
#include <qgspoint.h>
#include <qgspolygon.h>

#include <algorithm>
#include <cmath>

Qf3DGeometry::Qf3DGeometry( QQuick3DObject *parent )
  : QQuick3DGeometry( parent )
{
}

void Qf3DGeometry::setQgsGeometry( const QgsGeometry &geometry )
{
  if ( mGeometry.isExactlyEqual( geometry ) )
  {
    return;
  }

  mGeometry = geometry;
  mDirty = true;
  emit qgsGeometryChanged();
  updateGeometry();
}

void Qf3DGeometry::setCrs( const QgsCoordinateReferenceSystem &crs )
{
  if ( mCrs == crs )
  {
    return;
  }

  mCrs = crs;
  if ( mTerrainProvider && mTerrainProvider->mapSettings() )
  {
    mTransform = QgsCoordinateTransform( mCrs,
                                         mTerrainProvider->mapSettings()->mapSettings().destinationCrs(),
                                         mTerrainProvider->mapSettings()->mapSettings().transformContext() );
  }

  mDirty = true;
  emit crsChanged();
  updateGeometry();
}

void Qf3DGeometry::setTerrainProvider( Qf3DTerrainProvider *provider )
{
  if ( mTerrainProvider == provider )
  {
    return;
  }

  if ( mTerrainProvider )
  {
    disconnect( mTerrainProvider, nullptr, this, nullptr );
  }

  mTerrainProvider = provider;

  if ( mTerrainProvider )
  {
    connect( mTerrainProvider, &Qf3DTerrainProvider::extentChanged, this, &Qf3DGeometry::markDirtyAndUpdate );
    connect( mTerrainProvider, &Qf3DTerrainProvider::normalizedDataChanged, this, &Qf3DGeometry::markDirtyAndUpdate );

    if ( mTerrainProvider->mapSettings() && mCrs.isValid() )
    {
      mTransform = QgsCoordinateTransform( mCrs,
                                           mTerrainProvider->mapSettings()->mapSettings().destinationCrs(),
                                           mTerrainProvider->mapSettings()->mapSettings().transformContext() );
    }
  }

  mDirty = true;
  emit terrainProviderChanged();
  updateGeometry();
}

void Qf3DGeometry::setLineWidth( float width )
{
  width = std::max( 0.1f, width );
  if ( qFuzzyCompare( mLineWidth, width ) )
  {
    return;
  }

  mLineWidth = width;
  mDirty = true;
  emit lineWidthChanged();
  updateGeometry();
}

void Qf3DGeometry::setColor( const QColor &color )
{
  if ( mColor == color )
  {
    return;
  }

  mColor = color;
  mDirty = true;
  emit colorChanged();
  updateGeometry();
}

void Qf3DGeometry::setHeightOffset( float offset )
{
  if ( qFuzzyCompare( mHeightOffset, offset ) )
  {
    return;
  }

  mHeightOffset = offset;
  mDirty = true;
  emit heightOffsetChanged();
  updateGeometry();
}

void Qf3DGeometry::setFillPolygons( bool fill )
{
  if ( mFillPolygons == fill )
  {
    return;
  }

  mFillPolygons = fill;
  mDirty = true;
  emit fillPolygonsChanged();
  updateGeometry();
}

void Qf3DGeometry::setAltitudeClamping( AltitudeClamping clamping )
{
  if ( mAltitudeClamping == clamping )
  {
    return;
  }

  mAltitudeClamping = clamping;
  mDirty = true;
  emit altitudeClampingChanged();
  updateGeometry();
}

void Qf3DGeometry::setExtrusion( float extrusion )
{
  extrusion = std::max( 0.0f, extrusion );
  if ( qFuzzyCompare( mExtrusion, extrusion ) )
  {
    return;
  }

  mExtrusion = extrusion;
  mDirty = true;
  emit extrusionChanged();
  updateGeometry();
}

void Qf3DGeometry::markDirtyAndUpdate()
{
  mDirty = true;
  updateGeometry();
}

QVector3D Qf3DGeometry::vertexTo3D( double geoX, double geoY, double geoZ ) const
{
  switch ( mAltitudeClamping )
  {
    case AltitudeClamping::Absolute:
    {
      QVector3D pos = mTerrainProvider->geoTo3D( QgsPoint( geoX, geoY, geoZ ), mHeightOffset );
      if ( std::isnan( pos.x() ) )
      {
        return pos;
      }
      return pos;
    }

    case AltitudeClamping::ClampToGround:
    {
      const float extraZ = std::isnan( geoZ ) ? 0.0f : static_cast<float>( geoZ );
      return mTerrainProvider->geoTo3D( geoX, geoY, mHeightOffset + extraZ );
    }

    case AltitudeClamping::Ignore:
      break;
  }

  return mTerrainProvider->geoTo3D( geoX, geoY, mHeightOffset );
}

QVector<QVector3D> Qf3DGeometry::lineToPath( const QgsLineString *lineString ) const
{
  QVector<QVector3D> path;
  path.reserve( lineString->numPoints() );
  const bool hasZ = lineString->is3D();
  for ( int i = 0; i < lineString->numPoints(); ++i )
  {
    const double z = hasZ ? lineString->zAt( i ) : std::numeric_limits<double>::quiet_NaN();
    const QVector3D pos = vertexTo3D( lineString->xAt( i ), lineString->yAt( i ), z );
    if ( !std::isnan( pos.x() ) )
    {
      path.append( pos );
    }
  }
  return path;
}

QVector<QVector<QVector3D>> Qf3DGeometry::buildPaths( const QgsAbstractGeometry *geom ) const
{
  QVector<QVector<QVector3D>> result;
  if ( !geom || !mTerrainProvider )
  {
    return result;
  }

  const QgsLineString *lineString = dynamic_cast<const QgsLineString *>( geom );
  if ( lineString )
  {
    QVector<QVector3D> path = lineToPath( lineString );
    if ( path.size() > 1 )
    {
      result.append( std::move( path ) );
    }
    return result;
  }

  const QgsPolygon *polygon = dynamic_cast<const QgsPolygon *>( geom );
  if ( polygon )
  {
    const QgsLineString *exteriorRing = dynamic_cast<const QgsLineString *>( polygon->exteriorRing() );
    if ( exteriorRing )
    {
      QVector<QVector3D> path = lineToPath( exteriorRing );
      if ( path.size() > 1 )
      {
        result.append( std::move( path ) );
      }
    }
    for ( int ringIndex = 0; ringIndex < polygon->numInteriorRings(); ++ringIndex )
    {
      const QgsLineString *ring = dynamic_cast<const QgsLineString *>( polygon->interiorRing( ringIndex ) );
      if ( ring )
      {
        QVector<QVector3D> path = lineToPath( ring );
        if ( path.size() > 1 )
        {
          result.append( std::move( path ) );
        }
      }
    }
    return result;
  }

  const QgsGeometryCollection *collection = dynamic_cast<const QgsGeometryCollection *>( geom );
  if ( collection )
  {
    for ( int i = 0; i < collection->numGeometries(); ++i )
    {
      result.append( buildPaths( collection->geometryN( i ) ) );
    }
  }

  return result;
}

void Qf3DGeometry::resetGeometry()
{
  clear();
  setPrimitiveType( QQuick3DGeometry::PrimitiveType::Triangles );
  mDirty = false;
  update();
}

void Qf3DGeometry::finalize( const QByteArray &vertexData, const QByteArray &indexData, const QVector3D &minBound, const QVector3D &maxBound )
{
  clear();
  setVertexData( vertexData );
  setIndexData( indexData );
  setStride( Qf3DGeometryUtils::VERTEX_STRIDE );
  addAttribute( QQuick3DGeometry::Attribute::PositionSemantic, 0, QQuick3DGeometry::Attribute::F32Type );
  addAttribute( QQuick3DGeometry::Attribute::NormalSemantic, 3 * sizeof( float ), QQuick3DGeometry::Attribute::F32Type );
  addAttribute( QQuick3DGeometry::Attribute::ColorSemantic, 6 * sizeof( float ), QQuick3DGeometry::Attribute::F32Type );
  addAttribute( QQuick3DGeometry::Attribute::IndexSemantic, 0, QQuick3DGeometry::Attribute::U32Type );
  setPrimitiveType( QQuick3DGeometry::PrimitiveType::Triangles );
  setBounds( minBound, maxBound );
  mDirty = false;
  update();
}

void Qf3DGeometry::updateGeometry()
{
  if ( !mDirty )
  {
    return;
  }

  if ( mGeometry.isNull() || !mTerrainProvider || mTerrainProvider->extent().isEmpty() )
  {
    resetGeometry();
    return;
  }

  QgsGeometry workingGeom = mGeometry;
  if ( mTransform.isValid() && !mTransform.isShortCircuited() )
  {
    try
    {
      workingGeom.transform( mTransform );
    }
    catch ( const QgsCsException & )
    {
      resetGeometry();
      return;
    }
  }

  workingGeom = workingGeom.intersection( QgsGeometry::fromRect( mTerrainProvider->extent() ) );
  if ( workingGeom.isNull() || workingGeom.isEmpty() )
  {
    resetGeometry();
    return;
  }

  const QgsAbstractGeometry *abstractGeom = workingGeom.constGet();
  if ( !abstractGeom )
  {
    resetGeometry();
    return;
  }

  const Qgis::GeometryType geomType = workingGeom.type();

  const int segments = 8;
  const int sphereStacks = 6;
  const int sphereSlices = 8;
  const float sphereRadius = mLineWidth * 2.25f;
  const int singleSphereVertexCount = Qf3DGeometryUtils::sphereVertexCount( sphereStacks, sphereSlices );
  const int singleSphereIndexCount = Qf3DGeometryUtils::sphereIndexCount( sphereStacks, sphereSlices );

  const float r = mColor.redF();
  const float g = mColor.greenF();
  const float b = mColor.blueF();
  const float a = mColor.alphaF();
  const float fillAlpha = 0.35f;

  QVector3D minBound( std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
  QVector3D maxBound( std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() );
  const int stride = Qf3DGeometryUtils::VERTEX_STRIDE;

  if ( geomType == Qgis::GeometryType::Point )
  {
    QVector<QVector3D> points;
    QgsVertexIterator vertexIterator = workingGeom.vertices();
    while ( vertexIterator.hasNext() )
    {
      const QgsPoint point = vertexIterator.next();
      const double z = point.is3D() ? point.z() : std::numeric_limits<double>::quiet_NaN();
      const QVector3D pos = vertexTo3D( point.x(), point.y(), z );
      if ( !std::isnan( pos.x() ) )
      {
        points.append( pos );
      }
    }

    if ( points.isEmpty() )
    {
      resetGeometry();
      return;
    }

    const int totalVertexCount = points.size() * singleSphereVertexCount;
    const int totalIndexCount = points.size() * singleSphereIndexCount;

    QByteArray vertexData;
    vertexData.resize( static_cast<qsizetype>( totalVertexCount ) * stride );
    QByteArray indexData;
    indexData.resize( static_cast<qsizetype>( totalIndexCount ) * sizeof( quint32 ) );

    float *vptr = reinterpret_cast<float *>( vertexData.data() );
    quint32 *iptr = reinterpret_cast<quint32 *>( indexData.data() );
    quint32 vertexOffset = 0;

    const float pointSphereRadius = mLineWidth * 3.5f;
    for ( const QVector3D &center : points )
    {
      Qf3DGeometryUtils::generateSphere( center, pointSphereRadius, sphereStacks, sphereSlices, r, g, b, a, vptr, iptr, vertexOffset, minBound, maxBound );
    }

    finalize( vertexData, indexData, minBound, maxBound );
    return;
  }

  const QVector<QVector<QVector3D>> paths = buildPaths( abstractGeom );
  if ( paths.isEmpty() )
  {
    resetGeometry();
    return;
  }

  const bool isPolygon = ( geomType == Qgis::GeometryType::Polygon );

  int totalPoints = 0;
  int totalConnections = 0;
  int totalFillVertices = 0;
  int totalFillIndices = 0;

  for ( const QVector<QVector3D> &path : paths )
  {
    totalPoints += path.size();
    totalConnections += path.size() - 1;

    if ( isPolygon && mFillPolygons && path.size() >= 3 )
    {
      int ringSize = path.size();
      if ( ringSize > 3 && ( path.first() - path.last() ).length() < 0.001f )
      {
        --ringSize;
      }
      totalFillVertices += ringSize;
      totalFillIndices += ( ringSize - 2 ) * 3;
      if ( mExtrusion > 0.0f )
      {
        totalFillVertices += Qf3DGeometryUtils::polygonWallsVertexCount( ringSize );
        totalFillIndices += Qf3DGeometryUtils::polygonWallsIndexCount( ringSize );
      }
    }
  }

  const int tubeVertexCount = totalPoints * segments;
  const int tubeIndexCount = totalConnections * segments * 2 * 3;
  const int sphereVertexTotal = totalPoints * singleSphereVertexCount;
  const int sphereIndexTotal = totalPoints * singleSphereIndexCount;

  const int totalVertexCount = tubeVertexCount + sphereVertexTotal + totalFillVertices;
  const int totalIndexCount = tubeIndexCount + sphereIndexTotal + totalFillIndices;

  QByteArray vertexData;
  vertexData.resize( static_cast<qsizetype>( totalVertexCount ) * stride );
  QByteArray indexData;
  indexData.resize( static_cast<qsizetype>( totalIndexCount ) * sizeof( quint32 ) );

  float *vptr = reinterpret_cast<float *>( vertexData.data() );
  quint32 *iptr = reinterpret_cast<quint32 *>( indexData.data() );
  quint32 vertexOffset = 0;

  for ( const QVector<QVector3D> &path : paths )
  {
    Qf3DGeometryUtils::generateTube( path, segments, mLineWidth, r, g, b, a, vptr, iptr, vertexOffset, minBound, maxBound );
  }

  for ( const QVector<QVector3D> &path : paths )
  {
    for ( const QVector3D &center : path )
    {
      Qf3DGeometryUtils::generateSphere( center, sphereRadius, sphereStacks, sphereSlices, r, g, b, a, vptr, iptr, vertexOffset, minBound, maxBound );
    }
  }

  if ( isPolygon && mFillPolygons )
  {
    for ( const QVector<QVector3D> &path : paths )
    {
      if ( path.size() >= 3 )
      {
        if ( mExtrusion > 0.0f )
        {
          Qf3DGeometryUtils::generatePolygonWalls( path, mExtrusion, r, g, b, a, vptr, iptr, vertexOffset, minBound, maxBound );
        }
        const float capAlpha = mExtrusion > 0.0f ? a : fillAlpha;
        Qf3DGeometryUtils::generatePolygonFill( path, r, g, b, capAlpha, vptr, iptr, vertexOffset, minBound, maxBound, mExtrusion );
      }
    }
  }

  finalize( vertexData, indexData, minBound, maxBound );
}
```


