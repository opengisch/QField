

# File abstractgnssreceiver.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**abstractgnssreceiver.h**](abstractgnssreceiver_8h.md)

[Go to the documentation of this file](abstractgnssreceiver_8h.md)


```C++
/***************************************************************************
 abstractgnssreceiver.h - AbstractGnssReceiver

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
#ifndef ABSTRACTGNSSRECEIVER_H
#define ABSTRACTGNSSRECEIVER_H

#include "gnsspositioninformation.h"

#include <QAbstractSocket>
#include <QObject>

class AbstractGnssReceiver : public QObject
{
    Q_OBJECT

  public:
    enum Capability
    {
      NoCapabilities = 0,      
      OrthometricAltitude = 1, 
      Logging = 1 << 1,        
    };
    Q_DECLARE_FLAGS( Capabilities, Capability )
    Q_FLAGS( Capabilities )

    explicit AbstractGnssReceiver( QObject *parent = nullptr );
    virtual ~AbstractGnssReceiver() = default;

    bool valid() const { return mValid; }
    void setValid( bool valid ) { mValid = valid; }

    void connectDevice() { handleConnectDevice(); }
    void disconnectDevice() { handleDisconnectDevice(); }

    void startLogging( const QString &path ) { handleStartLogging( path ); }
    void stopLogging() { handleStopLogging(); }

    GnssPositionInformation lastGnssPositionInformation() const { return mLastGnssPositionInformation; }

    QString lastError() const { return mLastError; }

    virtual GnssPositionDetails details() const { return {}; }
    virtual QAbstractSocket::SocketState socketState() const { return mSocketState; }
    virtual QString socketStateString();

  protected:
    void setSocketState( const QAbstractSocket::SocketState &state );

  signals:
    void validChanged();
    void lastGnssPositionInformationChanged( const GnssPositionInformation &lastGnssPositionInformation );
    void socketStateChanged( const QAbstractSocket::SocketState socketState );
    void socketStateStringChanged( const QString &socketStateString );
    void lastErrorChanged( const QString &lastError );

  private:
    friend class InternalGnssReceiver;
    friend class EgenioussReceiver;
    friend class NmeaGnssReceiver;
    friend class BluetoothReceiver;
    friend class TcpReceiver;
    friend class UdpReceiver;
    friend class SerialPortReceiver;

    virtual void handleConnectDevice() {}
    virtual void handleDisconnectDevice() {}

    virtual void handleStartLogging( const QString &path ) {}
    virtual void handleStopLogging() {}

    bool mValid = false;
    GnssPositionInformation mLastGnssPositionInformation;
    QAbstractSocket::SocketState mSocketState = QAbstractSocket::UnconnectedState;
    QString mLastError;
};

#endif // ABSTRACTGNSSRECEIVER_H
```


