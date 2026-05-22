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

#include "abstractgnssreceiver.h"
#include "ntripsettings.h"

#include <QFile>
#include <QObject>
#include <QPointer>
#include <QSslSocket>
#include <QTimer>

class NtripSocket;


/**
 * \brief A simple NTRIP client
 * \ingroup core
 */
class NtripClient : public QObject
{
    Q_OBJECT

  public:
    //! The NTRIP client constructor
    explicit NtripClient( QObject *parent = nullptr );

    //! The NTRIP client destructor
    ~NtripClient() noexcept override;

    /**
     * Starts a connection to an NTRIP server
     * \param ntripSettings the NTRIP settings object
     * \param receiver the GNSS receiver that will receive corrections and,
     * optionally, forward NMEA sentence from
     */
    void start( const NtripSettings &ntripSettings, AbstractGnssReceiver *receiver );

    //! Stops any ongoing connection to an NTRIP server
    void stop();

    //! Sends an NMEA \a sentence to the NTRIP server
    void sendNmeaSentence( const QString &sentence );

    /**
     * Start logging received correction to a given folder \a path
     * \note the actual log file name will be generated to include
     * the current date and time
     */
    void startLogging( const QString &path );

    //! Stops logging received corrections
    void stopLogging();

    /**
     * Returns the number of bytes sent since a connection to an
     * NTRIP server has been started.
     */
    qint64 bytesSent() const { return mBytesSent; }

    /**
     * Returns the number of bytes received since a connection to an
     * NTRIP server has been started.
     */
    qint64 bytesReceived() const { return mBytesReceived; }

  signals:
    //! Emmitted when correction \a data has been received from an NTRIP server
    void correctionDataReceived( const QByteArray &data );

    //! Emmitted when an error occured while communicating with an NTRIP server
    void errorOccurred( const QString &message, bool isPermanent );

    //! Emitted when the received bytes counts have changed
    void bytesReceivedChanged();

    //! Emitted when the sent bytes counts have changed
    void bytesSentChanged();

    //! Emitted when successfully connected to an NTRIP server
    void streamConnected();

    //! Emitted when successfully disconnected from an NTRIP server
    void streamDisconnected();

  private slots:
    void nmeaSentenceReceived( const QString &sentence );

  private:
    void logData( const QByteArray &data );

    NtripSocket *mSocket = nullptr;
    qint64 mBytesSent = 0;
    qint64 mBytesReceived = 0;

    QFile mLogFile;
    QDataStream mLogStream;
    int mLogBlockCount = 0;

    qint64 mLastNtripGgaSent = 0;

    QPointer<AbstractGnssReceiver> mReceiver;
};


/**
 * \brief A simple NTRIP socket
 * \ingroup core
 */
class NtripSocket : public QObject
{
    Q_OBJECT

  public:
    //! The NTRIP socket constructor
    explicit NtripSocket( QObject *parent = nullptr );

    //! The NTRIP socket destructor
    ~NtripSocket() noexcept override;

    /**
     * Connects the socket to an NTRIP server
     * \param ntripSettings the NTRIP settings object
     */
    qint64 connectToHost( const NtripSettings &ntripSettings );

    //! Write  an NMEA \a sentence to the NTRIP server
    qint64 writeNmeaSentence( const QByteArray &sentence );

    //! Abort any socket connection to an NTRIP server
    void abort();

  signals:
    //! Emmitted when the socket has received \a data from an NTRIP server
    void correctionDataReceived( const QByteArray &data );

    //! Emmitted when an error occured while the socket communicates with an NTRIP server
    void errorOccurred( const QString &message, bool isPermanent );

    //! Emitted when the socket has successfully connected to an NTRIP server
    void streamConnected();

    //! Emitted when the socket has disconnected from an NTRIP server
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

    QSslSocket *mSocket;
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
