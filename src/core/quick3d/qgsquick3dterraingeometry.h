/***************************************************************************
  qgsquick3dterraingeometry.h - QgsQuick3DTerrainGeometry

 ---------------------
 begin                : 5.1.2026
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

#ifndef QGSQUICK3DTERRAINGEOMETRY_H
#define QGSQUICK3DTERRAINGEOMETRY_H

#include <QQuick3DGeometry>
#include <QVector3D>
#include <QVector>

class QgsQuick3DTerrainGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY( int resolution READ resolution WRITE setResolution NOTIFY resolutionChanged )
    Q_PROPERTY( float terrainWidth READ terrainWidth WRITE setTerrainWidth NOTIFY terrainWidthChanged )
    Q_PROPERTY( float terrainDepth READ terrainDepth WRITE setTerrainDepth NOTIFY terrainDepthChanged )
    Q_PROPERTY( QVariantList heightData WRITE setHeightData NOTIFY heightDataChanged )

  public:
    explicit QgsQuick3DTerrainGeometry( QQuick3DObject *parent = nullptr );

    int resolution() const { return mResolution; }
    void setResolution( int resolution );

    float terrainWidth() const { return mTerrainWidth; }
    void setTerrainWidth( float width );

    float terrainDepth() const { return mTerrainDepth; }
    void setTerrainDepth( float depth );

    void setHeightData( const QVariantList &data );

  signals:
    void resolutionChanged();
    void terrainWidthChanged();
    void terrainDepthChanged();
    void heightDataChanged();

  private:
    void updateGeometry();
    QVector3D calculateNormal( int x, int z ) const;
    float getHeight( int x, int z ) const;

    int mResolution = 64;
    float mTerrainWidth = 1000.0f;
    float mTerrainDepth = 1000.0f;
    QVector<float> mHeights;
    bool mDirty = true;
};

#endif // QGSQUICK3DTERRAINGEOMETRY_H
