/***************************************************************************
 egeniousseeceiver.h - EgenioussReceiver

 ---------------------
 begin                : October 2024
 copyright            : (C) 2024 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EGENIOUSSRECEIVER_H
#define EGENIOUSSRECEIVER_H

#include "abstractgnssreceiver.h"

#include <QJsonObject>
#include <QTcpSocket>

/**
 * \ingroup core
 */
class EgenioussReceiver : public AbstractGnssReceiver
{
    Q_OBJECT

  public:
    explicit EgenioussReceiver( QObject *parent = nullptr );
    ~EgenioussReceiver();

    GnssPositionDetails details() const override;

    static QLatin1String identifier;

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;

  private slots:
    void onReadyRead();
    void handleError( QAbstractSocket::SocketError error );

  private:
    void processReceivedData();

  private:
    QTcpSocket *mTcpSocket = nullptr;
    QJsonObject mPayload;
    const QHostAddress::SpecialAddress mAddress = QHostAddress::LocalHost;
    const int mPort = 1235;
};

#endif // EGENIOUSSRECEIVER_H
