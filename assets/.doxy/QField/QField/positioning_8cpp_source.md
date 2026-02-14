

# File positioning.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**positioning.cpp**](positioning_8cpp.md)

[Go to the documentation of this file](positioning_8cpp.md)


```C++
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
#include "tcpreceiver.h"
#include "udpreceiver.h"
#ifdef WITH_SERIALPORT
#include "serialportreceiver.h"
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
    mProperties["backgroundMode"] = false;
  }

  connect( QgsApplication::instance(), &QGuiApplication::applicationStateChanged, this, &Positioning::onApplicationStateChanged );
}

void Positioning::setupSource()
{
  mPositioningSourceReplica.reset();
  mNode.reset();
  if ( mPositioningSource )
  {
    mHost.disableRemoting( mPositioningSource );
    mPositioningSource->deleteLater();
    mPositioningSource = nullptr;
  }

  QString nodeUrl;
  if ( mServiceMode && ( PlatformUtilities::instance()->capabilities() & PlatformUtilities::PositioningService ) )
  {
    nodeUrl = PlatformUtilities::instance()->startPositioningService();
  }
  else
  {
    PlatformUtilities::instance()->stopPositioningService();

    if ( mHost.hostUrl().isEmpty() )
    {
      mHost.setHostUrl( QUrl( QStringLiteral( "local:replica" ) ) );
    }

    mPositioningSource = new PositioningSource( this );
    mHost.enableRemoting( mPositioningSource, "PositioningSource" );
    nodeUrl = QStringLiteral( "local:replica" );
  }

  mNode.reset( new QRemoteObjectNode( this ) );
  mNode->connectToNode( QUrl( nodeUrl ) );
  mPositioningSourceReplica.reset( mNode->acquireDynamic( "PositioningSource" ) );
  mPositioningSourceReplica->waitForSource();

  connect( mPositioningSourceReplica.data(), SIGNAL( activeChanged() ), this, SLOT( onActiveChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( validChanged() ), this, SLOT( onValidChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( deviceIdChanged() ), this, SLOT( onDeviceIdChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( elevationCorrectionModeChanged() ), this, SLOT( onElevationCorrectionModeChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( antennaHeightChanged() ), this, SLOT( onAntennaHeightChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( loggingChanged() ), this, SLOT( onLoggingChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( loggingPathChanged() ), this, SLOT( onLoggingPathChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( positionInformationChanged() ), this, SLOT( onPositionInformationChanged() ) );

  connect( mPositioningSourceReplica.data(), SIGNAL( deviceLastErrorChanged() ), this, SIGNAL( deviceLastErrorChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( deviceSocketStateChanged() ), this, SIGNAL( deviceSocketStateChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( deviceSocketStateStringChanged() ), this, SIGNAL( deviceSocketStateStringChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( orientationChanged() ), this, SIGNAL( orientationChanged() ) );

  connect( this, SIGNAL( triggerConnectDevice() ), mPositioningSourceReplica.data(), SLOT( triggerConnectDevice() ) );
  connect( this, SIGNAL( triggerDisconnectDevice() ), mPositioningSourceReplica.data(), SLOT( triggerDisconnectDevice() ) );

  // Synchronize properties
  const QList<QString> properties = mProperties.keys();
  for ( const QString &property : properties )
  {
    mPositioningSourceReplica->setProperty( property.toLatin1(), mProperties[property] );
  }
}

bool Positioning::isSourceAvailable() const
{
  return mPositioningSourceReplica && mPositioningSourceReplica->isInitialized();
}

void Positioning::onApplicationStateChanged( Qt::ApplicationState state )
{
#if defined( Q_OS_ANDROID ) || defined( Q_OS_IOS )
  // Google Play policy only allows for background access if it's explicitly stated and justified
  // Not stopping on Activity::onPause is detected as violation
  if ( mServiceMode )
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

void Positioning::onActiveChanged()
{
  if ( mProperties["active"] == mPositioningSourceReplica->property( "active" ) )
  {
    return;
  }

  mProperties["active"] = mPositioningSourceReplica->property( "active" );
  emit activeChanged();
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

void Positioning::onValidChanged()
{
  if ( mProperties["valid"] == mPositioningSourceReplica->property( "valid" ) )
  {
    return;
  }

  mValid = mPositioningSourceReplica->property( "valid" ).toBool();
  emit validChanged();
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

void Positioning::onDeviceIdChanged()
{
  if ( mProperties["deviceId"] == mPositioningSourceReplica->property( "deviceId" ) )
  {
    return;
  }

  mProperties["deviceId"] = mPositioningSourceReplica->property( "deviceId" );
  emit deviceIdChanged();
}

QString Positioning::deviceId() const
{
  return ( isSourceAvailable() ? mPositioningSourceReplica->property( "deviceId" ) : mProperties.value( "deviceId" ) ).toString();
}

void Positioning::setDeviceId( const QString &id )
{
  if ( isSourceAvailable() )
  {
    mPositioningSourceReplica->setProperty( "deviceId", id );
  }
  else
  {
    mProperties["deviceId"] = id;
    emit deviceIdChanged();
  }
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

GnssPositionDetails Positioning::deviceDetails() const
{
  GnssPositionDetails list;
  if ( isSourceAvailable() )
  {
    list = mPositioningSourceReplica->property( "deviceDetails" ).value<GnssPositionDetails>();
  }
  return list;
}

AbstractGnssReceiver::Capabilities Positioning::deviceCapabilities() const
{
  const QString deviceId = ( isSourceAvailable() ? mPositioningSourceReplica->property( "deviceId" ) : mProperties.value( "deviceId" ) ).toString();
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
  return static_cast<int>( mCollectedPositionInformations.size() );
}

bool Positioning::averagedPosition() const
{
  return mAveragedPosition;
}

void Positioning::setAveragedPosition( bool averaged )
{
  if ( mAveragedPosition == averaged )
    return;

  mAveragedPosition = averaged;
  if ( mAveragedPosition )
  {
    mCollectedPositionInformations << mPositionInformation;
  }
  else
  {
    mCollectedPositionInformations.clear();
  }

  emit averagedPositionCountChanged();
  emit averagedPositionChanged();
}

void Positioning::onLoggingChanged()
{
  if ( mProperties["logging"] == mPositioningSourceReplica->property( "logging" ) )
  {
    return;
  }

  mProperties["logging"] = mPositioningSourceReplica->property( "logging" );
  emit loggingChanged();
}

bool Positioning::logging() const
{
  return ( isSourceAvailable() ? mPositioningSourceReplica->property( "logging" ) : mProperties.value( "logging", false ) ).toBool();
}

void Positioning::setLogging( bool logging )
{
  if ( isSourceAvailable() )
  {
    mPositioningSourceReplica->setProperty( "logging", logging );
  }
  else
  {
    mProperties["logging"] = logging;
    emit loggingChanged();
  }
}

void Positioning::onLoggingPathChanged()
{
  if ( mProperties["loggingPath"] == mPositioningSourceReplica->property( "loggingPath" ) )
  {
    return;
  }

  mProperties["loggingPath"] = mPositioningSourceReplica->property( "loggingPath" );
  emit loggingPathChanged();
}

QString Positioning::loggingPath() const
{
  return ( isSourceAvailable() ? mPositioningSourceReplica->property( "loggingPath" ) : mProperties.value( "loggingPath" ) ).toString();
}

void Positioning::setLoggingPath( const QString &path )
{
  if ( isSourceAvailable() )
  {
    mPositioningSourceReplica->setProperty( "loggingPath", path );
  }
  else
  {
    mProperties["loggingPath"] = path;
    emit loggingPathChanged();
  }
}

bool Positioning::serviceMode() const
{
  return mServiceMode;
}

void Positioning::setServiceMode( bool enabled )
{
  if ( mServiceMode == enabled )
    return;

  mServiceMode = enabled;

  if ( active() )
  {
    setupSource();
  }

  emit serviceModeChanged();
}

bool Positioning::backgroundMode() const
{
  return mBackgroundMode;
}

void Positioning::setBackgroundMode( bool enabled )
{
  if ( mBackgroundMode == enabled )
    return;

  mBackgroundMode = enabled;

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
    mPositioningSourceReplica->setProperty( "backgroundMode", mBackgroundMode );
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

void Positioning::onElevationCorrectionModeChanged()
{
  if ( mProperties["elevationCorrectionMode"] == mPositioningSourceReplica->property( "elevationCorrectionMode" ) )
  {
    return;
  }

  mProperties["elevationCorrectionMode"] = mPositioningSourceReplica->property( "elevationCorrectionMode" );
  emit elevationCorrectionModeChanged();
}

PositioningSource::ElevationCorrectionMode Positioning::elevationCorrectionMode() const
{
  return static_cast<PositioningSource::ElevationCorrectionMode>( ( isSourceAvailable() ? mPositioningSourceReplica->property( "elevationCorrectionMode" ) : mProperties.value( "elevationCorrectionMode", static_cast<int>( PositioningSource::ElevationCorrectionMode::None ) ) ).toInt() );
}

void Positioning::setElevationCorrectionMode( PositioningSource::ElevationCorrectionMode elevationCorrectionMode )
{
  if ( isSourceAvailable() )
  {
    mPositioningSourceReplica->setProperty( "elevationCorrectionMode", static_cast<int>( elevationCorrectionMode ) );
  }
  else
  {
    mProperties["elevationCorrectionMode"] = static_cast<int>( elevationCorrectionMode );
    emit elevationCorrectionModeChanged();
  }
}

void Positioning::onAntennaHeightChanged()
{
  if ( mProperties["antennaHeight"] == mPositioningSourceReplica->property( "antennaHeight" ) )
  {
    return;
  }

  mProperties["antennaHeight"] = mPositioningSourceReplica->property( "antennaHeight" );
  emit antennaHeightChanged();
}

double Positioning::antennaHeight() const
{
  return ( isSourceAvailable() ? mPositioningSourceReplica->property( "antennaHeight" ) : mProperties.value( "antennaHeight", 0.0 ) ).toDouble();
}

void Positioning::setAntennaHeight( double antennaHeight )
{
  if ( isSourceAvailable() )
  {
    mPositioningSourceReplica->setProperty( "antennaHeight", antennaHeight );
  }
  else
  {
    mProperties["antennaHeight"] = antennaHeight;
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
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::transformContextChanged, this, &Positioning::processProjectedPosition );
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::deltaZChanged, this, &Positioning::processProjectedPosition );
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::skipAltitudeTransformationChanged, this, &Positioning::processProjectedPosition );
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::verticalGridChanged, this, &Positioning::processProjectedPosition );
  }

  mCoordinateTransformer = coordinateTransformer;

  if ( mCoordinateTransformer )
  {
    connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::destinationCrsChanged, this, &Positioning::processProjectedPosition );
    connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::transformContextChanged, this, &Positioning::processProjectedPosition );
    connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::deltaZChanged, this, &Positioning::processProjectedPosition );
    connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::skipAltitudeTransformationChanged, this, &Positioning::processProjectedPosition );
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

void Positioning::onPositionInformationChanged()
{
  mPositionInformation = mPositioningSourceReplica->property( "positionInformation" ).value<GnssPositionInformation>();

  GnssPositionInformation::AccuracyQuality quality = GnssPositionInformation::AccuracyQuality::AccuracyBad;
  const double hacc = mPositionInformation.hacc();
  const bool isExcellentThresholdDefined = !std::isnan( excellentAccuracyThreshold() );
  const bool isBadThresholdDefined = !std::isnan( badAccuracyThreshold() );

  if ( isExcellentThresholdDefined && isBadThresholdDefined )
  {
    if ( !std::isnan( hacc ) )
    {
      if ( hacc <= excellentAccuracyThreshold() )
      {
        quality = GnssPositionInformation::AccuracyExcellent;
      }
      else if ( hacc <= badAccuracyThreshold() )
      {
        quality = GnssPositionInformation::AccuracyOk;
      }
      else
      {
        quality = GnssPositionInformation::AccuracyBad;
      }
    }
    else
    {
      quality = GnssPositionInformation::AccuracyBad;
    }
  }
  else
  {
    quality = GnssPositionInformation::AccuracyUndetermined;
  }

  mPositionInformation.setAccuracyQuality( quality );


  if ( mAveragedPosition )
  {
    if ( !mAveragedPositionFilterAccuracy || mPositionInformation.accuracyQuality() != GnssPositionInformation::AccuracyBad )
    {
      mCollectedPositionInformations << mPositionInformation;
    }
    mPositionInformation = PositioningUtils::averagedPositionInformation( mCollectedPositionInformations );
    emit averagedPositionCountChanged();
  }

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


void Positioning::setBadAccuracyThreshold( double threshold )
{
  if ( mBadAccuracyThreshold == threshold )
    return;

  mBadAccuracyThreshold = threshold;
  emit badAccuracyThresholdChanged();
}

void Positioning::setExcellentAccuracyThreshold( double threshold )
{
  if ( mExcellentAccuracyThreshold == threshold )
    return;

  mExcellentAccuracyThreshold = threshold;
  emit excellentAccuracyThresholdChanged();
}

bool Positioning::averagedPositionFilterAccuracy() const
{
  return mAveragedPositionFilterAccuracy;
}

void Positioning::setAveragedPositionFilterAccuracy( bool enabled )
{
  if ( mAveragedPositionFilterAccuracy == enabled )
    return;

  mAveragedPositionFilterAccuracy = enabled;
  emit averagedPositionFilterAccuracyChanged();
}
```


