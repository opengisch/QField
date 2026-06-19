/***************************************************************************
  quick3dgeometryconfiguration.h - Quick3DGeometryConfiguration

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

#ifndef QUICK3DGEOMETRYCONFIGURATION_H
#define QUICK3DGEOMETRYCONFIGURATION_H

#include <QColor>
#include <QQuickItem>
#include <qgscoordinatereferencesystem.h>

/**
 * \brief Lightweight data item used by plugins to inject geometry into the 3D map scene.
 *
 * \note QML Type: Quick3DGeometryConfiguration
 * \ingroup core
 */
class Quick3DGeometryConfiguration : public QQuickItem
{
    Q_OBJECT

    //! WKT representation of the geometry to render
    Q_PROPERTY( QString wkt READ wkt WRITE setWkt NOTIFY wktChanged )
    //! Coordinate reference system the geometry is expressed in
    Q_PROPERTY( QgsCoordinateReferenceSystem crs READ crs WRITE setCrs NOTIFY crsChanged )
    //! Color of the rendered geometry
    Q_PROPERTY( QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged )
    //! Outline tube thickness in scene units
    Q_PROPERTY( float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged )

  public:
    explicit Quick3DGeometryConfiguration( QQuickItem *parent = nullptr );

    QString wkt() const { return mWkt; }
    void setWkt( const QString &wkt );

    QgsCoordinateReferenceSystem crs() const { return mCrs; }
    void setCrs( const QgsCoordinateReferenceSystem &crs );

    QColor lineColor() const { return mLineColor; }
    void setLineColor( const QColor &color );

    float lineWidth() const { return mLineWidth; }
    void setLineWidth( float width );

  signals:
    void wktChanged();
    void crsChanged();
    void lineColorChanged();
    void lineWidthChanged();

  private:
    QString mWkt;
    QgsCoordinateReferenceSystem mCrs;
    QColor mLineColor = QColor( 255, 102, 0 );
    float mLineWidth = 4.0f;
};

#endif // QUICK3DGEOMETRYCONFIGURATION_H
