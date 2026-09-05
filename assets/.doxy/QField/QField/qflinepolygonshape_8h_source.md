

# File qflinepolygonshape.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qflinepolygonshape.h**](qflinepolygonshape_8h.md)

[Go to the documentation of this file](qflinepolygonshape_8h.md)


```C++
/***************************************************************************
              qflinepolygonshape.h
               ----------------------------------------------------
              date                 : 21.11.2023
              copyright            : (C) 2023 by Mathieu Pellerin
              email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFLINEPOLYGONSHAPE_H
#define QFLINEPOLYGONSHAPE_H

#include "qfgeometrywrapper.h"
#include "qgsquickmapsettings.h"

#include <QQuickItem>

class QgsGeometry;

class QfLinePolygonShape : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )
    Q_PROPERTY( float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( QfGeometryWrapper *geometry READ geometry WRITE setGeometry NOTIFY geometryChanged )

    
    Q_PROPERTY( QList<QPolygonF> polylines READ polylines NOTIFY polylinesChanged )
    Q_PROPERTY( Qgis::GeometryType polylinesType READ polylinesType NOTIFY polylinesTypeChanged )

  public:
    explicit QfLinePolygonShape( QQuickItem *parent = nullptr );

    QfGeometryWrapper *geometry() const;
    void setGeometry( QfGeometryWrapper *geometry );

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QColor color() const;
    void setColor( const QColor &color );

    float lineWidth() const;
    void setLineWidth( float width );

    QList<QPolygonF> polylines() const { return mPolylines; }

    Qgis::GeometryType polylinesType() const { return mPolylinesType; }

  signals:
    void colorChanged();
    void lineWidthChanged();
    void mapSettingsChanged();
    void geometryChanged();
    void updated();
    void polylinesChanged();
    void polylinesTypeChanged();

  private slots:
    void rotationChanged();
    void mapCrsChanged();
    void visibleExtentChanged();
    void makeDirty();
    void catchGeometryWrapperChange();

  private:
    void updateTransform();
    void createPolylines();

    QColor mColor;
    float mWidth = 0;
    bool mDirty = false;
    QgsQuickMapSettings *mMapSettings = nullptr;
    QfGeometryWrapper *mGeometry = nullptr;
    QgsPointXY mGeometryCorner;
    double mGeometryMUPP = 0.0;
    QList<QPolygonF> mPolylines;
    Qgis::GeometryType mPolylinesType = Qgis::GeometryType::Null;

    static constexpr double MAX_SIZE = 250000;
    static constexpr double MAX_OFFSET = 200000;
};

#endif // QFLINEPOLYGONSHAPE_H
```


