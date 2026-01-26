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

Quick3DTerrainGeometry::Quick3DTerrainGeometry( QQuick3DObject *parent )
  : QQuick3DGeometry( parent )
{
  mHeights.resize( mResolution * mResolution );
  mHeights.fill( 0.0f );
  updateGeometry();
}

void Quick3DTerrainGeometry::setResolution( int resolution )
{
  resolution = qBound( 2, resolution, 512 );
  if ( mResolution == resolution )
  {
    return;
  }

  mResolution = resolution;
  mDirty = true;
  emit resolutionChanged();
  updateGeometry();
}

void Quick3DTerrainGeometry::setTerrainWidth( float width )
{
  if ( width <= 0 )
  {
    width = 1.0f;
  }

  if ( qFuzzyCompare( mTerrainWidth, width ) )
  {
    return;
  }

  mTerrainWidth = width;
  mDirty = true;
  emit terrainWidthChanged();
  updateGeometry();
}

void Quick3DTerrainGeometry::setTerrainDepth( float depth )
{
  if ( depth <= 0 )
  {
    depth = 1.0f;
  }

  if ( qFuzzyCompare( mTerrainDepth, depth ) )
  {
    return;
  }

  mTerrainDepth = depth;
  mDirty = true;
  emit terrainDepthChanged();
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

  x = qBound( 0, x, mResolution - 1 );
  z = qBound( 0, z, mResolution - 1 );

  return mHeights[z * mResolution + x];
}

QVector3D Quick3DTerrainGeometry::calculateNormal( int x, int z ) const
{
  const float hL = getHeight( x - 1, z );
  const float hR = getHeight( x + 1, z );
  const float hD = getHeight( x, z - 1 );
  const float hU = getHeight( x, z + 1 );

  return QVector3D( hL - hR, 2.0f / 1, hD - hU ).normalized();
}

void Quick3DTerrainGeometry::updateGeometry()
{
  if ( !mDirty )
  {
    return;
  }

  const int expectedSize = mResolution * mResolution;
  if ( mHeights.size() != expectedSize )
  {
    mHeights.resize( expectedSize );
    mHeights.fill( 0.0f );
  }

  const int vertexCount = expectedSize;
  const int triangleCount = ( mResolution - 1 ) * ( mResolution - 1 ) * 2;
  const int indexCount = triangleCount * 3;
  const int stride = 8 * sizeof( float );

  QByteArray vertexData;
  vertexData.resize( vertexCount * stride );
  float *vptr = reinterpret_cast<float *>( vertexData.data() );

  QByteArray indexData;
  indexData.resize( indexCount * sizeof( quint32 ) );
  quint32 *iptr = reinterpret_cast<quint32 *>( indexData.data() );

  const float cellWidth = mTerrainWidth / ( mResolution - 1 );
  const float cellDepth = mTerrainDepth / ( mResolution - 1 );
  const float halfWidth = mTerrainWidth / 2.0f;
  const float halfDepth = mTerrainDepth / 2.0f;

  for ( int z = 0; z < mResolution; ++z )
  {
    for ( int x = 0; x < mResolution; ++x )
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

      *vptr++ = static_cast<float>( x ) / qMax( 1, mResolution - 1 );
      *vptr++ = static_cast<float>( z ) / qMax( 1, mResolution - 1 );
    }
  }

  for ( int z = 0; z < mResolution - 1; ++z )
  {
    for ( int x = 0; x < mResolution - 1; ++x )
    {
      const quint32 topLeft = z * mResolution + x;
      const quint32 topRight = topLeft + 1;
      const quint32 bottomLeft = topLeft + mResolution;
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
