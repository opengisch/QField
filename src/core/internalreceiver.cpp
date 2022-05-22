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

InternalReceiver::InternalReceiver( QObject *parent )
  : AbstractReceiver( parent )
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

void InternalReceiver::disconnectDevice()
{
  mGeoPositionSource->stopUpdates();
}

void InternalReceiver::connectDevice()
{
  mGeoPositionSource->startUpdates();
}

void InternalReceiver::handlePositionUpdated( const QGeoPositionInfo &positionInfo )
{
  if ( mLastGnssPositionValid && !positionInfo.coordinate().isValid() )
  {
    return;
  }

  qDebug() << positionInfo.attribute( QGeoPositionInfo::GroundSpeed );
  GnssPositionInformation positionInformation( positionInfo.coordinate().latitude(),
                                               positionInfo.coordinate().longitude(),
                                               positionInfo.coordinate().altitude(),
                                               positionInfo.attribute( QGeoPositionInfo::GroundSpeed ),
                                               positionInfo.attribute( QGeoPositionInfo::Direction ),
                                               QList<QgsSatelliteInfo>(), 0, 0, 0,
                                               positionInfo.attribute( QGeoPositionInfo::HorizontalAccuracy ),
                                               positionInfo.attribute( QGeoPositionInfo::VerticalAccuracy ),
                                               positionInfo.timestamp(),
                                               QChar(), 0, -1, 0, QChar( 'A' ), QList<int>(), false,
                                               positionInfo.attribute( QGeoPositionInfo::VerticalSpeed ),
                                               positionInfo.attribute( QGeoPositionInfo::MagneticVariation ),
                                               0, mGeoPositionSource->sourceName() );
  if ( mLastGnssPositionInformation != positionInformation )
  {
    mLastGnssPositionInformation = positionInformation;

    emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
  }
}

void InternalReceiver::handleError( QGeoPositionInfoSource::Error positioningError )
{
  Q_UNUSED( positioningError )
  return;
}
