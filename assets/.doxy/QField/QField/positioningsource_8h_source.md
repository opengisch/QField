

# File positioningsource.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**positioningsource.h**](positioningsource_8h.md)

[Go to the documentation of this file](positioningsource_8h.md)


```C++
/***************************************************************************
  positioningsource.h - PositioningSource

 ---------------------
 begin                : 20.12.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef POSITIONINGSOURCE_H
#define POSITIONINGSOURCE_H

#include "abstractgnssreceiver.h"
#include "gnsspositioninformation.h"

#include <QCompass>
#include <QObject>
#include <QTimer>

class PositioningSource : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool active READ active WRITE setActive NOTIFY activeChanged )
    Q_PROPERTY( bool valid READ valid NOTIFY validChanged )

    Q_PROPERTY( QString deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged )
    Q_PROPERTY( GnssPositionDetails deviceDetails READ deviceDetails NOTIFY positionInformationChanged )
    Q_PROPERTY( QString deviceLastError READ deviceLastError NOTIFY deviceLastErrorChanged )
    Q_PROPERTY( QAbstractSocket::SocketState deviceSocketState READ deviceSocketState NOTIFY deviceSocketStateChanged )
    Q_PROPERTY( QString deviceSocketStateString READ deviceSocketStateString NOTIFY deviceSocketStateStringChanged )

    Q_PROPERTY( GnssPositionInformation positionInformation READ positionInformation NOTIFY positionInformationChanged )

    Q_PROPERTY( ElevationCorrectionMode elevationCorrectionMode READ elevationCorrectionMode WRITE setElevationCorrectionMode NOTIFY elevationCorrectionModeChanged )
    Q_PROPERTY( double antennaHeight READ antennaHeight WRITE setAntennaHeight NOTIFY antennaHeightChanged )

    Q_PROPERTY( double orientation READ orientation NOTIFY orientationChanged );

    Q_PROPERTY( bool logging READ logging WRITE setLogging NOTIFY loggingChanged )
    Q_PROPERTY( QString loggingPath READ loggingPath WRITE setLoggingPath NOTIFY loggingPathChanged )

    Q_PROPERTY( bool backgroundMode READ backgroundMode WRITE setBackgroundMode NOTIFY backgroundModeChanged )

  public:
    enum class ElevationCorrectionMode
    {
      None,                    
      OrthometricFromDevice,   
      OrthometricFromGeoidFile 
    };
    Q_ENUM( ElevationCorrectionMode )

    explicit PositioningSource( QObject *parent = nullptr );
    virtual ~PositioningSource() = default;

    bool active() const { return mActive; }

    void setActive( bool active );

    bool valid() const { return mValid; }

    void setValid( bool valid );

    Q_INVOKABLE QString deviceId() const { return mDeviceId; }

    void setDeviceId( const QString &id );

    AbstractGnssReceiver *device() const { return mReceiver.get(); }

    GnssPositionDetails deviceDetails() const { return mReceiver ? mReceiver->details() : GnssPositionDetails(); }

    QString deviceLastError() const { return mReceiver ? mReceiver->lastError() : QString(); }

    QAbstractSocket::SocketState deviceSocketState() const { return mReceiver ? mReceiver->socketState() : QAbstractSocket::UnconnectedState; }

    QString deviceSocketStateString() const { return mReceiver ? mReceiver->socketStateString() : QString(); }

    GnssPositionInformation positionInformation() const { return mPositionInformation; };

    ElevationCorrectionMode elevationCorrectionMode() const { return mElevationCorrectionMode; }

    void setElevationCorrectionMode( ElevationCorrectionMode elevationCorrectionMode );

    double antennaHeight() const { return mAntennaHeight; }

    void setAntennaHeight( double antennaHeight );

    double orientation() const { return mOrientation; }

    bool logging() const { return mLogging; }

    void setLogging( bool logging );

    QString loggingPath() const { return mLoggingPath; }

    void setLoggingPath( const QString &path );

    bool backgroundMode() const { return mBackgroundMode; }

    void setBackgroundMode( bool backgroundMode );

    Q_INVOKABLE QList<GnssPositionInformation> getBackgroundPositionInformation() const;

    static QString backgroundFilePath;

  signals:
    void activeChanged();
    void validChanged();
    void deviceIdChanged();
    void deviceChanged();
    void deviceLastErrorChanged();
    void deviceSocketStateChanged();
    void deviceSocketStateStringChanged();
    void positionInformationChanged();
    void elevationCorrectionModeChanged();
    void antennaHeightChanged();
    void orientationChanged();
    void loggingChanged();
    void loggingPathChanged();
    void backgroundModeChanged();

  public slots:

    void triggerConnectDevice();
    void triggerDisconnectDevice();

  private slots:

    void lastGnssPositionInformationChanged( const GnssPositionInformation &lastGnssPositionInformation );
    void processCompassReading();

  private:
    void setupDevice();

    bool mActive = false;

    QString mDeviceId;
    bool mValid = false;

    GnssPositionInformation mPositionInformation;

    ElevationCorrectionMode mElevationCorrectionMode = ElevationCorrectionMode::None;
    double mAntennaHeight = 0.0;

    bool mLogging = false;
    QString mLoggingPath;

    bool mBackgroundMode = false;

    std::unique_ptr<AbstractGnssReceiver> mReceiver;

    QCompass mCompass;
    QTimer mCompassTimer;
    double mOrientation = std::numeric_limits<double>::quiet_NaN();
};

Q_DECLARE_METATYPE( PositioningSource::ElevationCorrectionMode )

#endif // POSITIONINGSOURCE_H
```


