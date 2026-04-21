/***************************************************************************
  quick3dgeometryutils.h - Quick3DGeometryUtils

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

#ifndef QUICK3DGEOMETRYUTILS_H
#define QUICK3DGEOMETRYUTILS_H

#include <QVector3D>
#include <QVector>

#include <algorithm>
#include <cmath>

/**
 * Mesh-building helpers shared between the rubberband and feature highlight
 * geometries. Each helper appends to a raw vertex/index buffer using a fixed
 * 10-float vertex layout: position(3) + normal(3) + rgba(4).
 *
 * \ingroup core
 */
namespace Quick3DGeometryUtils
{
  constexpr int VERTEX_STRIDE = 10 * sizeof( float );

  inline void updateBounds( QVector3D &minBound, QVector3D &maxBound, const QVector3D &pos )
  {
    minBound.setX( std::min( minBound.x(), pos.x() ) );
    minBound.setY( std::min( minBound.y(), pos.y() ) );
    minBound.setZ( std::min( minBound.z(), pos.z() ) );
    maxBound.setX( std::max( maxBound.x(), pos.x() ) );
    maxBound.setY( std::max( maxBound.y(), pos.y() ) );
    maxBound.setZ( std::max( maxBound.z(), pos.z() ) );
  }

  inline void writeVertex( float *&vptr, const QVector3D &pos, const QVector3D &normal, float r, float g, float b, float a )
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

  //! Extrudes a circular tube of \a segments sides and given \a radius along \a path.
  inline void generateTube( const QVector<QVector3D> &path,
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
        forward = QVector3D( 0, 0, 1 );

      QVector3D up( 0, 1, 0 );
      if ( std::fabs( QVector3D::dotProduct( forward, up ) ) > 0.99f )
        up = QVector3D( 1, 0, 0 );

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

  //! Generates a UV sphere centered at \a center.
  inline void generateSphere( const QVector3D &center,
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

  inline int sphereVertexCount( int stacks, int slices ) { return ( stacks + 1 ) * ( slices + 1 ); }
  inline int sphereIndexCount( int stacks, int slices ) { return stacks * slices * 6; }
} // namespace Quick3DGeometryUtils

#endif // QUICK3DGEOMETRYUTILS_H
