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

#include <QGuiApplication>
#include <qgsapplication.h>

InternalGnssReceiver::InternalGnssReceiver( QObject *parent )
  : AbstractGnssReceiver( parent )
  , mGeoPositionSource( std::unique_ptr<QGeoPositionInfoSource>( QGeoPositionInfoSource::createDefaultSource( nullptr ) ) )
{
  if ( mGeoPositionSource && mGeoPositionSource->error() == QGeoPositionInfoSource::NoError )
  {
    mGeoPositionSource->setPreferredPositioningMethods( QGeoPositionInfoSource::AllPositioningMethods );
    mGeoPositionSource->setUpdateInterval( 1000 );

    connect( mGeoPositionSource.get(), &QGeoPositionInfoSource::positionUpdated, this, &InternalGnssReceiver::handlePositionUpdated );
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
    connect( mGeoPositionSource.get(), qOverload<QGeoPositionInfoSource::Error>( &QGeoPositionInfoSource::error ), this, &InternalGnssReceiver::handleError );
#else
    connect( mGeoPositionSource.get(), qOverload<QGeoPositionInfoSource::Error>( &QGeoPositionInfoSource::errorOccurred ), this, &InternalGnssReceiver::handleError );
#endif

    mSocketState = QAbstractSocket::ConnectedState;

    setValid( true );
  }

  mGeoSatelliteSource = std::unique_ptr<QGeoSatelliteInfoSource>( QGeoSatelliteInfoSource::createDefaultSource( nullptr ) );
  if ( mGeoSatelliteSource && mGeoSatelliteSource->error() == QGeoSatelliteInfoSource::NoError )
  {
    mGeoSatelliteSource->setUpdateInterval( 1000 );

    connect( mGeoSatelliteSource.get(), &QGeoSatelliteInfoSource::satellitesInUseUpdated, this, &InternalGnssReceiver::handleSatellitesInUseUpdated );
    connect( mGeoSatelliteSource.get(), &QGeoSatelliteInfoSource::satellitesInViewUpdated, this, &InternalGnssReceiver::handleSatellitesInViewUpdated );
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
    connect( mGeoSatelliteSource.get(), qOverload<QGeoSatelliteInfoSource::Error>( &QGeoSatelliteInfoSource::error ), this, &InternalGnssReceiver::handleSatelliteError );
#else
    connect( mGeoSatelliteSource.get(), qOverload<QGeoSatelliteInfoSource::Error>( &QGeoSatelliteInfoSource::errorOccurred ), this, &InternalGnssReceiver::handleSatelliteError );
#endif
  }

  connect( QgsApplication::instance(), &QGuiApplication::applicationStateChanged, this, &InternalGnssReceiver::onApplicationStateChanged );
}

void InternalGnssReceiver::onApplicationStateChanged( Qt::ApplicationState state )
{
#ifdef Q_OS_ANDROID
  // Google Play policy only allows for background access if it's explicitly stated and justified
  // Not stopping on Activity::onPause is detected as violation
  switch ( state )
  {
    case Qt::ApplicationState::ApplicationActive:
      if ( mActive )
      {
        mGeoPositionSource->startUpdates();
        if ( mGeoSatelliteSource )
          mGeoSatelliteSource->startUpdates();
      }
      break;
    default:
      if ( mActive )
      {
        mGeoPositionSource->stopUpdates();
        if ( mGeoSatelliteSource )
          mGeoSatelliteSource->stopUpdates();
      }
  }
#else
  Q_UNUSED( state )
#endif
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
    mGeoPositionSource->startUpdates();
    mActive = true;
  }
  if ( mGeoSatelliteSource )
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
                                                            positionInfo.timestamp(),
                                                            QChar(), 0, -1, mSatellitesID.size(), QChar( 'A' ), mSatellitesID, mSatelliteInformationValid,
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
                                                              QChar(), 0, -1, mSatellitesID.size(), QChar( 'A' ), mSatellitesID, mSatelliteInformationValid,
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

  QList<QgsSatelliteInfo> satellitesInfo;
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
  if ( satelliteError == QGeoSatelliteInfoSource::ClosedError )
  {
    mSatelliteInformationValid = false;
    mSatellitesID.clear();
    mSatellitesInfo.clear();
  }
  return;
}
