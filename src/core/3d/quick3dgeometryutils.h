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

#include "qfield_core_export.h"

#include <QVector3D>
#include <QVector>

/**
 * Mesh-building helpers shared between the rubberband and feature highlight
 * geometries. Each helper appends to a raw vertex/index buffer using a fixed
 * 10-float vertex layout: position(3) + normal(3) + rgba(4).
 *
 * \ingroup core
 */
class QFIELD_CORE_EXPORT Quick3DGeometryUtils
{
  public:
    //! Byte stride of a single vertex: position(3) + normal(3) + rgba(4) = 10 floats.
    static constexpr int VERTEX_STRIDE = 10 * sizeof( float );

    //! Expands \a minBound / \a maxBound to include \a pos.
    static void updateBounds( QVector3D &minBound, QVector3D &maxBound, const QVector3D &pos );

    //! Writes a single vertex (position + normal + rgba) to \a vptr and advances the pointer.
    static void writeVertex( float *&vptr, const QVector3D &pos, const QVector3D &normal, float r, float g, float b, float a );

    //! Extrudes a circular tube of \a segments sides and given \a radius along \a path.
    static void generateTube( const QVector<QVector3D> &path,
                              int segments, float radius,
                              float r, float g, float b, float a,
                              float *&vptr, quint32 *&iptr,
                              quint32 &vertexOffset,
                              QVector3D &minBound, QVector3D &maxBound );

    //! Generates a UV sphere centered at \a center.
    static void generateSphere( const QVector3D &center,
                                float radius, int stacks, int slices,
                                float r, float g, float b, float a,
                                float *&vptr, quint32 *&iptr,
                                quint32 &vertexOffset,
                                QVector3D &minBound, QVector3D &maxBound );

    //! Returns the number of vertices produced by generateSphere() for the given subdivisions.
    static int sphereVertexCount( int stacks, int slices );

    //! Returns the number of indices produced by generateSphere() for the given subdivisions.
    static int sphereIndexCount( int stacks, int slices );

    /**
     * Triangulates a closed polygon ring on the XZ plane with ear clipping and
     * appends the resulting triangle fan to the buffer. Falls back to a simple
     * fan from vertex 0 when ear clipping cannot make further progress (which
     * keeps degenerate or self-touching rings from leaving uninitialised
     * indices in the buffer).
     */
    static void generatePolygonFill( const QVector<QVector3D> &vertices,
                                     float r, float g, float b, float a,
                                     float *&vptr, quint32 *&iptr,
                                     quint32 &vertexOffset,
                                     QVector3D &minBound, QVector3D &maxBound );

  private:
    //! Returns true if the triangle ( prev, cur, next ) of \a indices is an ear of \a ring on the XZ plane.
    static bool polygonIsEar( const QVector<QVector3D> &ring, const QVector<int> &indices, float eps, bool ccw, int prev, int cur, int next );
};

#endif // QUICK3DGEOMETRYUTILS_H
