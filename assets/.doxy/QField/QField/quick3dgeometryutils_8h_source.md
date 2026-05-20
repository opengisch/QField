

# File quick3dgeometryutils.h

[**File List**](files.md) **>** [**3d**](dir_8623093fea4462c874263d69f8dca7e9.md) **>** [**quick3dgeometryutils.h**](quick3dgeometryutils_8h.md)

[Go to the documentation of this file](quick3dgeometryutils_8h.md)


```C++
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

class QFIELD_CORE_EXPORT Quick3DGeometryUtils
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
                                     QVector3D &minBound, QVector3D &maxBound );

  private:
    static bool polygonIsEar( const QVector<QVector3D> &ring, const QVector<int> &indices, float eps, bool ccw, int prev, int cur, int next );
};

#endif // QUICK3DGEOMETRYUTILS_H
```


