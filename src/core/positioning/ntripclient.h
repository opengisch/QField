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

  public slots:
    void stop();

  signals:
    void correctionDataReceived( const QByteArray &rtcmData );
    void errorOccurred( const QString &message );

  private slots:
    //void onReadyRead();
    void onFinished();
    void onError( QNetworkReply::NetworkError code );

  private:
    QNetworkAccessManager mNetworkManager;
    QNetworkReply *mReply = nullptr;
};
