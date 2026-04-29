/***************************************************************************
  quick3dgeometryutils.cpp - Quick3DGeometryUtils

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

#include "quick3dgeometryutils.h"

#include <algorithm>
#include <cmath>

void Quick3DGeometryUtils::updateBounds( QVector3D &minBound, QVector3D &maxBound, const QVector3D &pos )
{
  minBound.setX( std::min( minBound.x(), pos.x() ) );
  minBound.setY( std::min( minBound.y(), pos.y() ) );
  minBound.setZ( std::min( minBound.z(), pos.z() ) );
  maxBound.setX( std::max( maxBound.x(), pos.x() ) );
  maxBound.setY( std::max( maxBound.y(), pos.y() ) );
  maxBound.setZ( std::max( maxBound.z(), pos.z() ) );
}

void Quick3DGeometryUtils::writeVertex( float *&vptr, const QVector3D &pos, const QVector3D &normal, float r, float g, float b, float a )
{
  *vptr++ = pos.x();
  *vptr++ = pos.y();
  *vptr++ = pos.z();
  *vptr++ = normal.x();
  *vptr++ = normal.y();
  *vptr++ = normal.z();
  *vptr++ = r;
  *vptr++ = g;
  *vptr++ = b;
  *vptr++ = a;
}

void Quick3DGeometryUtils::generateTube( const QVector<QVector3D> &path,
                                         int segments, float radius,
                                         float r, float g, float b, float a,
                                         float *&vptr, quint32 *&iptr,
                                         quint32 &vertexOffset,
                                         QVector3D &minBound, QVector3D &maxBound )
{
  const int ringCount = static_cast<int>( path.size() );

  for ( int ring = 0; ring < ringCount; ++ring )
  {
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

    QVector3D up( 0, 1, 0 );
    if ( std::fabs( QVector3D::dotProduct( forward, up ) ) > 0.99f )
    {
      up = QVector3D( 1, 0, 0 );
    }

    const QVector3D right = QVector3D::crossProduct( forward, up ).normalized();
    const QVector3D actualUp = QVector3D::crossProduct( right, forward ).normalized();

    for ( int seg = 0; seg < segments; ++seg )
    {
      const float angle = 2.0f * static_cast<float>( M_PI ) * seg / segments;
      const QVector3D normal = ( right * std::cos( angle ) + actualUp * std::sin( angle ) ).normalized();
      const QVector3D pos = path[ring] + normal * radius;

      writeVertex( vptr, pos, normal, r, g, b, a );
      updateBounds( minBound, maxBound, pos );
    }
  }

  for ( int ring = 0; ring < ringCount - 1; ++ring )
  {
    const quint32 ringOffset = vertexOffset + ring * segments;
    const quint32 nextRingOffset = vertexOffset + ( ring + 1 ) * segments;

    for ( int seg = 0; seg < segments; ++seg )
    {
      const quint32 nextSeg = ( seg + 1 ) % segments;

      *iptr++ = ringOffset + seg;
      *iptr++ = nextRingOffset + seg;
      *iptr++ = ringOffset + nextSeg;

      *iptr++ = ringOffset + nextSeg;
      *iptr++ = nextRingOffset + seg;
      *iptr++ = nextRingOffset + nextSeg;
    }
  }

  vertexOffset += ringCount * segments;
}

void Quick3DGeometryUtils::generateSphere( const QVector3D &center,
                                           float radius, int stacks, int slices,
                                           float r, float g, float b, float a,
                                           float *&vptr, quint32 *&iptr,
                                           quint32 &vertexOffset,
                                           QVector3D &minBound, QVector3D &maxBound )
{
  const quint32 baseVertex = vertexOffset;

  for ( int stack = 0; stack <= stacks; ++stack )
  {
    const float phi = static_cast<float>( M_PI ) * stack / stacks;
    const float sinPhi = std::sin( phi );
    const float cosPhi = std::cos( phi );

    for ( int slice = 0; slice <= slices; ++slice )
    {
      const float theta = 2.0f * static_cast<float>( M_PI ) * slice / slices;
      const QVector3D normal( sinPhi * std::cos( theta ), cosPhi, sinPhi * std::sin( theta ) );
      const QVector3D pos = center + normal * radius;

      writeVertex( vptr, pos, normal, r, g, b, a );
      updateBounds( minBound, maxBound, pos );
    }
  }

  for ( int stack = 0; stack < stacks; ++stack )
  {
    for ( int slice = 0; slice < slices; ++slice )
    {
      const quint32 first = baseVertex + stack * ( slices + 1 ) + slice;
      const quint32 second = first + slices + 1;

      *iptr++ = first;
      *iptr++ = first + 1;
      *iptr++ = second;

      *iptr++ = first + 1;
      *iptr++ = second + 1;
      *iptr++ = second;
    }
  }

  vertexOffset += ( stacks + 1 ) * ( slices + 1 );
}

int Quick3DGeometryUtils::sphereVertexCount( int stacks, int slices )
{
  return ( stacks + 1 ) * ( slices + 1 );
}

int Quick3DGeometryUtils::sphereIndexCount( int stacks, int slices )
{
  return stacks * slices * 6;
}

bool Quick3DGeometryUtils::polygonIsEar( const QVector<QVector3D> &ring, const QVector<int> &indices, float eps, bool ccw, int prev, int cur, int next )
{
  const QVector3D &A = ring[indices[prev]];
  const QVector3D &B = ring[indices[cur]];
  const QVector3D &C = ring[indices[next]];

  const float cross = ( B.x() - A.x() ) * ( C.z() - A.z() ) - ( B.z() - A.z() ) * ( C.x() - A.x() );
  if ( ( ccw && cross < -eps ) || ( !ccw && cross > eps ) )
  {
    return false;
  }

  const int sz = indices.size();
  for ( int i = 0; i < sz; ++i )
  {
    if ( i == prev || i == cur || i == next )
    {
      continue;
    }

    const QVector3D &P = ring[indices[i]];
    const float d1 = ( P.x() - B.x() ) * ( A.z() - B.z() ) - ( A.x() - B.x() ) * ( P.z() - B.z() );
    const float d2 = ( P.x() - C.x() ) * ( B.z() - C.z() ) - ( B.x() - C.x() ) * ( P.z() - C.z() );
    const float d3 = ( P.x() - A.x() ) * ( C.z() - A.z() ) - ( C.x() - A.x() ) * ( P.z() - A.z() );

    const bool hasNeg = d1 < -eps || d2 < -eps || d3 < -eps;
    const bool hasPos = d1 > eps || d2 > eps || d3 > eps;
    if ( !( hasNeg && hasPos ) )
    {
      return false;
    }
  }
  return true;
}

void Quick3DGeometryUtils::generatePolygonFill( const QVector<QVector3D> &vertices,
                                                float r, float g, float b, float a,
                                                float *&vptr, quint32 *&iptr,
                                                quint32 &vertexOffset,
                                                QVector3D &minBound, QVector3D &maxBound )
{
  QVector<QVector3D> ring = vertices;
  if ( ring.size() > 3 && ( ring.first() - ring.last() ).length() < 0.001f )
  {
    ring.removeLast();
  }

  const int n = ring.size();
  if ( n < 3 )
  {
    return;
  }

  const quint32 baseVertex = vertexOffset;
  const QVector3D upNormal( 0.0f, 1.0f, 0.0f );

  for ( int i = 0; i < n; ++i )
  {
    writeVertex( vptr, ring[i], upNormal, r, g, b, a );
    updateBounds( minBound, maxBound, ring[i] );
  }
  vertexOffset += n;

  float minX = ring[0].x();
  float maxX = ring[0].x();
  float minZ = ring[0].z();
  float maxZ = ring[0].z();
  for ( int i = 1; i < n; ++i )
  {
    minX = std::min( minX, ring[i].x() );
    maxX = std::max( maxX, ring[i].x() );
    minZ = std::min( minZ, ring[i].z() );
    maxZ = std::max( maxZ, ring[i].z() );
  }
  const float eps = ( maxX - minX ) * ( maxZ - minZ ) * 1e-7f;

  QVector<int> indices;
  indices.reserve( n );
  for ( int i = 0; i < n; ++i )
  {
    indices.append( i );
  }

  // Shoelace winding
  double area2 = 0;
  for ( int i = 0; i < n; ++i )
  {
    const int j = ( i + 1 ) % n;
    area2 += static_cast<double>( ring[i].x() ) * ring[j].z() - static_cast<double>( ring[j].x() ) * ring[i].z();
  }
  const bool ccw = area2 > 0;

  const int expectedTriangles = n - 2;
  int trianglesWritten = 0;
  int safety = n * n;

  while ( indices.size() > 2 && safety-- > 0 )
  {
    bool earFound = false;
    const int sz = indices.size();
    for ( int i = 0; i < sz; ++i )
    {
      const int prev = ( i + sz - 1 ) % sz;
      const int next = ( i + 1 ) % sz;

      if ( polygonIsEar( ring, indices, eps, ccw, prev, i, next ) )
      {
        *iptr++ = baseVertex + indices[prev];
        *iptr++ = baseVertex + indices[i];
        *iptr++ = baseVertex + indices[next];
        indices.remove( i );
        earFound = true;
        ++trianglesWritten;
        break;
      }
    }
    if ( !earFound )
    {
      break;
    }
  }

  // Fan fallback for the unprocessed remainder
  for ( int i = 1; i < indices.size() - 1 && trianglesWritten < expectedTriangles; ++i )
  {
    *iptr++ = baseVertex + indices[0];
    *iptr++ = baseVertex + indices[i];
    *iptr++ = baseVertex + indices[i + 1];
    ++trianglesWritten;
  }
  while ( trianglesWritten < expectedTriangles )
  {
    *iptr++ = baseVertex;
    *iptr++ = baseVertex;
    *iptr++ = baseVertex;
    ++trianglesWritten;
  }
}
