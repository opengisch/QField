/***************************************************************************
 qfexternalaccessoryreceiver.h - QfExternalAccessoryReceiver

 ---------------------
 begin                : 26.09.2026
 copyright            : (C) 2026 by James Curtin
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFEXTERNALACCESSORYRECEIVER_H
#define QFEXTERNALACCESSORYRECEIVER_H

#include "qfnmeagnssreceiver.h"

#include <QBuffer>

/**
 * The external accessory receiver connects to a GNSS receiver through the iOS
 * External Accessory framework and feeds the QgsNmeaConnection over a proxy QBuffer.
 * Unlike the iOS location services, this gives access to the receiver's own
 * accuracy (GST) values.
 * \ingroup core
 */
class QfExternalAccessoryReceiver : public QfNmeaGnssReceiver
{
    Q_OBJECT

  public:
    explicit QfExternalAccessoryReceiver( QObject *parent = nullptr );
    ~QfExternalAccessoryReceiver() override;

    static QLatin1String identifier;

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;

    //! Opens a session with the first connected accessory supporting the protocol, if any
    void openSession();
    //! Closes the current session, if any
    void closeSession();

    void handleBytesReceived( const QByteArray &bytes );
    void handleAccessoryDisconnected( unsigned long connectionId );
    void handleSessionEnded();

    QBuffer *mBuffer = nullptr;
    QByteArray mBufferData;

    class SessionContainer;
    SessionContainer *mContainer = nullptr;
};

#endif // QFEXTERNALACCESSORYRECEIVER_H
