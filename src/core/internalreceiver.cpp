/***************************************************************************
 internalreceiver.cpp - InternalReceiver

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

#include "internalreceiver.h"

#include <QDebug>

InternalReceiver::InternalReceiver( QObject *parent )
  : AbstractGnssReceiver( parent )
  , mGeoPositionSource( std::unique_ptr<QGeoPositionInfoSource>( QGeoPositionInfoSource::createDefaultSource( nullptr ) ) )
{
  ;

  if ( mGeoPositionSource.get() && mGeoPositionSource->error() == QGeoPositionInfoSource::NoError )
  {
    mGeoPositionSource->setUpdateInterval( 1000 );

    connect( mGeoPositionSource.get(), &QGeoPositionInfoSource::positionUpdated, this, &InternalReceiver::handlePositionUpdated );
    connect( mGeoPositionSource.get(), qOverload<QGeoPositionInfoSource::Error>( &QGeoPositionInfoSource::error ), this, &InternalReceiver::handleError );
  }
}

void InternalReceiver::handleDisconnectDevice()
{
  mGeoPositionSource->stopUpdates();
  mLastGnssPositionValid = false;
}

void InternalReceiver::handleConnectDevice()
{
  mGeoPositionSource->startUpdates();
}

void InternalReceiver::handlePositionUpdated( const QGeoPositionInfo &positionInfo )
{
  if ( mLastGnssPositionValid && !positionInfo.coordinate().isValid() )
  {
    return;
  }

  GnssPositionInformation positionInformation( positionInfo.coordinate().latitude(),
                                               positionInfo.coordinate().longitude(),
                                               positionInfo.coordinate().altitude(),
                                               positionInfo.hasAttribute( QGeoPositionInfo::GroundSpeed )
                                                 ? positionInfo.attribute( QGeoPositionInfo::GroundSpeed )
                                                 : std::numeric_limits<double>::quiet_NaN(),
                                               positionInfo.hasAttribute( QGeoPositionInfo::Direction )
                                                 ? positionInfo.attribute( QGeoPositionInfo::Direction )
                                                 : std::numeric_limits<double>::quiet_NaN(),
                                               QList<QgsSatelliteInfo>(), 0, 0, 0,
                                               positionInfo.hasAttribute( QGeoPositionInfo::HorizontalAccuracy )
                                                 ? positionInfo.attribute( QGeoPositionInfo::HorizontalAccuracy )
                                                 : std::numeric_limits<double>::quiet_NaN(),
                                               positionInfo.hasAttribute( QGeoPositionInfo::VerticalAccuracy )
                                                 ? positionInfo.attribute( QGeoPositionInfo::VerticalAccuracy )
                                                 : std::numeric_limits<double>::quiet_NaN(),
                                               positionInfo.timestamp(),
                                               QChar(), 0, -1, 0, QChar( 'A' ), QList<int>(), false,
                                               positionInfo.hasAttribute( QGeoPositionInfo::VerticalSpeed )
                                                 ? positionInfo.attribute( QGeoPositionInfo::VerticalSpeed )
                                                 : std::numeric_limits<double>::quiet_NaN(),
                                               positionInfo.hasAttribute( QGeoPositionInfo::MagneticVariation )
                                                 ? positionInfo.attribute( QGeoPositionInfo::MagneticVariation )
                                                 : std::numeric_limits<double>::quiet_NaN(),
                                               0, mGeoPositionSource->sourceName() );
  if ( mLastGnssPositionInformation != positionInformation )
  {
    mLastGnssPositionInformation = positionInformation;

    emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
  }
}

void InternalReceiver::handleError( QGeoPositionInfoSource::Error positioningError )
{
  qDebug() << positioningError;
  return;
}
