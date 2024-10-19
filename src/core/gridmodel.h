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

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    Q_PROPERTY( double xInterval READ xInterval WRITE setXInterval NOTIFY xIntervalChanged )
    Q_PROPERTY( double yInterval READ yInterval WRITE setYInterval NOTIFY yIntervalChanged )
    Q_PROPERTY( double xOffset READ xOffset WRITE setXOffset NOTIFY xOffsetChanged )
    Q_PROPERTY( double yOffset READ yOffset WRITE setYOffset NOTIFY yOffsetChanged )

    Q_PROPERTY( bool prepareLines READ prepareLines WRITE setPrepareLines NOTIFY prepareLinesChanged )
    Q_PROPERTY( QList<QList<QPointF>> lines READ lines NOTIFY gridChanged )
    Q_PROPERTY( bool prepareMarkers READ prepareMarkers WRITE setPrepareMarkers NOTIFY prepareMarkersChanged )
    Q_PROPERTY( QList<QPointF> markers READ markers NOTIFY gridChanged )
    Q_PROPERTY( bool prepareAnnotations READ prepareAnnotations WRITE setPrepareAnnotations NOTIFY prepareAnnotationsChanged )
    Q_PROPERTY( QList<GridAnnotation> annotations READ annotations NOTIFY gridChanged )

  public:
    //! Default constructor
    explicit GridModel( QObject *parent = nullptr );

    //! Returns TRUE when grid elements will be prepared
    bool enabled() const { return mEnabled; }

    //! Sets whether grid elements will be prepared
    void setEnabled( bool enabled );

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

    //! Returns the grid lines
    QList<QList<QPointF>> lines() const { return mLines; }

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

  signals:
    //! Emitted when the grid enabled setting has changed
    void enabledChanged();

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

    //! Emitted when the grid lines, markers, and/or annotations have changed
    void gridChanged();

  private:
    void update();

    bool mEnabled = false;

    QgsQuickMapSettings *mMapSettings = nullptr;

    double mXInterval = 2500.0;
    double mYInterval = 2500.0;
    double mXOffset = 0.0;
    double mYOffset = 0.0;

    bool mPrepareLines = false;
    QList<QList<QPointF>> mLines;
    bool mPrepareMarkers = false;
    QList<QPointF> mMarkers;
    bool mPrepareAnnotations = false;
    QList<GridAnnotation> mAnnotations;

    QLocale mLocale;
};
Q_DECLARE_METATYPE( GridAnnotation )
#endif // GRIDMODEL_H
