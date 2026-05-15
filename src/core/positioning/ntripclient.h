/***************************************************************************
  ntripclient.h - NtripClient

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

#ifndef NTRIPCLIENT_H
#define NTRIPCLIENT_H

#include "ntripsettings.h"

#include <QFile>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class NtripSocketClient;

class NtripClient : public QObject
{
    Q_OBJECT
  public:
    explicit NtripClient( QObject *parent = nullptr );
    ~NtripClient() noexcept override;

    void start( const NtripSettings &ntripSettings );
    void stop();

    void sendNmeaSentence( const QString &sentence );

    void startLogging( const QString &path );
    void stopLogging();

    qint64 bytesSent() const { return mBytesSent; }
    qint64 bytesReceived() const { return mBytesReceived; }

  signals:
    void correctionDataReceived( const QByteArray &rtcmData );
    void errorOccurred( const QString &message, bool isPermanent );
    void bytesCountersChanged();
    void streamConnected();
    void streamDisconnected();

  private:
    void logRtcmData( const QByteArray &data );

    NtripSocketClient *mSocketClient = nullptr;
    qint64 mBytesSent = 0;
    qint64 mBytesReceived = 0;
    QFile mLogFile;
    int mLogBlockCount = 0;
};

class NtripSocketClient : public QObject
{
    Q_OBJECT
  public:
    explicit NtripSocketClient( QObject *parent = nullptr );
    ~NtripSocketClient() noexcept override;

    qint64 start( const NtripSettings &ntripSettings );

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

  private:
    qint64 estimateRequestSize() const;
    void processChunkedData( const QByteArray &data );
    int parseHttpStatusCode( const QByteArray &headerBlock );
    bool isPermanentHttpError( int statusCode );

    QTcpSocket *mSocket;
    bool mHeadersSent = false;
    QByteArray mHeaderBuffer;
    QString mHost;
    quint16 mPort = 0;
    QString mMountPoint;
    QString mUsername;
    QString mPassword;
    bool mPendingError = false;
    NtripSettings::Protocol mProtocol = NtripSettings::NtripVersion1;
    bool mChunkedEncoding = false;
    QByteArray mChunkBuffer;
    int mChunkRemaining = -1;
};

#endif // NTRIPCLIENT_H
