

# File qfmaptoview3d.h

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**qfmaptoview3d.h**](qfmaptoview3d_8h.md)

[Go to the documentation of this file](qfmaptoview3d_8h.md)


```C++
/***************************************************************************
  qfmaptoview3d.h - QfMapToView3D

 ---------------------
 begin                : 9.3.2026
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

#ifndef QFMAPTOVIEW3D_H
#define QFMAPTOVIEW3D_H

#include "qf3dterrainprovider.h"

#include <QObject>
#include <QVector3D>
#include <qgspoint.h>
#include <qqmlintegration.h>

class QfMapToView3D : public QObject
{
    Q_OBJECT

    Q_PROPERTY( Qf3DTerrainProvider *terrainProvider READ terrainProvider WRITE setTerrainProvider NOTIFY terrainProviderChanged )

    
    Q_PROPERTY( QgsPoint mapPoint READ mapPoint WRITE setMapPoint NOTIFY mapPointChanged )

    Q_PROPERTY( float heightOffset READ heightOffset WRITE setHeightOffset NOTIFY heightOffsetChanged )

    Q_PROPERTY( QVector3D viewPoint READ viewPoint NOTIFY viewPointChanged )

  public:
    explicit QfMapToView3D( QObject *parent = nullptr );

    Qf3DTerrainProvider *terrainProvider() const;
    void setTerrainProvider( Qf3DTerrainProvider *provider );

    QgsPoint mapPoint() const;
    void setMapPoint( const QgsPoint &point );

    float heightOffset() const;
    void setHeightOffset( float offset );

    QVector3D viewPoint() const;

  signals:
    void terrainProviderChanged();
    void mapPointChanged();
    void heightOffsetChanged();
    void viewPointChanged();

  private slots:
    void transformPoint();

  private:
    Qf3DTerrainProvider *mTerrainProvider = nullptr;
    QgsPoint mMapPoint;
    float mHeightOffset = 0.0f;
    QVector3D mViewPoint;
};

#endif // QFMAPTOVIEW3D_H
```


