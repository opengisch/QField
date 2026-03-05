

# File quick3dterraingeometry.h

[**File List**](files.md) **>** [**3d**](dir_8623093fea4462c874263d69f8dca7e9.md) **>** [**quick3dterraingeometry.h**](quick3dterraingeometry_8h.md)

[Go to the documentation of this file](quick3dterraingeometry_8h.md)


```C++
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

class Quick3DTerrainGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY( QSize gridSize READ gridSize WRITE setGridSize NOTIFY gridSizeChanged )

    
    Q_PROPERTY( QSizeF size READ size WRITE setSize NOTIFY sizeChanged )

    Q_PROPERTY( QVariantList heightData WRITE setHeightData NOTIFY heightDataChanged )

  public:
    explicit Quick3DTerrainGeometry( QQuick3DObject *parent = nullptr );

    QSize gridSize() const { return mGridSize; }

    void setGridSize( const QSize &size );

    QSizeF size() const { return mSize; }

    void setSize( QSizeF size );

    void setHeightData( const QVariantList &data );

  signals:
    void gridSizeChanged();
    void sizeChanged();
    void heightDataChanged();

  private:
    void updateGeometry();

    QVector3D calculateNormal( int x, int z ) const;
    float getHeight( int x, int z ) const;

    QSize mGridSize = QSize( 64, 64 );
    QSizeF mSize;

    QVector<float> mHeights;
    bool mDirty = true;
};

#endif // QUICK3DTERRAINGEOMETRY_H
```


