/***************************************************************************
              maptoscreen.h
               ----------------------------------------------------
              date                 : 22.08.2018
              copyright            : (C) 2018 by Denis Rouzaud
              email                : denis (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MAPTOSCREEN_H
#define MAPTOSCREEN_H

#include "qgsquickmapsettings.h"

#include <QObject>
#include <QPointF>
#include <qgspoint.h>

/**
 * @brief The MapToScreen class transform a map point to screen coordinates.
 * The map point CRS is the one from the map settings.
 * Screen point will be automatically updated on map extent changes.
 */
class MapToScreen : public QObject
{
    Q_OBJECT

    //! Map settings is used to define the map canvas CRS and detect any extent change
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    //! the point in map coordinates
    Q_PROPERTY( QgsPoint mapPoint READ mapPoint WRITE setMapPoint NOTIFY mapPointChanged )
    //! the distance in map unit
    Q_PROPERTY( double mapDistance READ mapDistance WRITE setMapDistance NOTIFY mapDistanceChanged )
    //! the point in screen coordinates (read-only)
    Q_PROPERTY( QPointF screenPoint READ screenPoint NOTIFY screenPointChanged )
    //! the distance in screen coordinates (read-only)
    Q_PROPERTY( double screenDistance READ screenDistance NOTIFY screenDistanceChanged )

  public:
    explicit MapToScreen( QObject *parent = nullptr );

    //! \copydoc mapSettings
    void setMapSettings( QgsQuickMapSettings *mapSettings );
    //! \copydoc mapSettings
    QgsQuickMapSettings *mapSettings() const;

    //! \copydoc mapPoint
    void setMapPoint( const QgsPoint &point );
    //! \copydoc mapPoint
    QgsPoint mapPoint() const;

    //! \copydoc mapDistance
    void setMapDistance( const double distance );
    //! \copydoc mapDistance
    double mapDistance() const;

    //! \copydoc screenPoint
    QPointF screenPoint() const;

    //! \copydoc screenDistance
    double screenDistance() const;

  signals:
    //! \copydoc mapSettings
    void mapSettingsChanged();
    //! \copydoc mapPoint
    void mapPointChanged();
    //! \copydoc mapDistance
    void mapDistanceChanged();
    //! \copydoc screenPoint
    void screenPointChanged();
    //! \copydoc screenDistance
    void screenDistanceChanged();

  private slots:
    void transformPoint();
    void transformDistance();

  private:
    QgsQuickMapSettings *mMapSettings = nullptr;
    QgsPoint mMapPoint = QgsPoint();
    double mMapDistance = 0.0;
    QPointF mScreenPoint = QPointF();
    double mScreenDistance = 0.0;
};

#endif // MAPTOSCREEN_H
