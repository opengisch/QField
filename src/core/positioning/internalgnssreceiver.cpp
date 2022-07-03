/***************************************************************************
 internalgnssreceiver.cpp - InternalGnssReceiver

 ---------------------
 begin                : 22.05.2022
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

#include "internalgnssreceiver.h"

#include <QDebug>

InternalGnssReceiver::InternalGnssReceiver( QObject *parent )
  : AbstractGnssReceiver( parent )
  , mGeoPositionSource( std::unique_ptr<QGeoPositionInfoSource>( QGeoPositionInfoSource::createDefaultSource( nullptr ) ) )
{
  if ( mGeoPositionSource.get() && mGeoPositionSource->error() == QGeoPositionInfoSource::NoError )
  {
    mGeoPositionSource->setPreferredPositioningMethods( QGeoPositionInfoSource::AllPositioningMethods );
    mGeoPositionSource->setUpdateInterval( 1000 );

    connect( mGeoPositionSource.get(), &QGeoPositionInfoSource::positionUpdated, this, &InternalGnssReceiver::handlePositionUpdated );
    connect( mGeoPositionSource.get(), qOverload<QGeoPositionInfoSource::Error>( &QGeoPositionInfoSource::error ), this, &InternalGnssReceiver::handleError );

    mSocketState = QAbstractSocket::ConnectedState;

    setValid( true );

    mGeoSatelliteSource = std::unique_ptr<QGeoSatelliteInfoSource>( QGeoSatelliteInfoSource::createDefaultSource( nullptr ) );
    if ( mGeoSatelliteSource.get() && mGeoSatelliteSource->error() == QGeoSatelliteInfoSource::NoError )
    {
      connect( mGeoSatelliteSource.get(), &QGeoSatelliteInfoSource::satellitesInUseUpdated, this, &InternalGnssReceiver::handleSatellitesInUseUpdated );
      connect( mGeoSatelliteSource.get(), &QGeoSatelliteInfoSource::satellitesInViewUpdated, this, &InternalGnssReceiver::handleSatellitesInViewUpdated );
      connect( mGeoSatelliteSource.get(), qOverload<QGeoSatelliteInfoSource::Error>( &QGeoSatelliteInfoSource::error ), this, &InternalGnssReceiver::handleSatelliteError );
    }
  }
}

void InternalGnssReceiver::handleDisconnectDevice()
{
  mGeoPositionSource->stopUpdates();
  mGeoSatelliteSource->stopUpdates();
  mLastGnssPositionValid = false;
  mSatelliteInformationValid = false;
}

void InternalGnssReceiver::handleConnectDevice()
{
  mGeoPositionSource->startUpdates();
  mGeoSatelliteSource->startUpdates();
}

void InternalGnssReceiver::handlePositionUpdated( const QGeoPositionInfo &positionInfo )
{
  if ( mLastGnssPositionValid && !positionInfo.coordinate().isValid() )
  {
    return;
  }

  bool updatePositionInformation = false;

  double latitude = mLastGnssPositionInformation.latitude();
  if ( !qgsDoubleNear( positionInfo.coordinate().latitude(), latitude ) )
  {
    latitude = positionInfo.coordinate().latitude();
    updatePositionInformation = true;
  }
  double longitude = mLastGnssPositionInformation.latitude();
  if ( !qgsDoubleNear( positionInfo.coordinate().longitude(), longitude ) )
  {
    longitude = positionInfo.coordinate().longitude();
    updatePositionInformation = true;
  }
  double elevation = mLastGnssPositionInformation.elevation();
  if ( !qgsDoubleNear( positionInfo.coordinate().altitude(), elevation ) )
  {
    elevation = positionInfo.coordinate().altitude();
    updatePositionInformation = true;
  }

  /**
   * Per Qt header documentation, the only details that are _always_ provided in an update are
   * the latitude, longitude, and elevation. Other attributes are not always served, we therefore
   * have to skip updating those until the next time they are present in an update.
   */
  std::function<double( QGeoPositionInfo::Attribute, double )> updateAttribute =
    [&positionInfo, &updatePositionInformation]( QGeoPositionInfo::Attribute attribute, double value ) -> double {
    if ( positionInfo.hasAttribute( attribute ) && !qgsDoubleNear( positionInfo.attribute( attribute ), value ) )
    {
      updatePositionInformation = true;
      return positionInfo.attribute( attribute );
    }
    return value;
  };

  const double speed = updateAttribute( QGeoPositionInfo::GroundSpeed, mLastGnssPositionInformation.speed() );
  const double direction = updateAttribute( QGeoPositionInfo::Direction, mLastGnssPositionInformation.direction() );
  const double hacc = updateAttribute( QGeoPositionInfo::HorizontalAccuracy, mLastGnssPositionInformation.hacc() );
  const double vacc = updateAttribute( QGeoPositionInfo::VerticalAccuracy, mLastGnssPositionInformation.vacc() );
  const double verticalSpeed = updateAttribute( QGeoPositionInfo::VerticalSpeed, mLastGnssPositionInformation.verticalSpeed() );
  const double magneticVariation = updateAttribute( QGeoPositionInfo::VerticalSpeed, mLastGnssPositionInformation.magneticVariation() );
  if ( updatePositionInformation )
  {
    mLastGnssPositionInformation = GnssPositionInformation( latitude,
                                                            longitude,
                                                            elevation,
                                                            speed,
                                                            direction,
                                                            mSatellitesInfo, 0, 0, 0,
                                                            hacc,
                                                            vacc,
                                                            positionInfo.timestamp(),
                                                            QChar(), 0, -1, mSatellitesUsed, QChar( 'A' ), mSatellitesID, mSatelliteInformationValid,
                                                            verticalSpeed,
                                                            magneticVariation,
                                                            0, mGeoPositionSource->sourceName() );
    emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
  }
}

