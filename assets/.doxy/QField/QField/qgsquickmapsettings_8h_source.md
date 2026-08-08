

# File qgsquickmapsettings.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgsquick**](dir_19c3d7c8ba6dc95926fecc96295d1560.md) **>** [**qgsquickmapsettings.h**](qgsquickmapsettings_8h.md)

[Go to the documentation of this file](qgsquickmapsettings_8h.md)


```C++
/***************************************************************************
  qgsquickmapsettings.h
  --------------------------------------
  Date                 : 27.12.2014
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

#ifndef QGSQUICKMAPSETTINGS_H
#define QGSQUICKMAPSETTINGS_H

#include "qfield_core_export.h"

#include <QObject>
#include <qgscoordinatetransformcontext.h>
#include <qgsmaplayer.h>
#include <qgsmapsettings.h>
#include <qgsmapthemecollection.h>
#include <qgspoint.h>
#include <qgsrectangle.h>

class QgsProject;

class QFIELD_CORE_EXPORT QgsQuickMapSettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )

    
    Q_PROPERTY( QgsPoint center READ getCenter WRITE setCenter NOTIFY extentChanged )
    Q_PROPERTY( QgsRectangle extent READ extent WRITE setExtent NOTIFY extentChanged )
    Q_PROPERTY( QgsRectangle visibleExtent READ visibleExtent NOTIFY visibleExtentChanged )
    Q_PROPERTY( double scale READ scale NOTIFY extentChanged )
    Q_PROPERTY( double mapUnitsPerPoint READ mapUnitsPerPoint NOTIFY mapUnitsPerPointChanged )

    Q_PROPERTY( double rotation READ rotation WRITE setRotation NOTIFY rotationChanged )

    Q_PROPERTY( QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged )

    Q_PROPERTY( QSize outputSize READ outputSize WRITE setOutputSize NOTIFY outputSizeChanged )

    Q_PROPERTY( double outputDpi READ outputDpi WRITE setOutputDpi NOTIFY outputDpiChanged )

    Q_PROPERTY( QgsCoordinateReferenceSystem destinationCrs READ destinationCrs WRITE setDestinationCrs NOTIFY destinationCrsChanged )

    Q_PROPERTY( QList<QgsMapLayer *> layers READ layers WRITE setLayers NOTIFY layersChanged )

    Q_PROPERTY( bool isTemporal READ isTemporal WRITE setIsTemporal NOTIFY temporalStateChanged )

    Q_PROPERTY( QDateTime temporalBegin READ temporalBegin WRITE setTemporalBegin NOTIFY temporalStateChanged )

    Q_PROPERTY( QDateTime temporalEnd READ temporalEnd WRITE setTemporalEnd NOTIFY temporalStateChanged )

    Q_PROPERTY( double zRangeLower READ zRangeLower WRITE setZRangeLower NOTIFY zRangeChanged )

    Q_PROPERTY( double zRangeUpper READ zRangeUpper WRITE setZRangeUpper NOTIFY zRangeChanged )

    Q_PROPERTY( double bottomMargin READ bottomMargin WRITE setBottomMargin NOTIFY bottomMarginChanged )

    Q_PROPERTY( double rightMargin READ rightMargin WRITE setRightMargin NOTIFY rightMarginChanged )

  public:
    explicit QgsQuickMapSettings( QObject *parent = nullptr );
    ~QgsQuickMapSettings() = default;

    QgsMapSettings mapSettings() const;

    QgsRectangle extent() const;

    Q_INVOKABLE void setExtent( const QgsRectangle &extent, bool handleMargins = false );

    void setProject( QgsProject *project );

    QgsProject *project() const;

    Q_INVOKABLE QgsPoint getCenter( bool handleMargins = false ) const;

    Q_INVOKABLE void setCenter( const QgsPoint &center, bool handleMargins = false );

    Q_INVOKABLE void setCenter( const QPointF &center, bool handleMargins = false );

    Q_INVOKABLE void setCenterToLayer( QgsMapLayer *layer, bool shouldZoom = true );

    Q_INVOKABLE void setExtentFromPoints( const QVariantList &points, const double &minimumScale = 0, bool handleMargins = false );

    double mapUnitsPerPoint() const;

    QgsRectangle visibleExtent() const;

    double scale() const;

    Q_INVOKABLE QgsCoordinateTransformContext transformContext() const;

    Q_INVOKABLE QPointF coordinateToScreen( const QgsPoint &point ) const;

    Q_INVOKABLE QgsPoint screenToCoordinate( const QPointF &point ) const;

    Q_INVOKABLE double computeScaleForExtent( const QgsRectangle &extent, bool handleMargins = false );

    void setTransformContext( const QgsCoordinateTransformContext &context );

    double rotation() const;

    void setRotation( double rotation );

    QColor backgroundColor() const;

    void setBackgroundColor( const QColor &color );

    QSize outputSize() const;

    void setOutputSize( QSize outputSize );

    double outputDpi() const;

    void setOutputDpi( double outputDpi );

    QgsCoordinateReferenceSystem destinationCrs() const;

    void setDestinationCrs( const QgsCoordinateReferenceSystem &destinationCrs );

    QList<QgsMapLayer *> layers() const;

    void setLayers( const QList<QgsMapLayer *> &layers );

    qreal devicePixelRatio() const;


    void setDevicePixelRatio( const qreal &devicePixelRatio );

    bool isTemporal() const;

    void setIsTemporal( bool temporal );

    QDateTime temporalBegin() const;

    void setTemporalBegin( const QDateTime &begin );

    QDateTime temporalEnd() const;

    void setTemporalEnd( const QDateTime &end );

    double zRangeLower() const;

    void setZRangeLower( double lower );

    double zRangeUpper() const;

    void setZRangeUpper( double upper );

    double bottomMargin() const;

    void setBottomMargin( double bottomMargin );

    double rightMargin() const;

    void setRightMargin( double rightMargin );

  signals:
    void projectChanged();

    void extentChanged();

    void destinationCrsChanged();

    void mapUnitsPerPointChanged();

    void rotationChanged();

    void backgroundColorChanged();

    void visibleExtentChanged();

    void outputSizeChanged();

    void outputDpiChanged();

    void layersChanged();

    void temporalStateChanged();

    void zRangeChanged();

    void bottomMarginChanged();

    void rightMarginChanged();

  private slots:

    void onReadProject( const QDomDocument &doc );

    void onCrsChanged();

  private:
    bool applyExtent( QgsMapSettings &mapSettings, const QgsRectangle &extent, bool handleMargins = false );

    QgsProject *mProject = nullptr;
    QgsMapSettings mMapSettings;
    qreal mDevicePixelRatio = 1.0;
    double mBottomMargin = 0;
    double mRightMargin = 0;
};

#endif // QGSQUICKMAPSETTINGS_H
```


