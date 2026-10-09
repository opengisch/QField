

# File qfpositioning.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfpositioning.cpp**](qfpositioning_8cpp.md)

[Go to the documentation of this file](qfpositioning_8cpp.md)


```C++
/***************************************************************************
  qfpositioning.cpp - QfPositioning

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

#include "qffilereceiver.h"
#include "qfplatformutilities.h"
#include "qfpositioning.h"
#include "qfpositioningutils.h"
#include "qftcpreceiver.h"
#include "qfudpreceiver.h"
#ifdef WITH_SERIALPORT
#include "qfserialportreceiver.h"
#endif

#include <QFile>
#include <QGuiApplication>
#include <QPermissions>
#include <QRemoteObjectPendingCall>
#include <QScreen>
#include <qgsapplication.h>
#include <qgsunittypes.h>

QfPositioning::QfPositioning( QObject *parent )
  : QObject( parent )
{
  if ( QFile::exists( QfPositioningSource::backgroundFilePath ) )
  {
    QFile::remove( QfPositioningSource::backgroundFilePath );
    mProperties["backgroundMode"] = false;
  }

  connect( QgsApplication::instance(), &QGuiApplication::applicationStateChanged, this, &QfPositioning::onApplicationStateChanged );
}

void QfPositioning::setupSource()
{
  mPositioningSourceReplica.reset();
  mNode.reset();
  if ( mPositioningSource )
  {
    mHost.disableRemoting( mPositioningSource );
    // Don't rely on deleteLater(), insure any device is disconnected prior to switching source
    mPositioningSource->setActive( false );
    mPositioningSource->deleteLater();
    mPositioningSource = nullptr;
  }

  QString nodeUrl;
  if ( mServiceMode && ( QfPlatformUtilities::instance()->capabilities() & QfPlatformUtilities::PositioningService ) )
  {
    nodeUrl = QfPlatformUtilities::instance()->startPositioningService();
  }
  else
  {
    QfPlatformUtilities::instance()->stopPositioningService();

    if ( mHost.hostUrl().isEmpty() )
    {
      mHost.setHostUrl( QUrl( QStringLiteral( "local:replica" ) ) );
    }

    mPositioningSource = new QfPositioningSource( this );
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
  connect( mPositioningSourceReplica.data(), SIGNAL( deviceChanged() ), this, SIGNAL( deviceChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( elevationCorrectionModeChanged() ), this, SLOT( onElevationCorrectionModeChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( antennaHeightChanged() ), this, SLOT( onAntennaHeightChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( loggingChanged() ), this, SLOT( onLoggingChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( loggingPathChanged() ), this, SLOT( onLoggingPathChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( positionInformationChanged() ), this, SLOT( onPositionInformationChanged() ) );

  connect( mPositioningSourceReplica.data(), SIGNAL( deviceLastErrorChanged() ), this, SIGNAL( deviceLastErrorChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( deviceSocketStateChanged() ), this, SIGNAL( deviceSocketStateChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( deviceSocketStateStringChanged() ), this, SIGNAL( deviceSocketStateStringChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( orientationChanged() ), this, SIGNAL( orientationChanged() ) );

  connect( mPositioningSourceReplica.data(), SIGNAL( enableNtripChanged() ), this, SIGNAL( enableNtripChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( ntripSettingsChanged() ), this, SIGNAL( ntripSettingsChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( ntripStateChanged() ), this, SIGNAL( ntripStateChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( ntripBytesSentChanged() ), this, SIGNAL( ntripBytesSentChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( ntripBytesReceivedChanged() ), this, SIGNAL( ntripBytesReceivedChanged() ) );
  connect( mPositioningSourceReplica.data(), SIGNAL( ntripLastBytesReceivedUtcDateTimeChanged() ), this, SIGNAL( ntripLastBytesReceivedUtcDateTimeChanged() ) );

  connect( this, SIGNAL( triggerConnectDevice() ), mPositioningSourceReplica.data(), SLOT( triggerConnectDevice() ) );
  connect( this, SIGNAL( triggerDisconnectDevice() ), mPositioningSourceReplica.data(), SLOT( triggerDisconnectDevice() ) );

  // Synchronize properties
  const QList<QString> properties = mProperties.keys();
  for ( const QString &property : properties )
  {
    if ( property != QStringLiteral( "active" ) )
    {
      mPositioningSourceReplica->setProperty( property.toLatin1(), mProperties[property] );
    }
  }

  // Give the OS 2 seconds to fully release the Bluetooth adapter from the dying local/remote source
  if ( mProperties.contains( "active" ) )
  {
    QTimer::singleShot( 2000, this, [this]() {
      if ( mPositioningSourceReplica && mProperties.contains( "active" ) )
      {
        const bool actualActiveValue = mProperties["active"].toBool();
        mProperties.remove( "active" );
        mPositioningSourceReplica->setProperty( "active", actualActiveValue );
      }
    } );
  }
}

bool QfPositioning::isSourceAvailable() const
{
  return mPositioningSourceReplica && mPositioningSourceReplica->isInitialized();
}

void QfPositioning::onApplicationStateChanged( Qt::ApplicationState state )
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

void QfPositioning::onActiveChanged()
{
  if ( mProperties["active"] == mPositioningSourceReplica->property( "active" ) )
  {
    return;
  }

  mProperties["active"] = mPositioningSourceReplica->property( "active" );
  emit activeChanged();
}

bool QfPositioning::active() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "active" ).toBool() : false;
}

void QfPositioning::setActive( bool active )
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
      !devId.startsWith( QfTcpReceiver::identifier + ":" )
      && !devId.startsWith( QfUdpReceiver::identifier + ":" )
      && !devId.startsWith( QfFileReceiver::identifier + ":" )
#ifdef WITH_SERIALPORT
      && !devId.startsWith( QfSerialPortReceiver::identifier + ":" )
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

void QfPositioning::onValidChanged()
{
  if ( mProperties["valid"] == mPositioningSourceReplica->property( "valid" ) )
  {
    return;
  }

  mValid = mPositioningSourceReplica->property( "valid" ).toBool();
  emit validChanged();
}

bool QfPositioning::valid() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "valid" ).toBool() : mValid;
}

void QfPositioning::setValid( bool valid )
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

void QfPositioning::onDeviceIdChanged()
{
  if ( mProperties["deviceId"] == mPositioningSourceReplica->property( "deviceId" ) )
  {
    return;
  }

  mProperties["deviceId"] = mPositioningSourceReplica->property( "deviceId" );
  emit deviceIdChanged();
}

QString QfPositioning::deviceId() const
{
  return ( isSourceAvailable() ? mPositioningSourceReplica->property( "deviceId" ) : mProperties.value( "deviceId" ) ).toString();
}

void QfPositioning::setDeviceId( const QString &id )
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

QString QfPositioning::deviceLastError() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "deviceLastError" ).toString() : QString();
}

QAbstractSocket::SocketState QfPositioning::deviceSocketState() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "deviceSocketState" ).value<QAbstractSocket::SocketState>() : QAbstractSocket::UnconnectedState;
}

QString QfPositioning::deviceSocketStateString() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "deviceSocketStateString" ).toString() : QString();
}

double QfPositioning::deviceBatteryLevel() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "deviceBatteryLevel" ).toDouble() : std::numeric_limits<double>::quiet_NaN();
}

QfGnssPositionDetails QfPositioning::deviceDetails() const
{
  QfGnssPositionDetails list;
  if ( isSourceAvailable() )
  {
    list = mPositioningSourceReplica->property( "deviceDetails" ).value<QfGnssPositionDetails>();
  }
  return list;
}

QfAbstractGnssReceiver::Capabilities QfPositioning::deviceCapabilities() const
{
  return isSourceAvailable() ? static_cast<QfAbstractGnssReceiver::Capabilities>( mPositioningSourceReplica->property( "deviceCapabilities" ).toInt() ) : QfAbstractGnssReceiver::NoCapabilities;
}

int QfPositioning::averagedPositionCount() const
{
  return static_cast<int>( mCollectedPositionInformations.size() );
}

bool QfPositioning::averagedPosition() const
{
  return mAveragedPosition;
}

void QfPositioning::setAveragedPosition( bool averaged )
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

void QfPositioning::onLoggingChanged()
{
  if ( mProperties["logging"] == mPositioningSourceReplica->property( "logging" ) )
  {
    return;
  }

  mProperties["logging"] = mPositioningSourceReplica->property( "logging" );
  emit loggingChanged();
}

bool QfPositioning::logging() const
{
  return ( isSourceAvailable() ? mPositioningSourceReplica->property( "logging" ) : mProperties.value( "logging", false ) ).toBool();
}

void QfPositioning::setLogging( bool logging )
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

void QfPositioning::onLoggingPathChanged()
{
  if ( mProperties["loggingPath"] == mPositioningSourceReplica->property( "loggingPath" ) )
  {
    return;
  }

  mProperties["loggingPath"] = mPositioningSourceReplica->property( "loggingPath" );
  emit loggingPathChanged();
}

QString QfPositioning::loggingPath() const
{
  return ( isSourceAvailable() ? mPositioningSourceReplica->property( "loggingPath" ) : mProperties.value( "loggingPath" ) ).toString();
}

void QfPositioning::setLoggingPath( const QString &path )
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

bool QfPositioning::serviceMode() const
{
  return mServiceMode;
}

void QfPositioning::setServiceMode( bool enabled )
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

bool QfPositioning::backgroundMode() const
{
  return mBackgroundMode;
}

void QfPositioning::setBackgroundMode( bool enabled )
{
  if ( mBackgroundMode == enabled )
    return;

  mBackgroundMode = enabled;

  QFile backgroundFile( QfPositioningSource::backgroundFilePath );
  if ( mBackgroundMode )
  {
    if ( backgroundFile.open( QFile::WriteOnly ) )
    {
      backgroundFile.close();
    }
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

bool QfPositioning::enableNtrip() const
{
  return ( isSourceAvailable() ? mPositioningSourceReplica->property( "enableNtrip" ) : mProperties.value( "enableNtrip", false ) ).toBool();
}

void QfPositioning::setEnableNtrip( bool enableNtrip )
{
  if ( isSourceAvailable() )
  {
    mPositioningSourceReplica->setProperty( "enableNtrip", enableNtrip );
  }
  else
  {
    mProperties["enableNtrip"] = enableNtrip;
    emit enableNtripChanged();
  }
}

QfNtripSettings QfPositioning::ntripSettings() const
{
  if ( isSourceAvailable() )
  {
    return mPositioningSourceReplica->property( "ntripSettings" ).value<QfNtripSettings>();
  }
  else if ( mProperties.contains( "ntripSettings" ) )
  {
    return mProperties.value( "ntripSettings" ).value<QfNtripSettings>();
  }

  return QfNtripSettings();
}

void QfPositioning::setNtripSettings( const QfNtripSettings &ntripSettings )
{
  if ( isSourceAvailable() )
  {
    mPositioningSourceReplica->setProperty( "ntripSettings", QVariant::fromValue<QfNtripSettings>( ntripSettings ) );
  }
  else
  {
    mProperties["ntripSettings"] = QVariant::fromValue<QfNtripSettings>( ntripSettings );
    emit ntripSettingsChanged();
  }
}

QfPositioningSource::NtripState QfPositioning::ntripState() const
{
  return static_cast<QfPositioningSource::NtripState>( ( isSourceAvailable() ? mPositioningSourceReplica->property( "ntripState" ).toInt() : static_cast<int>( QfPositioningSource::NtripState::Disconnected ) ) );
}

qint64 QfPositioning::ntripBytesSent() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "ntripBytesSent" ).toLongLong() : 0;
}

qint64 QfPositioning::ntripBytesReceived() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "ntripBytesReceived" ).toLongLong() : 0;
}

QDateTime QfPositioning::ntripLastBytesReceivedUtcDateTime() const
{
  return isSourceAvailable() ? mPositioningSourceReplica->property( "ntripLastBytesReceivedUtcDateTime" ).toDateTime() : QDateTime();
}

QList<QfGnssPositionInformation> QfPositioning::getBackgroundPositionInformation() const
{
  QList<QfGnssPositionInformation> positionInformationList;

  if ( isSourceAvailable() )
  {
    QRemoteObjectPendingCall call;
    QMetaObject::invokeMethod( mPositioningSourceReplica.data(), "getBackgroundPositionInformation", Qt::DirectConnection, Q_RETURN_ARG( QRemoteObjectPendingCall, call ) );
    call.waitForFinished();
    positionInformationList = call.returnValue().value<QList<QfGnssPositionInformation>>();
  }

  return positionInformationList;
}

void QfPositioning::onElevationCorrectionModeChanged()
{
  if ( mProperties["elevationCorrectionMode"] == mPositioningSourceReplica->property( "elevationCorrectionMode" ) )
  {
    return;
  }

  mProperties["elevationCorrectionMode"] = mPositioningSourceReplica->property( "elevationCorrectionMode" );
  emit elevationCorrectionModeChanged();
}

QfPositioningSource::ElevationCorrectionMode QfPositioning::elevationCorrectionMode() const
{
  return static_cast<QfPositioningSource::ElevationCorrectionMode>( ( isSourceAvailable() ? mPositioningSourceReplica->property( "elevationCorrectionMode" ) : mProperties.value( "elevationCorrectionMode", static_cast<int>( QfPositioningSource::ElevationCorrectionMode::None ) ) ).toInt() );
}

void QfPositioning::setElevationCorrectionMode( QfPositioningSource::ElevationCorrectionMode elevationCorrectionMode )
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

void QfPositioning::onAntennaHeightChanged()
{
  if ( mProperties["antennaHeight"] == mPositioningSourceReplica->property( "antennaHeight" ) )
  {
    return;
  }

  mProperties["antennaHeight"] = mPositioningSourceReplica->property( "antennaHeight" );
  emit antennaHeightChanged();
}

double QfPositioning::antennaHeight() const
{
  return ( isSourceAvailable() ? mPositioningSourceReplica->property( "antennaHeight" ) : mProperties.value( "antennaHeight", 0.0 ) ).toDouble();
}

void QfPositioning::setAntennaHeight( double antennaHeight )
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

QfGnssPositionInformation QfPositioning::positionInformation() const
{
  return mPositionInformation;
}

double QfPositioning::orientation() const
{
  return isSourceAvailable() ? adjustOrientation( mPositioningSourceReplica->property( "orientation" ).toDouble() ) : std::numeric_limits<double>::quiet_NaN();
}

double QfPositioning::adjustOrientation( double orientation ) const
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

void QfPositioning::setCoordinateTransformer( QgsQuickCoordinateTransformer *coordinateTransformer )
{
  if ( mCoordinateTransformer == coordinateTransformer )
    return;

  if ( mCoordinateTransformer )
  {
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::destinationCrsChanged, this, &QfPositioning::processProjectedPosition );
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::transformContextChanged, this, &QfPositioning::processProjectedPosition );
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::deltaZChanged, this, &QfPositioning::processProjectedPosition );
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::skipAltitudeTransformationChanged, this, &QfPositioning::processProjectedPosition );
    disconnect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::verticalGridChanged, this, &QfPositioning::processProjectedPosition );
  }

  mCoordinateTransformer = coordinateTransformer;

  if ( mCoordinateTransformer )
  {
    connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::destinationCrsChanged, this, &QfPositioning::processProjectedPosition );
    connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::transformContextChanged, this, &QfPositioning::processProjectedPosition );
    connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::deltaZChanged, this, &QfPositioning::processProjectedPosition );
    connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::skipAltitudeTransformationChanged, this, &QfPositioning::processProjectedPosition );
    connect( mCoordinateTransformer, &QgsQuickCoordinateTransformer::verticalGridChanged, this, &QfPositioning::processProjectedPosition );
  }

  emit coordinateTransformerChanged();
}

QgsPoint QfPositioning::sourcePosition() const
{
  return mSourcePosition;
}

QgsPoint QfPositioning::projectedPosition() const
{
  return mProjectedPosition;
}

double QfPositioning::projectedHorizontalAccuracy() const
{
  return mProjectedHorizontalAccuracy;
}

void QfPositioning::onPositionInformationChanged()
{
  mPositionInformation = mPositioningSourceReplica->property( "positionInformation" ).value<QfGnssPositionInformation>();

  QfGnssPositionInformation::AccuracyQuality quality = QfGnssPositionInformation::AccuracyQuality::AccuracyBad;
  const double hacc = mPositionInformation.hacc();
  const bool isExcellentThresholdDefined = !std::isnan( excellentAccuracyThreshold() );
  const bool isBadThresholdDefined = !std::isnan( badAccuracyThreshold() );

  if ( isExcellentThresholdDefined && isBadThresholdDefined )
  {
    if ( !std::isnan( hacc ) )
    {
      if ( hacc <= excellentAccuracyThreshold() )
      {
        quality = QfGnssPositionInformation::AccuracyExcellent;
      }
      else if ( hacc <= badAccuracyThreshold() )
      {
        quality = QfGnssPositionInformation::AccuracyOk;
      }
      else
      {
        quality = QfGnssPositionInformation::AccuracyBad;
      }
    }
    else
    {
      quality = QfGnssPositionInformation::AccuracyBad;
    }
  }
  else
  {
    quality = QfGnssPositionInformation::AccuracyUndetermined;
  }

  mPositionInformation.setAccuracyQuality( quality );


  if ( mAveragedPosition )
  {
    if ( !mAveragedPositionFilterAccuracy || mPositionInformation.accuracyQuality() != QfGnssPositionInformation::AccuracyBad )
    {
      mCollectedPositionInformations << mPositionInformation;
    }
    mPositionInformation = QfPositioningUtils::averagedPositionInformation( mCollectedPositionInformations );
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

void QfPositioning::processProjectedPosition()
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


void QfPositioning::setBadAccuracyThreshold( double threshold )
{
  if ( mBadAccuracyThreshold == threshold )
    return;

  mBadAccuracyThreshold = threshold;
  emit badAccuracyThresholdChanged();
}

void QfPositioning::setExcellentAccuracyThreshold( double threshold )
{
  if ( mExcellentAccuracyThreshold == threshold )
    return;

  mExcellentAccuracyThreshold = threshold;
  emit excellentAccuracyThresholdChanged();
}

bool QfPositioning::averagedPositionFilterAccuracy() const
{
  return mAveragedPositionFilterAccuracy;
}

void QfPositioning::setAveragedPositionFilterAccuracy( bool enabled )
{
  if ( mAveragedPositionFilterAccuracy == enabled )
    return;

  mAveragedPositionFilterAccuracy = enabled;
  emit averagedPositionFilterAccuracyChanged();
}
```


