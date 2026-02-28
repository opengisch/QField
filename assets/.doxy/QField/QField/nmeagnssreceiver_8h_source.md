

# File nmeagnssreceiver.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**nmeagnssreceiver.h**](nmeagnssreceiver_8h.md)

[Go to the documentation of this file](nmeagnssreceiver_8h.md)


```C++
/***************************************************************************
 nmeagnssreceiver.h - NmeaGnssReceiver

 ---------------------
 begin                : 21.10.2022
 copyright            : (C) 202 by Matthias Kuhn
 email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef NMEAGNSSRECEIVER_H
#define NMEAGNSSRECEIVER_H

#include "abstractgnssreceiver.h"
#include "qgsnmeaconnection.h"

#include <QFile>
#include <QObject>

class NmeaGnssReceiver : public AbstractGnssReceiver
{
    Q_OBJECT

  public:
    explicit NmeaGnssReceiver( QObject *parent = nullptr );
    ~NmeaGnssReceiver() override = default;

    void initNmeaConnection( QIODevice *ioDevice );

  protected:
    std::unique_ptr<QgsNmeaConnection> mNmeaConnection;

    bool mLastGnssPositionValid = false;

  private slots:
    void stateChanged( const QgsGpsInformation &info );
    void nmeaSentenceReceived( const QString &substring );

  private:
    void handleStartLogging( const QString &path ) override;
    void handleStopLogging() override;
    GnssPositionDetails details() const override;

    void processImuSentence( const QString &sentence );

    QTime mLastGnssPositionUtcTime;

    QFile mLogFile;
    QTextStream mLogStream;

    GnssPositionInformation mCurrentNmeaGnssPositionInformation;

    struct ImuPosition
    {
        bool valid = false;
        QDateTime utcDateTime;
        double latitude = std::numeric_limits<double>::quiet_NaN();
        double longitude = std::numeric_limits<double>::quiet_NaN();
        double altitude = std::numeric_limits<double>::quiet_NaN();
        double speed = std::numeric_limits<double>::quiet_NaN();
        double speedDown = std::numeric_limits<double>::quiet_NaN();
        double direction = std::numeric_limits<double>::quiet_NaN();
        double roll = std::numeric_limits<double>::quiet_NaN();
        double pitch = std::numeric_limits<double>::quiet_NaN();
        double heading = std::numeric_limits<double>::quiet_NaN();
        double steering = std::numeric_limits<double>::quiet_NaN();
        double accelerometerX = std::numeric_limits<double>::quiet_NaN();
        double accelerometerY = std::numeric_limits<double>::quiet_NaN();
        double accelerometerZ = std::numeric_limits<double>::quiet_NaN();
        double gyroX = std::numeric_limits<double>::quiet_NaN();
        double gyroY = std::numeric_limits<double>::quiet_NaN();
        double gyroZ = std::numeric_limits<double>::quiet_NaN();
        double steeringZ = std::numeric_limits<double>::quiet_NaN();
    };
    ImuPosition mImuPosition;
};

#endif // NMEAGNSSRECEIVER_H
```


