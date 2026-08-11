/***************************************************************************
  qfntripclient.h - QfNtripSourceTableFetcher

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

#ifndef QFNTRIPSOURCETABLEFETCHER_H
#define QFNTRIPSOURCETABLEFETCHER_H

#include "qfntripsettings.h"

#include <QObject>
#include <QSslSocket>
#include <QStringList>
#include <qgspoint.h>


/**
 * \brief A class containing information on an NTRIP mount point
 * \ingroup core
 */
class QfNtripMountPoint
{
    Q_GADGET

    Q_PROPERTY( QString mountPoint MEMBER mountPoint )
    Q_PROPERTY( QString identifier MEMBER identifier )
    Q_PROPERTY( QString format MEMBER format )
    Q_PROPERTY( QgsPoint point MEMBER point )

  public:
    explicit QfNtripMountPoint( const QString &mountPoint = QString(), const QString &identifier = QString(), const QString &format = QString(), const QgsPointXY &point = QgsPointXY() )
      : mountPoint( mountPoint )
      , identifier( identifier )
      , format( format )
      , point( point )
    {}

    bool operator==( const QfNtripMountPoint &other ) const
    {
      return mountPoint == other.mountPoint && identifier == other.identifier && format == other.format && point == other.point;
    }
    bool operator!=( const QfNtripMountPoint &other ) const { return !operator==( other ); }

    QString mountPoint;
    QString identifier;
    QString format;
    QgsPoint point;
};

Q_DECLARE_METATYPE( QfNtripMountPoint )


/**
 * \brief A class to fetch information such as mount points from an NTRIP server's source table.
 * \ingroup core
 */
class QfNtripSourceTableFetcher : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool isFetching READ isFetching NOTIFY isFetchingChanged )
    Q_PROPERTY( QList<QfNtripMountPoint> mountPoints READ mountPoints NOTIFY mountPointsChanged )

  public:
    //! The NTRIP source table fetcher constructor
    explicit QfNtripSourceTableFetcher( QObject *parent = nullptr );

    //! The NTRIP source table fetcher destructor
    ~QfNtripSourceTableFetcher() noexcept override = default;

    //! Returns TRUE if on ongoing source table fetching operation is ongoing
    bool isFetching() const { return mIsFetching; }

    //! Returns the mount points collected during the last source table fetching operation
    QList<QfNtripMountPoint> mountPoints() const { return mMountPoints; }

    /**
     * Fetches details from an NTRIP server source table
     * \param ntripSettings The NTRIP settings of the server
     */
    Q_INVOKABLE void fetch( const QfNtripSettings &ntripSettings );

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
    QList<QfNtripMountPoint> parseSourceTable( const QByteArray &data ) const;

    bool mIsFetching = false;

    QString mHost;
    quint16 mPort = 0;
    QString mUsername;
    QString mPassword;
    QfNtripSettings::Protocol mProtocol = QfNtripSettings::NtripVersion1;

    QSslSocket *mSocket = nullptr;
    QByteArray mBuffer;
    bool mHeadersParsed = false;
    QList<QfNtripMountPoint> mMountPoints;
};

#endif // QFNTRIPSOURCETABLEFETCHER_H
