/***************************************************************************
  ntripsocketclient.h - NtripSocketClient

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
#include <QTcpSocket>
#include <QTimer>

class NtripSocketClient : public QObject
{
    Q_OBJECT
  public:
    explicit NtripSocketClient( QObject *parent = nullptr );
    ~NtripSocketClient() noexcept override;

    qint64 start(
      const QString &host,
      quint16 port,
      const QString &mountpoint,
      const QString &username,
      const QString &password,
      int version = 1 );

    qint64 sendNmeaSentence( const QByteArray &sentence );

    void stop();

  signals:
    void correctionDataReceived( const QByteArray &data );
    void errorOccurred( const QString &message, bool isPermanent );
    void streamConnected();
    void streamDisconnected();

  private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onSocketError( QAbstractSocket::SocketError error );
    qint64 estimateRequestSize() const;

  private:
    void processChunkedData( const QByteArray &data );

    QTcpSocket *mSocket;
    bool mHeadersSent = false;
    QByteArray mHeaderBuffer;
    QString mHost;
    quint16 mPort = 0;
    QString mMountpoint;
    QString mUsername;
    QString mPassword;
    bool mPendingError = false;
    int mVersion = 1;
    bool mChunkedEncoding = false;
    QByteArray mChunkBuffer;
    int mChunkRemaining = -1;
};