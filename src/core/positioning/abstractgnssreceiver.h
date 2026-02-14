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

/**
 * \ingroup core
 */
class AbstractGnssReceiver : public QObject
{
    Q_OBJECT

  public:
    enum Capability
    {
      NoCapabilities = 0,      //!< No capabilities
      OrthometricAltitude = 1, //!< Orthometric altitude support
      Logging = 1 << 1,        //!< Logging support
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

    /**
     * Writes raw data (e.g. RTCM corrections) to the receiver device.
     * Subclasses with a writable transport (Bluetooth, TCP, UDP, Serial) should
     * override this to forward the data to their socket/port.
     * \param data The raw bytes to write to the device.
     */
    virtual void writeRawData( const QByteArray &data ) { Q_UNUSED( data ) }

    GnssPositionInformation lastGnssPositionInformation() const { return mLastGnssPositionInformation; }

    QString lastError() const { return mLastError; }

    /**
     * Returns extra details (such as hdop, vdop, pdop) provided by the positioning device.
     */
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
