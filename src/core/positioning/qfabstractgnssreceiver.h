/***************************************************************************
 qfabstractgnssreceiver.h - QfAbstractGnssReceiver

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
#ifndef QFABSTRACTGNSSRECEIVER_H
#define QFABSTRACTGNSSRECEIVER_H

#include "qfgnsspositioninformation.h"

#include <QAbstractSocket>
#include <QObject>

/**
 * \ingroup core
 */
class QfAbstractGnssReceiver : public QObject
{
    Q_OBJECT

  public:
    enum Capability
    {
      NoCapabilities = 0,       //!< No capabilities
      OrthometricAltitude = 1,  //!< Orthometric altitude support
      Logging = 1 << 1,         //!< Logging support
      NtripCorrection = 1 << 2, //!< NTRIP correction support
    };
    Q_DECLARE_FLAGS( Capabilities, Capability )
    Q_FLAGS( Capabilities )

    explicit QfAbstractGnssReceiver( QObject *parent = nullptr );
    virtual ~QfAbstractGnssReceiver() = default;

    virtual QfAbstractGnssReceiver::Capabilities capabilities() const;

    bool valid() const { return mValid; }
    void setValid( bool valid ) { mValid = valid; }

    void connectDevice() { handleConnectDevice(); }
    void disconnectDevice() { handleDisconnectDevice(); }

    void startLogging( const QString &path ) { handleStartLogging( path ); }
    void stopLogging() { handleStopLogging(); }

    QfGnssPositionInformation lastGnssPositionInformation() const { return mLastGnssPositionInformation; }

    QString lastError() const { return mLastError; }

    double batteryLevel() const { return mBatteryLevel; }

    /**
     * Returns extra details (such as hdop, vdop, pdop) provided by the positioning device.
     */
    virtual QfGnssPositionDetails details() const { return {}; }
    virtual QAbstractSocket::SocketState socketState() const { return mSocketState; }
    virtual QString socketStateString();

  protected:
    void setSocketState( const QAbstractSocket::SocketState &state );

  signals:
    void validChanged();
    void lastGnssPositionInformationChanged( const QfGnssPositionInformation &lastGnssPositionInformation );
    void socketStateChanged( const QAbstractSocket::SocketState socketState );
    void socketStateStringChanged( const QString &socketStateString );
    void lastErrorChanged( const QString &lastError );
    void batteryLevelChanged( const double batteryLevel );

  public slots:
    virtual void onCorrectionDataReceived( const QByteArray &data ) {}

  private:
    friend class QfInternalGnssReceiver;
    friend class QfEgenioussReceiver;
    friend class QfNmeaGnssReceiver;
    friend class QfBluetoothLowEnergyReceiver;
    friend class QfBluetoothReceiver;
    friend class QfTcpReceiver;
    friend class QfUdpReceiver;
    friend class QfSerialPortReceiver;

    virtual void handleConnectDevice() {}
    virtual void handleDisconnectDevice() {}

    virtual void handleStartLogging( const QString &path ) {}
    virtual void handleStopLogging() {}

    bool mValid = false;
    QfGnssPositionInformation mLastGnssPositionInformation;
    QAbstractSocket::SocketState mSocketState = QAbstractSocket::UnconnectedState;
    QString mLastError;
    double mBatteryLevel = std::numeric_limits<double>::quiet_NaN();
};

Q_DECLARE_METATYPE( QfAbstractGnssReceiver )

#endif // QFABSTRACTGNSSRECEIVER_H