void InternalGnssReceiver::handleSatellitesInUseUpdated( const QList<QGeoSatelliteInfo> &satellites )
{
  const int satellitesUsed = satellites.size();
  QList<int> satellitesID;
  for ( const QGeoSatelliteInfo &satellite : satellites )
  {
    satellitesID << satellite.satelliteIdentifier();
  }
  if ( mSatellitesUsed != satellitesUsed
       || mSatellitesID != satellitesID )
  {
    mSatelliteInformationValid = true;
    mSatellitesUsed = satellitesUsed;
    mSatellitesID = satellitesID;

    for ( QgsSatelliteInfo &satelliteInfo : mSatellitesInfo )
    {
      satelliteInfo.inUse = mSatellitesID.contains( satelliteInfo.id );
    }

    mLastGnssPositionInformation = GnssPositionInformation( mLastGnssPositionInformation.latitude(),
                                                            mLastGnssPositionInformation.longitude(),
                                                            mLastGnssPositionInformation.elevation(),
                                                            mLastGnssPositionInformation.speed(),
                                                            mLastGnssPositionInformation.direction(),
                                                            mSatellitesInfo, 0, 0, 0,
                                                            mLastGnssPositionInformation.hacc(),
                                                            mLastGnssPositionInformation.vacc(),
                                                            mLastGnssPositionInformation.utcDateTime(),
                                                            QChar(), 0, -1, mSatellitesUsed, QChar( 'A' ), mSatellitesID, mSatelliteInformationValid,
                                                            mLastGnssPositionInformation.verticalSpeed(),
                                                            mLastGnssPositionInformation.magneticVariation(),
                                                            0, mGeoPositionSource->sourceName() );
    emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
  }
}

void InternalGnssReceiver::handleSatellitesInViewUpdated( const QList<QGeoSatelliteInfo> &satellites )
{
  QList<QgsSatelliteInfo> satellitesInfo;
  for ( const QGeoSatelliteInfo satellite : satellites )
  {
    QgsSatelliteInfo satelliteInfo;
    satelliteInfo.elevation = satellite.hasAttribute( QGeoSatelliteInfo::Elevation )
                                ? satellite.attribute( QGeoSatelliteInfo::Elevation )
                                : std::numeric_limits<double>::quiet_NaN();
    satelliteInfo.azimuth = satellite.hasAttribute( QGeoSatelliteInfo::Azimuth )
                              ? satellite.attribute( QGeoSatelliteInfo::Azimuth )
                              : std::numeric_limits<double>::quiet_NaN();
    satelliteInfo.id = satellite.satelliteIdentifier();
    satelliteInfo.signal = satellite.signalStrength();
    satelliteInfo.inUse = mSatellitesID.contains( satellite.satelliteIdentifier() );
  }
}

void InternalGnssReceiver::handleError( QGeoPositionInfoSource::Error positioningError )
{
  qDebug() << positioningError;
  return;
}

void InternalGnssReceiver::handleSatelliteError( QGeoSatelliteInfoSource::Error satelliteError )
{
  qDebug() << satelliteError;
  return;
}
