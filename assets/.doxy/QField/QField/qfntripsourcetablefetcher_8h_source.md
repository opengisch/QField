

# File qfntripsourcetablefetcher.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfntripsourcetablefetcher.h**](qfntripsourcetablefetcher_8h.md)

[Go to the documentation of this file](qfntripsourcetablefetcher_8h.md)


```C++
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



class QfNtripSourceTableFetcher : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool isFetching READ isFetching NOTIFY isFetchingChanged )
    Q_PROPERTY( QList<QfNtripMountPoint> mountPoints READ mountPoints NOTIFY mountPointsChanged )

  public:
    explicit QfNtripSourceTableFetcher( QObject *parent = nullptr );

    ~QfNtripSourceTableFetcher() noexcept override = default;

    bool isFetching() const { return mIsFetching; }

    QList<QfNtripMountPoint> mountPoints() const { return mMountPoints; }

    Q_INVOKABLE void fetch( const QfNtripSettings &ntripSettings );

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
```


