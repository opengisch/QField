/***************************************************************************
  quick3dgeometryhighlight.h - Quick3DGeometryHighlight

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

#ifndef QUICK3DGEOMETRYHIGHLIGHT_H
#define QUICK3DGEOMETRYHIGHLIGHT_H

#include "quick3dterrainprovider.h"

#include <QColor>
#include <QQuick3DGeometry>
#include <qgscoordinatereferencesystem.h>
#include <qgscoordinatetransform.h>
#include <qgsgeometry.h>

class QgsLineString;

/**
 * Generates 3D highlight geometry for an identified or selected feature on
 * top of the terrain mesh.
 *
 * Points become spheres, lines become extruded tubes with sphere joints, and
 * polygons get both an outline and a semi-transparent triangulated fill. The
 * geometry is reprojected from the source CRS to the map CRS before being
 * draped on the terrain surface.
 *
 * \note QML Type: Quick3DGeometryHighlight
 * \ingroup core
 */
class Quick3DGeometryHighlight : public QQuick3DGeometry
{
    Q_OBJECT
    QML_ELEMENT

    //! The geometry to highlight
    Q_PROPERTY( QgsGeometry qgsGeometry READ qgsGeometry WRITE setQgsGeometry NOTIFY qgsGeometryChanged )
    //! Coordinate reference system the geometry is expressed in
    Q_PROPERTY( QgsCoordinateReferenceSystem crs READ crs WRITE setCrs NOTIFY crsChanged )
    //! The terrain provider used for geo-to-3D coordinate conversion
    Q_PROPERTY( Quick3DTerrainProvider *terrainProvider READ terrainProvider WRITE setTerrainProvider NOTIFY terrainProviderChanged )
    //! Outline tube thickness in scene units
    Q_PROPERTY( float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged )
    //! Per-vertex color applied to the highlight mesh
    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )
    //! Extra vertical offset above the terrain surface
    Q_PROPERTY( float heightOffset READ heightOffset WRITE setHeightOffset NOTIFY heightOffsetChanged )
    //! Whether polygons get a semi-transparent fill in addition to the outline
    Q_PROPERTY( bool fillPolygons READ fillPolygons WRITE setFillPolygons NOTIFY fillPolygonsChanged )

  public:
    explicit Quick3DGeometryHighlight( QQuick3DObject *parent = nullptr );

    QgsGeometry qgsGeometry() const { return mGeometry; }
    void setQgsGeometry( const QgsGeometry &geometry );

    QgsCoordinateReferenceSystem crs() const { return mCrs; }
    void setCrs( const QgsCoordinateReferenceSystem &crs );

    Quick3DTerrainProvider *terrainProvider() const { return mTerrainProvider; }
    void setTerrainProvider( Quick3DTerrainProvider *provider );

    float lineWidth() const { return mLineWidth; }
    void setLineWidth( float width );

    QColor color() const { return mColor; }
    void setColor( const QColor &color );

    float heightOffset() const { return mHeightOffset; }
    void setHeightOffset( float offset );

    bool fillPolygons() const { return mFillPolygons; }
    void setFillPolygons( bool fill );

  signals:
    void qgsGeometryChanged();
    void crsChanged();
    void terrainProviderChanged();
    void lineWidthChanged();
    void colorChanged();
    void heightOffsetChanged();
    void fillPolygonsChanged();

  private slots:
    void markDirtyAndUpdate();

  private:
    void updateGeometry();

    //! Walks an abstract geometry and returns its rings/lines as 3D paths
    QVector<QVector<QVector3D>> buildPaths( const QgsAbstractGeometry *geom ) const;

    //! Converts the vertices of \a lineString into a 3D scene-space path, dropping un-projectable points
    QVector<QVector3D> ringToPath( const QgsLineString *lineString ) const;

    //! Resets the geometry buffers to an empty triangle mesh and triggers an update
    void resetGeometry();

    //! Uploads \a vertexData and \a indexData and configures the vertex attributes
    void finalize( const QByteArray &vertexData, const QByteArray &indexData, const QVector3D &lo, const QVector3D &hi );

    QgsGeometry mGeometry;
    QgsCoordinateReferenceSystem mCrs;
    QgsCoordinateTransform mTransform;
    Quick3DTerrainProvider *mTerrainProvider = nullptr;

    float mLineWidth = 3.0f;
    float mHeightOffset = 15.0f;
    bool mFillPolygons = true;
    bool mDirty = true;
    QColor mColor = QColor( 255, 255, 0 );
};

#endif // QUICK3DGEOMETRYHIGHLIGHT_H
