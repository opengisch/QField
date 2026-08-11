/***************************************************************************
  qf3drubberbandgeometry.h - Qf3DRubberbandGeometry

 ---------------------
 begin                : 9.2.2026
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

#ifndef QF3DRUBBERBANDGEOMETRY_H
#define QF3DRUBBERBANDGEOMETRY_H

#include "qf3dterrainprovider.h"
#include "qfrubberbandmodel.h"

#include <QColor>
#include <QQuick3DGeometry>

/**
 * Generates 3D tube + sphere geometry to visualize a rubberband path on terrain.
 *
 * Geo coordinates from a QfRubberbandModel are projected onto the terrain surface
 * and connected by extruded tube segments. A sphere joint is placed at each vertex.
 * Everything is packed into a single indexed triangle mesh with per-vertex colors.
 *
 * \note QML Type: Qf3DRubberbandGeometry
 * \ingroup core
 */
class Qf3DRubberbandGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_ELEMENT

    //! The rubberband model providing the vertex coordinates
    Q_PROPERTY( QfRubberbandModel *rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )
    //! The terrain provider used for geo-to-3D coordinate conversion
    Q_PROPERTY( Qf3DTerrainProvider *terrainProvider READ terrainProvider WRITE setTerrainProvider NOTIFY terrainProviderChanged )
    //! Tube thickness in scene units
    Q_PROPERTY( float radius READ radius WRITE setRadius NOTIFY radiusChanged )
    //! Number of sides in the tube cross-section
    Q_PROPERTY( int segments READ segments WRITE setSegments NOTIFY segmentsChanged )
    //! Extra vertical offset above the terrain surface
    Q_PROPERTY( float heightOffset READ heightOffset WRITE setHeightOffset NOTIFY heightOffsetChanged )
    //! Per-vertex color applied to both tubes and joint spheres
    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )

  public:
    explicit Qf3DRubberbandGeometry( QQuick3DObject *parent = nullptr );

    //! Returns the rubberband model providing vertex coordinates.
    QfRubberbandModel *rubberbandModel() const { return mRubberbandModel; }
    //! Sets the rubberband model.
    void setRubberbandModel( QfRubberbandModel *model );

    //! Returns the terrain provider used for geo-to-3D conversion.
    Qf3DTerrainProvider *terrainProvider() const { return mTerrainProvider; }
    //! Sets the terrain provider.
    void setTerrainProvider( Qf3DTerrainProvider *provider );

    //! Returns the tube radius in 3D scene units.
    float radius() const { return mRadius; }
    //! Sets the tube radius.
    void setRadius( float radius );

    //! Returns the number of segments around the tube circumference.
    int segments() const { return mSegments; }
    //! Sets the segment count.
    void setSegments( int segments );

    //! Returns the height offset above the terrain surface.
    float heightOffset() const { return mHeightOffset; }
    //! Sets the height offset.
    void setHeightOffset( float offset );

    //! Returns the color used for both tubes and sphere joints.
    QColor color() const { return mColor; }
    //! Sets the rubberband color.
    void setColor( const QColor &color );

  signals:
    void rubberbandModelChanged();
    void terrainProviderChanged();
    void radiusChanged();
    void segmentsChanged();
    void heightOffsetChanged();
    void colorChanged();

  private slots:
    void markDirtyAndUpdate();

  private:
    //! Rebuilds the whole tube + sphere mesh from scratch
    void updateGeometry();

    QfRubberbandModel *mRubberbandModel = nullptr;
    Qf3DTerrainProvider *mTerrainProvider = nullptr;

    float mRadius = 3.0f;
    int mSegments = 8;
    float mHeightOffset = 15.0f;
    bool mDirty = true;

    QColor mColor = QColor( 255, 240, 50 );
};

#endif // QF3DRUBBERBANDGEOMETRY_H
