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

/**
 * Transforms a map point to a 3D position using a terrain provider.
 *
 * The 3D point is automatically updated when the map point or terrain
 * provider extent changes, enabling dynamic positioning of 3D elements.
 *
 * \note QML Type: QfMapToView3D
 * \ingroup _3d
 */
class QfMapToView3D : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    //! The terrain provider used for geo-to-3D coordinate conversion
    Q_PROPERTY( Qf3DTerrainProvider *terrainProvider READ terrainProvider WRITE setTerrainProvider NOTIFY terrainProviderChanged )

    //! The point in map coordinates
    Q_PROPERTY( QgsPoint mapPoint READ mapPoint WRITE setMapPoint NOTIFY mapPointChanged )

    //! Additional vertical offset above the terrain surface
    Q_PROPERTY( float heightOffset READ heightOffset WRITE setHeightOffset NOTIFY heightOffsetChanged )

    //! The point in 3D coordinates
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
