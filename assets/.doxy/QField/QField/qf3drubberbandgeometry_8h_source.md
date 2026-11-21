

# File qf3drubberbandgeometry.h

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**qf3drubberbandgeometry.h**](qf3drubberbandgeometry_8h.md)

[Go to the documentation of this file](qf3drubberbandgeometry_8h.md)


```C++
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

class Qf3DRubberbandGeometry : public QQuick3DGeometry
{
    Q_OBJECT

    Q_PROPERTY( QfRubberbandModel *rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )
    Q_PROPERTY( Qf3DTerrainProvider *terrainProvider READ terrainProvider WRITE setTerrainProvider NOTIFY terrainProviderChanged )
    Q_PROPERTY( float radius READ radius WRITE setRadius NOTIFY radiusChanged )
    Q_PROPERTY( int segments READ segments WRITE setSegments NOTIFY segmentsChanged )
    Q_PROPERTY( float heightOffset READ heightOffset WRITE setHeightOffset NOTIFY heightOffsetChanged )
    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )

  public:
    explicit Qf3DRubberbandGeometry( QQuick3DObject *parent = nullptr );

    QfRubberbandModel *rubberbandModel() const { return mRubberbandModel; }
    void setRubberbandModel( QfRubberbandModel *model );

    Qf3DTerrainProvider *terrainProvider() const { return mTerrainProvider; }
    void setTerrainProvider( Qf3DTerrainProvider *provider );

    float radius() const { return mRadius; }
    void setRadius( float radius );

    int segments() const { return mSegments; }
    void setSegments( int segments );

    float heightOffset() const { return mHeightOffset; }
    void setHeightOffset( float offset );

    QColor color() const { return mColor; }
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
```


