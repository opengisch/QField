

# File serialportreceiver.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**serialportreceiver.h**](serialportreceiver_8h.md)

[Go to the documentation of this file](serialportreceiver_8h.md)


```C++
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

class SerialPortReceiver : public NmeaGnssReceiver
{
    Q_OBJECT

  public:
    explicit SerialPortReceiver( const QString &address = QString(), QObject *parent = nullptr );
    ~SerialPortReceiver() override;

    static QLatin1String identifier;

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;
    void handleError( QSerialPort::SerialPortError error );

    QString mAddress;

    QSerialPort *mSerialPort = nullptr;
};

#endif // SERIALPORTRECEIVER_H
```


