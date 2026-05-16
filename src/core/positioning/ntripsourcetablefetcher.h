/***************************************************************************
  ntripclient.h - NtripSourceTableFetcher

 ---------------------
 begin                : 05.02.2026
 copyright            : (C) 2026 by Vincent LAMBERT
 email                :
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef NTRIPSOURCETABLEFETCHER_H
#define NTRIPSOURCETABLEFETCHER_H

#include "ntripsettings.h"

#include <QObject>
#include <QStringList>
#include <QTcpSocket>


/**
 * \brief A class to fetch information such as mount points from an NTRIP server's source table.
 * \ingroup core
 */
class NtripSourceTableFetcher : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool isFetching READ isFetching NOTIFY isFetchingChanged )
    Q_PROPERTY( QStringList mountPoints READ mountPoints NOTIFY mountPointsChanged )

  public:
    //! The NTRIP source table fetcher constructor
    explicit NtripSourceTableFetcher( QObject *parent = nullptr );

    //! The NTRIP source table fetcher destructor
    ~NtripSourceTableFetcher() noexcept override = default;

    //! Returns TRUE if on ongoing source table fetching operation is ongoing
    bool isFetching() const { return mIsFetching; }

    //! Returns the mount points collected during the last source table fetching operation
    QStringList mountPoints() const { return mMountPoints; }

    /**
     * Fetches details from an NTRIP server source table
     * \param ntripSettings The NTRIP settings of the server
     */
    Q_INVOKABLE void fetch( const NtripSettings &ntripSettings );

    //! Cancels any ongoing fetching operation
    Q_INVOKABLE void cancel();

  signals:
    //! Emitted when a fetching operation has begun or ended
    void isFetchingChanged();

    //! Emitted when the list of collected mount points has changed
    void mountPointsChanged();

    //! Emitted when an error has occured during a fetching operation
    void fetchError( const QString &message );

  private slots:
    void onSocketConnected();
    void onSocketReadyRead();
    void onSocketDisconnected();
    void onSocketError( QAbstractSocket::SocketError error );

  private:
    void cleanup();
    QStringList parseSourceTable( const QByteArray &data ) const;

    bool mIsFetching = false;

    QString mHost;
    quint16 mPort = 0;
    QString mUsername;
    QString mPassword;
    NtripSettings::Protocol mProtocol = NtripSettings::NtripVersion1;

    QTcpSocket *mSocket = nullptr;
    QByteArray mBuffer;
    bool mHeadersParsed = false;
    QStringList mMountPoints;
};

#endif // NTRIPSOURCETABLEFETCHER_H
