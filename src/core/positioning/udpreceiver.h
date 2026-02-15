/***************************************************************************
 udpreceiver.h - UdpReceiver

 ---------------------
 begin                : December 2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include "nmeagnssreceiver.h"

#include <QBuffer>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>

/**
 * The UdpReceiver connects to a device and feeds the QgsNmeaConnection over a UDP socket.
 * It receives QgsGpsInformation and converts it to GnssPositionInformation
 * \ingroup core
 */
class UdpReceiver : public NmeaGnssReceiver
{
    Q_OBJECT

  public:
    explicit UdpReceiver( const QString &address = QString(), const int port = 0, QObject *parent = nullptr );
    ~UdpReceiver();

    static QLatin1String identifier;

  public slots:
    QString socketStateString() override;

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;
    void handleError( QAbstractSocket::SocketError error );

    QString mAddress;
    int mPort = 0;
    QUdpSocket *mSocket = nullptr;
    QBuffer *mBuffer = nullptr;

    bool mReconnectOnDisconnect = false;
    QTimer mReconnectTimer;
};

#endif // UDPRECEIVER_H
