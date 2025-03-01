/***************************************************************************
  qgsquickmapcanvasmap.h
  --------------------------------------
  Date                 : 10.12.2014
  Copyright            : (C) 2014 by Matthias Kuhn
  Email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSQUICKMAPCANVASMAP_H
#define QGSQUICKMAPCANVASMAP_H

#include "qgsquickmapsettings.h"

#include <QFutureSynchronizer>
#include <QQuickItem>
#include <QTimer>
#include <qgsmapsettings.h>
#include <qgspoint.h>

#include <memory>

class QgsMapRendererParallelJob;
class QgsMapRendererQImageJob;
class QgsMapRendererCache;
class QgsLabelingResults;

/**
 * This class implements a visual Qt Quick Item that does map rendering
 * according to the current map settings. Client code is expected to use
 * MapCanvas item rather than using this class directly.
 *
 * QgsQuickMapCanvasMap instance internally creates QgsQuickMapSettings in
 * constructor. The QgsProject should be attached to the QgsQuickMapSettings.
 * The map settings for other QgsQuick components should be initialized from
 * QgsQuickMapCanvasMap's mapSettings
 *
 * \note QML Type: MapCanvasMap
 *
 * \sa QgsQuickMapCanvas
 *
 */
class QgsQuickMapCanvasMap : public QQuickItem
{
    Q_OBJECT

