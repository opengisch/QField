

# File quick3dterraingeometry.cpp

[**File List**](files.md) **>** [**3d**](dir_8623093fea4462c874263d69f8dca7e9.md) **>** [**quick3dterraingeometry.cpp**](quick3dterraingeometry_8cpp.md)

[Go to the documentation of this file](quick3dterraingeometry_8cpp.md)


```C++
/***************************************************************************
  quick3dterraingeometry.cpp - Quick3DTerrainGeometry

 ---------------------
 begin                : 26.1.2026
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

#include "quick3dterraingeometry.h"

#include <algorithm>

Quick3DTerrainGeometry::Quick3DTerrainGeometry( QQuick3DObject *parent )
  : QQuick3DGeometry( parent )
{
  mHeights.resize( static_cast<qsizetype>( mGridSize.width() ) * mGridSize.height() );
  mHeights.fill( 0.0f );
  updateGeometry();
}

void Quick3DTerrainGeometry::setGridSize( const QSize &size )
{
  QSize boundedSize( qBound( 2, size.width(), 512 ), qBound( 2, size.height(), 512 ) );
  if ( mGridSize == boundedSize )
  {
    return;
  }

  mGridSize = boundedSize;
  mDirty = true;
  emit gridSizeChanged();
  updateGeometry();
}

void Quick3DTerrainGeometry::setSize( QSizeF size )
{
  if ( mSize == size )
  {
    return;
  }

  mSize = size;
  emit sizeChanged();

  mDirty = true;
  updateGeometry();
}

void Quick3DTerrainGeometry::setHeightData( const QVariantList &data )
{
  mHeights.clear();
  mHeights.reserve( data.size() );

  for ( const QVariant &v : data )
  {
    mHeights.append( v.toFloat() );
  }

  mDirty = true;
  emit heightDataChanged();
  updateGeometry();
}

float Quick3DTerrainGeometry::getHeight( int x, int z ) const
{
  if ( mHeights.isEmpty() )
  {
    return 0.0f;
  }

  const int gridWidth = mGridSize.width();
  const int gridHeight = mGridSize.height();

  x = qBound( 0, x, gridWidth - 1 );
  z = qBound( 0, z, gridHeight - 1 );

  return mHeights[z * gridWidth + x];
}

QVector3D Quick3DTerrainGeometry::calculateNormal( int x, int z ) const
{
  const float cellWidth = mSize.width() / std::max( 1, mGridSize.width() - 1 );
  const float cellDepth = mSize.height() / std::max( 1, mGridSize.height() - 1 );

  const float hL = getHeight( x - 1, z );
  const float hR = getHeight( x + 1, z );
  const float hD = getHeight( x, z - 1 );
  const float hU = getHeight( x, z + 1 );

  // Calculate tangent vectors
  const QVector3D tangentX( 2.0f * cellWidth, hR - hL, 0.0f );
  const QVector3D tangentZ( 0.0f, hU - hD, 2.0f * cellDepth );

  // Normal is cross product of tangents
  return QVector3D::crossProduct( tangentZ, tangentX ).normalized();
}

void Quick3DTerrainGeometry::updateGeometry()
{
  if ( !mDirty )
  {
    return;
  }

  const int gridWidth = mGridSize.width();
  const int gridHeight = mGridSize.height();
  const int expectedSize = gridWidth * gridHeight;

  if ( mHeights.size() != expectedSize )
  {
    mHeights.resize( expectedSize );
    mHeights.fill( 0.0f );
  }

  const int vertexCount = expectedSize;
  const int triangleCount = ( gridWidth - 1 ) * ( gridHeight - 1 ) * 2;
  const int indexCount = triangleCount * 3;
  const int stride = 8 * sizeof( float );

  QByteArray vertexData;
  vertexData.resize( static_cast<qsizetype>( vertexCount ) * stride );
  float *vptr = reinterpret_cast<float *>( vertexData.data() );

  QByteArray indexData;
  indexData.resize( indexCount * sizeof( quint32 ) );
  quint32 *iptr = reinterpret_cast<quint32 *>( indexData.data() );

  const float cellWidth = mSize.width() / ( gridWidth - 1 );
  const float cellDepth = mSize.height() / ( gridHeight - 1 );
  const float halfWidth = mSize.width() / 2.0f;
  const float halfDepth = mSize.height() / 2.0f;

  for ( int z = 0; z < gridHeight; ++z )
  {
    for ( int x = 0; x < gridWidth; ++x )
    {
      const float px = x * cellWidth - halfWidth;
      const float py = getHeight( x, z );
      const float pz = z * cellDepth - halfDepth;

      *vptr++ = px;
      *vptr++ = py;
      *vptr++ = pz;

      const QVector3D normal = calculateNormal( x, z );
      *vptr++ = normal.x();
      *vptr++ = normal.y();
      *vptr++ = normal.z();

      *vptr++ = static_cast<float>( x ) / std::max( 1, gridWidth - 1 );
      *vptr++ = static_cast<float>( z ) / std::max( 1, gridHeight - 1 );
    }
  }

  for ( int z = 0; z < gridHeight - 1; ++z )
  {
    for ( int x = 0; x < gridWidth - 1; ++x )
    {
      const quint32 topLeft = z * gridWidth + x;
      const quint32 topRight = topLeft + 1;
      const quint32 bottomLeft = topLeft + gridWidth;
      const quint32 bottomRight = bottomLeft + 1;

      *iptr++ = topLeft;
      *iptr++ = bottomLeft;
      *iptr++ = topRight;

      *iptr++ = topRight;
      *iptr++ = bottomLeft;
      *iptr++ = bottomRight;
    }
  }

  clear();
  setVertexData( vertexData );
  setIndexData( indexData );
  setStride( stride );

  addAttribute( QQuick3DGeometry::Attribute::PositionSemantic, 0, QQuick3DGeometry::Attribute::F32Type );
  addAttribute( QQuick3DGeometry::Attribute::NormalSemantic, 3 * sizeof( float ), QQuick3DGeometry::Attribute::F32Type );
  addAttribute( QQuick3DGeometry::Attribute::TexCoordSemantic, 6 * sizeof( float ), QQuick3DGeometry::Attribute::F32Type );
  addAttribute( QQuick3DGeometry::Attribute::IndexSemantic, 0, QQuick3DGeometry::Attribute::U32Type );

  setPrimitiveType( QQuick3DGeometry::PrimitiveType::Triangles );

  const QVector3D minBound( -halfWidth, 0, -halfDepth );
  const QVector3D maxBound( halfWidth, 1, halfDepth );
  setBounds( minBound, maxBound );

  mDirty = false;
  update();
}
```


