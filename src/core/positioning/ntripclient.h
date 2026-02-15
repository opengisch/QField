#pragma once

#include <QFile>
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