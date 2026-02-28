

# File maptoscreen.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**maptoscreen.h**](maptoscreen_8h.md)

[Go to the documentation of this file](maptoscreen_8h.md)


```C++
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

class MapToScreen : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( QgsPoint mapPoint READ mapPoint WRITE setMapPoint NOTIFY mapPointChanged )
    Q_PROPERTY( double mapDistance READ mapDistance WRITE setMapDistance NOTIFY mapDistanceChanged )
    Q_PROPERTY( QPointF screenPoint READ screenPoint NOTIFY screenPointChanged )
    Q_PROPERTY( double screenDistance READ screenDistance NOTIFY screenDistanceChanged )

  public:
    explicit MapToScreen( QObject *parent = nullptr );

    void setMapSettings( QgsQuickMapSettings *mapSettings );
    QgsQuickMapSettings *mapSettings() const;

    void setMapPoint( const QgsPoint &point );
    QgsPoint mapPoint() const;

    void setMapDistance( const double distance );
    double mapDistance() const;

    QPointF screenPoint() const;

    double screenDistance() const;

  signals:
    void mapSettingsChanged();
    void mapPointChanged();
    void mapDistanceChanged();
    void screenPointChanged();
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
```


