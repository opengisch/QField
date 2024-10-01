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

    Q_PROPERTY( GnssPositionInformation lastGnssPositionInformation READ lastGnssPositionInformation NOTIFY lastGnssPositionInformationChanged )
    Q_PROPERTY( QAbstractSocket::SocketState socketState READ socketState NOTIFY socketStateChanged )
    Q_PROPERTY( QString socketStateString READ socketStateString NOTIFY socketStateStringChanged )
    Q_PROPERTY( QString lastError READ lastError NOTIFY lastErrorChanged )

  public:
    enum Capability
    {
      NoCapabilities = 0,      //!< No capabilities
      OrthometricAltitude = 1, //!< Orthometric altitude support
      Logging = 1 << 1,        //!< Logging support
    };
    Q_DECLARE_FLAGS( Capabilities, Capability )
    Q_FLAGS( Capabilities )

    explicit AbstractGnssReceiver( QObject *parent = nullptr )
      : QObject( parent ) {}
    virtual ~AbstractGnssReceiver() = default;

    bool valid() const { return mValid; }
    void setValid( bool valid ) { mValid = valid; }

    Q_INVOKABLE void connectDevice() { handleConnectDevice(); }
    Q_INVOKABLE void disconnectDevice() { handleDisconnectDevice(); }

    Q_INVOKABLE void startLogging() { handleStartLogging(); }
    Q_INVOKABLE void stopLogging() { handleStopLogging(); }

    GnssPositionInformation lastGnssPositionInformation() const { return mLastGnssPositionInformation; }

    QAbstractSocket::SocketState socketState() const { return mSocketState; }
    QString socketStateString() const { return mSocketStateString; }
    QString lastError() const { return mLastError; }

    Q_INVOKABLE virtual AbstractGnssReceiver::Capabilities capabilities() const { return NoCapabilities; }

    virtual QList<QPair<QString, QVariant>> details() { return {}; }

  signals:
    void validChanged();
    void lastGnssPositionInformationChanged( GnssPositionInformation &lastGnssPositionInformation );
    void socketStateChanged( QAbstractSocket::SocketState socketState );
    void socketStateStringChanged( QString &socketStateString );
    void lastErrorChanged( QString &lastError );

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

    virtual void handleStartLogging() {}
    virtual void handleStopLogging() {}

    bool mValid = false;
    GnssPositionInformation mLastGnssPositionInformation;
    QAbstractSocket::SocketState mSocketState = QAbstractSocket::UnconnectedState;
    QString mSocketStateString;
    QString mLastError;
};

#endif // ABSTRACTGNSSRECEIVER_H
