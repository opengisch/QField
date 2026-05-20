

# File ntripsourcetablefetcher.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**ntripsourcetablefetcher.h**](ntripsourcetablefetcher_8h.md)

[Go to the documentation of this file](ntripsourcetablefetcher_8h.md)


```C++
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
#include <qgspoint.h>


class NtripMountPoint
{
    Q_GADGET

    Q_PROPERTY( QString mountPoint MEMBER mountPoint )
    Q_PROPERTY( QString identifier MEMBER identifier )
    Q_PROPERTY( QString format MEMBER format )
    Q_PROPERTY( QgsPoint point MEMBER point )

  public:
    explicit NtripMountPoint( const QString &mountPoint = QString(), const QString &identifier = QString(), const QString &format = QString(), const QgsPointXY &point = QgsPointXY() )
      : mountPoint( mountPoint )
      , identifier( identifier )
      , format( format )
      , point( point )
    {}

    bool operator==( const NtripMountPoint &other ) const
    {
      return mountPoint == other.mountPoint && identifier == other.identifier && format == other.format && point == other.point;
    }
    bool operator!=( const NtripMountPoint &other ) const { return !operator==( other ); }

    QString mountPoint;
    QString identifier;
    QString format;
    QgsPoint point;
};

Q_DECLARE_METATYPE( NtripMountPoint )



class NtripSourceTableFetcher : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool isFetching READ isFetching NOTIFY isFetchingChanged )
    Q_PROPERTY( QList<NtripMountPoint> mountPoints READ mountPoints NOTIFY mountPointsChanged )

  public:
    explicit NtripSourceTableFetcher( QObject *parent = nullptr );

    ~NtripSourceTableFetcher() noexcept override = default;

    bool isFetching() const { return mIsFetching; }

    QList<NtripMountPoint> mountPoints() const { return mMountPoints; }

    Q_INVOKABLE void fetch( const NtripSettings &ntripSettings );

    Q_INVOKABLE void cancel();

  signals:
    void isFetchingChanged();

    void mountPointsChanged();

    void fetchError( const QString &message );

  private slots:
    void onSocketConnected();
    void onSocketReadyRead();
    void onSocketDisconnected();
    void onSocketError( QAbstractSocket::SocketError error );

  private:
    void cleanup();
    QList<NtripMountPoint> parseSourceTable( const QByteArray &data ) const;

    bool mIsFetching = false;

    QString mHost;
    quint16 mPort = 0;
    QString mUsername;
    QString mPassword;
    NtripSettings::Protocol mProtocol = NtripSettings::NtripVersion1;

    QTcpSocket *mSocket = nullptr;
    QByteArray mBuffer;
    bool mHeadersParsed = false;
    QList<NtripMountPoint> mMountPoints;
};

#endif // NTRIPSOURCETABLEFETCHER_H
```


