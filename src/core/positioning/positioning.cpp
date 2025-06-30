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
#include "tcpreceiver.h"
#include "udpreceiver.h"
#ifdef WITH_SERIALPORT
#include "serialportreceiver.h"
#endif

#if defined( Q_OS_ANDROID )
#include "platformutilities.h"
#include "qfield_android.h"
#endif

#include <QFile>
#include <QGuiApplication>
#include <QPermissions>
#include <QRemoteObjectPendingCall>
#include <QScreen>
#include <qgsapplication.h>
#include <qgsunittypes.h>

Positioning::Positioning( QObject *parent )
  : QObject( parent )
{
  if ( QFile::exists( PositioningSource::backgroundFilePath ) )
  {
    QFile::remove( PositioningSource::backgroundFilePath );
    mPropertiesToSync["backgroundMode"] = false;
  }
}

void Positioning::setupSource()
{
  bool positioningService = false;

#if defined( Q_OS_ANDROID )
  PlatformUtilities::instance()->startPositioningService();
  mNode.connectToNode( QUrl( QStringLiteral( "localabstract:" APP_PACKAGE_NAME "replica" ) ) );
  positioningService = true;
#endif

  //cppcheck-suppress knownConditionTrueFalse
  if ( !positioningService )
  {
    // Non-service path, we are both the host and the node
    mPositioningSource = new PositioningSource( this );
    mHost.setHostUrl( QUrl( QStringLiteral( "local:replica" ) ) );
    mHost.enableRemoting( mPositioningSource, "PositioningSource" );
    mNode.connectToNode( QUrl( QStringLiteral( "local:replica" ) ) );
  }

  mPositioningSourceReplica.reset( mNode.acquireDynamic( "PositioningSource" ) );
  mPositioningSourceReplica->waitForSource();
  const QList<QString> properties = mPropertiesToSync.keys();
  for ( const QString &property : properties )
  {
    mPositioningSourceReplica->setProperty( property.toLatin1(), mPropertiesToSync[property] );
  }

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

bool Positioning::isSourceAvailable() const
{
  return mPositioningSourceReplica && mPositioningSourceReplica->isInitialized();
}

void Positioning::onApplicationStateChanged( Qt::ApplicationState state )
{
#ifdef Q_OS_ANDROID
  // Google Play policy only allows for background access if it's explicitly stated and justified
  // Not stopping on Activity::onPause is detected as violation
  if ( !mPositioningSource )
  {
    // Service path
    setBackgroundMode( state != Qt::ApplicationState::ApplicationActive );
  }
  else
  {
    // Non-service path
    const bool isActive = active();
    if ( isActive )
    {
      switch ( state )
      {
        case Qt::ApplicationState::ApplicationActive:
          if ( isActive )
          {
            emit triggerConnectDevice();
          }
          break;

        default:
          if ( isActive )
          {
            emit triggerDisconnectDevice();
          }
      }
    }
  }
#else
  Q_UNUSED( state )
#endif
}

bool Positioning::active() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "active" ).toBool() : false;
}

