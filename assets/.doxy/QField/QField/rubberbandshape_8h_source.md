

# File rubberbandshape.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**rubberbandshape.h**](rubberbandshape_8h.md)

[Go to the documentation of this file](rubberbandshape_8h.md)


```C++
/***************************************************************************
  rubberbandshape.h - RubberbandShape

 ---------------------
 begin                : 11.6.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef RUBBERBANDSHAPE_H
#define RUBBERBANDSHAPE_H

#include <QList>
#include <QQuickItem>
#include <qgspoint.h>
#include <qgswkbtypes.h>

class RubberbandModel;
class VertexModel;
class QgsQuickMapSettings;

class RubberbandShape : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( bool freeze READ freeze WRITE setFreeze NOTIFY freezeChanged )

    Q_PROPERTY( RubberbandModel *model READ model WRITE setModel NOTIFY modelChanged )
    Q_PROPERTY( VertexModel *vertexModel READ vertexModel WRITE setVertexModel NOTIFY vertexModelChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    
    Q_PROPERTY( Qgis::GeometryType geometryType READ geometryType WRITE setGeometryType NOTIFY geometryTypeChanged )

    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )
    Q_PROPERTY( QColor outlineColor READ outlineColor WRITE setOutlineColor NOTIFY outlineColorChanged )
    Q_PROPERTY( qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged )

    Q_PROPERTY( QList<QList<QPointF>> polylines READ polylines NOTIFY polylinesChanged )
    Q_PROPERTY( Qgis::GeometryType polylinesType READ polylinesType NOTIFY polylinesTypeChanged )

  public:
    explicit RubberbandShape( QQuickItem *parent = nullptr );

    RubberbandModel *model() const;
    void setModel( RubberbandModel *model );

    VertexModel *vertexModel() const;
    void setVertexModel( VertexModel *vertexModel );

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    bool freeze() const;
    void setFreeze( bool freeze );

    QColor color() const;
    void setColor( const QColor &color );

    QColor outlineColor() const;
    void setOutlineColor( const QColor &color );

    float lineWidth() const;
    void setLineWidth( float width );

    Qgis::GeometryType geometryType() const { return mGeometryType; }
    void setGeometryType( const Qgis::GeometryType geometryType );

    QList<QList<QPointF>> polylines() const { return mPolylines; }

    Qgis::GeometryType polylinesType() const { return mPolylinesType; }

  signals:
    void modelChanged();
    void vertexModelChanged();
    void mapSettingsChanged();
    void freezeChanged();
    void colorChanged();
    void outlineColorChanged();
    void lineWidthChanged();
    void geometryTypeChanged();
    void polylinesChanged();
    void polylinesTypeChanged();

  private slots:
    void markDirty();
    void visibleExtentChanged();
    void rotationChanged();

  private:
    void updateTransform();
    void createPolylines();

    RubberbandModel *mRubberbandModel = nullptr;
    VertexModel *mVertexModel = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;
    bool mFreeze = false;
    bool mDirty = false;
    QColor mColor = QColor( 192, 57, 43, 150 );
    QColor mOutlineColor = QColor( 255, 255, 255, 100 );
    float mWidth = 4;
    Qgis::GeometryType mGeometryType = Qgis::GeometryType::Null;
    QgsPoint mGeometryCorner;
    double mGeometryMUPP = 0.0;
    QList<QList<QPointF>> mPolylines;
    Qgis::GeometryType mPolylinesType = Qgis::GeometryType::Null;
};


#endif // RUBBERBANDSHAPE_H
```


