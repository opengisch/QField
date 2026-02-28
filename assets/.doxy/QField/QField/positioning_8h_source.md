

# File positioning.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**positioning.h**](positioning_8h.md)

[Go to the documentation of this file](positioning_8h.md)


```C++
/***************************************************************************
  positioning.h - Positioning

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

#ifndef POSITIONING_H
#define POSITIONING_H

#include "gnsspositioninformation.h"
#include "positioningsource.h"
#include "qgsquickcoordinatetransformer.h"

#include <QObject>
#include <QRemoteObjectDynamicReplica>
#include <QRemoteObjectHost>
#include <QRemoteObjectNode>
#include <qgscoordinatereferencesystem.h>
#include <qgscoordinatetransformcontext.h>
#include <qgspoint.h>

class Positioning : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool active READ active WRITE setActive NOTIFY activeChanged )
    Q_PROPERTY( bool valid READ valid NOTIFY validChanged )

    Q_PROPERTY( QString deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged )
    Q_PROPERTY( AbstractGnssReceiver::Capabilities deviceCapabilities READ deviceCapabilities NOTIFY deviceIdChanged )
    Q_PROPERTY( QString deviceLastError READ deviceLastError NOTIFY deviceLastErrorChanged )
    Q_PROPERTY( QAbstractSocket::SocketState deviceSocketState READ deviceSocketState NOTIFY deviceSocketStateChanged )
    Q_PROPERTY( QString deviceSocketStateString READ deviceSocketStateString NOTIFY deviceSocketStateStringChanged )

    Q_PROPERTY( QgsQuickCoordinateTransformer *coordinateTransformer READ coordinateTransformer WRITE setCoordinateTransformer NOTIFY coordinateTransformerChanged )

    Q_PROPERTY( GnssPositionInformation positionInformation READ positionInformation NOTIFY positionInformationChanged )

    Q_PROPERTY( QgsPoint sourcePosition READ sourcePosition NOTIFY positionInformationChanged )
    Q_PROPERTY( QgsPoint projectedPosition READ projectedPosition NOTIFY positionInformationChanged )
    Q_PROPERTY( double projectedHorizontalAccuracy READ projectedHorizontalAccuracy NOTIFY positionInformationChanged )

    Q_PROPERTY( bool averagedPositionFilterAccuracy READ averagedPositionFilterAccuracy WRITE setAveragedPositionFilterAccuracy NOTIFY averagedPositionFilterAccuracyChanged )
    Q_PROPERTY( bool averagedPosition READ averagedPosition WRITE setAveragedPosition NOTIFY averagedPositionChanged )
    Q_PROPERTY( int averagedPositionCount READ averagedPositionCount NOTIFY averagedPositionCountChanged )

    Q_PROPERTY( PositioningSource::ElevationCorrectionMode elevationCorrectionMode READ elevationCorrectionMode WRITE setElevationCorrectionMode NOTIFY elevationCorrectionModeChanged )
    Q_PROPERTY( double antennaHeight READ antennaHeight WRITE setAntennaHeight NOTIFY antennaHeightChanged )

    Q_PROPERTY( double orientation READ orientation NOTIFY orientationChanged );

    Q_PROPERTY( bool logging READ logging WRITE setLogging NOTIFY loggingChanged )
    Q_PROPERTY( QString loggingPath READ loggingPath WRITE setLoggingPath NOTIFY loggingPathChanged )

    Q_PROPERTY( bool serviceMode READ serviceMode WRITE setServiceMode NOTIFY serviceModeChanged )
    Q_PROPERTY( bool backgroundMode READ backgroundMode WRITE setBackgroundMode NOTIFY backgroundModeChanged )

    Q_PROPERTY( double badAccuracyThreshold READ badAccuracyThreshold WRITE setBadAccuracyThreshold NOTIFY badAccuracyThresholdChanged )
    Q_PROPERTY( double excellentAccuracyThreshold READ excellentAccuracyThreshold WRITE setExcellentAccuracyThreshold NOTIFY excellentAccuracyThresholdChanged )

  public:
    explicit Positioning( QObject *parent = nullptr );
    virtual ~Positioning() = default;

    bool active() const;

    void setActive( bool active );

    bool valid() const;

    void setValid( bool valid );

    QString deviceId() const;

    GnssPositionDetails deviceDetails() const;

    QString deviceLastError() const;

    QAbstractSocket::SocketState deviceSocketState() const;

    QString deviceSocketStateString() const;

    void setDeviceId( const QString &id );

    AbstractGnssReceiver::Capabilities deviceCapabilities() const;

    QgsQuickCoordinateTransformer *coordinateTransformer() const { return mCoordinateTransformer; }

    void setCoordinateTransformer( QgsQuickCoordinateTransformer *coordinateTransformer );

    GnssPositionInformation positionInformation() const;

    QgsPoint sourcePosition() const;

    QgsPoint projectedPosition() const;

    double projectedHorizontalAccuracy() const;

    bool averagedPositionFilterAccuracy() const;

    void setAveragedPositionFilterAccuracy( bool enabled );

    bool averagedPosition() const;

    void setAveragedPosition( bool averaged );

    int averagedPositionCount() const;

    PositioningSource::ElevationCorrectionMode elevationCorrectionMode() const;

    void setElevationCorrectionMode( PositioningSource::ElevationCorrectionMode elevationCorrectionMode );

    double antennaHeight() const;

    void setAntennaHeight( double antennaHeight );

    double orientation() const;

    bool logging() const;

    void setLogging( bool logging );

    QString loggingPath() const;

    void setLoggingPath( const QString &path );

    bool serviceMode() const;

    void setServiceMode( bool enabled );

    bool backgroundMode() const;

    void setBackgroundMode( bool enabled );

    Q_INVOKABLE QList<GnssPositionInformation> getBackgroundPositionInformation() const;

    double badAccuracyThreshold() const { return mBadAccuracyThreshold; }

    void setBadAccuracyThreshold( double threshold );

    double excellentAccuracyThreshold() const { return mExcellentAccuracyThreshold; }

    void setExcellentAccuracyThreshold( double threshold );

  signals:
    // Signals from positioning source properties cached locally and forwarded onwards
    void activeChanged();
    void validChanged();
    void deviceIdChanged();
    void elevationCorrectionModeChanged();
    void antennaHeightChanged();
    void loggingChanged();
    void loggingPathChanged();
    void positionInformationChanged();

    // Signals from positioning source properties forwarded onwards but not cached
    void deviceLastErrorChanged();
    void deviceSocketStateChanged();
    void deviceSocketStateStringChanged();
    void orientationChanged();

    // Signals forwarded to positioning source
    void triggerConnectDevice();
    void triggerDisconnectDevice();

    // Positioning signal
    void coordinateTransformerChanged();
    void projectedPositionChanged();
    void averagedPositionChanged();
    void averagedPositionCountChanged();
    void averagedPositionFilterAccuracyChanged();
    void badAccuracyThresholdChanged();
    void excellentAccuracyThresholdChanged();
    void serviceModeChanged();
    void backgroundModeChanged();

  private slots:
    void onActiveChanged();
    void onValidChanged();
    void onDeviceIdChanged();
    void onElevationCorrectionModeChanged();
    void onAntennaHeightChanged();
    void onLoggingChanged();
    void onLoggingPathChanged();
    void onPositionInformationChanged();

    void onApplicationStateChanged( Qt::ApplicationState state );

  private:
    void setupSource();
    bool isSourceAvailable() const;

    void processProjectedPosition();
    double adjustOrientation( double orientation ) const;

    PositioningSource *mPositioningSource = nullptr;
    QRemoteObjectHost mHost;

    std::unique_ptr<QRemoteObjectNode> mNode;
    QSharedPointer<QRemoteObjectDynamicReplica> mPositioningSourceReplica; //skip-keyword-check

    bool mValid = true;
    GnssPositionInformation mPositionInformation;
    QVariantMap mProperties;

    QgsQuickCoordinateTransformer *mCoordinateTransformer = nullptr;
    QgsPoint mSourcePosition;
    QgsPoint mProjectedPosition;
    double mProjectedHorizontalAccuracy = 0.0;
    virtual QList<QPair<QString, QVariant>> details() const { return {}; }

    bool mInternalPermissionChecked = false;
    bool mBluetoothPermissionChecked = false;

    bool mServiceMode = false;
    bool mBackgroundMode = false;

    bool mAveragedPosition = false;
    QList<GnssPositionInformation> mCollectedPositionInformations;

    bool mAveragedPositionFilterAccuracy = false;
    double mBadAccuracyThreshold = std::numeric_limits<double>::quiet_NaN();
    double mExcellentAccuracyThreshold = std::numeric_limits<double>::quiet_NaN();
};

#endif // POSITIONING_H
```


