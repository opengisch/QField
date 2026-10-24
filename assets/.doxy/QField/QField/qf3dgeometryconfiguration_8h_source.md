

# File qf3dgeometryconfiguration.h

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**qf3dgeometryconfiguration.h**](qf3dgeometryconfiguration_8h.md)

[Go to the documentation of this file](qf3dgeometryconfiguration_8h.md)


```C++
/***************************************************************************
  qf3dgeometryconfiguration.h - Qf3DGeometryConfiguration

 ---------------------
 begin                : 16.6.2026
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

#ifndef QF3DGEOMETRYCONFIGURATION_H
#define QF3DGEOMETRYCONFIGURATION_H

#include "qf3dgeometry.h"

#include <QColor>
#include <QQuickItem>
#include <qgscoordinatereferencesystem.h>

class Qf3DGeometryConfiguration : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( QString wkt READ wkt WRITE setWkt NOTIFY wktChanged )
    Q_PROPERTY( float heightOffset READ heightOffset WRITE setHeightOffset NOTIFY heightOffsetChanged )
    Q_PROPERTY( Qf3DGeometry::AltitudeClamping altitudeClamping READ altitudeClamping WRITE setAltitudeClamping NOTIFY altitudeClampingChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem crs READ crs WRITE setCrs NOTIFY crsChanged )

    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )
    Q_PROPERTY( float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged )

  public:
    explicit Qf3DGeometryConfiguration( QQuickItem *parent = nullptr );

    QString wkt() const { return mWkt; }
    void setWkt( const QString &wkt );

    QgsCoordinateReferenceSystem crs() const { return mCrs; }
    void setCrs( const QgsCoordinateReferenceSystem &crs );

    float heightOffset() const { return mHeightOffset; }
    void setHeightOffset( float offset );

    Qf3DGeometry::AltitudeClamping altitudeClamping() const { return mAltitudeClamping; }
    void setAltitudeClamping( Qf3DGeometry::AltitudeClamping clamping );

    QColor color() const { return mColor; }
    void setColor( const QColor &color );

    float lineWidth() const { return mLineWidth; }
    void setLineWidth( float width );

  signals:
    void wktChanged();
    void crsChanged();
    void heightOffsetChanged();
    void altitudeClampingChanged();
    void colorChanged();
    void lineWidthChanged();

  private:
    QString mWkt;
    QgsCoordinateReferenceSystem mCrs;
    float mHeightOffset = 20.0f;
    Qf3DGeometry::AltitudeClamping mAltitudeClamping = Qf3DGeometry::AltitudeClamping::Ignore;
    QColor mColor = QColor( 255, 102, 0 );
    float mLineWidth = 4.0f;
};

#endif // QF3DGEOMETRYCONFIGURATION_H
```