    /**
     * The mapSettings property contains configuration for rendering of the map.
     *
     * It should be used as a primary source of map settings (and project) for
     * all other components in the application.
     *
     * This is a readonly property.
     */
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings )

    /**
     * The bottom margin used by the map settings when calculating map extent or center.
     */
    Q_PROPERTY( double bottomMargin READ bottomMargin WRITE setBottomMargin NOTIFY bottomMarginChanged )

    /**
     * The right margin used by the map settings when calculating map extent or center.
     */
    Q_PROPERTY( double rightMargin READ rightMargin WRITE setRightMargin NOTIFY rightMarginChanged )

    /**
     * When freeze property is set to true, the map canvas does not refresh.
     * The value temporary changes during the rendering process.
     */
    Q_PROPERTY( bool freeze READ freeze WRITE setFreeze NOTIFY freezeChanged )

    /**
     * The isRendering property is set to true while a rendering job is pending for this map canvas map.
     * It can be used to show a notification icon about an ongoing rendering job.
     * This is a readonly property.
     */
    Q_PROPERTY( bool isRendering READ isRendering NOTIFY isRenderingChanged )

    /**
     * Interval in milliseconds after which the map canvas will be updated while a rendering job is ongoing.
     * This only has an effect if incrementalRendering is activated.
     * Default is 250 [ms].
     */
    Q_PROPERTY( int mapUpdateInterval READ mapUpdateInterval WRITE setMapUpdateInterval NOTIFY mapUpdateIntervalChanged )

    /**
     * When the incrementalRendering property is set to true, the automatic refresh of map canvas during rendering is allowed.
     */
    Q_PROPERTY( bool incrementalRendering READ incrementalRendering WRITE setIncrementalRendering NOTIFY incrementalRenderingChanged )

    /**
     * The quality property allows for an increase in rendering speed and memory usage reduction at the
     * cost of rendering quality.
     *
     * By default, the value is set to 1.0, providing for the best rendering. The lowest quality
     * value is 0.5.
     */
    Q_PROPERTY( double quality READ quality WRITE setQuality NOTIFY qualityChanged )

    /**
     * When the forceDeferredLayersRepaint property is set to true, all layer repaint signals will be deferred.
     */
    Q_PROPERTY( double forceDeferredLayersRepaint READ forceDeferredLayersRepaint WRITE setForceDeferredLayersRepaint NOTIFY forceDeferredLayersRepaintChanged )

    /**
     * When previewJobsEnabled is set to true, canvas map preview jobs (low priority
     * render jobs which render portions of the view just outside of the canvas
     * extent, to allow preview of these out-of-canvas areas when panning or zooming out
     * the map) while be rendered.
     */
    Q_PROPERTY( bool previewJobsEnabled READ previewJobsEnabled WRITE setPreviewJobsEnabled NOTIFY previewJobsEnabledChanged )

  public:
    //! Create map canvas map
    explicit QgsQuickMapCanvasMap( QQuickItem *parent = nullptr );
    ~QgsQuickMapCanvasMap();

    QSGNode *updatePaintNode( QSGNode *oldNode, QQuickItem::UpdatePaintNodeData * ) override;

    //! \copydoc QgsQuickMapCanvasMap::mapSettings
    QgsQuickMapSettings *mapSettings() const;

    //! \copydoc QgsQuickMapCanvasMap::freeze
    bool freeze() const;

    //! \copydoc QgsQuickMapCanvasMap::freeze
    void setFreeze( bool freeze );

    //! \copydoc QgsQuickMapCanvasMap::isRendering
    bool isRendering() const;

    //! \copydoc QgsQuickMapCanvasMap::mapUpdateInterval
    int mapUpdateInterval() const;

    //! \copydoc QgsQuickMapCanvasMap::mapUpdateInterval
    void setMapUpdateInterval( int mapUpdateInterval );

    //! \copydoc QgsQuickMapCanvasMap::incrementalRendering
    bool incrementalRendering() const;

    //! \copydoc QgsQuickMapCanvasMap::incrementalRendering
    void setIncrementalRendering( bool incrementalRendering );

    //! \copydoc QgsQuickMapCanvasMap::quality
    double quality() const;

    //! \copydoc QgsQuickMapCanvasMap::incrementalRendering
    void setQuality( double quality );

    //!\copydoc QgsQuickMapCanvasMap::forceDeferredLayersRepaint
    bool forceDeferredLayersRepaint() const;

    //!\copydoc QgsQuickMapCanvasMap::forceDeferredLayersRepaint
    void setForceDeferredLayersRepaint( bool deferred );

    //!\copydoc QgsQuickMapCanvasMap::bottomMargin
    double bottomMargin() const;

    //!\copydoc QgsQuickMapCanvasMap::bottomMargin
    void setBottomMargin( double bottomMargin );

    //!\copydoc QgsQuickMapCanvasMap::rightMargin
    double rightMargin() const;

    //!\copydoc QgsQuickMapCanvasMap::rightMargin
    void setRightMargin( double rightMargin );

    //!\copydoc QgsQuickMapCanvasMap::previewJobsEnabled
    bool previewJobsEnabled() const;

    //!\copydoc QgsQuickMapCanvasMap::previewJobsEnabled
    void setPreviewJobsEnabled( bool enabled );

    /**
     * Returns an image of the last successful map canvas rendering
     */
    QImage image() const { return mImage; }

  signals:

    /**
     * Signal is emitted when a rendering is starting
     */
    void renderStarting();

    /**
     * Signal is emitted when a canvas is refreshed
     */
    void mapCanvasRefreshed();

    //! \copydoc QgsQuickMapCanvasMap::freeze
    void freezeChanged();

    //! \copydoc QgsQuickMapCanvasMap::isRendering
    void isRenderingChanged();

    //!\copydoc QgsQuickMapCanvasMap::mapUpdateInterval
    void mapUpdateIntervalChanged();

    //!\copydoc QgsQuickMapCanvasMap::incrementalRendering
    void incrementalRenderingChanged();

    //!\copydoc QgsQuickMapCanvasMap::quality
    void qualityChanged();

    //!\copydoc QgsQuickMapCanvasMap::forceDeferredLayersRepaint
    void forceDeferredLayersRepaintChanged();

    //!\copydoc QgsQuickMapCanvasMap::bottomMargin
    void bottomMarginChanged();

    //!\copydoc QgsQuickMapCanvasMap::rightMargin
    void rightMarginChanged();

    //!\copydoc QgsQuickMapCanvasMap::previewJobsEnabled
    bool previewJobsEnabledChanged() const;

  protected:
    void geometryChange( const QRectF &newGeometry, const QRectF &oldGeometry ) override;

  public slots:
    //! Stop map rendering
    void stopRendering();

    /**
     * Change the map rotation by \a degrees.
     */
    void rotate( double degrees );

    /**
     * Set map setting's extent (zoom the map) on the center by given scale
     */
    void zoom( QPointF center, qreal scale );

    /**
     * Set map setting's extent (pan the map) based on the difference of positions
     */
    void pan( QPointF oldPos, QPointF newPos );

    /**
     * Refresh the map canvas.
     * Does nothing when output size of map settings is not set
     */
    void refresh();

    void startPreviewJobs();
    void stopPreviewJobs();
    void schedulePreviewJob( int number );

  private slots:
    void refreshMap();
    void renderJobUpdated();
    void renderJobFinished();
    void layerRepaintRequested( bool deferred );
    void startPreviewJob( int number );
    void previewJobFinished();

    void onWindowChanged( QQuickWindow *window );
    void onScreenChanged( QScreen *screen );
    void onExtentChanged();
    void onRotationChanged();
    void onLayersChanged();
    void onTemporalStateChanged();

  private:
    /**
     * Should only be called by stopRendering()!
     */
    void destroyJob( QgsMapRendererJob *job );
    QgsMapSettings prepareMapSettings() const;
    void updateTransform();
    void zoomToFullExtent();
    void clearTemporalCache();

    std::unique_ptr<QgsQuickMapSettings> mMapSettings;
    bool mPinching = false;
    QPoint mPinchStartPoint;
    QgsMapRendererParallelJob *mJob = nullptr;
    std::unique_ptr<QgsMapRendererCache> mCache;
    QgsLabelingResults *mLabelingResults = nullptr;
    QImage mImage;
    QgsMapSettings mImageMapSettings;
    QTimer mRefreshTimer;
    bool mDirty = false;
    bool mFreeze = false;
    QList<QMetaObject::Connection> mLayerConnections;
    QTimer mMapUpdateTimer;
    bool mIncrementalRendering = false;
    bool mSilentRefresh = false;
    bool mDeferredRefreshPending = false;
    double mQuality = 1.0;
    bool mForceDeferredLayersRepaint = false;

    QHash<QString, int> mLastLayerRenderTime;

    bool mPreviewJobsEnabled = false;
    QList<QgsMapRendererQImageJob *> mPreviewJobs;
    QTimer mPreviewTimer;
    QMetaObject::Connection mPreviewTimerConnection;
    QMap<int, QImage> mPreviewImages;

    QQuickWindow *mWindow = nullptr;
};

#endif // QGSQUICKMAPCANVASMAP_H
