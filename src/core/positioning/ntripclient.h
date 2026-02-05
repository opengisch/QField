#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>

class NtripClient : public QObject
{
    Q_OBJECT
  public:
    explicit NtripClient( QObject *parent = nullptr );
    ~NtripClient();

    void start( const QString &ntripHost, const quint16 &port, const QString &mountpoint, const QString &username, const QString &password );
    void stop();

    qint64 bytesSent() const { return mBytesSent; }
    qint64 bytesReceived() const { return mBytesReceived; }

  signals:
    void correctionDataReceived( const QByteArray &rtcmData );
    void errorOccurred( const QString &message );
    void bytesCountersChanged();
    void streamConnected();

  private slots:
    void onFinished();
    void onError( QNetworkReply::NetworkError code );

  private:
    QNetworkAccessManager mNetworkManager;
    QNetworkReply *mReply = nullptr;
    qint64 mBytesSent = 0;
    qint64 mBytesReceived = 0;
};