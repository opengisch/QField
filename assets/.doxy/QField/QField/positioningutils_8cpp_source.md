

# File positioningutils.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**positioningutils.cpp**](positioningutils_8cpp.md)

[Go to the documentation of this file](positioningutils_8cpp.md)


```C++
/***************************************************************************
  positioningutils.cpp - PositioningUtils

 ---------------------
 begin                : 20.05.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "gnsspositioninformation.h"
#include "platformutilities.h"
#include "positioningutils.h"

#include <qgsbearingutils.h>
#include <qgscoordinatetransformcontext.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

#include <mutex>

typedef QMap<int, QgsRectangle> TimeZoneBounds;
Q_GLOBAL_STATIC( TimeZoneBounds, sTimeZones )

PositioningUtils::PositioningUtils( QObject *parent )
  : QObject( parent )
{
}

GnssPositionInformation PositioningUtils::createGnssPositionInformation( double latitude, double longitude, double altitude, double speed, double direction, double horizontalAccuracy, double verticalAcurracy, double verticalSpeed, double magneticVariation, const QDateTime &timestamp, const QString &sourceName )
{
  return GnssPositionInformation( latitude, longitude, altitude,
                                  speed, direction, QList<QgsSatelliteInfo>(), 0, 0, 0,
                                  horizontalAccuracy, verticalAcurracy, timestamp,
                                  QChar(), 0, -1, 0, QChar( 'A' ), QList<int>(), false,
                                  verticalSpeed, magneticVariation, 0, sourceName );
}

GnssPositionInformation PositioningUtils::createEmptyGnssPositionInformation()
{
  return GnssPositionInformation();
}

GnssPositionInformation PositioningUtils::averagedPositionInformation( const QList<QVariant> &positionsInformation )
{
  QList<GnssPositionInformation> convertedList;
  for ( const QVariant &pi : positionsInformation )
  {
    if ( pi.canConvert<GnssPositionInformation>() )
    {
      convertedList << pi.value<GnssPositionInformation>();
    }
  }
  return averagedPositionInformation( convertedList );
}

GnssPositionInformation PositioningUtils::averagedPositionInformation( const QList<GnssPositionInformation> &positionsInformation )
{
  if ( positionsInformation.isEmpty() )
    return GnssPositionInformation();

  std::vector<double> latitude;
  std::vector<double> longitude;
  std::vector<double> elevation;
  std::vector<double> speed;
  std::vector<double> direction;
  std::vector<double> pdop;
  std::vector<double> hdop;
  std::vector<double> vdop;
  std::vector<double> hacc;
  std::vector<double> vacc;
  std::vector<double> hvacc;
  std::vector<double> verticalSpeed;
  std::vector<double> magneticVariation;
  std::vector<double> orientation;
  std::vector<double> imuRoll;
  std::vector<double> imuPitch;
  std::vector<double> imuHeading;
  std::vector<double> imuSteering;

  QDateTime utcDateTime = positionsInformation.last().utcDateTime();

  QList<QgsSatelliteInfo> satellitesInView = positionsInformation.at( 0 ).satellitesInView();
  int satellitesUsed = static_cast<int>( satellitesInView.size() );
  bool satInfoComplete = positionsInformation.at( 0 ).satInfoComplete();
  QList<int> satPrn = positionsInformation.at( 0 ).satPrn();
  QChar status = positionsInformation.at( 0 ).status();
  QChar fixMode = positionsInformation.at( 0 ).fixMode();
  int fixType = positionsInformation.at( 0 ).fixType();
  int quality = positionsInformation.at( 0 ).quality();
  QString sourceName = QStringLiteral( "%1 (%2)" ).arg( positionsInformation.at( 0 ).sourceName(), QObject::tr( "averaged" ) );

  for ( const GnssPositionInformation &pi : positionsInformation )
  {
    if ( std::isnan( pi.latitude() ) )
    {
      continue;
    }

    if ( !std::isnan( pi.latitude() ) )
    {
      latitude.push_back( pi.latitude() );
    }
    if ( !std::isnan( pi.longitude() ) )
    {
      longitude.push_back( pi.longitude() );
    }
    if ( !std::isnan( pi.elevation() ) )
    {
      elevation.push_back( pi.elevation() );
    }
    if ( !std::isnan( pi.speed() ) )
    {
      speed.push_back( pi.speed() );
    }
    if ( !std::isnan( pi.direction() ) )
    {
      direction.push_back( pi.direction() );
    }
    pdop.push_back( pi.pdop() );
    hdop.push_back( pi.hdop() );
    vdop.push_back( pi.vdop() );
    if ( !std::isnan( pi.hacc() ) )
    {
      hacc.push_back( pi.hacc() );
    }
    if ( !std::isnan( pi.vacc() ) )
    {
      vacc.push_back( pi.vacc() );
    }
    if ( !std::isnan( pi.hvacc() ) )
    {
      hvacc.push_back( pi.hvacc() );
    }
    if ( !std::isnan( pi.verticalSpeed() ) )
    {
      verticalSpeed.push_back( pi.verticalSpeed() );
    }
    if ( !std::isnan( pi.magneticVariation() ) )
    {
      magneticVariation.push_back( pi.magneticVariation() );
    }
    if ( !std::isnan( pi.orientation() ) )
    {
      orientation.push_back( pi.orientation() );
    }

    if ( pi.imuCorrection() )
    {
      if ( !std::isnan( pi.imuRoll() ) )
      {
        imuRoll.push_back( pi.imuRoll() );
      }
      if ( !std::isnan( pi.imuPitch() ) )
      {
        imuPitch.push_back( pi.imuPitch() );
      }
      if ( !std::isnan( pi.imuHeading() ) )
      {
        imuHeading.push_back( pi.imuHeading() );
      }
      if ( !std::isnan( pi.imuSteering() ) )
      {
        imuSteering.push_back( pi.imuSteering() );
      }
    }
  }

  if ( latitude.empty() )
  {
    return GnssPositionInformation(); // No valid data to average
  }

  const double nan = std::numeric_limits<double>::quiet_NaN();
  return GnssPositionInformation( std::accumulate( latitude.begin(), latitude.end(), 0.0 ) / latitude.size(),
                                  std::accumulate( longitude.begin(), longitude.end(), 0.0 ) / longitude.size(),
                                  elevation.empty() ? nan : std::accumulate( elevation.begin(), elevation.end(), 0.0 ) / elevation.size(),
                                  speed.empty() ? nan : std::accumulate( speed.begin(), speed.end(), 0.0 ) / speed.size(),
                                  direction.empty() ? nan : std::accumulate( direction.begin(), direction.end(), 0.0 ) / direction.size(),
                                  satellitesInView,
                                  pdop.empty() ? 0.0 : std::accumulate( pdop.begin(), pdop.end(), 0.0 ) / pdop.size(),
                                  hdop.empty() ? 0.0 : std::accumulate( hdop.begin(), hdop.end(), 0.0 ) / hdop.size(),
                                  vdop.empty() ? 0.0 : std::accumulate( vdop.begin(), vdop.end(), 0.0 ) / vdop.size(),
                                  hacc.empty() ? nan : std::accumulate( hacc.begin(), hacc.end(), 0.0 ) / hacc.size(),
                                  vacc.empty() ? nan : std::accumulate( vacc.begin(), vacc.end(), 0.0 ) / vacc.size(),
                                  utcDateTime, fixMode, fixType, quality, satellitesUsed, status, satPrn, satInfoComplete,
                                  verticalSpeed.empty() ? nan : std::accumulate( verticalSpeed.begin(), verticalSpeed.end(), 0.0 ) / verticalSpeed.size(),
                                  magneticVariation.empty() ? nan : std::accumulate( magneticVariation.begin(), magneticVariation.end(), 0.0 ) / magneticVariation.size(),
                                  latitude.size(), sourceName,
                                  !imuRoll.empty(),
                                  imuRoll.empty() ? nan : std::accumulate( imuRoll.begin(), imuRoll.end(), 0.0 ) / imuRoll.size(),
                                  imuPitch.empty() ? nan : std::accumulate( imuPitch.begin(), imuPitch.end(), 0.0 ) / imuPitch.size(),
                                  imuHeading.empty() ? nan : std::accumulate( imuHeading.begin(), imuHeading.end(), 0.0 ) / imuHeading.size(),
                                  imuSteering.empty() ? nan : std::accumulate( imuSteering.begin(), imuSteering.end(), 0.0 ) / imuSteering.size(),
                                  orientation.empty() ? nan : std::accumulate( orientation.begin(), orientation.end(), 0.0 ) / orientation.size() );
}

double PositioningUtils::bearingTrueNorth( const QgsPoint &position, const QgsCoordinateReferenceSystem &crs )
{
  const QgsCoordinateTransformContext transformContext = QgsProject::instance()->transformContext();

  double bearing = 0.0;
  try
  {
    bearing = QgsBearingUtils::bearingTrueNorth( crs, transformContext, position );
  }
  catch ( QgsException &e )
  {
    Q_UNUSED( e )
    QgsDebugMsgLevel( QStringLiteral( "Caught exception %1" ).arg( e.what() ), 2 );
  }

  return bearing;
}

QgsRectangle PositioningUtils::createExtentForDevice( const GnssPositionInformation &positionInformation, const QgsCoordinateReferenceSystem &crs, const QgsRectangle &withinRectangle )
{
  QgsRectangle extent;
  if ( positionInformation.latitudeValid() && positionInformation.longitudeValid() && positionInformation.hvaccValid() )
  {
    extent = QgsRectangle( positionInformation.longitude() - 1.0, positionInformation.latitude() - 1.0, positionInformation.longitude() + 1.0, positionInformation.latitude() + 1.0 );
  }
  else
  {
    PositioningUtils::initTimeZones();

    QTimeZone tz = QTimeZone::systemTimeZone();
    const int offset = static_cast<double>( tz.offsetFromUtc( QDateTime( QDate( 2026, 1, 1 ), QTime( 0, 0 ), QTimeZone::utc() ) ) );
    if ( sTimeZones->contains( offset ) )
    {
      extent = sTimeZones->value( offset );
    }
  }

  if ( extent.isEmpty() )
  {
    extent = crs.bounds();
  }

  if ( !extent.isEmpty() )
  {
    const QgsCoordinateReferenceSystem wgs84Crs = QgsCoordinateReferenceSystem::fromEpsgId( 4326 );
    QgsCoordinateTransform transform( wgs84Crs, crs, QgsProject::instance()->transformContext() );
    try
    {
      extent = transform.transform( extent );
    }
    catch ( const QgsException &e )
    {
      extent = QgsRectangle();
    }

    if ( !withinRectangle.isEmpty() )
    {
      extent = extent.intersect( withinRectangle );
      if ( extent.isEmpty() )
      {
        extent = withinRectangle;
      }
    }
    else
    {
      QgsRectangle crsBounds = crs.bounds();
      try
      {
        crsBounds = transform.transform( crsBounds );
      }
      catch ( const QgsException &e )
      {
        crsBounds = QgsRectangle();
      }
      if ( !crsBounds.isEmpty() )
      {
        extent = extent.intersect( crsBounds );
        if ( extent.isEmpty() )
        {
          extent = crsBounds;
        }
      }
    }
  }

  return extent;
}

void PositioningUtils::initTimeZones()
{
  static std::once_flag initialized;
  std::call_once( initialized, buildTimeZones );
}

void PositioningUtils::buildTimeZones()
{
  sTimeZones->insert( 0, QgsRectangle( -25.013, -61.709, 7.670, 82.999 ) );
  sTimeZones->insert( 3600, QgsRectangle( -17.319, -61.664, 34.688, 82.984 ) );
  sTimeZones->insert( 7200, QgsRectangle( -5.499, -61.703, 41.054, 82.969 ) );
  sTimeZones->insert( 10800, QgsRectangle( 37.499, -61.574, 52.500, 82.954 ) );
  sTimeZones->insert( 12600, QgsRectangle( 44.031, 24.835, 63.333, 39.782 ) );
  sTimeZones->insert( 14400, QgsRectangle( 39.884, -61.529, 67.500, 82.940 ) );
  sTimeZones->insert( 16200, QgsRectangle( 60.517, 29.377, 74.889, 38.491 ) );
  sTimeZones->insert( 18000, QgsRectangle( 67.500, -61.484, 82.499, 82.925 ) );
  sTimeZones->insert( 19800, QgsRectangle( 67.954, 5.7189, 97.395, 35.6745 ) );
  sTimeZones->insert( 20700, QgsRectangle( 80.057, 26.347, 88.201, 30.471 ) );
  sTimeZones->insert( 21600, QgsRectangle( 69.264, -61.439, 97.500, 82.910 ) );
  sTimeZones->insert( 23400, QgsRectangle( 92.172, -12.411, 101.170, 28.547 ) );
  sTimeZones->insert( 25200, QgsRectangle( 97.500, -61.393, 112.500, 82.895 ) );
  sTimeZones->insert( 28800, QgsRectangle( 112.500, -61.348, 127.499, 82.881 ) );
  sTimeZones->insert( 31500, QgsRectangle( 125.500, -32.777, 129.031, -31.299 ) );
  sTimeZones->insert( 32400, QgsRectangle( 127.499, -61.303, 142.500, 82.866 ) );
  sTimeZones->insert( 34200, QgsRectangle( 129.000, -38.274, 142.000, -10.701 ) );
  sTimeZones->insert( 36000, QgsRectangle( 120.000, -61.326, 159.999, 73.113 ) );
  sTimeZones->insert( 37800, QgsRectangle( 158.801, -31.988, 159.515, -31.285 ) );
  sTimeZones->insert( 39600, QgsRectangle( 138.675, -61.213, 172.499, 82.836 ) );
  sTimeZones->insert( 43200, QgsRectangle( 172.499, -61.168, 180.000, 82.822 ) );
  sTimeZones->insert( 45900, QgsRectangle( -177.244, -44.634, -175.541, -43.310 ) );
  sTimeZones->insert( 46800, QgsRectangle( -179.395, -24.163, -170.507, -2.565 ) );
  sTimeZones->insert( 50400, QgsRectangle( -160.609, -11.645, -149.992, 4.900 ) );
  sTimeZones->insert( -3600, QgsRectangle( -31.557, -61.754, -7.500, 83.014 ) );
  sTimeZones->insert( -7200, QgsRectangle( -69.107, -63.093, -10.028, 83.059 ) );
  sTimeZones->insert( -10800, QgsRectangle( -80.000, -64.250, -17.000, 83.043 ) );
  sTimeZones->insert( -12600, QgsRectangle( -60.476, 46.410, -52.323, 53.701 ) );
  sTimeZones->insert( -14400, QgsRectangle( -81.078, -61.547, -50.207, 83.058 ) );
  sTimeZones->insert( -18000, QgsRectangle( -92.268, -58.399, -59.613, 83.081 ) );
  sTimeZones->insert( -21600, QgsRectangle( -110.006, -58.345, -82.181, 83.087 ) );
  sTimeZones->insert( -25200, QgsRectangle( -141.002, -58.292, -89.000, 83.102 ) );
  sTimeZones->insert( -28800, QgsRectangle( -139.061, -58.239, -112.499, 83.117 ) );
  sTimeZones->insert( -32400, QgsRectangle( -173.521, -58.185, -127.499, 83.132 ) );
  sTimeZones->insert( -34200, QgsRectangle( -140.925, -10.754, -138.210, -7.659 ) );
  sTimeZones->insert( -36000, QgsRectangle( -176.216, -58.132, -136.083, 83.147 ) );
  sTimeZones->insert( -39600, QgsRectangle( -178.601, -58.079, -157.499, 83.161 ) );
  sTimeZones->insert( -43200, QgsRectangle( -179.999, -58.025, -172.499, 83.169 ) );
}
```


