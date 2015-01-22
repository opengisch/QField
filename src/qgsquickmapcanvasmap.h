/***************************************************************************
                            qgsquickmapcanvasmap.h
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSQUICKMAPCANVASMAP_H
#define QGSQUICKMAPCANVASMAP_H

#include <QtQuick/QQuickPaintedItem>
#include <QTimer>

#include "qgsmapcanvas.h"
#include "mapsettings.h"

class QgsQuickMapCanvasMap : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY( MapSettings* mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( bool parallelRendering READ parallelRendering WRITE setParallelRendering NOTIFY parallelRenderingChanged )

  public:
    QgsQuickMapCanvasMap( QQuickItem* parent = 0 );
    ~QgsQuickMapCanvasMap();

    QgsMapCanvas* mapCanvas();

    // QQuickPaintedItem interface
    void paint( QPainter* painter );

    void setParallelRendering( bool pr );
    bool parallelRendering();

    QgsPoint toMapCoordinates( QPoint canvasCoordinates );

    void setMapSettings( MapSettings* mapSettings );
    MapSettings* mapSettings() const;

  signals:
    void parallelRenderingChanged();
    void mapSettingsChanged();

  protected:
    void geometryChanged( const QRectF& newGeometry, const QRectF& oldGeometry );

  public slots:
    void zoom( QPointF center, qreal scale );
    void pan( QPointF oldPos, QPointF newPos );
    void refresh();

  private:
    QgsMapCanvas* mMapCanvas;
    bool mPinching;
    QPoint mPinchStartPoint;
    MapSettings* mMapSettings;
};

#endif // QGSQUICKMAPCANVASMAP_H
