/***************************************************************************
 qfserialportreceiver.h - QfSerialPortReceiver

 ---------------------
 begin                : 15.01.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFSERIALPORTRECEIVER_H
#define QFSERIALPORTRECEIVER_H

#include "qfnmeagnssreceiver.h"

#include <QSerialPort>

/**
 * The serial port receiver connects to a device and feeds the QgsNmeaConnection over QSerialPort.
 * It receives QgsGpsInformation and converts it to QfGnssPositionInformation
 * \ingroup core
 */
class QfSerialPortReceiver : public QfNmeaGnssReceiver
{
    Q_OBJECT

  public:
    explicit QfSerialPortReceiver( const QString &address = QString(), QObject *parent = nullptr );
    ~QfSerialPortReceiver() override;

    static QLatin1String identifier;

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;
    void handleError( QSerialPort::SerialPortError error );

    QString mAddress;

    QSerialPort *mSerialPort = nullptr;
};

#endif // QFSERIALPORTRECEIVER_H
