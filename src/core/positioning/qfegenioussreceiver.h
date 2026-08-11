/***************************************************************************
 egeniousseeceiver.h - QfEgenioussReceiver

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

#ifndef QFEGENIOUSSRECEIVER_H
#define QFEGENIOUSSRECEIVER_H

#include "qfabstractgnssreceiver.h"

#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QTcpSocket>

/**
 * \ingroup core
 */
class QfEgenioussReceiver : public QfAbstractGnssReceiver
{
    Q_OBJECT

  public:
    explicit QfEgenioussReceiver( const QString &address = QString(), const int port = 0, QObject *parent = nullptr );
    ~QfEgenioussReceiver() override;

    QfGnssPositionDetails details() const override;

    static QLatin1String identifier;

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;

  private slots:
    void onReadyRead();
    void handleError( QAbstractSocket::SocketError error );

  private:
    void processReceivedData();
    void handleErrorMessage( const QString &errorMessage );

  private:
    QTcpSocket *mTcpSocket = nullptr;
    QJsonObject mPayload;
    const QHostAddress mAddress;
    const int mPort;
};

#endif // QFEGENIOUSSRECEIVER_H
