/***************************************************************************
 tcpreceiver.h - TcpReceiver

 ---------------------
 begin                : September 2022
 copyright            : (C) 2022 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef TCPRECEIVER_H
#define TCPRECEIVER_H

#include "nmeagnssreceiver.h"

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

/**
 * The TcpReceiver connects to a device and feeds the QgsNmeaConnection over a TCP socket.
 * It receives QgsGpsInformation and converts it to GnssPositionInformation
 * \ingroup core
 */
class TcpReceiver : public NmeaGnssReceiver
{
    Q_OBJECT

  public:
    explicit TcpReceiver( const QString &address = QString(), const int port = 0, QObject *parent = nullptr );
    ~TcpReceiver() override;

    static QLatin1String identifier;

  public slots:
    QString socketStateString() override;

    void writeRawData( const QByteArray &data ) override;

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;
    void handleError( QAbstractSocket::SocketError error );

    QString mAddress;
    int mPort = 0;
    QTcpSocket *mSocket = nullptr;

    bool mReconnectOnDisconnect = false;
    QTimer mReconnectTimer;
};

#endif // TCPRECEIVER_H
