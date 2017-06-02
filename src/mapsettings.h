/***************************************************************************

               ----------------------------------------------------
              date                 : 27.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H

#include <QObject>

#include <qgsrectangle.h>
#include <qgsmapthemecollection.h>
#include <qgsmapsettings.h>
#include <qgsproject.h>
#include <qgspointv2.h>

class MapSettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsRectangle extent READ extent WRITE setExtent NOTIFY extentChanged )
    Q_PROPERTY( QgsRectangle visibleExtent READ visibleExtent NOTIFY visibleExtentChanged )
    Q_PROPERTY( double mapUnitsPerPixel READ mapUnitsPerPixel NOTIFY mapUnitsPerPixelChanged )
    Q_PROPERTY( double rotation READ rotation WRITE setRotation NOTIFY rotationChanged )
    Q_PROPERTY( QSize outputSize READ outputSize WRITE setOutputSize NOTIFY outputSizeChanged )
    Q_PROPERTY( double outputDpi READ outputDpi WRITE setOutputDpi NOTIFY outputDpiChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem destinationCrs READ destinationCrs WRITE setDestinationCrs NOTIFY destinationCrsChanged )
    Q_PROPERTY( QList<QgsMapLayer*> layers READ layers WRITE setLayers NOTIFY layersChanged )

  public:
    MapSettings( QObject* parent = 0 );
    ~MapSettings();

    QgsRectangle extent() const;
    void setExtent( const QgsRectangle& extent );

    Q_INVOKABLE void setCenter( const QgsPointV2& center );

    double mapUnitsPerPixel() const;

    QgsRectangle visibleExtent() const;

    /**
     * Convert a map coordinate to screen pixel coordinates
     *
     * @param p A coordinate in map coordinates
     *
     * @return A coordinate in pixel / screen space
     */
    Q_INVOKABLE QPointF coordinateToScreen( const QgsPointV2& p ) const;


    /**
     * Convert a screen coordinate to a map coordinate
     *
     * @param p A coordinate in pixel / screen coordinates
     *
     * @return A coordinate in map coordinates
     */
    Q_INVOKABLE QgsPointV2 screenToCoordinate( const QPointF& p ) const;

    double rotation() const;
    void setRotation( double rotation );

    QgsMapSettings mapSettings() const;

    QSize outputSize() const;
    void setOutputSize( const QSize& outputSize );

    double outputDpi() const;
    void setOutputDpi( double outputDpi );

    QgsCoordinateReferenceSystem destinationCrs() const;
    void setDestinationCrs( const QgsCoordinateReferenceSystem& destinationCrs );

    QList<QgsMapLayer*> layers() const;
    void setLayers( const QList<QgsMapLayer*>& layers );

  signals:
    void extentChanged();
    void destinationCrsChanged();
    void mapUnitsPerPixelChanged();
    void rotationChanged();
    void visibleExtentChanged();
    void outputSizeChanged();
    void outputDpiChanged();
    void layersChanged();

  private slots:
    void onReadProject( const QDomDocument& doc );

  private:
    QgsMapSettings mMapSettings;

};

#endif // MAPSETTINGS_H
