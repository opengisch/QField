/***************************************************************************
  qfrubberbandshape.h - QfRubberbandShape

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
#ifndef QFRUBBERBANDSHAPE_H
#define QFRUBBERBANDSHAPE_H

#include <QList>
#include <QQuickItem>
#include <qgspoint.h>
#include <qgswkbtypes.h>

class QfRubberbandModel;
class QfVertexModel;
class QgsQuickMapSettings;

/**
 * @brief The QfRubberbandShape class is used to provide the shape data to draw rubber bands
 * on the map canvas using the QML Shape item.
 * It is aimed to be used with either a QfVertexModel or a QfRubberbandModel.
 * \ingroup core
 */
class QfRubberbandShape : public QQuickItem
{
    Q_OBJECT

    //! When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched
    Q_PROPERTY( bool freeze READ freeze WRITE setFreeze NOTIFY freezeChanged )

    Q_PROPERTY( QfRubberbandModel *model READ model WRITE setModel NOTIFY modelChanged )
    Q_PROPERTY( QfVertexModel *vertexModel READ vertexModel WRITE setVertexModel NOTIFY vertexModelChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    //! QfGeometry type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)
    Q_PROPERTY( Qgis::GeometryType geometryType READ geometryType WRITE setGeometryType NOTIFY geometryTypeChanged )

    //! Color of the rubberband
    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )
    //! Color of the rubberband outline
    Q_PROPERTY( QColor outlineColor READ outlineColor WRITE setOutlineColor NOTIFY outlineColorChanged )
    //! Line width of the rubberband
    Q_PROPERTY( qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged )

    //! List of polylines representing the rubber band
    Q_PROPERTY( QList<QPolygonF> polylines READ polylines NOTIFY polylinesChanged )
    //! The geometry type associated to the polylines
    Q_PROPERTY( Qgis::GeometryType polylinesType READ polylinesType NOTIFY polylinesTypeChanged )

  public:
    explicit QfRubberbandShape( QQuickItem *parent = nullptr );

    QfRubberbandModel *model() const;
    void setModel( QfRubberbandModel *model );

    QfVertexModel *vertexModel() const;
    void setVertexModel( QfVertexModel *vertexModel );

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    //! \copydoc freeze
    bool freeze() const;
    //! \copydoc freeze
    void setFreeze( bool freeze );

    //! \copydoc color
    QColor color() const;
    //! \copydoc color
    void setColor( const QColor &color );

    //! \copydoc outlineColor
    QColor outlineColor() const;
    //! \copydoc outlineColor
    void setOutlineColor( const QColor &color );

    //! \copydoc lineWidth
    float lineWidth() const;
    //! \copydoc lineWidth
    void setLineWidth( float width );

    //! \copydoc geometryType
    Qgis::GeometryType geometryType() const { return mGeometryType; }
    //! \copydoc geometryType
    void setGeometryType( const Qgis::GeometryType geometryType );

    //! \copydoc polylines
    QList<QPolygonF> polylines() const { return mPolylines; }

    //! \copydoc polylinesType
    Qgis::GeometryType polylinesType() const { return mPolylinesType; }

  signals:
    void modelChanged();
    void vertexModelChanged();
    void mapSettingsChanged();
    //! \copydoc freeze
    void freezeChanged();
    //! \copydoc color
    void colorChanged();
    //! \copydoc outlineColor
    void outlineColorChanged();
    //! \copydoc lineWidth
    void lineWidthChanged();
    //! \copydoc geometryType
    void geometryTypeChanged();
    //! \copydoc polylines
    void polylinesChanged();
    //! \copydoc polylinesType
    void polylinesTypeChanged();

  private slots:
    void markDirty();
    void visibleExtentChanged();
    void rotationChanged();

  private:
    void updateTransform();
    void createPolylines();

    QfRubberbandModel *mRubberbandModel = nullptr;
    QfVertexModel *mVertexModel = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;
    bool mFreeze = false;
    bool mDirty = false;
    QColor mColor = QColor( 192, 57, 43, 150 );
    QColor mOutlineColor = QColor( 255, 255, 255, 100 );
    float mWidth = 4;
    Qgis::GeometryType mGeometryType = Qgis::GeometryType::Null;
    QgsPoint mGeometryCorner;
    double mGeometryMUPP = 0.0;
    QList<QPolygonF> mPolylines;
    Qgis::GeometryType mPolylinesType = Qgis::GeometryType::Null;
};


#endif // QFRUBBERBANDSHAPE_H
