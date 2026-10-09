

# File qf3drubberbandgeometry.cpp

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**qf3drubberbandgeometry.cpp**](qf3drubberbandgeometry_8cpp.md)

[Go to the documentation of this file](qf3drubberbandgeometry_8cpp.md)


```C++
/***************************************************************************
  qf3drubberbandgeometry.cpp - Qf3DRubberbandGeometry

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

#include "qf3dgeometryutils.h"
#include "qf3drubberbandgeometry.h"

#include <QVector3D>
#include <QtMath>
#include <qgslinestring.h>
#include <qgsmultilinestring.h>

#include <algorithm>
#include <cmath>

Qf3DRubberbandGeometry::Qf3DRubberbandGeometry( QQuick3DObject *parent )
  : QQuick3DGeometry( parent )
{
}

void Qf3DRubberbandGeometry::setRubberbandModel( QfRubberbandModel *model )
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
    connect( mRubberbandModel, &QfRubberbandModel::currentCoordinateChanged, this, &Qf3DRubberbandGeometry::markDirtyAndUpdate );
    connect( mRubberbandModel, &QfRubberbandModel::vertexCountChanged, this, &Qf3DRubberbandGeometry::markDirtyAndUpdate );
    connect( mRubberbandModel, &QfRubberbandModel::verticesInserted, this, &Qf3DRubberbandGeometry::markDirtyAndUpdate );
    connect( mRubberbandModel, &QfRubberbandModel::verticesRemoved, this, &Qf3DRubberbandGeometry::markDirtyAndUpdate );
  }

  mDirty = true;
  emit rubberbandModelChanged();
  updateGeometry();
}

void Qf3DRubberbandGeometry::setTerrainProvider( Qf3DTerrainProvider *provider )
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
    connect( mTerrainProvider, &Qf3DTerrainProvider::extentChanged, this, &Qf3DRubberbandGeometry::markDirtyAndUpdate );
    connect( mTerrainProvider, &Qf3DTerrainProvider::normalizedDataChanged, this, &Qf3DRubberbandGeometry::markDirtyAndUpdate );
  }

  mDirty = true;
  emit terrainProviderChanged();
  updateGeometry();
}

void Qf3DRubberbandGeometry::setRadius( float radius )
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

void Qf3DRubberbandGeometry::setSegments( int segments )
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

void Qf3DRubberbandGeometry::setHeightOffset( float offset )
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

void Qf3DRubberbandGeometry::setColor( const QColor &color )
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

void Qf3DRubberbandGeometry::markDirtyAndUpdate()
{
  mDirty = true;
  updateGeometry();
}

void Qf3DRubberbandGeometry::updateGeometry()
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
  const int sphereVertexCount = Qf3DGeometryUtils::sphereVertexCount( sphereStacks, sphereSlices );
  const int sphereIndexCount = Qf3DGeometryUtils::sphereIndexCount( sphereStacks, sphereSlices );

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
  const int stride = Qf3DGeometryUtils::VERTEX_STRIDE;
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

  quint32 vertexOffset = 0;

  // --- Tubes: one circular cross-section ("ring") per path point ---
  for ( const QVector<QVector3D> &path : subPaths )
  {
    Qf3DGeometryUtils::generateTube( path, mSegments, mRadius, colR, colG, colB, colA, vptr, iptr, vertexOffset, minBound, maxBound );
  }

  // --- Sphere joints: a UV sphere at each path vertex ---
  for ( const QVector<QVector3D> &path : subPaths )
  {
    for ( const QVector3D &center : path )
    {
      Qf3DGeometryUtils::generateSphere( center, sphereRadius, sphereStacks, sphereSlices, colR, colG, colB, colA, vptr, iptr, vertexOffset, minBound, maxBound );
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


