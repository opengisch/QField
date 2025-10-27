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

  double latitude = std::numeric_limits<double>::quiet_NaN();
  double longitude = std::numeric_limits<double>::quiet_NaN();
  double elevation = std::numeric_limits<double>::quiet_NaN();
  double speed = std::numeric_limits<double>::quiet_NaN();
  double direction = std::numeric_limits<double>::quiet_NaN();
  double pdop = 0;
  double hdop = 0;
  double vdop = 0;
  double hacc = std::numeric_limits<double>::quiet_NaN();
  double vacc = std::numeric_limits<double>::quiet_NaN();
  double hvacc = std::numeric_limits<double>::quiet_NaN();
  double verticalSpeed = std::numeric_limits<double>::quiet_NaN();
  double magneticVariation = std::numeric_limits<double>::quiet_NaN();

  QDateTime utcDateTime = positionsInformation.last().utcDateTime();

  QList<QgsSatelliteInfo> satellitesInView = positionsInformation.at( 0 ).satellitesInView();
  int satellitesUsed = satellitesInView.size();
  bool satInfoComplete = positionsInformation.at( 0 ).satInfoComplete();
  QList<int> satPrn = positionsInformation.at( 0 ).satPrn();
  QChar status = positionsInformation.at( 0 ).status();
  QChar fixMode = positionsInformation.at( 0 ).fixMode();
  int fixType = positionsInformation.at( 0 ).fixType();
  int quality = positionsInformation.at( 0 ).quality();
  QString sourceName = QStringLiteral( "%1 (%2)" ).arg( positionsInformation.at( 0 ).sourceName(), QObject::tr( "averaged" ) );

  int validPositionsCount = 0;
  for ( const GnssPositionInformation &pi : positionsInformation )
  {
    if ( !std::isnan( pi.latitude() ) )
      latitude = !std::isnan( latitude ) ? latitude + pi.latitude() : pi.latitude();
    if ( !std::isnan( pi.longitude() ) )
      longitude = !std::isnan( longitude ) ? longitude + pi.longitude() : pi.longitude();
    if ( !std::isnan( pi.elevation() ) )
      elevation = !std::isnan( elevation ) ? elevation + pi.elevation() : pi.elevation();
    if ( !std::isnan( pi.speed() ) )
      speed = !std::isnan( speed ) ? speed + pi.speed() : pi.speed();
    if ( !std::isnan( pi.direction() ) )
      direction = !std::isnan( direction ) ? direction + pi.direction() : pi.direction();
    pdop += pi.pdop();
    hdop += pi.hdop();
    vdop += pi.vdop();
    if ( !std::isnan( pi.hacc() ) )
      hacc = !std::isnan( hacc ) ? hacc + pi.hacc() : pi.hacc();
    if ( !std::isnan( pi.vacc() ) )
      vacc = !std::isnan( vacc ) ? vacc + pi.vacc() : pi.vacc();
    if ( !std::isnan( pi.hvacc() ) )
      hvacc = !std::isnan( hvacc ) ? hvacc + pi.hvacc() : pi.hvacc();
    if ( !std::isnan( pi.verticalSpeed() ) )
      verticalSpeed = !std::isnan( verticalSpeed ) ? verticalSpeed + pi.verticalSpeed() : pi.verticalSpeed();
    if ( !std::isnan( pi.magneticVariation() ) )
      magneticVariation = !std::isnan( magneticVariation ) ? magneticVariation + pi.magneticVariation() : pi.magneticVariation();

    ++validPositionsCount;
  }

  if ( validPositionsCount == 0 )
    return GnssPositionInformation(); // No valid data to average

  return GnssPositionInformation( latitude / validPositionsCount, longitude / validPositionsCount, elevation / validPositionsCount,
                                  speed / validPositionsCount, direction / validPositionsCount, satellitesInView,
                                  pdop / validPositionsCount, hdop / validPositionsCount, vdop / validPositionsCount,
                                  hacc / validPositionsCount, vacc / validPositionsCount, utcDateTime,
                                  fixMode, fixType, quality, satellitesUsed, status, satPrn, satInfoComplete,
                                  verticalSpeed / validPositionsCount, magneticVariation / validPositionsCount, validPositionsCount, sourceName );
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

QgsRectangle PositioningUtils::createExtentForDevice( const GnssPositionInformation &positionInformation, const QgsCoordinateReferenceSystem &crs )
{
  QgsRectangle extent;
  QgsCoordinateReferenceSystem extentCrs;
  if ( positionInformation.latitudeValid() && positionInformation.longitudeValid() && positionInformation.hvaccValid() )
  {
    extent = QgsRectangle( positionInformation.longitude() - 1.0, positionInformation.latitude() - 1.0, positionInformation.longitude() + 1.0, positionInformation.latitude() + 1.0 );
    extentCrs = QgsCoordinateReferenceSystem::fromEpsgId( 4326 );
  }
  else
  {
    const QString timeZoneName = QTimeZone::systemTimeZone().displayName( QTimeZone::StandardTime, QTimeZone::OffsetName, QLocale::c() );
    std::unique_ptr<QgsVectorLayer> timeZoneLayer = std::make_unique<QgsVectorLayer>( QStringLiteral( "%1/qfield/data/timezones.gpkg" ).arg( PlatformUtilities::instance()->systemSharedDataLocation() ) );
    QgsFeatureIterator it = timeZoneLayer->getFeatures( QStringLiteral( "UTC_offset_ST = '%1'" ).arg( timeZoneName.mid( 3 ) ) );
    QgsFeature feature;
    if ( it.nextFeature( feature ) )
    {
      extent = feature.geometry().boundingBox();
      extentCrs = timeZoneLayer->crs();
    }
  }

  if ( extent.isEmpty() )
  {
    extent = crs.bounds();
    extentCrs = QgsCoordinateReferenceSystem::fromEpsgId( 4326 );
  }

  if ( extentCrs != crs )
  {
    QgsCoordinateTransform transform( extentCrs, crs, QgsProject::instance()->transformContext() );
    try
    {
      extent = transform.transform( extent );
    }
    catch ( const QgsException &e )
    {
      extent = QgsRectangle();
    }
  }

  return extent;
}
