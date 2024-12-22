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

#include "platformutilities.h"
#include "positioning.h"
#include "positioningutils.h"
#include "qgismobileapp.h"
#include "tcpreceiver.h"
#include "udpreceiver.h"
#ifdef WITH_SERIALPORT
#include "serialportreceiver.h"
#endif

#include <QGuiApplication>
#include <QPermissions>
#include <QScreen>
#include <qgsapplication.h>
#include <qgsunittypes.h>

Positioning::Positioning( QObject *parent )
  : QObject( parent )
{
  connect( QgsApplication::instance(), &QGuiApplication::applicationStateChanged, this, &Positioning::onApplicationStateChanged );
}

void Positioning::setupSource()
{
#if defined( Q_OS_ANDROID )
  PlatformUtilities::instance()->startPositioningService();
  mNode.connectToNode( QUrl( QStringLiteral( "localabstract:replica" ) ) );
#else
  // Non-service path, we are both the host and the node
  mPositioningSource = new PositioningSource( this );
  mHost.setHostUrl( QUrl( QStringLiteral( "local:replica" ) ) );
  mHost.enableRemoting( mPositioningSource, "PositioningSource" );
  mNode.connectToNode( QUrl( QStringLiteral( "local:replica" ) ) );
#endif

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

  const QList<QString> properties = mPropertiesToSync.keys();
  for ( const QString property : properties )
  {
    mPositioningSourceReplica->setProperty( property.toLatin1(), mPropertiesToSync[property] );
  }
}

void Positioning::onApplicationStateChanged( Qt::ApplicationState state )
{
  //#ifdef Q_OS_ANDROID
  //  // Google Play policy only allows for background access if it's explicitly stated and justified
  //  // Not stopping on Activity::onPause is detected as violation
  //  const bool isActive = active();
  //  switch ( state )
  //  {
  //    case Qt::ApplicationState::ApplicationActive:
  //      if ( isActive )
  //      {
  //        emit triggerConnectDevice();
  //      }
  //      break;
  //    default:
  //      if ( isActive )
  //      {
  //        emit triggerDisconnectDevice();
  //      }
  //  }
  //#else
  Q_UNUSED( state )
  //#endif
}

bool Positioning::active() const
{
  return mPositioningSourceReplica ? mPositioningSourceReplica->property( "active" ).toBool() : false;
}

void Positioning::setActive( bool active )
{
  if ( !mPermissionChecked )
  {
    QLocationPermission locationPermission;
    locationPermission.setAccuracy( QLocationPermission::Precise );
    Qt::PermissionStatus permissionStatus = qApp->checkPermission( locationPermission );
    if ( permissionStatus == Qt::PermissionStatus::Undetermined )
    {
      qApp->requestPermission( locationPermission, this, [=]( const QPermission &permission ) {
        if ( permission.status() == Qt::PermissionStatus::Granted )
        {
          mPermissionChecked = true;
          setActive( true );
        }
        else
        {
          setValid( false );
        }
      } );
      return;
    }
    else if ( permissionStatus == Qt::PermissionStatus::Denied )
    {
      setValid( false );
      return;
    }
  }

  if ( !mPositioningSourceReplica )
  {
    setupSource();
  }

  mPositioningSourceReplica->setProperty( "active", active );
}

bool Positioning::valid() const
{
  return mPositioningSourceReplica ? mPositioningSourceReplica->property( "valid" ).toBool() : mValid;
}

void Positioning::setValid( bool valid )
{
  if ( mPositioningSourceReplica )
  {
    mPositioningSourceReplica->setProperty( "valid", valid );
  }
  else
  {
    mValid = valid;
  }
}

QString Positioning::deviceId() const
{
  return ( mPositioningSourceReplica ? mPositioningSourceReplica->property( "deviceId" ) : mPropertiesToSync.value( "deviceId" ) ).toString();
}

void Positioning::setDeviceId( const QString &id )
{
  if ( mPositioningSourceReplica )
  {
    mPositioningSourceReplica->setProperty( "deviceId", id );
  }
  else
  {
    mPropertiesToSync["deviceId"] = id;
    emit deviceIdChanged();
  }
}

GnssPositionDetails Positioning::deviceDetails() const
{
  GnssPositionDetails list;
  if ( mPositioningSourceReplica )
  {
    list = mPositioningSourceReplica->property( "deviceDetails" ).value<GnssPositionDetails>();
  }
  return list;
}

QString Positioning::deviceLastError() const
{
  return mPositioningSourceReplica ? mPositioningSourceReplica->property( "deviceLastError" ).toString() : QString();
}

QAbstractSocket::SocketState Positioning::deviceSocketState() const
{
  return mPositioningSourceReplica ? mPositioningSourceReplica->property( "deviceSocketState" ).value<QAbstractSocket::SocketState>() : QAbstractSocket::UnconnectedState;
}

