/***************************************************************************
 abstractreceiver.h - AbstractReceiver

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
#ifndef ABSTRACTRECEIVER_H
#define ABSTRACTRECEIVER_H

#include "gnsspositioninformation.h"

#include <QAbstractSocket>
#include <QObject>

class AbstractReceiver : public QObject
{
    Q_OBJECT

    Q_PROPERTY( GnssPositionInformation lastGnssPositionInformation READ lastGnssPositionInformation NOTIFY lastGnssPositionInformationChanged )
    Q_PROPERTY( QAbstractSocket::SocketState socketState READ socketState NOTIFY socketStateChanged )
    Q_PROPERTY( QString socketStateString READ socketStateString NOTIFY socketStateStringChanged )

  public:
    explicit AbstractReceiver( QObject *parent = nullptr )
      : QObject( parent ) {}
    virtual ~AbstractReceiver() = default;

    virtual void disconnectDevice() = 0;
    virtual void connectDevice() = 0;

    GnssPositionInformation lastGnssPositionInformation() const { return mLastGnssPositionInformation; }
    QAbstractSocket::SocketState socketState() const { return mSocketState; }
    QString socketStateString() const { return mSocketStateString; }

  signals:

    void lastGnssPositionInformationChanged( GnssPositionInformation &lastGnssPositionInformation );
    void socketStateChanged( QAbstractSocket::SocketState socketState );
    void socketStateStringChanged( QString socketStateString );

  private:
    friend class BluetoothReceiver;
    friend class InternalReceiver;

    GnssPositionInformation mLastGnssPositionInformation;
    QAbstractSocket::SocketState mSocketState;
    QString mSocketStateString;
};

#endif // ABSTRACTRECEIVER_H
