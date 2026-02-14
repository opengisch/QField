#pragma once

#include <QObject>

class NtripSocketClient;

class NtripClient : public QObject
{
    Q_OBJECT
  public:
    explicit NtripClient( QObject *parent = nullptr );
    ~NtripClient();

    void start( const QString &ntripHost, const quint16 &port, const QString &mountpoint, const QString &username, const QString &password, int version = 1 );
    void stop();

    void sendNmeaSentence( const QString &sentence );

    qint64 bytesSent() const { return mBytesSent; }
    qint64 bytesReceived() const { return mBytesReceived; }

  signals:
    void correctionDataReceived( const QByteArray &rtcmData );
    void errorOccurred( const QString &message, bool isPermanent );
    void bytesCountersChanged();
    void streamConnected();
    void streamDisconnected();

  private:
    NtripSocketClient *mSocketClient = nullptr;
    qint64 mBytesSent = 0;
    qint64 mBytesReceived = 0;
};