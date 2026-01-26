/***************************************************************************
  quick3dterraingeometry.h - Quick3DTerrainGeometry

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

#ifndef QUICK3DTERRAINGEOMETRY_H
#define QUICK3DTERRAINGEOMETRY_H

#include <QQuick3DGeometry>
#include <QVector3D>
#include <QVector>

/**
 * Custom Qt Quick 3D geometry for rendering terrain meshes with height data.
 *
 * This class generates a triangulated mesh grid with vertex positions, normals, and
 * texture coordinates based on provided elevation data. The geometry is suitable for
 * use with Qt Quick 3D Model items and supports dynamic updates when height data changes.
 *
 * \note QML Type: Quick3DTerrainGeometry
 * \ingroup core
 *
 */
class Quick3DTerrainGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_ELEMENT

    //! Number of vertices per terrain axis, determines mesh detail level
    Q_PROPERTY( int resolution READ resolution WRITE setResolution NOTIFY resolutionChanged )

    //! Width of the terrain mesh in 3D scene units
    Q_PROPERTY( float terrainWidth READ terrainWidth WRITE setTerrainWidth NOTIFY terrainWidthChanged )

    //! Depth of the terrain mesh in 3D scene units
    Q_PROPERTY( float terrainDepth READ terrainDepth WRITE setTerrainDepth NOTIFY terrainDepthChanged )

    //! Height data array for terrain elevation values
    Q_PROPERTY( QVariantList heightData WRITE setHeightData NOTIFY heightDataChanged )

  public:
    //! Creates a new terrain geometry
    explicit Quick3DTerrainGeometry( QQuick3DObject *parent = nullptr );

    //! Returns the number of vertices per terrain axis.
    int resolution() const { return mResolution; }

    //! Sets the resolution.
    void setResolution( int resolution );

    //! Returns the width of the terrain mesh in 3D scene units.
    float terrainWidth() const { return mTerrainWidth; }

    //! Sets the terrain width.
    void setTerrainWidth( float width );

    //! Returns the depth of the terrain mesh in 3D scene units.
    float terrainDepth() const { return mTerrainDepth; }

    //! Sets the terrain depth.
    void setTerrainDepth( float depth );

    //! Sets the height data array.
    void setHeightData( const QVariantList &data );

  signals:
    void resolutionChanged();
    void terrainWidthChanged();
    void terrainDepthChanged();
    void heightDataChanged();

  private:
    /**
     * Regenerates the terrain mesh geometry from current height data.
     *
     * This method constructs a triangulated grid by generating vertex positions,
     * per-vertex normals, and UV texture coordinates. The mesh is built using
     * a resolution×resolution grid where each quad is split into two triangles.
     * Vertex normals are calculated using the cross product of adjacent edges
     * for proper lighting. The geometry buffers are uploaded to the GPU through
     * Qt Quick 3D's geometry API.
     */
    void updateGeometry();

    QVector3D calculateNormal( int x, int z ) const;
    float getHeight( int x, int z ) const;

    int mResolution = 64;
    float mTerrainWidth = 1000.0f;
    float mTerrainDepth = 1000.0f;
    QVector<float> mHeights;
    bool mDirty = true;
};

#endif // QUICK3DTERRAINGEOMETRY_H
