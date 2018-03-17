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

#include <QtQuick/QQuickItem>
#include <QFutureSynchronizer>
#include <QTimer>
#include <qgspoint.h>
#include <qgsmapsettings.h>

class QgsMapRendererParallelJob;
class QgsMapRendererCache;
class QgsLabelingResults;
class MapSettings;

class QgsQuickMapCanvasMap : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( QgsCoordinateReferenceSystem destinationCrs READ destinationCrs WRITE setDestinationCrs NOTIFY destinationCrsChanged )
    Q_PROPERTY( MapSettings* mapSettings READ mapSettings )
    Q_PROPERTY( bool freeze READ freeze WRITE setFreeze NOTIFY freezeChanged )
    Q_PROPERTY( bool isRendering READ isRendering NOTIFY isRenderingChanged )
    /**
     * Interval in milliseconds after which the map canvas will be updated while a rendering job is ongoing.
     * This only has an effect if incrementalRendering is activated.
     * Default is 250 [ms].
     */
    Q_PROPERTY( int mapUpdateInterval READ mapUpdateInterval WRITE setMapUpdateInterval NOTIFY mapUpdateIntervalChanged )
    Q_PROPERTY( bool incrementalRendering READ incrementalRendering WRITE setIncrementalRendering NOTIFY incrementalRenderingChanged )

  public:
    QgsQuickMapCanvasMap( QQuickItem* parent = 0 );
    ~QgsQuickMapCanvasMap();

    QgsPoint toMapCoordinates( QPoint canvasCoordinates );

    MapSettings* mapSettings() const;

    QgsUnitTypes::DistanceUnit mapUnits() const;
    void setMapUnits( const QgsUnitTypes::DistanceUnit& mapUnits );

    QList<QgsMapLayer*> layerSet() const;
    void setLayerSet( const QList<QgsMapLayer*>& layerSet );

    bool hasCrsTransformEnabled() const;
    void setCrsTransformEnabled( bool hasCrsTransformEnabled );

    QgsCoordinateReferenceSystem destinationCrs() const;
    void setDestinationCrs( const QgsCoordinateReferenceSystem& destinationCrs );

    QgsRectangle extent() const;
    void setExtent( const QgsRectangle& extent );

    virtual QSGNode* updatePaintNode( QSGNode* oldNode, QQuickItem::UpdatePaintNodeData* );

    bool freeze() const;
    void setFreeze( bool freeze );

    bool isRendering() const;

    int mapUpdateInterval() const;
    void setMapUpdateInterval( int mapUpdateInterval );

    bool incrementalRendering() const;
    void setIncrementalRendering( bool incrementalRendering );

  signals:
    void renderStarting();

    void mapCanvasRefreshed();

    void extentChanged();

    void destinationCrsChanged();

    void freezeChanged();

    void isRenderingChanged();

    void mapUpdateIntervalChanged();

    void incrementalRenderingChanged();

  protected:
    void geometryChanged( const QRectF& newGeometry, const QRectF& oldGeometry );

  public slots:
    void stopRendering();

    void zoomToFullExtent();

    void zoom( QPointF center, qreal scale );
    void pan( QPointF oldPos, QPointF newPos );
    void refresh();

  private slots:
    void refreshMap();
    void renderJobUpdated();
    void renderJobFinished();
    void onWindowChanged( QQuickWindow* window );
    void onScreenChanged( QScreen* screen );
    void onExtentChanged();
    void onLayersChanged();

  private:
    /**
     * Should only be called ba stopRendering()!
     */
    void destroyJob( QgsMapRendererJob* job );
    QgsMapSettings prepareMapSettings() const;
    void updateTransform();

    MapSettings* mMapSettings;

    bool mPinching;
    QPoint mPinchStartPoint;
    QgsMapRendererParallelJob* mJob;
    QgsMapRendererCache* mCache;
    QgsLabelingResults* mLabelingResults;
    QImage mImage;
    QgsMapSettings mImageMapSettings;
    QTimer mRefreshTimer;
    bool mDirty;
    bool mFreeze;
    QList<QMetaObject::Connection> mLayerConnections;
    QTimer mMapUpdateTimer;
    int mMapUpdateInterval;
    bool mIncrementalRendering;
};

#endif // QGSQUICKMAPCANVASMAP_H
