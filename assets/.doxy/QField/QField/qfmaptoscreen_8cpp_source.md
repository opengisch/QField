

# File qfmaptoscreen.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfmaptoscreen.cpp**](qfmaptoscreen_8cpp.md)

[Go to the documentation of this file](qfmaptoscreen_8cpp.md)


```C++
/***************************************************************************
              qfmaptoscreen.cpp
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

#include "qfmaptoscreen.h"
#include "qgspoint.h"

QfMapToScreen::QfMapToScreen( QObject *parent )
  : QObject( parent )
{
}

void QfMapToScreen::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, &QfMapToScreen::transformPoint );
    disconnect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &QfMapToScreen::transformPoint );
    disconnect( mMapSettings, &QgsQuickMapSettings::outputSizeChanged, this, &QfMapToScreen::transformPoint );
  }

  mMapSettings = mapSettings;

  connect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, &QfMapToScreen::transformPoint );
  connect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &QfMapToScreen::transformPoint );
  connect( mMapSettings, &QgsQuickMapSettings::outputSizeChanged, this, &QfMapToScreen::transformPoint );

  transformPoint();
  transformDistance();

  emit mapSettingsChanged();
}

QgsQuickMapSettings *QfMapToScreen::mapSettings() const
{
  return mMapSettings;
}

void QfMapToScreen::setMapPoint( const QgsPoint &point )
{
  if ( mMapPoint == point )
    return;

  mMapPoint = point;
  emit mapPointChanged();
  transformPoint();
}

QgsPoint QfMapToScreen::mapPoint() const
{
  return mMapPoint;
}

QPointF QfMapToScreen::screenPoint() const
{
  return mScreenPoint;
}

void QfMapToScreen::transformPoint()
{
  if ( !mMapSettings )
  {
    mScreenPoint = QPointF();
  }
  else
  {
    mScreenPoint = mMapSettings->coordinateToScreen( mMapPoint );
  }
  emit screenPointChanged();
}

void QfMapToScreen::setMapDistance( const double distance )
{
  if ( mMapDistance == distance )
    return;

  mMapDistance = distance;
  emit mapDistanceChanged();
  transformDistance();
}

double QfMapToScreen::mapDistance() const
{
  return mMapDistance;
}

double QfMapToScreen::screenDistance() const
{
  return mScreenDistance;
}

void QfMapToScreen::transformDistance()
{
  if ( !mMapSettings )
  {
    mScreenDistance = std::numeric_limits<double>::quiet_NaN();
  }
  else if ( qgsDoubleNear( mMapDistance, 0.0 ) || qgsDoubleNear( mMapSettings->mapUnitsPerPoint(), 0.0 ) )
  {
    mScreenDistance = 0.0;
  }
  else
  {
    mScreenDistance = mMapDistance / mMapSettings->mapUnitsPerPoint();
  }
  emit screenDistanceChanged();
}
```


