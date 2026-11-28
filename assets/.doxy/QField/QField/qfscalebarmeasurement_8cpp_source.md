

# File qfscalebarmeasurement.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfscalebarmeasurement.cpp**](qfscalebarmeasurement_8cpp.md)

[Go to the documentation of this file](qfscalebarmeasurement_8cpp.md)


```C++
/***************************************************************************
  qfscalebarmeasurement.h - QfScaleBarMeasurement

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

#include "qfscalebarmeasurement.h"

#include <qgis.h>
#include <qgsproject.h>
#include <qgsunittypes.h>

QfScaleBarMeasurement::QfScaleBarMeasurement( QObject *parent )
  : QObject( parent )
{
}

void QfScaleBarMeasurement::resetDistanceArea()
{
  if ( mProject && mMapSettings && mMapSettings->mapSettings().destinationCrs().isValid() )
  {
    mDistanceArea.setEllipsoid( mProject->ellipsoid() );
    mDistanceArea.setSourceCrs( mMapSettings->mapSettings().destinationCrs(), mProject->transformContext() );
  }
  else
  {
    mDistanceArea.setEllipsoid( Qgis::geoNone() );
  }
}

QgsProject *QfScaleBarMeasurement::project() const
{
  return mProject;
}

void QfScaleBarMeasurement::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  if ( mProject )
  {
    disconnect( mProject, &QgsProject::readProject, this, &QfScaleBarMeasurement::resetDistanceArea );
    disconnect( mProject, &QgsProject::ellipsoidChanged, this, &QfScaleBarMeasurement::resetDistanceArea );
  }

  mProject = project;
  if ( mProject )
  {
    connect( mProject, &QgsProject::readProject, this, &QfScaleBarMeasurement::resetDistanceArea );
    connect( mProject, &QgsProject::ellipsoidChanged, this, &QfScaleBarMeasurement::resetDistanceArea );
  }
  resetDistanceArea();
  measure();

  emit projectChanged();
}

void QfScaleBarMeasurement::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &QfScaleBarMeasurement::resetDistanceArea );
    disconnect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, &QfScaleBarMeasurement::measure );
  }

  mMapSettings = mapSettings;
  connect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &QfScaleBarMeasurement::resetDistanceArea );
  connect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, &QfScaleBarMeasurement::measure );
  resetDistanceArea();
  measure();

  emit mapSettingsChanged();
}

QgsQuickMapSettings *QfScaleBarMeasurement::mapSettings() const
{
  return mMapSettings;
}

void QfScaleBarMeasurement::measure()
{
  if ( mProject && mMapSettings )
  {
    const QgsRectangle extent = mMapSettings->mapSettings().extent();
    const double yPosition = 0.5 * ( extent.yMinimum() + extent.yMaximum() );

    const Qgis::DistanceUnit mapUnits = mDistanceArea.lengthUnits();
    double extentLength = 0.0;
    try
    {
      extentLength = mDistanceArea.measureLine( QgsPointXY( extent.xMinimum(), yPosition ),
                                                QgsPointXY( extent.xMaximum(), yPosition ) );
    }
    catch ( const QgsException & )
    {
      extentLength = 0.0;
    }

    if ( extentLength > 0 )
    {
      Qgis::DistanceUnit distanceUnit = mProject->distanceUnits();
      if ( distanceUnit == Qgis::DistanceUnit::Unknown )
      {
        // In this context, unknown is meant as a statement to adopt map units
        distanceUnit = mapUnits;
      }

      extentLength = mDistanceArea.convertLengthMeasurement( extentLength, distanceUnit );
      QgsUnitTypes::DistanceValue scaledDistance = QgsUnitTypes::scaledDistance( extentLength, distanceUnit, 2 );
      distanceUnit = scaledDistance.unit;
      extentLength = scaledDistance.value;

      const double factor = ( extentLength / extent.width() ) * mMapSettings->mapUnitsPerPoint();
      const double range = mReferenceScreenLength * factor;
      const double exponent = std::floor( std::log( range ) / 2.302585092994046 );
      const double magnitude = std::pow( 10, exponent );
      const double adjustedMagnitude = ( distanceUnit == Qgis::DistanceUnit::Degrees
                                           ? magnitude / ( 1 + ( magnitude / factor ) / mReferenceScreenLength )
                                           : magnitude / ( 1 + std::round( ( magnitude / factor ) / mReferenceScreenLength ) ) );
      mScreenLength = adjustedMagnitude / factor;

      if ( std::isnan( adjustedMagnitude ) )
      {
        mLabel = tr( "Unknown" );
      }
      else if ( adjustedMagnitude >= 0.1 )
      {
        mLabel = QStringLiteral( "%1 %2" ).arg( adjustedMagnitude ).arg( QgsUnitTypes::toAbbreviatedString( distanceUnit ) );
      }
      else
      {
        // when going to mm scale, better show all the decimals
        scaledDistance = QgsUnitTypes::scaledDistance( adjustedMagnitude, mapUnits, 10 );
        mLabel = QStringLiteral( "%1 %2" ).arg( scaledDistance.value ).arg( QgsUnitTypes::toAbbreviatedString( scaledDistance.unit ) );
      }

      const bool impreciseUnits = mMapSettings->mapSettings().mapUnits() == Qgis::DistanceUnit::Degrees;
      if ( impreciseUnits )
      {
        mLabel = QStringLiteral( "~" ) + mLabel;
      }
    }
    else
    {
      mScreenLength = 0.0;
      mLabel = tr( "Unknown" );
    }
  }
  else
  {
    mScreenLength = 0.0;
    mLabel = QString();
  }

  emit screenLengthChanged();
  emit labelChanged();
}

void QfScaleBarMeasurement::setReferenceScreenLength( double referenceScreenLength )
{
  if ( mReferenceScreenLength == referenceScreenLength )
    return;

  mReferenceScreenLength = referenceScreenLength;

  emit referenceScreenLengthChanged();
}

double QfScaleBarMeasurement::referenceScreenLength() const
{
  return mReferenceScreenLength;
}

double QfScaleBarMeasurement::screenLength() const
{
  return mScreenLength;
}

QString QfScaleBarMeasurement::label() const
{
  return mLabel;
}
```


