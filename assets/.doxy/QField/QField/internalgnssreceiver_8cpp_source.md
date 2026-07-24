

# File internalgnssreceiver.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**internalgnssreceiver.cpp**](internalgnssreceiver_8cpp.md)

[Go to the documentation of this file](internalgnssreceiver_8cpp.md)


```C++
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
#include "positioningsource.h"


InternalGnssReceiver::InternalGnssReceiver( QObject *parent )
  : AbstractGnssReceiver( parent )
  , mGeoPositionSource( std::unique_ptr<QGeoPositionInfoSource>( QGeoPositionInfoSource::createDefaultSource( { { "desktopId", "ch.opengis.qfield" } }, nullptr ) ) )
  , mGeoSatelliteSource( std::unique_ptr<QGeoSatelliteInfoSource>( QGeoSatelliteInfoSource::createDefaultSource( { { "desktopId", "ch.opengis.qfield" } }, nullptr ) ) )
{
  if ( mGeoPositionSource )
  {
    mGeoPositionSource->setPreferredPositioningMethods( QGeoPositionInfoSource::AllPositioningMethods );
    mGeoPositionSource->setUpdateInterval( 1000 );

    connect( mGeoPositionSource.get(), &QGeoPositionInfoSource::positionUpdated, this, &InternalGnssReceiver::handlePositionUpdated );
    connect( mGeoPositionSource.get(), qOverload<QGeoPositionInfoSource::Error>( &QGeoPositionInfoSource::errorOccurred ), this, &InternalGnssReceiver::handleError );

    setSocketState( QAbstractSocket::ConnectedState );

    setValid( true );
  }

  if ( mGeoSatelliteSource )
  {
    mGeoSatelliteSource->setUpdateInterval( 1000 );

    connect( mGeoSatelliteSource.get(), &QGeoSatelliteInfoSource::satellitesInUseUpdated, this, &InternalGnssReceiver::handleSatellitesInUseUpdated );
    connect( mGeoSatelliteSource.get(), &QGeoSatelliteInfoSource::satellitesInViewUpdated, this, &InternalGnssReceiver::handleSatellitesInViewUpdated );
    connect( mGeoSatelliteSource.get(), qOverload<QGeoSatelliteInfoSource::Error>( &QGeoSatelliteInfoSource::errorOccurred ), this, &InternalGnssReceiver::handleSatelliteError );
  }
}

InternalGnssReceiver::~InternalGnssReceiver()
{
  disconnectDevice();
}

void InternalGnssReceiver::handleDisconnectDevice()
{
  if ( mGeoPositionSource )
  {
    mGeoPositionSource->stopUpdates();
    mLastGnssPositionValid = false;
    mActive = false;
  }
  if ( mGeoSatelliteSource )
  {
    mGeoSatelliteSource->stopUpdates();
    mSatelliteInformationValid = false;
  }
}

void InternalGnssReceiver::handleConnectDevice()
{
  if ( mGeoPositionSource )
  {
    // Capture previous satellite position capture to keep track of _new_ satellite position for this session
    mSatellitePositionReceived = false;
    const QGeoPositionInfo satellitePosition = mGeoPositionSource->lastKnownPosition( true );
    mPreconnectSatellitePositionTimestamp = satellitePosition.isValid() ? satellitePosition.timestamp() : QDateTime();

    mGeoPositionSource->startUpdates();
    mActive = true;
  }
  if ( mGeoSatelliteSource )
  {
    mGeoSatelliteSource->startUpdates();
  }
}

void InternalGnssReceiver::handlePositionUpdated( const QGeoPositionInfo &positionInfo )
{
  const QGeoPositionInfo satellitePosition = mGeoPositionSource ? mGeoPositionSource->lastKnownPosition( true ) : QGeoPositionInfo();
  if ( !mSatellitePositionReceived )
  {
    if ( satellitePosition.isValid()
         && ( mPreconnectSatellitePositionTimestamp.isNull() || satellitePosition.timestamp() > mPreconnectSatellitePositionTimestamp ) )
    {
      // Fresh satellite fix received during this session
      mSatellitePositionReceived = true;
    }
  }

  const QGeoPositionInfo pickedPositionInfo = mSatellitePositionReceived ? satellitePosition : positionInfo;
  if ( mLastGnssPositionValid && !pickedPositionInfo.coordinate().isValid() )
  {
    return;
  }

  bool updatePositionInformation = false;

  double latitude = mLastGnssPositionInformation.latitude();
  if ( !qgsDoubleNear( pickedPositionInfo.coordinate().latitude(), latitude ) )
  {
    latitude = pickedPositionInfo.coordinate().latitude();
    updatePositionInformation = true;
  }
  double longitude = mLastGnssPositionInformation.longitude();
  if ( !qgsDoubleNear( pickedPositionInfo.coordinate().longitude(), longitude ) )
  {
    longitude = pickedPositionInfo.coordinate().longitude();
    updatePositionInformation = true;
  }

  double antennaHeight = 0.0;
  if ( const PositioningSource *positioningSource = qobject_cast<PositioningSource *>( parent() ) )
  {
    antennaHeight = positioningSource->antennaHeight();
  }

  double elevation = mLastGnssPositionInformation.elevation();
  if ( !qgsDoubleNear( pickedPositionInfo.coordinate().altitude(), elevation ) )
  {
    elevation = pickedPositionInfo.coordinate().altitude() - antennaHeight;
    updatePositionInformation = true;
  }

  std::function<double( QGeoPositionInfo::Attribute, double )> updateAttribute =
    [&pickedPositionInfo, &updatePositionInformation]( QGeoPositionInfo::Attribute attribute, double value ) -> double {
    if ( pickedPositionInfo.hasAttribute( attribute ) && !qgsDoubleNear( pickedPositionInfo.attribute( attribute ), value ) )
    {
      updatePositionInformation = true;
      return pickedPositionInfo.attribute( attribute );
    }
    return value;
  };

  const double speed = updateAttribute( QGeoPositionInfo::GroundSpeed, mLastGnssPositionInformation.speed() );
  const double direction = updateAttribute( QGeoPositionInfo::Direction, mLastGnssPositionInformation.direction() );
  const double hacc = updateAttribute( QGeoPositionInfo::HorizontalAccuracy, mLastGnssPositionInformation.hacc() );
  const double vacc = updateAttribute( QGeoPositionInfo::VerticalAccuracy, mLastGnssPositionInformation.vacc() );
  const double verticalSpeed = updateAttribute( QGeoPositionInfo::VerticalSpeed, mLastGnssPositionInformation.verticalSpeed() );
  const double magneticVariation = updateAttribute( QGeoPositionInfo::MagneticVariation, mLastGnssPositionInformation.magneticVariation() );
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
                                                            pickedPositionInfo.timestamp(),
                                                            QChar(), 0, -1, static_cast<int>( mSatellitesID.size() ), QChar( 'A' ), mSatellitesID, mSatelliteInformationValid,
                                                            verticalSpeed,
                                                            magneticVariation,
                                                            0, mGeoPositionSource->sourceName() );
    emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
  }
}

void InternalGnssReceiver::handleSatellitesInUseUpdated( const QList<QGeoSatelliteInfo> &satellites )
{
  if ( satellites.isEmpty() )
    return;

  QList<int> satellitesID;
  for ( const QGeoSatelliteInfo &satellite : satellites )
  {
    satellitesID << satellite.satelliteIdentifier();
  }
  if ( mSatellitesID != satellitesID )
  {
    mSatelliteInformationValid = true;
    mSatellitesID = satellitesID;

    for ( QgsSatelliteInfo &satelliteInfo : mSatellitesInfo ) // Not const as items modified in loop
    {
      satelliteInfo.inUse = mSatellitesID.contains( satelliteInfo.id );
    }

    if ( mLastGnssPositionValid )
    {
      mLastGnssPositionInformation = GnssPositionInformation( mLastGnssPositionInformation.latitude(),
                                                              mLastGnssPositionInformation.longitude(),
                                                              mLastGnssPositionInformation.elevation(),
                                                              mLastGnssPositionInformation.speed(),
                                                              mLastGnssPositionInformation.direction(),
                                                              mSatellitesInfo, 0, 0, 0,
                                                              mLastGnssPositionInformation.hacc(),
                                                              mLastGnssPositionInformation.vacc(),
                                                              mLastGnssPositionInformation.utcDateTime(),
                                                              QChar(), 0, -1, static_cast<int>( mSatellitesID.size() ), QChar( 'A' ), mSatellitesID, mSatelliteInformationValid,
                                                              mLastGnssPositionInformation.verticalSpeed(),
                                                              mLastGnssPositionInformation.magneticVariation(),
                                                              0, mGeoPositionSource->sourceName() );
      emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
    }
  }
}

void InternalGnssReceiver::handleSatellitesInViewUpdated( const QList<QGeoSatelliteInfo> &satellites )
{
  if ( satellites.isEmpty() )
    return;

  mSatellitesInfo.clear();
  for ( const QGeoSatelliteInfo &satellite : satellites )
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
    mSatellitesInfo << satelliteInfo;
  }
}

void InternalGnssReceiver::handleError( QGeoPositionInfoSource::Error positioningError )
{
  switch ( positioningError )
  {
    case QGeoPositionInfoSource::AccessError:
    case QGeoPositionInfoSource::ClosedError:
    {
      qDebug() << positioningError;
      break;
    }

    case QGeoPositionInfoSource::UnknownSourceError:
    case QGeoPositionInfoSource::UpdateTimeoutError:
    case QGeoPositionInfoSource::NoError:
    {
      break;
    }
  }

  return;
}

void InternalGnssReceiver::handleSatelliteError( QGeoSatelliteInfoSource::Error satelliteError )
{
  switch ( satelliteError )
  {
    case QGeoSatelliteInfoSource::AccessError:
    {
      qDebug() << satelliteError;
      break;
    }

    case QGeoSatelliteInfoSource::ClosedError:
    {
      qDebug() << satelliteError;
      mSatelliteInformationValid = false;
      mSatellitesID.clear();
      mSatellitesInfo.clear();
    }

    case QGeoSatelliteInfoSource::UnknownSourceError:
    case QGeoSatelliteInfoSource::UpdateTimeoutError:
    case QGeoSatelliteInfoSource::NoError:
    {
      break;
    }
  }

  return;
}
```


