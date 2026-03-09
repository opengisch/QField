/***************************************************************************
  maptoscreen3d.h - MapToScreen3D

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

#ifndef MAPTOSCREEN3D_H
#define MAPTOSCREEN3D_H

#include "quick3dterrainprovider.h"

#include <QObject>
#include <QVector3D>
#include <qgspoint.h>
#include <qqmlintegration.h>

/**
 * Transforms a map point to a 3D scene position using a terrain provider.
 *
 * The scene point is automatically updated when the map point or terrain
 * provider extent changes, enabling dynamic positioning of 3D elements.
 *
 * \note QML Type: MapToScreen3D
 * \ingroup core
 */
class MapToScreen3D : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    //! The terrain provider used for geo-to-3D coordinate conversion
    Q_PROPERTY( Quick3DTerrainProvider *terrainProvider READ terrainProvider WRITE setTerrainProvider NOTIFY terrainProviderChanged )

    //! The point in map coordinates
    Q_PROPERTY( QgsPoint mapPoint READ mapPoint WRITE setMapPoint NOTIFY mapPointChanged )

    //! Additional vertical offset above the terrain surface
    Q_PROPERTY( float heightOffset READ heightOffset WRITE setHeightOffset NOTIFY heightOffsetChanged )

    //! The point in 3D scene coordinates
    Q_PROPERTY( QVector3D scenePoint READ scenePoint NOTIFY scenePointChanged )

  public:
    explicit MapToScreen3D( QObject *parent = nullptr );

    Quick3DTerrainProvider *terrainProvider() const;
    void setTerrainProvider( Quick3DTerrainProvider *provider );

    QgsPoint mapPoint() const;
    void setMapPoint( const QgsPoint &point );

    float heightOffset() const;
    void setHeightOffset( float offset );

    QVector3D scenePoint() const;

  signals:
    void terrainProviderChanged();
    void mapPointChanged();
    void heightOffsetChanged();
    void scenePointChanged();

  private slots:
    void transformPoint();

  private:
    Quick3DTerrainProvider *mTerrainProvider = nullptr;
    QgsPoint mMapPoint;
    float mHeightOffset = 0.0f;
    QVector3D mScenePoint;
};

#endif // MAPTOSCREEN3D_H
