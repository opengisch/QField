/***************************************************************************
  scalebarmeasurement.h - ScaleBarMeasurement

 ---------------------
 begin                : 9.10.2021
 copyright            : (C) 2021 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "scalebarmeasurement.h"

#include <qgsproject.h>
#include <qgsunittypes.h>

ScaleBarMeasurement::ScaleBarMeasurement( QObject *parent )
  : QObject( parent )
{
}

void ScaleBarMeasurement::resetDistanceArea()
{
  if ( mProject && mMapSettings && mMapSettings->mapSettings().destinationCrs().isValid() )
  {
    mDistanceArea.setEllipsoid( mProject->ellipsoid() );
    mDistanceArea.setSourceCrs( mMapSettings->mapSettings().destinationCrs(), mProject->transformContext() );
  }
  else
  {
    mDistanceArea.setEllipsoid( geoNone() );
  }
}

QgsProject *ScaleBarMeasurement::project() const
{
  return mProject;
}

void ScaleBarMeasurement::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  if ( mProject )
  {
    disconnect( mProject, &QgsProject::readProject, this, &ScaleBarMeasurement::resetDistanceArea );
    disconnect( mProject, &QgsProject::ellipsoidChanged, this, &ScaleBarMeasurement::resetDistanceArea );
  }

  mProject = project;
  if ( mProject )
  {
    connect( mProject, &QgsProject::readProject, this, &ScaleBarMeasurement::resetDistanceArea );
    connect( mProject, &QgsProject::ellipsoidChanged, this, &ScaleBarMeasurement::resetDistanceArea );
  }
  resetDistanceArea();
  measure();

  emit projectChanged();
}

void ScaleBarMeasurement::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &ScaleBarMeasurement::resetDistanceArea );
    disconnect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, &ScaleBarMeasurement::measure );
  }

  mMapSettings = mapSettings;
  connect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &ScaleBarMeasurement::resetDistanceArea );
  connect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, &ScaleBarMeasurement::measure );
  resetDistanceArea();
  measure();

  emit mapSettingsChanged();
}

QgsQuickMapSettings *ScaleBarMeasurement::mapSettings() const
{
  return mMapSettings;
}

void ScaleBarMeasurement::measure()
{
  if ( mProject && mMapSettings )
  {
    const QgsUnitTypes::DistanceUnit mapUnits = mDistanceArea.lengthUnits();

    // convert to meters
    const QgsRectangle extent = mMapSettings->mapSettings().extent();
    const double yPosition = 0.5 * ( extent.yMinimum() + extent.yMaximum() );
    const double extentLength = mDistanceArea.measureLine( QgsPointXY( extent.xMinimum(), yPosition ),
                                                           QgsPointXY( extent.xMaximum(), yPosition ) );
    const double factor = ( extentLength / extent.width() ) * mMapSettings->mapUnitsPerPoint();

    const double range = mReferenceScreenLength * factor;
    const double exponent = std::floor( std::log( range ) / 2.302585092994046 );
    const double magnitude = std::pow( 10, exponent );
    const double adjustedMagnitude = ( mapUnits == QgsUnitTypes::DistanceDegrees
                                         ? magnitude / ( 1 + ( magnitude / factor ) / mReferenceScreenLength )
                                         : magnitude / ( 1 + std::round( ( magnitude / factor ) / mReferenceScreenLength ) ) );

    mScreenLength = adjustedMagnitude / factor;

    if ( mapUnits == QgsUnitTypes::DistanceMeters && adjustedMagnitude >= 1000 )
    {
      mLabel = QStringLiteral( "%1 %2" ).arg( adjustedMagnitude / 1000 ).arg( QgsUnitTypes::toAbbreviatedString( QgsUnitTypes::DistanceKilometers ) );
    }
    else
    {
      // better show 0.1m than 0m. No way to force it to show 10cm.
      QgsUnitTypes::DistanceValue scaledDistance = QgsUnitTypes::scaledDistance( adjustedMagnitude, mapUnits, 1 );

      if ( std::isnan( scaledDistance.value ) )
      {
        mLabel = tr( "Unknown" );
      }
      else if ( scaledDistance.value >= 0.1 )
      {
        mLabel = QStringLiteral( "%1 %2" ).arg( scaledDistance.value ).arg( QgsUnitTypes::toAbbreviatedString( scaledDistance.unit ) );
      }
      else
      {
        // when going to mm scale, better show all the decimals
        scaledDistance = QgsUnitTypes::scaledDistance( adjustedMagnitude, mapUnits, 10 );
        mLabel = QStringLiteral( "%1 %2" ).arg( scaledDistance.value ).arg( QgsUnitTypes::toAbbreviatedString( scaledDistance.unit ) );
      }
    }
    const bool impreciseUnits = mMapSettings->mapSettings().mapUnits() == QgsUnitTypes::DistanceDegrees;
    if ( impreciseUnits )
      mLabel = QStringLiteral( "~" ) + mLabel;
  }
  else
  {
    mScreenLength = 0.0;
    mLabel = QString();
  }

  emit screenLengthChanged();
  emit labelChanged();
}

void ScaleBarMeasurement::setReferenceScreenLength( double referenceScreenLength )
{
  if ( mReferenceScreenLength == referenceScreenLength )
    return;

  mReferenceScreenLength = referenceScreenLength;

  emit referenceScreenLengthChanged();
}

double ScaleBarMeasurement::referenceScreenLength() const
{
  return mReferenceScreenLength;
}

double ScaleBarMeasurement::screenLength() const
{
  return mScreenLength;
}

QString ScaleBarMeasurement::label() const
{
  return mLabel;
}
