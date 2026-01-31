

# File qgsquickmapcanvasmap.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgsquick**](dir_19c3d7c8ba6dc95926fecc96295d1560.md) **>** [**qgsquickmapcanvasmap.h**](qgsquickmapcanvasmap_8h.md)

[Go to the documentation of this file](qgsquickmapcanvasmap_8h.md)


```C++
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

class QgsQuickMapCanvasMap : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings )

    
    Q_PROPERTY( double bottomMargin READ bottomMargin WRITE setBottomMargin NOTIFY bottomMarginChanged )

    Q_PROPERTY( double rightMargin READ rightMargin WRITE setRightMargin NOTIFY rightMarginChanged )

    Q_PROPERTY( bool freeze READ freeze WRITE setFreeze NOTIFY freezeChanged )

    Q_PROPERTY( bool isRendering READ isRendering NOTIFY isRenderingChanged )

    Q_PROPERTY( int mapUpdateInterval READ mapUpdateInterval WRITE setMapUpdateInterval NOTIFY mapUpdateIntervalChanged )

    Q_PROPERTY( bool incrementalRendering READ incrementalRendering WRITE setIncrementalRendering NOTIFY incrementalRenderingChanged )

    Q_PROPERTY( double quality READ quality WRITE setQuality NOTIFY qualityChanged )

    Q_PROPERTY( bool smooth READ smooth WRITE setSmooth NOTIFY smoothChanged )

    Q_PROPERTY( double forceDeferredLayersRepaint READ forceDeferredLayersRepaint WRITE setForceDeferredLayersRepaint NOTIFY forceDeferredLayersRepaintChanged )

    Q_PROPERTY( bool previewJobsEnabled READ previewJobsEnabled WRITE setPreviewJobsEnabled NOTIFY previewJobsEnabledChanged )

    Q_PROPERTY( QList<int> previewJobsQuadrants READ previewJobsQuadrants WRITE setPreviewJobsQuadrants NOTIFY previewJobsQuadrantsChanged )

  public:
    explicit QgsQuickMapCanvasMap( QQuickItem *parent = nullptr );
    ~QgsQuickMapCanvasMap();

    QSGNode *updatePaintNode( QSGNode *oldNode, QQuickItem::UpdatePaintNodeData * ) override;

    QgsQuickMapSettings *mapSettings() const;

    bool freeze() const;

    void setFreeze( bool freeze );

    bool isRendering() const;

    int mapUpdateInterval() const;

    void setMapUpdateInterval( int mapUpdateInterval );

    bool incrementalRendering() const;

    void setIncrementalRendering( bool incrementalRendering );

    double quality() const;

    void setQuality( double quality );

    bool smooth() const;

    void setSmooth( bool smooth );

    bool forceDeferredLayersRepaint() const;

    void setForceDeferredLayersRepaint( bool deferred );

    double bottomMargin() const;

    void setBottomMargin( double bottomMargin );

    double rightMargin() const;

    void setRightMargin( double rightMargin );

    bool previewJobsEnabled() const;

    void setPreviewJobsEnabled( bool enabled );

    QList<int> previewJobsQuadrants() const;

    void setPreviewJobsQuadrants( const QList<int> &quadrants );

    QImage image() const { return mImage; }

  signals:

    void renderStarting();

    void mapCanvasRefreshed();

    void freezeChanged();

    void isRenderingChanged();

    void mapUpdateIntervalChanged();

    void incrementalRenderingChanged();

    void qualityChanged();

    void forceDeferredLayersRepaintChanged();

    void bottomMarginChanged();

    void rightMarginChanged();

    void previewJobsEnabledChanged() const;

    void previewJobsQuadrantsChanged() const;

    void smoothChanged() const;

  protected:
    void geometryChange( const QRectF &newGeometry, const QRectF &oldGeometry ) override;

  public slots:
    void stopRendering();

    void rotate( double degrees );

    void zoomByFactor( const QPointF center, qreal factor, bool handleMargins = false );

    void zoomScale( const QPointF center, qreal scale, bool handleMargins = false );

    void pan( const QPointF oldPos, const QPointF newPos );

    void refresh( bool ignoreFreeze = false );

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
    void onZRangeChanged();

  private:
    void destroyJob( QgsMapRendererJob *job );
    QgsMapSettings prepareMapSettings() const;
    void updateTransform( bool skipSmooth = false );
    void zoomToFullExtent();
    void clearTemporalCache();
    void clearElevationCache();

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
    bool mSmooth = false;
    bool mForceDeferredLayersRepaint = false;

    QHash<QString, int> mLastLayerRenderTime;

    bool mPreviewJobsEnabled = false;
    QList<int> mPreviewJobsQuadrants = { 0, 1, 2, 3, 5, 6, 7, 8 };
    QList<QgsMapRendererQImageJob *> mPreviewJobs;
    QTimer mPreviewTimer;
    QMetaObject::Connection mPreviewTimerConnection;
    QMap<int, QImage> mPreviewImages;

    QQuickWindow *mWindow = nullptr;
};

#endif // QGSQUICKMAPCANVASMAP_H
```


