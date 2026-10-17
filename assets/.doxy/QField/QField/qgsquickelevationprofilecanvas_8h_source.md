

# File qgsquickelevationprofilecanvas.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgsquick**](dir_19c3d7c8ba6dc95926fecc96295d1560.md) **>** [**qgsquickelevationprofilecanvas.h**](qgsquickelevationprofilecanvas_8h.md)

[Go to the documentation of this file](qgsquickelevationprofilecanvas_8h.md)


```C++
/***************************************************************************
                          qgselevationprofilecanvas.h
                          ---------------
    begin                : October 2022
    copyright            : (C) 2022 by Mathieu Pellerin
    email                : mathieu at opengis dot ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSELEVATIONPROFILECANVAS_H
#define QGSELEVATIONPROFILECANVAS_H

#include "qgscoordinatereferencesystem.h"
#include "qgsgeometry.h"
#include "qgsmaplayer.h"

#include <QQuickItem>

class QgsProfilePlotRenderer;
class QgsElevationProfilePlotItem;

class QgsQuickElevationProfileCanvas : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )

    Q_PROPERTY( QgsCoordinateReferenceSystem crs READ crs WRITE setCrs NOTIFY crsChanged )
    Q_PROPERTY( QgsGeometry profileCurve READ profileCurve WRITE setProfileCurve NOTIFY profileCurveChanged )
    Q_PROPERTY( double tolerance READ tolerance WRITE setTolerance NOTIFY toleranceChanged )

    Q_PROPERTY( QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged )
    Q_PROPERTY( QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged )
    Q_PROPERTY( QColor axisLabelColor READ axisLabelColor WRITE setAxisLabelColor NOTIFY axisLabelColorChanged )
    Q_PROPERTY( double axisLabelSize READ axisLabelSize WRITE setAxisLabelSize NOTIFY axisLabelSizeChanged )

    
    Q_PROPERTY( bool isRendering READ isRendering NOTIFY isRenderingChanged )

  public:
    explicit QgsQuickElevationProfileCanvas( QQuickItem *parent = nullptr );
    ~QgsQuickElevationProfileCanvas();

    QSGNode *updatePaintNode( QSGNode *oldNode, QQuickItem::UpdatePaintNodeData * ) override;

    void cancelJobs();

    bool isRendering() const;

    Q_INVOKABLE void refresh();

    QgsProject *project() const { return mProject; }

    void setProject( QgsProject *project );

    Q_INVOKABLE void populateLayersFromProject();

    QList<QgsMapLayer *> layers() const;

    QgsCoordinateReferenceSystem crs() const { return mCrs; }

    void setCrs( const QgsCoordinateReferenceSystem &crs );

    void setProfileCurve( QgsGeometry curve );

    QgsGeometry profileCurve() const { return mProfileCurve; };

    void setTolerance( double tolerance );

    double tolerance() const { return mTolerance; }

    void setVisiblePlotRange( double minimumDistance, double maximumDistance, double minimumElevation, double maximumElevation );

    QgsDoubleRange visibleDistanceRange() const;

    QgsDoubleRange visibleElevationRange() const;

    QColor backgroundColor() const;

    void setBackgroundColor( const QColor &color );

    QColor borderColor() const;

    void setBorderColor( const QColor &color );

    QColor axisLabelColor() const;

    void setAxisLabelColor( const QColor &color );

    double axisLabelSize() const;

    void setAxisLabelSize( double size );

  signals:

    void activeJobCountChanged( int count );

    void projectChanged();

    void crsChanged();

    void profileCurveChanged();

    void toleranceChanged();

    void isRenderingChanged();

    void backgroundColorChanged();

    void borderColorChanged();

    void axisLabelColorChanged();

    void axisLabelSizeChanged();

  protected:
    void geometryChange( const QRectF &newGeometry, const QRectF &oldGeometry ) override;

  public slots:

    Q_INVOKABLE void zoomFull();

    Q_INVOKABLE void zoomFullInRatio();

    Q_INVOKABLE void clear();

  private slots:

    void generationFinished();
    void onLayerProfileGenerationPropertyChanged();
    void onLayerProfileRendererPropertyChanged();
    void regenerateResultsForLayer();
    void scheduleDeferredRegeneration();
    void scheduleDeferredRedraw();
    void startDeferredRegeneration();
    void startDeferredRedraw();
    void refineResults();

  private:
    void setupLayerConnections( QgsMapLayer *layer, bool isDisconnect );
    void updateStyle();

    QgsCoordinateReferenceSystem mCrs;
    QgsProject *mProject = nullptr;

    QgsWeakMapLayerPointerList mLayers;

    QImage mImage;

    QgsElevationProfilePlotItem *mPlotItem = nullptr;
    QgsProfilePlotRenderer *mCurrentJob = nullptr;

    QTimer *mDeferredRegenerationTimer = nullptr;
    bool mDeferredRegenerationScheduled = false;
    QTimer *mDeferredRedrawTimer = nullptr;
    bool mDeferredRedrawScheduled = false;

    QgsGeometry mProfileCurve;
    double mTolerance = 0;

    bool mFirstDrawOccurred = false;

    bool mZoomFullWhenJobFinished = true;

    bool mForceRegenerationAfterCurrentJobCompletes = false;

    static constexpr double MAX_ERROR_PIXELS = 2;

    bool mDirty = false;

    QColor mBackgroundColor = QColor( 255, 255, 255 );
    QColor mBorderColor = QColor( 0, 0, 0 );
    QColor mAxisLabelColor = QColor( 0, 0, 0 );
    double mAxisLabelSize = 16;
};

#endif // QGSELEVATIONPROFILECANVAS_H
```


