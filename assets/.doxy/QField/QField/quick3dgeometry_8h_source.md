

# File quick3dgeometry.h

[**File List**](files.md) **>** [**3d**](dir_8623093fea4462c874263d69f8dca7e9.md) **>** [**quick3dgeometry.h**](quick3dgeometry_8h.md)

[Go to the documentation of this file](quick3dgeometry_8h.md)


```C++
/***************************************************************************
  quick3dgeometry.h - Quick3DGeometry

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

#ifndef QUICK3DGEOMETRY_H
#define QUICK3DGEOMETRY_H

#include "quick3dterrainprovider.h"

#include <QColor>
#include <QQuick3DGeometry>
#include <qgscoordinatereferencesystem.h>
#include <qgscoordinatetransform.h>
#include <qgsgeometry.h>

class QgsLineString;

class Quick3DGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_ELEMENT

  public:
    enum class AltitudeClamping
    {
      Ignore,
      ClampToGround,
      Absolute,
    };
    Q_ENUM( AltitudeClamping )

    
    Q_PROPERTY( QgsGeometry qgsGeometry READ qgsGeometry WRITE setQgsGeometry NOTIFY qgsGeometryChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem crs READ crs WRITE setCrs NOTIFY crsChanged )
    Q_PROPERTY( Quick3DTerrainProvider *terrainProvider READ terrainProvider WRITE setTerrainProvider NOTIFY terrainProviderChanged )
    Q_PROPERTY( float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged )
    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )
    Q_PROPERTY( float heightOffset READ heightOffset WRITE setHeightOffset NOTIFY heightOffsetChanged )
    Q_PROPERTY( bool fillPolygons READ fillPolygons WRITE setFillPolygons NOTIFY fillPolygonsChanged )
    Q_PROPERTY( AltitudeClamping altitudeClamping READ altitudeClamping WRITE setAltitudeClamping NOTIFY altitudeClampingChanged )

  public:
    explicit Quick3DGeometry( QQuick3DObject *parent = nullptr );

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

    AltitudeClamping altitudeClamping() const { return mAltitudeClamping; }
    void setAltitudeClamping( AltitudeClamping clamping );

  signals:
    void qgsGeometryChanged();
    void crsChanged();
    void terrainProviderChanged();
    void lineWidthChanged();
    void colorChanged();
    void heightOffsetChanged();
    void fillPolygonsChanged();
    void altitudeClampingChanged();

  private slots:
    void markDirtyAndUpdate();

  private:
    void updateGeometry();

    QVector3D vertexTo3D( double geoX, double geoY, double geometryZ ) const;

    QVector<QVector<QVector3D>> buildPaths( const QgsAbstractGeometry *geom ) const;

    QVector<QVector3D> lineToPath( const QgsLineString *lineString ) const;

    void resetGeometry();

    void finalize( const QByteArray &vertexData, const QByteArray &indexData, const QVector3D &minBound, const QVector3D &maxBound );

    QgsGeometry mGeometry;
    QgsCoordinateReferenceSystem mCrs;
    QgsCoordinateTransform mTransform;
    Quick3DTerrainProvider *mTerrainProvider = nullptr;

    float mLineWidth = 3.0f;
    float mHeightOffset = 15.0f;
    bool mFillPolygons = true;
    bool mDirty = true;
    AltitudeClamping mAltitudeClamping = AltitudeClamping::Ignore;
    QColor mColor = QColor( 255, 255, 0 );
};

#endif // QUICK3DGEOMETRY_H
```


