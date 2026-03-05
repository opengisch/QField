

# File quick3drubberbandgeometry.cpp

[**File List**](files.md) **>** [**3d**](dir_8623093fea4462c874263d69f8dca7e9.md) **>** [**quick3drubberbandgeometry.cpp**](quick3drubberbandgeometry_8cpp.md)

[Go to the documentation of this file](quick3drubberbandgeometry_8cpp.md)


```C++
/***************************************************************************
  quick3drubberbandgeometry.cpp - Quick3DRubberbandGeometry

 ---------------------
 begin                : 9.2.2026
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

#include "quick3drubberbandgeometry.h"

#include <QVector3D>
#include <QtMath>
#include <qgslinestring.h>
#include <qgsmultilinestring.h>

#include <algorithm>
#include <cmath>

Quick3DRubberbandGeometry::Quick3DRubberbandGeometry( QQuick3DObject *parent )
  : QQuick3DGeometry( parent )
{
}

void Quick3DRubberbandGeometry::setRubberbandModel( RubberbandModel *model )
{
  if ( mRubberbandModel == model )
  {
    return;
  }

  if ( mRubberbandModel )
  {
    disconnect( mRubberbandModel );
  }

  mRubberbandModel = model;

  if ( mRubberbandModel )
  {
    connect( mRubberbandModel, &RubberbandModel::currentCoordinateChanged, this, &Quick3DRubberbandGeometry::markDirtyAndUpdate );
    connect( mRubberbandModel, &RubberbandModel::vertexCountChanged, this, &Quick3DRubberbandGeometry::markDirtyAndUpdate );
    connect( mRubberbandModel, &RubberbandModel::verticesInserted, this, &Quick3DRubberbandGeometry::markDirtyAndUpdate );
    connect( mRubberbandModel, &RubberbandModel::verticesRemoved, this, &Quick3DRubberbandGeometry::markDirtyAndUpdate );
  }

  mDirty = true;
  emit rubberbandModelChanged();
  updateGeometry();
}

void Quick3DRubberbandGeometry::setTerrainProvider( Quick3DTerrainProvider *provider )
{
  if ( mTerrainProvider == provider )
  {
    return;
  }

  if ( mTerrainProvider )
  {
    disconnect( mTerrainProvider );
  }

  mTerrainProvider = provider;

  if ( mTerrainProvider )
  {
    connect( mTerrainProvider, &Quick3DTerrainProvider::terrainDataReady, this, &Quick3DRubberbandGeometry::markDirtyAndUpdate );
  }

  mDirty = true;
  emit terrainProviderChanged();
  updateGeometry();
}

void Quick3DRubberbandGeometry::setRadius( float radius )
{
  radius = std::max( 0.1f, radius );
  if ( qFuzzyCompare( mRadius, radius ) )
  {
    return;
  }

  mRadius = radius;
  mDirty = true;
  emit radiusChanged();
  updateGeometry();
}

void Quick3DRubberbandGeometry::setSegments( int segments )
{
  segments = qBound( 3, segments, 32 );
  if ( mSegments == segments )
  {
    return;
  }

  mSegments = segments;
  mDirty = true;
  emit segmentsChanged();
  updateGeometry();
}

void Quick3DRubberbandGeometry::setHeightOffset( float offset )
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

void Quick3DRubberbandGeometry::setColor( const QColor &color )
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

void Quick3DRubberbandGeometry::markDirtyAndUpdate()
{
  mDirty = true;
  updateGeometry();
}

void Quick3DRubberbandGeometry::updateGeometry()
{
  if ( !mDirty )
  {
    return;
  }

  if ( !mRubberbandModel || !mTerrainProvider || mRubberbandModel->vertexCount() < 2 )
  {
    clear();
    setPrimitiveType( QQuick3DGeometry::PrimitiveType::Triangles );
    mDirty = false;
    update();
    return;
  }

  QVector<QVector<QVector3D>> subPaths;
  //const ;
  const QgsMultiLineString multiLineString( QList<QgsLineString>() << QgsLineString( mRubberbandModel->vertices() ) );
  const QgsGeometry lineGeometry( multiLineString.clone() );
  const QVector<QgsGeometry> lineCollection = lineGeometry.intersection( QgsGeometry::fromRect( mTerrainProvider->extent() ) ).asGeometryCollection();
  for ( const QgsGeometry &line : lineCollection )
  {
    QVector<QVector3D> current;
    QgsVertexIterator vertices = line.vertices();
    while ( vertices.hasNext() )
    {
      const QgsPoint vertex = vertices.next();
      const QVector3D pos = mTerrainProvider->geoTo3D( vertex.x(), vertex.y(), mHeightOffset );
      if ( !std::isnan( pos.x() ) )
      {
        current.append( pos );
      }
    }
    if ( current.size() > 1 )
    {
      subPaths.append( std::move( current ) );
    }
  }

  if ( subPaths.isEmpty() )
  {
    clear();
    setPrimitiveType( QQuick3DGeometry::PrimitiveType::Triangles );
    mDirty = false;
    update();
    return;
  }

  // UV sphere at each vertex – stacks are latitude rings, slices are longitude rings
  const float sphereRadius = mRadius * 2.25f;
  const int sphereStacks = 6;
  const int sphereSlices = 8;
  const int sphereVertexCount = ( sphereStacks + 1 ) * ( sphereSlices + 1 );
  const int sphereIndexCount = sphereStacks * sphereSlices * 6;

  // Calculate totals across all sub-paths
  int totalPoints = 0;
  int totalTubeConnections = 0;
  for ( const QVector<QVector3D> &path : subPaths )
  {
    totalPoints += path.size();
    totalTubeConnections += ( path.size() - 1 );
  }

  const int tubeVertexCount = totalPoints * mSegments;
  const int tubeIndexCount = totalTubeConnections * mSegments * 2 * 3;
  const int totalVertexCount = tubeVertexCount + totalPoints * sphereVertexCount;
  const int totalIndexCount = tubeIndexCount + totalPoints * sphereIndexCount;

  // Per-vertex layout: pos(3) + normal(3) + rgba(4) = 10 floats
  const int stride = 10 * sizeof( float );
  const float colR = mColor.redF();
  const float colG = mColor.greenF();
  const float colB = mColor.blueF();
  const float colA = mColor.alphaF();

  QByteArray vertexData;
  vertexData.resize( static_cast<qsizetype>( totalVertexCount ) * stride );
  float *vptr = reinterpret_cast<float *>( vertexData.data() );

  QByteArray indexData;
  indexData.resize( static_cast<qsizetype>( totalIndexCount ) * sizeof( quint32 ) );
  quint32 *iptr = reinterpret_cast<quint32 *>( indexData.data() );

  QVector3D minBound( std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
  QVector3D maxBound( std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() );

  auto updateBounds = [&]( const QVector3D &pos ) {
    minBound.setX( std::min( minBound.x(), pos.x() ) );
    minBound.setY( std::min( minBound.y(), pos.y() ) );
    minBound.setZ( std::min( minBound.z(), pos.z() ) );
    maxBound.setX( std::max( maxBound.x(), pos.x() ) );
    maxBound.setY( std::max( maxBound.y(), pos.y() ) );
    maxBound.setZ( std::max( maxBound.z(), pos.z() ) );
  };

  // --- Tubes: one circular cross-section ("ring") per path point ---
  quint32 tubeVertexOffset = 0;

  for ( const QVector<QVector3D> &path : subPaths )
  {
    const int ringCount = static_cast<int>( path.size() );

    for ( int ring = 0; ring < ringCount; ++ring )
    {
      // Tangent direction at this ring – averaged from neighbours for smooth bends
      QVector3D forward;
      if ( ring == 0 )
      {
        forward = ( path[1] - path[0] ).normalized();
      }
      else if ( ring == ringCount - 1 )
      {
        forward = ( path[ring] - path[ring - 1] ).normalized();
      }
      else
      {
        forward = ( ( path[ring] - path[ring - 1] ).normalized() + ( path[ring + 1] - path[ring] ).normalized() ).normalized();
      }

      if ( forward.isNull() )
      {
        forward = QVector3D( 0, 0, 1 );
      }

      // Build a local coordinate frame (right, up) perpendicular to the tube direction.
      // If the tube is nearly vertical, switch the reference vector to avoid degeneracy.
      QVector3D up( 0, 1, 0 );
      if ( std::fabs( QVector3D::dotProduct( forward, up ) ) > 0.99f )
      {
        up = QVector3D( 1, 0, 0 );
      }
      const QVector3D right = QVector3D::crossProduct( forward, up ).normalized();
      const QVector3D actualUp = QVector3D::crossProduct( right, forward ).normalized();

      for ( int seg = 0; seg < mSegments; ++seg )
      {
        const float angle = 2.0f * M_PI * seg / mSegments;
        const float cosA = qCos( angle );
        const float sinA = qSin( angle );

        const QVector3D normal = ( right * cosA + actualUp * sinA ).normalized();
        const QVector3D pos = path[ring] + normal * mRadius;

        *vptr++ = pos.x();
        *vptr++ = pos.y();
        *vptr++ = pos.z();
        *vptr++ = normal.x();
        *vptr++ = normal.y();
        *vptr++ = normal.z();
        *vptr++ = colR;
        *vptr++ = colG;
        *vptr++ = colB;
        *vptr++ = colA;

        updateBounds( pos );
      }
    }

    // Stitch adjacent rings into quads (2 triangles each)
    for ( int ring = 0; ring < ringCount - 1; ++ring )
    {
      const quint32 ringOffset = tubeVertexOffset + ring * mSegments;
      const quint32 nextRingOffset = tubeVertexOffset + ( ring + 1 ) * mSegments;

      for ( int seg = 0; seg < mSegments; ++seg )
      {
        const quint32 nextSeg = ( seg + 1 ) % mSegments;

        *iptr++ = ringOffset + seg;
        *iptr++ = nextRingOffset + seg;
        *iptr++ = ringOffset + nextSeg;

        *iptr++ = ringOffset + nextSeg;
        *iptr++ = nextRingOffset + seg;
        *iptr++ = nextRingOffset + nextSeg;
      }
    }

    tubeVertexOffset += ringCount * mSegments;
  }

  // --- Sphere joints: a UV sphere at each path vertex ---
  quint32 sphereBaseVertex = tubeVertexCount;

  for ( const QVector<QVector3D> &path : subPaths )
  {
    for ( int si = 0; si < path.size(); ++si )
    {
      const QVector3D &center = path[si];
      const quint32 thisBaseVertex = sphereBaseVertex;

      // Walk latitude (phi: 0=north pole → PI=south pole)
      for ( int stack = 0; stack <= sphereStacks; ++stack )
      {
        const float phi = M_PI * stack / sphereStacks;
        const float sinPhi = qSin( phi );
        const float cosPhi = qCos( phi );

        for ( int slice = 0; slice <= sphereSlices; ++slice )
        {
          const float theta = 2.0f * M_PI * slice / sphereSlices;
          const float sinTheta = qSin( theta );
          const float cosTheta = qCos( theta );

          const QVector3D normal( sinPhi * cosTheta, cosPhi, sinPhi * sinTheta );
          const QVector3D pos = center + normal * sphereRadius;

          *vptr++ = pos.x();
          *vptr++ = pos.y();
          *vptr++ = pos.z();
          *vptr++ = normal.x();
          *vptr++ = normal.y();
          *vptr++ = normal.z();
          *vptr++ = colR;
          *vptr++ = colG;
          *vptr++ = colB;
          *vptr++ = colA;

          updateBounds( pos );
        }
      }

      for ( int stack = 0; stack < sphereStacks; ++stack )
      {
        for ( int slice = 0; slice < sphereSlices; ++slice )
        {
          const quint32 first = thisBaseVertex + stack * ( sphereSlices + 1 ) + slice;
          const quint32 second = first + sphereSlices + 1;

          *iptr++ = first;
          *iptr++ = first + 1;
          *iptr++ = second;

          *iptr++ = first + 1;
          *iptr++ = second + 1;
          *iptr++ = second;
        }
      }

      sphereBaseVertex += sphereVertexCount;
    }
  }

  clear();
  setVertexData( vertexData );
  setIndexData( indexData );
  setStride( stride );

  addAttribute( QQuick3DGeometry::Attribute::PositionSemantic, 0, QQuick3DGeometry::Attribute::F32Type );
  addAttribute( QQuick3DGeometry::Attribute::NormalSemantic, 3 * sizeof( float ), QQuick3DGeometry::Attribute::F32Type );
  addAttribute( QQuick3DGeometry::Attribute::ColorSemantic, 6 * sizeof( float ), QQuick3DGeometry::Attribute::F32Type );
  addAttribute( QQuick3DGeometry::Attribute::IndexSemantic, 0, QQuick3DGeometry::Attribute::U32Type );

  setPrimitiveType( QQuick3DGeometry::PrimitiveType::Triangles );
  setBounds( minBound, maxBound );

  mDirty = false;
  update();
}
```


