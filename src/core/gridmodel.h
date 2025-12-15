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

/**
 * Holds details for a given grid annotation.
 * \ingroup core
 */
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

/**
 * This model manages lists of lines, markers, and annotations representing a grid.
 * \ingroup core
 */
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
    //! Default constructor
    explicit GridModel( QObject *parent = nullptr );

    //! Returns TRUE when grid elements will be prepared
    bool enabled() const { return mEnabled; }

    //! Sets whether grid elements will be prepared
    void setEnabled( bool enabled );

    /**
     * Returns TRUE when grid will adopt an indeterminate behavior
     * and ignore the interval and offset values.
     */
    bool indeterminate() const { return mIndeterminate; }

    /**
     * Sets whether the grid will adopt an indeterminate behavior
     * and ignore the interval and offset values.
     */
    void setIndeterminate( bool indeterminate );

    //! Returns the map settings object
    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }

    //! Sets the map settings object
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    //! Returns the grid X interval
    double xInterval() const { return mXInterval; }

    //! Sets the grid X interval
    void setXInterval( double interval );

    //! Returns the grid Y interval
    double yInterval() const { return mYInterval; }

    //! Sets the grid Y interval
    void setYInterval( double interval );

    //! Returns the grid X interval
    double xOffset() const { return mXOffset; }

    //! Sets the grid X interval
    void setXOffset( double offset );

    //! Returns the grid Y interval
    double yOffset() const { return mYOffset; }

    //! Sets the grid Y interval
    void setYOffset( double offset );

    //! Returns whether grid lines will be prepared
    bool prepareLines() const { return mPrepareLines; }

    //! Sets whether grid lines will be prepared
    void setPrepareLines( bool prepare );

    //! Returns the grid major lines
    QList<QList<QPointF>> majorLines() const { return mMajorLines; }

    //! Returns the grid minor lines
    QList<QList<QPointF>> minorLines() const { return mMinorLines; }

    //! Returns whether grid markers will be prepared
    bool prepareMarkers() const { return mPrepareMarkers; }

    //! Sets whether grid markers will be prepared
    void setPrepareMarkers( bool prepare );

    //! Returns the grid markers
    QList<QPointF> markers() const { return mMarkers; }

    //! Returns whether grid annotations will be prepared
    bool prepareAnnotations() const { return mPrepareAnnotations; }

    //! Sets whether grid annotations will be prepared
    void setPrepareAnnotations( bool prepare );

    //! Returns the grid annotations
    QList<GridAnnotation> annotations() const { return mAnnotations; }

    /**
     * Returns whether grid line and marker colors will be automatically assigned to
     * colors derived from the map canvas background color.
     */
    bool autoColor() const { return mAutoColor; }

    /**
     * Set whether grid line and marker colors will be automatically assigned to
     * colors derived from the map canvas background color.
     */
    void setAutoColor( bool autoColor );

    //! Returns the grid major line color.
    QColor majorLineColor() const { return mMajorLineColor; }

    //! Sets the grid major line color.
    void setMajorLineColor( const QColor &color );

    //! Returns the grid minor line color.
    QColor minorLineColor() const { return mMinorLineColor; }

    //! Sets the grid minor line color.
    void setMinorLineColor( const QColor &color );

    //! Returns the grid marker color.
    QColor markerColor() const { return mMarkerColor; }

    //! Sets the grid marker color.
    void setMarkerColor( const QColor &color );

    //! Returns the grid annotation text color.
    QColor annotationColor() const { return mAnnotationColor; }

    //! Sets the grid annotation text color.
    void setAnnotationColor( const QColor &color );

    //! Returns the grid annotation text outline color.
    QColor annotationOutlineColor() const { return mAnnotationOutlineColor; }

    //! Sets the grid annotation text outline color.
    void setAnnotationOutlineColor( const QColor &color );

    //! Returns whether annotation text have an outline color.
    bool annotationHasOutline() const { return mAnnotationHasOutline; }

    //! Sets whether annotation text have an outline color.
    void setAnnotationHasOutline( bool hasOutline );

    //! Returns the annotation text decimal precision.
    int annotationPrecision() const { return mAnnotationPrecision; }

    //! Sets the annotation text decimal precision.
    void setAnnotationPrecision( int precision );

  signals:
    //! Emitted when the grid enabled setting has changed
    void enabledChanged();

    //! Emitted when the grid indeterminate setting has changed
    void indeterminateChanged();

    //! Emitted when the map settings object has changed
    void mapSettingsChanged();

    //! Emitted when the grid X interval has changed
    void xIntervalChanged();

    //! Emitted when the grid Y interval has changed
    void yIntervalChanged();

    //! Emitted when the grid X offset has changed
    void xOffsetChanged();

    //! Emitted when the grid Y offset has changed
    void yOffsetChanged();

    //! Emitted when grid lines preparation setting has changed
    void prepareLinesChanged();

    //! Emitted when grid markers preparation setting has changed
    void prepareMarkersChanged();

    //! Emitted when grid annotations preparation setting has changed
    void prepareAnnotationsChanged();

    //! Emitted when the grid major lines have changed
    void majorLinesChanged();

    //! Emitted when the grid minor lines have changed
    void minorLinesChanged();

    //! Emitted when the grid markers have changed
    void markersChanged();

    //! Emitted when the grid annotations have changed
    void annotationsChanged();

    //! Emitted when the grid line and marker colors are automatically assigned
    void autoColorChanged();

    //! Emitted when the grid major line color has changed
    void majorLineColorChanged();

    //! Emitted when the grid minor line color has changed
    void minorLineColorChanged();

    //! Emitted when the grid marker color has changed
    void markerColorChanged();

    //! Emitted when the grid annotation text color has changed
    void annotationColorChanged();

    //! Emitted when the grid annotation text outline color has changed
    void annotationOutlineColorChanged();

    //! Emitted when the grid annotation text outline visibility has changed
    void annotationHasOutlineChanged();

    //! Emitted when the grid annotation text decimal precision has changed
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
