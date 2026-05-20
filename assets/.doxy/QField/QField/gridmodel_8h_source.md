

# File gridmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**gridmodel.h**](gridmodel_8h.md)

[Go to the documentation of this file](gridmodel_8h.md)


```C++
/***************************************************************************
  gridmodel.h - GridModel

 ---------------------
 begin                : 4.10.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef GRIDMODEL_H
#define GRIDMODEL_H

#include "qfield_core_export.h"
#include "qgsquickmapsettings.h"

class QFIELD_CORE_EXPORT GridAnnotation
{
    Q_GADGET

    Q_PROPERTY( Positions position MEMBER position )
    Q_PROPERTY( QPointF coordinate MEMBER coordinate )
    Q_PROPERTY( double value MEMBER value )

  public:
    enum Positions
    {
      Top,
      Bottom,
      Left,
      Right,
    };
    Q_ENUM( Positions )

    GridAnnotation( const Positions position = Top, const QPointF coordinate = QPointF(), const double value = 0.0 )
      : position( position )
      , coordinate( coordinate )
      , value( value )
    {}

    Positions position = Top;
    QPointF coordinate;
    double value;
};

class QFIELD_CORE_EXPORT GridModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged )
    Q_PROPERTY( bool indeterminate READ indeterminate WRITE setIndeterminate NOTIFY indeterminateChanged )

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    Q_PROPERTY( double xInterval READ xInterval WRITE setXInterval NOTIFY xIntervalChanged )
    Q_PROPERTY( double yInterval READ yInterval WRITE setYInterval NOTIFY yIntervalChanged )
    Q_PROPERTY( double xOffset READ xOffset WRITE setXOffset NOTIFY xOffsetChanged )
    Q_PROPERTY( double yOffset READ yOffset WRITE setYOffset NOTIFY yOffsetChanged )

    Q_PROPERTY( bool prepareLines READ prepareLines WRITE setPrepareLines NOTIFY prepareLinesChanged )
    Q_PROPERTY( bool prepareMarkers READ prepareMarkers WRITE setPrepareMarkers NOTIFY prepareMarkersChanged )
    Q_PROPERTY( bool prepareAnnotations READ prepareAnnotations WRITE setPrepareAnnotations NOTIFY prepareAnnotationsChanged )

    Q_PROPERTY( QList<QList<QPointF>> majorLines READ majorLines NOTIFY majorLinesChanged )
    Q_PROPERTY( QList<QList<QPointF>> minorLines READ minorLines NOTIFY minorLinesChanged )
    Q_PROPERTY( QList<QPointF> markers READ markers NOTIFY markersChanged )
    Q_PROPERTY( QList<GridAnnotation> annotations READ annotations NOTIFY annotationsChanged )

    Q_PROPERTY( bool autoColor READ autoColor WRITE setAutoColor NOTIFY autoColorChanged )
    Q_PROPERTY( QColor majorLineColor READ majorLineColor WRITE setMajorLineColor NOTIFY majorLineColorChanged )
    Q_PROPERTY( QColor minorLineColor READ minorLineColor WRITE setMinorLineColor NOTIFY minorLineColorChanged )
    Q_PROPERTY( QColor markerColor READ markerColor WRITE setMarkerColor NOTIFY markerColorChanged )

    Q_PROPERTY( QColor annotationColor READ annotationColor WRITE setAnnotationColor NOTIFY annotationColorChanged )
    Q_PROPERTY( QColor annotationOutlineColor READ annotationOutlineColor WRITE setAnnotationOutlineColor NOTIFY annotationOutlineColorChanged )
    Q_PROPERTY( bool annotationHasOutline READ annotationHasOutline WRITE setAnnotationHasOutline NOTIFY annotationHasOutlineChanged )
    Q_PROPERTY( int annotationPrecision READ annotationPrecision WRITE setAnnotationPrecision NOTIFY annotationPrecisionChanged )

  public:
    explicit GridModel( QObject *parent = nullptr );

    bool enabled() const { return mEnabled; }

    void setEnabled( bool enabled );

    bool indeterminate() const { return mIndeterminate; }

    void setIndeterminate( bool indeterminate );

    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }

    void setMapSettings( QgsQuickMapSettings *mapSettings );

    double xInterval() const { return mXInterval; }

    void setXInterval( double interval );

    double yInterval() const { return mYInterval; }

    void setYInterval( double interval );

    double xOffset() const { return mXOffset; }

    void setXOffset( double offset );

    double yOffset() const { return mYOffset; }

    void setYOffset( double offset );

    bool prepareLines() const { return mPrepareLines; }

    void setPrepareLines( bool prepare );

    QList<QList<QPointF>> majorLines() const { return mMajorLines; }

    QList<QList<QPointF>> minorLines() const { return mMinorLines; }

    bool prepareMarkers() const { return mPrepareMarkers; }

    void setPrepareMarkers( bool prepare );

    QList<QPointF> markers() const { return mMarkers; }

    bool prepareAnnotations() const { return mPrepareAnnotations; }

    void setPrepareAnnotations( bool prepare );

    QList<GridAnnotation> annotations() const { return mAnnotations; }

    bool autoColor() const { return mAutoColor; }

    void setAutoColor( bool autoColor );

    QColor majorLineColor() const { return mMajorLineColor; }

    void setMajorLineColor( const QColor &color );

    QColor minorLineColor() const { return mMinorLineColor; }

    void setMinorLineColor( const QColor &color );

    QColor markerColor() const { return mMarkerColor; }

    void setMarkerColor( const QColor &color );

    QColor annotationColor() const { return mAnnotationColor; }

    void setAnnotationColor( const QColor &color );

    QColor annotationOutlineColor() const { return mAnnotationOutlineColor; }

    void setAnnotationOutlineColor( const QColor &color );

    bool annotationHasOutline() const { return mAnnotationHasOutline; }

    void setAnnotationHasOutline( bool hasOutline );

    int annotationPrecision() const { return mAnnotationPrecision; }

    void setAnnotationPrecision( int precision );

  signals:
    void enabledChanged();

    void indeterminateChanged();

    void mapSettingsChanged();

    void xIntervalChanged();

    void yIntervalChanged();

    void xOffsetChanged();

    void yOffsetChanged();

    void prepareLinesChanged();

    void prepareMarkersChanged();

    void prepareAnnotationsChanged();

    void majorLinesChanged();

    void minorLinesChanged();

    void markersChanged();

    void annotationsChanged();

    void autoColorChanged();

    void majorLineColorChanged();

    void minorLineColorChanged();

    void markerColorChanged();

    void annotationColorChanged();

    void annotationOutlineColorChanged();

    void annotationHasOutlineChanged();

    void annotationPrecisionChanged();

  private:
    void clear();
    void update();
    void updateColors();

    bool mEnabled = false;

    QgsQuickMapSettings *mMapSettings = nullptr;

    bool mIndeterminate = false;
    double mIndeterminateInterval = 0.0;

    double mXInterval = 0.0;
    double mYInterval = 0.0;
    double mXOffset = 0.0;
    double mYOffset = 0.0;

    bool mPrepareLines = false;
    QList<QList<QPointF>> mMajorLines;
    QList<QList<QPointF>> mMinorLines;
    bool mPrepareMarkers = false;
    QList<QPointF> mMarkers;
    bool mPrepareAnnotations = false;
    QList<GridAnnotation> mAnnotations;

    bool mAutoColor = false;
    QColor mMajorLineColor = QColor( 0, 0, 0, 100 );
    QColor mMinorLineColor = QColor( 0, 0, 0, 20 );
    QColor mMarkerColor = QColor( 0, 0, 0, 100 );

    QColor mAnnotationColor = QColor( 0, 0, 0 );
    QColor mAnnotationOutlineColor = QColor( 255, 255, 255, 100 );
    bool mAnnotationHasOutline = false;
    int mAnnotationPrecision = 0;

    QLocale mLocale;
};
Q_DECLARE_METATYPE( GridAnnotation )
#endif // GRIDMODEL_H
```


