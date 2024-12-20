/***************************************************************************
  positioning.cpp - Positioning

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

#include "positioning.h"
#include "positioningutils.h"

#include <qgsapplication.h>
#include <qgsunittypes.h>

Positioning::Positioning( QObject *parent )
  : QObject( parent )
{
  // Non-service path, we are both the host and the node
  mPositioningSource = new PositioningSource( this );
  mHost.setHostUrl( QUrl( QStringLiteral( "local:replica" ) ) );
  mHost.enableRemoting( mPositioningSource, "PositioningSource" );

  mNode.connectToNode( QUrl( QStringLiteral( "local:replica" ) ) );
  mPositioningSourceReplica.reset( mNode.acquireDynamic( "PositioningSource" ) );
  mPositioningSourceReplica->waitForSource();

  connect( mPositioningSourceReplica.data(), SIGNAL( activeChanged() ), this, SIGNAL( activeChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( validChanged() ), this, SIGNAL( validChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( deviceIdChanged() ), this, SIGNAL( deviceIdChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( deviceLastErrorChanged() ), this, SIGNAL( deviceLastErrorChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( deviceSocketStateChanged() ), this, SIGNAL( deviceSocketStateChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( deviceSocketStateStringChanged() ), this, SIGNAL( deviceSocketStateStringChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( averagedPositionChanged() ), this, SIGNAL( averagedPositionChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( averagedPositionCountChanged() ), this, SIGNAL( averagedPositionCountChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( elevationCorrectionModeChanged() ), this, SIGNAL( elevationCorrectionModeChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( antennaHeightChanged() ), this, SIGNAL( antennaHeightChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( orientationChanged() ), this, SIGNAL( orientationChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( loggingChanged() ), this, SIGNAL( loggingChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( positionInformationChanged() ), this, SLOT( processGnssPositionInformation() ) );

  connect( this, SIGNAL( triggerConnectDevice() ), mPositioningSourceReplica.data(), SLOT( triggerConnectDevice() ) );
  connect( this, SIGNAL( triggerDisconnectDevice() ), mPositioningSourceReplica.data(), SLOT( triggerDisconnectDevice() ) );

  connect( QgsApplication::instance(), &QGuiApplication::applicationStateChanged, this, &Positioning::onApplicationStateChanged );
}

void Positioning::onApplicationStateChanged( Qt::ApplicationState state )
{
#ifdef Q_OS_ANDROID
  // Google Play policy only allows for background access if it's explicitly stated and justified
  // Not stopping on Activity::onPause is detected as violation
  switch ( state )
  {
    case Qt::ApplicationState::ApplicationActive:
      if ( mActive )
      {
        emit triggerConnectDevice();
      }
      break;
    default:
      if ( mActive )
      {
        emit triggerDisconnectDevice();
      }
  }
#else
  Q_UNUSED( state )
#endif
}

bool Positioning::active() const
{
  return mPositioningSourceReplica->property( "active" ).toBool();
}

void Positioning::setActive( bool active )
{
  mPositioningSourceReplica->setProperty( "active", active );
}

bool Positioning::valid() const
{
  return mPositioningSourceReplica->property( "valid" ).toBool();
}

void Positioning::setValid( bool valid )
{
  mPositioningSourceReplica->setProperty( "valid", valid );
}

QString Positioning::deviceId() const
{
  return mPositioningSourceReplica->property( "deviceId" ).toString();
}

void Positioning::setDeviceId( const QString &id )
{
  mPositioningSourceReplica->setProperty( "deviceId", id );
}

QList<QPair<QString, QVariant>> Positioning::deviceDetails() const
{
  const QVariantList list = mPositioningSourceReplica->property( "deviceDetails" ).toList();
  QList<QPair<QString, QVariant>> details;
  for ( const QVariant &item : list )
  {
    details << item.value<QPair<QString, QVariant>>();
  }
  return details;
}

QString Positioning::deviceLastError() const
{
  return mPositioningSourceReplica->property( "deviceLastError" ).toString();
}

QAbstractSocket::SocketState Positioning::deviceSocketState() const
{
  return mPositioningSourceReplica->property( "deviceSocketState" ).value<QAbstractSocket::SocketState>();
}

QString Positioning::deviceSocketStateString() const
{
  return mPositioningSourceReplica->property( "deviceSocketStateString" ).toString();
}

AbstractGnssReceiver::Capabilities Positioning::deviceCapabilities() const
{
  const QString deviceId = mPositioningSourceReplica->property( "deviceId" ).toString();
  if ( !deviceId.isEmpty() || deviceId.startsWith( QStringLiteral( "tcp:" ) ) || deviceId.startsWith( QStringLiteral( "ucp:" ) ) || deviceId.startsWith( QStringLiteral( "serial:" ) ) )
  {
    // NMEA-based devices
    return AbstractGnssReceiver::Capabilities() | AbstractGnssReceiver::OrthometricAltitude | AbstractGnssReceiver::Logging;
  }

  return AbstractGnssReceiver::NoCapabilities;
}

int Positioning::averagedPositionCount() const
{
  return mPositioningSourceReplica->property( "averagedPositionCount" ).toInt();
}

bool Positioning::averagedPosition() const
{
  return mPositioningSourceReplica->property( "averagedPosition" ).toBool();
}

void Positioning::setAveragedPosition( bool averaged )
{
  mPositioningSourceReplica->setProperty( "averagedPosition", averaged );
}

bool Positioning::logging() const
{
  return mPositioningSourceReplica->property( "logging" ).toBool();
}

void Positioning::setLogging( bool logging )
{
  mPositioningSourceReplica->setProperty( "logging", logging );
}

PositioningSource::ElevationCorrectionMode Positioning::elevationCorrectionMode() const
{
  return static_cast<PositioningSource::ElevationCorrectionMode>( mPositioningSourceReplica->property( "elevationCorrectionMode" ).toInt() );
}

void Positioning::setElevationCorrectionMode( PositioningSource::ElevationCorrectionMode elevationCorrectionMode )
{
  mPositioningSourceReplica->setProperty( "elevationCorrectionMode", static_cast<int>( elevationCorrectionMode ) );
}

double Positioning::antennaHeight() const
{
  return mPositioningSourceReplica->property( "antennaHeight" ).toDouble();
}

void Positioning::setAntennaHeight( double antennaHeight )
{
  mPositioningSourceReplica->setProperty( "antennaHeight", antennaHeight );
}

GnssPositionInformation Positioning::positionInformation() const
{
  return mPositionInformation;
}

double Positioning::orientation() const
{
  return mPositioningSourceReplica->property( "orientation" ).toDouble();
}

void Positioning::setCoordinateTransformer( QgsQuickCoordinateTransformer *coordinateTransformer )
{
  if ( mCoordinateTransformer == coordinateTransformer )
    return;

  if ( mCoordinateTransformer )
  {
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::projectedPositionChanged, this, &Positioning::projectedPositionTransformed );
  }

  mCoordinateTransformer = coordinateTransformer;
  connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::projectedPositionChanged, this, &Positioning::projectedPositionTransformed );

  emit coordinateTransformerChanged();
}

QgsPoint Positioning::sourcePosition() const
{
  return mSourcePosition;
}

QgsPoint Positioning::projectedPosition() const
{
  return mProjectedPosition;
}

double Positioning::projectedHorizontalAccuracy() const
{
  return mProjectedHorizontalAccuracy;
}

void Positioning::processGnssPositionInformation()
{
  mPositionInformation = mPositioningSourceReplica->property( "positionInformation" ).value<GnssPositionInformation>();

  if ( mPositionInformation.isValid() )
  {
    mSourcePosition = QgsPoint( mPositionInformation.longitude(), mPositionInformation.latitude(), mPositionInformation.elevation() );
  }
  else
  {
    mSourcePosition.clear();
  }

  if ( mCoordinateTransformer )
  {
    mCoordinateTransformer->setSourcePosition( mSourcePosition );
  }

  emit positionInformationChanged();
}

void Positioning::projectedPositionTransformed()
{
  mProjectedPosition = mCoordinateTransformer->projectedPosition();
  mProjectedHorizontalAccuracy = mPositionInformation.hacc();
  if ( mPositionInformation.haccValid() )
  {
    if ( mCoordinateTransformer->destinationCrs().mapUnits() != Qgis::DistanceUnit::Unknown )
    {
      mProjectedHorizontalAccuracy *= QgsUnitTypes::fromUnitToUnitFactor( Qgis::DistanceUnit::Meters,
                                                                          mCoordinateTransformer->destinationCrs().mapUnits() );
    }
    else
    {
      mProjectedHorizontalAccuracy = 0.0;
    }
  }

  emit projectedPositionChanged();
}
