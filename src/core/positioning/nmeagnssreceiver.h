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

/**
 * The nmeareceiver connects to a device and feeds the QgsNmeaConnection.
 * It receives QgsGpsInformation and converts it to GnssPositionInformation
 */
class NmeaGnssReceiver : public AbstractGnssReceiver
{
    Q_OBJECT

  public:
    explicit NmeaGnssReceiver( QObject *parent = nullptr );
    ~NmeaGnssReceiver() override = default;

    void initNmeaConnection( QIODevice *ioDevice );

    AbstractGnssReceiver::Capabilities capabilities() const override { return AbstractGnssReceiver::OrthometricAltitude; }

  protected:
    std::unique_ptr<QgsNmeaConnection> mNmeaConnection;

    bool mLastGnssPositionValid = false;

  private slots:
    void stateChanged( const QgsGpsInformation &info );
    void nmeaSentenceReceived( const QString &substring );

  private:
    void handleStartLogging() override;
    void handleStopLogging() override;

    QTime mLastGnssPositionUtcTime;

    QFile mLogFile;
    QTextStream mLogStream;
};

#endif // NMEAGNSSRECEIVER_H
