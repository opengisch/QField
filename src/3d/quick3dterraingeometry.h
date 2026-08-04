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
#include <QSize>
#include <QVector3D>
#include <QVector>

class Quick3DTerrainProvider;

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

    //! Grid dimensions (width x height) for terrain vertices
    Q_PROPERTY( QSize gridSize READ gridSize WRITE setGridSize NOTIFY gridSizeChanged )

    //! Width of the terrain mesh in 3D scene units
    Q_PROPERTY( QSizeF size READ size WRITE setSize NOTIFY sizeChanged )

    //! Height data array for terrain elevation values
    Q_PROPERTY( QVariantList heightData READ heightData WRITE setHeightData NOTIFY heightDataChanged )

    //! Minimum bounds of the geometry
    Q_PROPERTY( QVector3D boundsMin READ boundsMin NOTIFY boundsMinMaxChanged )

    //! Maximum bounds of the geometry
    Q_PROPERTY( QVector3D boundsMax READ boundsMax NOTIFY boundsMinMaxChanged )

    //! Offset vector from the generated terrain heights
    Q_PROPERTY( QVector3D offsetVector READ offsetVector WRITE setOffsetVector NOTIFY offsetVectorChanged )

    //! Pffset scale from the last generated terrain data
    Q_PROPERTY( double offsetScale READ offsetScale WRITE setOffsetScale NOTIFY offsetScaleChanged )

  public:
    //! Creates a new terrain geometry
    explicit Quick3DTerrainGeometry( QQuick3DObject *parent = nullptr );

    //! Returns the grid dimensions.
    QSize gridSize() const { return mGridSize; }

    //! Sets the grid dimensions.
    void setGridSize( const QSize &size );

    //! Returns the width of the terrain mesh in 3D scene units.
    QSizeF size() const { return mSize; }

    //! Sets the terrain width.
    void setSize( const QSizeF &size );

    //! Returns the height data in a QML-friendly QVariantList
    QVariantList heightData() const;

    //! Sets the height data array.
    void setHeightData( const QVariantList &data );

    QVector3D offsetVector() const { return mOffsetVector; }

    void setOffsetVector( const QVector3D &offsetVector );

    double offsetScale() const { return mOffsetScale; }

    void setOffsetScale( double offsetScale );

    //! Builds and stores a 3x3 metagrid from the provider's normalized data
    Q_INVOKABLE void buildMetagridFromProvider( const Quick3DTerrainProvider *provider );

    //! Restores the original height data from the provider
    Q_INVOKABLE void restoreHeightsFromProvider( const Quick3DTerrainProvider *provider );

  signals:
    void gridSizeChanged();
    void sizeChanged();
    void heightDataChanged();
    void boundsMinMaxChanged();
    void offsetVectorChanged();
    void offsetScaleChanged();

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

    void applyShiftedHeights();

    QVector3D calculateNormal( int x, int z ) const;
    float getHeight( int x, int z ) const;

    QSize mGridSize = QSize( 64, 64 );
    QSizeF mSize;

    QVector<float> mHeights;

    QVector<float> mMetagridHeights;
    int mMetagridWidth = 0;
    int mMetagridHeight = 0;

    QVector3D mOffsetVector;
    double mOffsetScale = 1.0;

    bool mDirty = true;
};

#endif // QUICK3DTERRAINGEOMETRY_H
