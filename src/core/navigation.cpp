/***************************************************************************
 navigation.cpp - Navigation

 ---------------------
 begin                : 22.02.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "navigation.h"
#include "navigationmodel.h"

#include <qgslinestring.h>

Navigation::Navigation()
  : QObject()
{
  mModel.reset( new NavigationModel() );
  mModel->restore();

  connect( mModel.get(), &NavigationModel::destinationChanged, this, &Navigation::destinationChanged );
  connect( mModel.get(), &NavigationModel::pointsChanged, this, &Navigation::updatePath );
  connect( mModel.get(), &NavigationModel::modelReset, this, &Navigation::destinationChanged );
  connect( mModel.get(), &NavigationModel::modelReset, this, &Navigation::updatePath );
}

Navigation::~Navigation()
{
  mModel->save();
}

void Navigation::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &Navigation::crsChanged );
  }

  mMapSettings = mapSettings;

  connect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &Navigation::crsChanged );
  mModel->setCrs( mMapSettings->destinationCrs() );

  emit mapSettingsChanged();
}

void Navigation::crsChanged()
{
  mModel->setCrs( mMapSettings->destinationCrs() );
}

QgsPoint Navigation::location() const
{
  return mLocation;
}

void Navigation::setLocation( const QgsPoint &point )
{
  if ( mLocation == point )
    return;

  mLocation = point;
  emit locationChanged();

  updatePath();
}

QgsPoint Navigation::destination() const
{
  return mModel->destination();
}

void Navigation::setDestination( const QgsPoint &point )
{
  mModel->setDestination( point );
}

void Navigation::updatePath()
{
  QgsPointSequence points = mModel->points();
  if ( points.isEmpty() )//|| !mLocation.isEmpty() )
  {
    mPath = QgsGeometry();
    return;
  }

  points.prepend( QgsPoint( 1037250, 5907021 ) );
  mPath = QgsGeometry( new QgsLineString( points ) );
  qDebug() << mPath.asWkt();
  emit pathChanged();
}
