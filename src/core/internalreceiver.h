/***************************************************************************
 internalreceiver.h - InternalReceiver

 ---------------------
 begin                : 22.05.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef INTERNALRECEIVER_H
#define INTERNALRECEIVER_H

#include "abstractgnssreceiver.h"
#include "gnsspositioninformation.h"

#include <QGeoPositionInfoSource>
#include <QGeoSatelliteInfoSource>
#include <QObject>

class InternalReceiver : public AbstractGnssReceiver
{
    Q_OBJECT

  public:
    explicit InternalReceiver( QObject *parent = nullptr );
    ~InternalReceiver() override {}

    void disconnectDevice() override;
    void connectDevice() override;

  private slots:

    void handlePositionUpdated( const QGeoPositionInfo &positionInfo );
    void handleError( QGeoPositionInfoSource::Error positioningError );

  private:
    std::unique_ptr<QGeoPositionInfoSource> mGeoPositionSource;

    GnssPositionInformation mLastGnssPositionInformation;
    bool mLastGnssPositionValid = false;
};

#endif // INTERNALRECEIVER_H
