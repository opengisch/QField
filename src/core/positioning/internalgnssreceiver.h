/***************************************************************************
 internalgnssreceiver.h - InternalGnssReceiver

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
#ifndef INTERNALGNSSRECEIVER_H
#define INTERNALGNSSRECEIVER_H

#include "abstractgnssreceiver.h"
#include "gnsspositioninformation.h"

#include <QGeoPositionInfoSource>
#include <QGeoSatelliteInfoSource>
#include <QObject>

class InternalGnssReceiver : public AbstractGnssReceiver
{
    Q_OBJECT

  public:
    explicit InternalGnssReceiver( QObject *parent = nullptr );
    ~InternalGnssReceiver() override = default;

  private slots:

    void onApplicationStateChanged( Qt::ApplicationState state );
    void handlePositionUpdated( const QGeoPositionInfo &positionInfo );
    void handleError( QGeoPositionInfoSource::Error positioningError );

    void handleSatellitesInUseUpdated( const QList<QGeoSatelliteInfo> &satellites );
    void handleSatellitesInViewUpdated( const QList<QGeoSatelliteInfo> &satellites );
    void handleSatelliteError( QGeoSatelliteInfoSource::Error satelliteError );

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;

    std::unique_ptr<QGeoPositionInfoSource> mGeoPositionSource;
    std::unique_ptr<QGeoSatelliteInfoSource> mGeoSatelliteSource;
    bool mActive = false;

    GnssPositionInformation mLastGnssPositionInformation;
    bool mLastGnssPositionValid = false;

    QList<int> mSatellitesID;
    QList<QgsSatelliteInfo> mSatellitesInfo;
    bool mSatelliteInformationValid = true;
};

#endif // INTERNALGNSSRECEIVER_H
