/***************************************************************************
  mountpointmodel.h - MountpointModel

 ---------------------
 begin                : 14.02.2026
 copyright            : (C) 2026 by Pascal
 email                :
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MOUNTPOINTMODEL_H
#define MOUNTPOINTMODEL_H

#include "sourcetableparser.h"

#include <QAbstractListModel>
#include <QTcpSocket>

class MountpointModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( FetchStatus fetchStatus READ fetchStatus NOTIFY fetchStatusChanged )
    Q_PROPERTY( QString lastError READ lastError NOTIFY lastErrorChanged )

  public:
    enum FetchStatus
    {
      Idle,
      Fetching,
      Success,
      Error
    };
    Q_ENUM( FetchStatus )

    enum Role
    {
      NameRole = Qt::UserRole,
      FormatRole,
      IdentifierRole,
      CountryRole,
      NetworkRole,
      NavSystemRole,
      LatitudeRole,
      LongitudeRole,
      DistanceRole,
      IsNearestRole,
    };
    Q_ENUM( Role )

    explicit MountpointModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    Q_INVOKABLE void fetchMountpoints(
      const QString &host, int port,
      const QString &username, const QString &password,
      int version = 1 );

    Q_INVOKABLE void cancelFetch();

    Q_INVOKABLE void updatePosition( double latitude, double longitude );

    FetchStatus fetchStatus() const;
    QString lastError() const;

  signals:
    void fetchStatusChanged();
    void lastErrorChanged();

  private slots:
    void onSocketConnected();
    void onSocketReadyRead();
    void onSocketDisconnected();
    void onSocketError( QAbstractSocket::SocketError error );

  private:
    void setFetchStatus( FetchStatus status );
    void setLastError( const QString &error );
    void cleanup();

    static double haversineDistance( double lat1, double lon1, double lat2, double lon2 );

    QList<SourcetableParser::MountpointInfo> mMountpoints;
    QVector<double> mDistances;
    int mNearestIndex = -1;
    bool mHasPosition = false;
    QTcpSocket mSocket;
    QByteArray mBuffer;
    bool mHeadersParsed = false;
    FetchStatus mFetchStatus = Idle;
    QString mLastError;

    QString mHost;
    int mPort = 0;
    QString mUsername;
    QString mPassword;
    int mVersion = 1;
};

#endif // MOUNTPOINTMODEL_H
