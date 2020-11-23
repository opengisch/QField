/***************************************************************************
 bluetoothreceiver.h - BluetoothReceiver

 ---------------------
 begin                : 18.11.2020
 copyright            : (C) 2020 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef BLUETOOTHRECEIVER_H
#define BLUETOOTHRECEIVER_H

#include <QObject>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothSocket>
#include "qgsnmeaconnection.h"

class BluetoothReceiver : public QObject
{
    Q_OBJECT

    Q_PROPERTY( double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged )
    Q_PROPERTY( double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged )
    Q_PROPERTY( double elevation READ altitude WRITE setElevation NOTIFY elevationChanged )
    Q_PROPERTY( QString qualityDescription READ qualityDescription WRITE setQualityDescription NOTIFY qualityDescriptionChanged )

    public:
        explicit BluetoothReceiver( QObject *parent = nullptr );
        Q_INVOKABLE void reconnectDevice( const QString &address );
        void connectService( const QBluetoothAddress &address );
        void repairDevice( const QBluetoothAddress &address );

        double longitude() const { return m_longitude; }
        double latitude() const { return m_latitude; }
        double altitude() const { return m_elevation; }
        QString qualityDescription() const { return m_qualityDescription; }

public slots:
        void setLongitude(double longitude)
        {
            qWarning("Floating point comparison needs context sanity check");
            if (qFuzzyCompare(m_longitude, longitude))
                return;

            m_longitude = longitude;
            emit longitudeChanged(m_longitude);
        }

        void setLatitude(double latitude)
        {
            qWarning("Floating point comparison needs context sanity check");
            if (qFuzzyCompare(m_latitude, latitude))
                return;

            m_latitude = latitude;
            emit latitudeChanged(m_latitude);
        }

        void setElevation(double elevation)
        {
            qWarning("Floating point comparison needs context sanity check");
            if (qFuzzyCompare(m_elevation, elevation))
                return;

            m_elevation = elevation;
            emit elevationChanged(m_elevation);
        }

        void setQualityDescription(QString qualityDescription)
        {
            if (m_qualityDescription == qualityDescription)
                return;

            m_qualityDescription = qualityDescription;
            emit qualityDescriptionChanged(m_qualityDescription);
        }

signals:
        void longitudeChanged(int longitude);
        void latitudeChanged(int latitude);
        void elevationChanged(int elevation);
        void qualityDescriptionChanged(QString qualityDescription);

private slots:
        void pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status);
        void confirmPairing(const QBluetoothAddress &address, QString pin);
        void stateChanged(const QgsGpsInformation &info );

    private:
        QBluetoothLocalDevice *mLocalDevice = nullptr;
        QBluetoothSocket *mSocket = nullptr;
        QgsNmeaConnection *mGpsConnection = nullptr;
        double m_longitude;
        double m_latitude;
        double m_elevation;
        QString m_qualityDescription;
};

#endif // BLUETOOTHRECEIVER_H