void Positioning::setActive( bool active )
{
  const QString devId = deviceId();
  if ( devId.isEmpty() )
  {
    // Handle internal receiver permission
    if ( !mInternalPermissionChecked )
    {
      QLocationPermission locationPermission;
      locationPermission.setAccuracy( QLocationPermission::Precise );
      Qt::PermissionStatus permissionStatus = qApp->checkPermission( locationPermission );

      if ( permissionStatus == Qt::PermissionStatus::Undetermined )
      {
        qApp->requestPermission( locationPermission, this, [this]( const QPermission &permission ) {
          if ( permission.status() == Qt::PermissionStatus::Granted )
          {
            mInternalPermissionChecked = true;
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
      mInternalPermissionChecked = true;
    }
  }
  else
  {
    // Handle external receiver permission
    if (
      !devId.startsWith( TcpReceiver::identifier + ":" )
      && !devId.startsWith( UdpReceiver::identifier + ":" )
#ifdef WITH_SERIALPORT
      && !devId.startsWith( SerialPortReceiver::identifier + ":" )
#endif
    )
    {
      if ( !mBluetoothPermissionChecked )
      {
        QBluetoothPermission bluetoothPermission;
        bluetoothPermission.setCommunicationModes( QBluetoothPermission::Access );
        Qt::PermissionStatus permissionStatus = qApp->checkPermission( bluetoothPermission );
        if ( permissionStatus == Qt::PermissionStatus::Undetermined )
        {
          qApp->requestPermission( bluetoothPermission, this, [this]( const QPermission &permission ) {
            if ( permission.status() == Qt::PermissionStatus::Granted )
            {
              mBluetoothPermissionChecked = true;
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
        mBluetoothPermissionChecked = true;
      }
    }
  }

  if ( !mPositioningSourceReplica )
  {
    setupSource();
  }

  if ( mPositioningSourceReplica->property( "active" ).toBool() != active )
  {
    mPositioningSourceReplica->setProperty( "active", active );
  }
  else
  {
    emit activeChanged();
  }
}

bool Positioning::valid() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "valid" ).toBool() : mValid;
}

void Positioning::setValid( bool valid )
{
  if ( isSourceAvailable() )
  {
    mPositioningSourceReplica->setProperty( "valid", valid );
  }
  else
  {
    mValid = valid;
    emit validChanged();
  }
}

QString Positioning::deviceId() const
{
  return ( isSourceAvailable() ? mPositioningSourceReplica->property( "deviceId" ) : mPropertiesToSync.value( "deviceId" ) ).toString();
}

void Positioning::setDeviceId( const QString &id )
{
  if ( isSourceAvailable() )
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
  if ( isSourceAvailable() )
  {
    list = mPositioningSourceReplica->property( "deviceDetails" ).value<GnssPositionDetails>();
  }
  return list;
}

QString Positioning::deviceLastError() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "deviceLastError" ).toString() : QString();
}

QAbstractSocket::SocketState Positioning::deviceSocketState() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "deviceSocketState" ).value<QAbstractSocket::SocketState>() : QAbstractSocket::UnconnectedState;
}

QString Positioning::deviceSocketStateString() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "deviceSocketStateString" ).toString() : QString();
}

AbstractGnssReceiver::Capabilities Positioning::deviceCapabilities() const
{
  const QString deviceId = ( isSourceAvailable() ? mPositioningSourceReplica->property( "deviceId" ) : mPropertiesToSync.value( "deviceId" ) ).toString();
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
  return isSourceAvailable() ? mPositioningSourceReplica->property( "averagedPositionCount" ).toInt() : 0;
}

bool Positioning::averagedPosition() const
{
  return ( isSourceAvailable() ? mPositioningSourceReplica->property( "averagedPosition" ) : mPropertiesToSync.value( "averagedPosition", false ) ).toBool();
}

void Positioning::setAveragedPosition( bool averaged )
{
  if ( isSourceAvailable() )
  {
    mPositioningSourceReplica->setProperty( "averagedPosition", averaged );
  }
  else
  {
    mPropertiesToSync["averagedPosition"] = averaged;
    emit averagedPositionChanged();
  }
}

bool Positioning::logging() const
{
  return ( isSourceAvailable() ? mPositioningSourceReplica->property( "logging" ) : mPropertiesToSync.value( "logging", false ) ).toBool();
}

void Positioning::setLogging( bool logging )
{
  if ( isSourceAvailable() )
  {
    mPositioningSourceReplica->setProperty( "logging", logging );
  }
  else
  {
    mPropertiesToSync["logging"] = logging;
    emit loggingChanged();
  }
}

bool Positioning::backgroundMode() const
{
  return mBackgroundMode;
}

void Positioning::setBackgroundMode( bool backgroundMode )
{
  if ( mBackgroundMode == backgroundMode )
    return;

  mBackgroundMode = backgroundMode;

  QFile backgroundFile( PositioningSource::backgroundFilePath );
  if ( mBackgroundMode )
  {
    backgroundFile.open( QFile::WriteOnly );
    backgroundFile.close();
  }
  else
  {
    if ( backgroundFile.exists() )
    {
      backgroundFile.remove();
    }
  }

  if ( isSourceAvailable() )
  {
    // Note that on Android, the property will not be set if the application is suspended _until_ it has become active again
    mPositioningSourceReplica->setProperty( "backgroundMode", backgroundMode );
  }

  emit backgroundModeChanged();
}

