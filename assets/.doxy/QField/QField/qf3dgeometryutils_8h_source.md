

# File qf3dgeometryutils.h

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**qf3dgeometryutils.h**](qf3dgeometryutils_8h.md)

[Go to the documentation of this file](qf3dgeometryutils_8h.md)


```C++
/***************************************************************************
  qf3dgeometryutils.h - Qf3DGeometryUtils

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

#ifndef QF3DGEOMETRYUTILS_H
#define QF3DGEOMETRYUTILS_H

#include "qfield_3d_export.h"

#include <QVector3D>
#include <QVector>

class QFIELD_3D_EXPORT Qf3DGeometryUtils
{
  public:
    static constexpr int VERTEX_STRIDE = 10 * sizeof( float );

    static void updateBounds( QVector3D &minBound, QVector3D &maxBound, const QVector3D &pos );

    static void writeVertex( float *&vptr, const QVector3D &pos, const QVector3D &normal, float r, float g, float b, float a );

    static void generateTube( const QVector<QVector3D> &path,
                              int segments, float radius,
                              float r, float g, float b, float a,
                              float *&vptr, quint32 *&iptr,
                              quint32 &vertexOffset,
                              QVector3D &minBound, QVector3D &maxBound );

    static void generateSphere( const QVector3D &center,
                                float radius, int stacks, int slices,
                                float r, float g, float b, float a,
                                float *&vptr, quint32 *&iptr,
                                quint32 &vertexOffset,
                                QVector3D &minBound, QVector3D &maxBound );

    static int sphereVertexCount( int stacks, int slices );

    static int sphereIndexCount( int stacks, int slices );

    static void generatePolygonFill( const QVector<QVector3D> &vertices,
                                     float r, float g, float b, float a,
                                     float *&vptr, quint32 *&iptr,
                                     quint32 &vertexOffset,
                                     QVector3D &minBound, QVector3D &maxBound,
                                     float extrusion = 0.0f );

    static void generatePolygonWalls( const QVector<QVector3D> &vertices,
                                      float extrusion,
                                      float r, float g, float b, float a,
                                      float *&vptr, quint32 *&iptr,
                                      quint32 &vertexOffset,
                                      QVector3D &minBound, QVector3D &maxBound );

    static int polygonWallsVertexCount( int ringSize ) { return ringSize * 4; }

    static int polygonWallsIndexCount( int ringSize ) { return ringSize * 6; }

  private:
    static bool polygonIsEar( const QVector<QVector3D> &ring, const QVector<int> &indices, float eps, bool ccw, int prev, int cur, int next );
};

#endif // QF3DGEOMETRYUTILS_H
```


