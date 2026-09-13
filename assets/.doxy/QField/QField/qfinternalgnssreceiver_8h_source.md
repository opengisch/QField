

# File qfinternalgnssreceiver.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfinternalgnssreceiver.h**](qfinternalgnssreceiver_8h.md)

[Go to the documentation of this file](qfinternalgnssreceiver_8h.md)


```C++
/***************************************************************************
 qfinternalgnssreceiver.h - QfInternalGnssReceiver

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
#ifndef QFINTERNALGNSSRECEIVER_H
#define QFINTERNALGNSSRECEIVER_H

#include "qfabstractgnssreceiver.h"
#include "qfgnsspositioninformation.h"

#include <QGeoPositionInfoSource>
#include <QGeoSatelliteInfoSource>
#include <QObject>

class QfInternalGnssReceiver : public QfAbstractGnssReceiver
{
    Q_OBJECT

  public:
    explicit QfInternalGnssReceiver( QObject *parent = nullptr );
    ~QfInternalGnssReceiver();

  private slots:

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

    QfGnssPositionInformation mLastGnssPositionInformation;
    bool mLastGnssPositionValid = false;

    QList<int> mSatellitesID;
    QList<QgsSatelliteInfo> mSatellitesInfo;
    bool mSatelliteInformationValid = true;

    QDateTime mPreconnectSatellitePositionTimestamp;
    bool mSatellitePositionReceived = false;
};

#endif // QFINTERNALGNSSRECEIVER_H
```