QList<GnssPositionInformation> Positioning::getBackgroundPositionInformation() const
{
  QList<GnssPositionInformation> positionInformationList;

  if ( isSourceAvailable() )
  {
    QRemoteObjectPendingCall call;
    QMetaObject::invokeMethod( mPositioningSourceReplica.data(), "getBackgroundPositionInformation", Qt::DirectConnection, Q_RETURN_ARG( QRemoteObjectPendingCall, call ) );
    call.waitForFinished();
    positionInformationList = call.returnValue().value<QList<GnssPositionInformation>>();
  }

  return positionInformationList;
}

PositioningSource::ElevationCorrectionMode Positioning::elevationCorrectionMode() const
{
  return static_cast<PositioningSource::ElevationCorrectionMode>( ( isSourceAvailable() ? mPositioningSourceReplica->property( "elevationCorrectionMode" ) : mPropertiesToSync.value( "elevationCorrectionMode", static_cast<int>( PositioningSource::ElevationCorrectionMode::None ) ) ).toInt() );
}

void Positioning::setElevationCorrectionMode( PositioningSource::ElevationCorrectionMode elevationCorrectionMode )
{
  if ( isSourceAvailable() )
  {
    mPositioningSourceReplica->setProperty( "elevationCorrectionMode", static_cast<int>( elevationCorrectionMode ) );
  }
  else
  {
    mPropertiesToSync["elevationCorrectionMode"] = static_cast<int>( elevationCorrectionMode );
    emit elevationCorrectionModeChanged();
  }
}

double Positioning::antennaHeight() const
{
  return ( isSourceAvailable() ? mPositioningSourceReplica->property( "antennaHeight" ) : mPropertiesToSync.value( "antennaHeight", 0.0 ) ).toDouble();
}

void Positioning::setAntennaHeight( double antennaHeight )
{
  if ( isSourceAvailable() )
  {
    mPositioningSourceReplica->setProperty( "antennaHeight", antennaHeight );
  }
  else
  {
    mPropertiesToSync["antennaHeight"] = antennaHeight;
    emit antennaHeightChanged();
  }
}

GnssPositionInformation Positioning::positionInformation() const
{
  return mPositionInformation;
}

double Positioning::orientation() const
{
  return isSourceAvailable() ? adjustOrientation( mPositioningSourceReplica->property( "orientation" ).toDouble() ) : std::numeric_limits<double>::quiet_NaN();
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
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::destinationCrsChanged, this, &Positioning::processProjectedPosition );
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::sourceCrsChanged, this, &Positioning::processProjectedPosition );
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::deltaZChanged, this, &Positioning::processProjectedPosition );
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::verticalGridChanged, this, &Positioning::processProjectedPosition );
  }

  mCoordinateTransformer = coordinateTransformer;

  if ( mCoordinateTransformer )
  {
    connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::destinationCrsChanged, this, &Positioning::processProjectedPosition );
    connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::sourceCrsChanged, this, &Positioning::processProjectedPosition );
    connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::deltaZChanged, this, &Positioning::processProjectedPosition );
    connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::verticalGridChanged, this, &Positioning::processProjectedPosition );
  }

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
    mProjectedPosition.clear();
  }

  if ( mPositionInformation.orientationValid() )
  {
    mPositionInformation.setOrientation( adjustOrientation( mPositionInformation.orientation() ) );
  }

  if ( mCoordinateTransformer && !mSourcePosition.isEmpty() )
  {
    // positionInformationChanged() will be emitted in this function
    processProjectedPosition();
  }
  else
  {
    emit positionInformationChanged();
  }
}

void Positioning::processProjectedPosition()
{
  if ( !mSourcePosition.isEmpty() )
  {
    mProjectedPosition = mCoordinateTransformer->transformPosition( mSourcePosition );
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
  }

  emit positionInformationChanged();
}
