/***************************************************************************
 serialportreceiver.h - SerialPortReceiver

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

#ifndef SERIALPORTRECEIVER_H
#define SERIALPORTRECEIVER_H

#include "nmeagnssreceiver.h"

#include <QSerialPort>

/**
 * The serial port receiver connects to a device and feeds the QgsNmeaConnection over QSerialPort.
 * It receives QgsGpsInformation and converts it to GnssPositionInformation
 * \ingroup core
 */
class SerialPortReceiver : public NmeaGnssReceiver
{
    Q_OBJECT

  public:
    explicit SerialPortReceiver( const QString &address = QString(), QObject *parent = nullptr );
    ~SerialPortReceiver() override;

    static QLatin1String identifier;

  public:
    void writeRawData( const QByteArray &data ) override;

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;
    void handleError( QSerialPort::SerialPortError error );

    QString mAddress;

    QSerialPort *mSerialPort = nullptr;
};

#endif // SERIALPORTRECEIVER_H
