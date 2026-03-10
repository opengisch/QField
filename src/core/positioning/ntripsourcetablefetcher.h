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
#pragma once

#include <QObject>
#include <QStringList>
#include <QTcpSocket>

class NtripSourceTableFetcher : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool fetching READ fetching NOTIFY fetchingChanged )
    Q_PROPERTY( QStringList mountpoints READ mountpoints NOTIFY mountpointsChanged )

  public:
    explicit NtripSourceTableFetcher( QObject *parent = nullptr );

    bool fetching() const { return mFetching; }
    QStringList mountpoints() const { return mMountpoints; }

    Q_INVOKABLE void fetch( const QString &host, int port, const QString &username, const QString &password, int version );
    Q_INVOKABLE void cancel();

  signals:
    void fetchingChanged();
    void mountpointsChanged();
    void fetchError( const QString &message );

  private slots:
    void onSocketConnected();
    void onSocketReadyRead();
    void onSocketDisconnected();
    void onSocketError( QAbstractSocket::SocketError error );

  private:
    void cleanup();
    QStringList parseSourceTable( const QByteArray &data ) const;

    QTcpSocket *mSocket = nullptr;
    QString mHost;
    quint16 mPort = 0;
    QString mUsername;
    QString mPassword;
    int mVersion = 1;
    QByteArray mBuffer;
    bool mHeadersParsed = false;
    bool mFetching = false;
    QStringList mMountpoints;
};
