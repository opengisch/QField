/***************************************************************************
 filereceiver.h - FileReceiver

 ---------------------
 begin                : August 2025
 copyright            : (C) 2025 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include "nmeagnssreceiver.h"

#include <QBuffer>
#include <QObject>
#include <QTimer>

/**
 * The FileReceiver replays a provided NMEA log file in a loop. Useful to
 * debug external GNSS devices.
 * \ingroup core
 */
class FileReceiver : public NmeaGnssReceiver
{
    Q_OBJECT

  public:
    explicit FileReceiver( const QString &filePath = QString(), const int interval = 0, QObject *parent = nullptr );
    ~FileReceiver() override;

    static QLatin1String identifier;

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;

    void readLogsLine();

    QFile mLogs;
    QTimer mTimer;
    QBuffer *mBuffer = nullptr;
};

#endif // FILERECEIVER_H