QString Positioning::deviceSocketStateString() const
{
  return mPositioningSourceReplica ? mPositioningSourceReplica->property( "deviceSocketStateString" ).toString() : QString();
}

AbstractGnssReceiver::Capabilities Positioning::deviceCapabilities() const
{
  const QString deviceId = ( mPositioningSourceReplica ? mPositioningSourceReplica->property( "deviceId" ) : mPropertiesToSync.value( "deviceId" ) ).toString();
  if ( !deviceId.isEmpty() || deviceId.startsWith( TcpReceiver::identifier + ":" ) || deviceId.startsWith( UdpReceiver::identifier + ":" ) )
  {
    // NMEA-based devices
    return AbstractGnssReceiver::Capabilities() | AbstractGnssReceiver::OrthometricAltitude | AbstractGnssReceiver::Logging;
  }
#ifdef WITH_SERIALPORT
  else if ( deviceId.startsWith( SerialPortReceiver::identifier + ":" ) )
  {
    // NMEA-based device
    return AbstractGnssReceiver::Capabilities() | AbstractGnssReceiver::OrthometricAltitude | AbstractGnssReceiver::Logging;
  }
#endif

  return AbstractGnssReceiver::NoCapabilities;
}

int Positioning::averagedPositionCount() const
{
  return mPositioningSourceReplica ? mPositioningSourceReplica->property( "averagedPositionCount" ).toInt() : 0;
}

bool Positioning::averagedPosition() const
{
  return ( mPositioningSourceReplica ? mPositioningSourceReplica->property( "averagedPosition" ) : mPropertiesToSync.value( "averagedPosition", false ) ).toBool();
}

void Positioning::setAveragedPosition( bool averaged )
{
  if ( mPositioningSourceReplica )
  {
    mPositioningSourceReplica->setProperty( "averagedPosition", averaged );
  }
  else
  {
    mPropertiesToSync["averagedPosition"] = averaged;
  }
}

bool Positioning::logging() const
{
  return ( mPositioningSourceReplica ? mPositioningSourceReplica->property( "logging" ) : mPropertiesToSync.value( "logging", false ) ).toBool();
}

void Positioning::setLogging( bool logging )
{
  if ( mPositioningSourceReplica )
  {
    mPositioningSourceReplica->setProperty( "logging", logging );
  }
  else
  {
    mPropertiesToSync["logging"] = logging;
  }
}

PositioningSource::ElevationCorrectionMode Positioning::elevationCorrectionMode() const
{
  return static_cast<PositioningSource::ElevationCorrectionMode>( ( mPositioningSourceReplica ? mPositioningSourceReplica->property( "elevationCorrectionMode" ) : mPropertiesToSync.value( "elevationCorrectionMode", static_cast<int>( PositioningSource::ElevationCorrectionMode::None ) ) ).toInt() );
}

void Positioning::setElevationCorrectionMode( PositioningSource::ElevationCorrectionMode elevationCorrectionMode )
{
  if ( mPositioningSourceReplica )
  {
    mPositioningSourceReplica->setProperty( "elevationCorrectionMode", static_cast<int>( elevationCorrectionMode ) );
  }
  else
  {
    mPropertiesToSync["elevationCorrectionMode"] = static_cast<int>( elevationCorrectionMode );
  }
}

double Positioning::antennaHeight() const
{
  return ( mPositioningSourceReplica ? mPositioningSourceReplica->property( "antennaHeight" ) : mPropertiesToSync.value( "antennaHeight", 0.0 ) ).toDouble();
}

void Positioning::setAntennaHeight( double antennaHeight )
{
  if ( mPositioningSourceReplica )
  {
    mPositioningSourceReplica->setProperty( "antennaHeight", antennaHeight );
  }
  else
  {
    mPropertiesToSync["antennaHeight"] = antennaHeight;
  }
}

GnssPositionInformation Positioning::positionInformation() const
{
  return mPositionInformation;
}

double Positioning::orientation() const
{
  return mPositioningSourceReplica ? adjustOrientation( mPositioningSourceReplica->property( "orientation" ).toDouble() ) : std::numeric_limits<double>::quiet_NaN();
}

double Positioning::adjustOrientation( double orientation ) const
{
  // Take into account the orientation of the device
  QScreen *screen = QgsApplication::instance()->primaryScreen();
  switch ( screen->orientation() )
  {
    case Qt::LandscapeOrientation:
      orientation += 90;
      break;
    case Qt::InvertedLandscapeOrientation:
      orientation += 270;
      break;
    case Qt::PortraitOrientation:
    default:
      break;
  }

  return std::fmod( orientation, 360 );
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

  if ( mPositionInformation.orientationValid() )
  {
    mPositionInformation.setOrientation( adjustOrientation( mPositionInformation.orientation() ) );
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
