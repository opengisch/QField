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

    setValid( true );
  }
}

void InternalGnssReceiver::handleDisconnectDevice()
{
  mGeoPositionSource->stopUpdates();
  mLastGnssPositionValid = false;
}

void InternalGnssReceiver::handleConnectDevice()
{
  mGeoPositionSource->startUpdates();
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
                                                            QList<QgsSatelliteInfo>(), 0, 0, 0,
                                                            hacc,
                                                            vacc,
                                                            positionInfo.timestamp(),
                                                            QChar(), 0, -1, 0, QChar( 'A' ), QList<int>(), false,
                                                            verticalSpeed,
                                                            magneticVariation,
                                                            0, mGeoPositionSource->sourceName() );
    emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
  }
}

void InternalGnssReceiver::handleError( QGeoPositionInfoSource::Error positioningError )
{
  qDebug() << positioningError;
  return;
}
