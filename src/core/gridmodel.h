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
 * This model manages lists of lines or points representing a grid.
 */

class QFIELD_CORE_EXPORT GridAnnotation
{
    Q_GADGET

    Q_PROPERTY( Positions position MEMBER position )
    Q_PROPERTY( QPointF coordinate MEMBER coordinate )
    Q_PROPERTY( QString label MEMBER label )

  public:
    enum Positions
    {
      Top,
      Bottom,
      Left,
      Right,
    };
    Q_ENUM( Positions )

    GridAnnotation( const Positions position = Top, const QPointF coordinate = QPointF(), const QString label = QString() )
      : position( position )
      , coordinate( coordinate )
      , label( label )
    {}

    Positions position = Top;
    QPointF coordinate;
    QString label;
};

class QFIELD_CORE_EXPORT GridModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    Q_PROPERTY( double xInterval READ xInterval WRITE setXInterval NOTIFY xIntervalChanged )
    Q_PROPERTY( double yInterval READ yInterval WRITE setYInterval NOTIFY yIntervalChanged )
    Q_PROPERTY( double xOffset READ xOffset WRITE setXOffset NOTIFY xOffsetChanged )
    Q_PROPERTY( double yOffset READ yOffset WRITE setYOffset NOTIFY yOffsetChanged )

    Q_PROPERTY( QList<QList<QPointF>> lines READ lines NOTIFY gridChanged )
    Q_PROPERTY( QList<QPointF> markers READ markers NOTIFY gridChanged )
    Q_PROPERTY( QList<GridAnnotation> annotations READ annotations NOTIFY gridChanged )

  public:
    //! Default constructor
    explicit GridModel( QObject *parent = nullptr );

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

    //! Returns the grid lines
    QList<QList<QPointF>> lines() const { return mLines; }

    //! Returns the grid markers
    QList<QPointF> markers() const { return mMarkers; }

    //! Returns the grid annotations
    QList<GridAnnotation> annotations() const { return mAnnotations; }

  signals:
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

    //! Emitted when the grid lines, markers, and/or annotations have changed
    void gridChanged();

  private:
    void update();

    QgsQuickMapSettings *mMapSettings = nullptr;

    double mXInterval = 2500.0;
    double mYInterval = 2500.0;
    double mXOffset = 0.0;
    double mYOffset = 0.0;

    bool mDrawLines = true;
    QList<QList<QPointF>> mLines;
    bool mDrawMarkers = true;
    QList<QPointF> mMarkers;
    bool mDrawAnnotations = true;
    QList<GridAnnotation> mAnnotations;

    QLocale mLocale;
};
Q_DECLARE_METATYPE( GridAnnotation )
#endif // GRIDMODEL_H
