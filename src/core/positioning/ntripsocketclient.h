#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class NtripSocketClient : public QObject
{
    Q_OBJECT
  public:
    explicit NtripSocketClient( QObject *parent = nullptr );
    ~NtripSocketClient();

    qint64 start(
      const QString &host,
      quint16 port,
      const QString &mountpoint,
      const QString &username,
      const QString &password );

    void stop();

  signals:
    void correctionDataReceived( const QByteArray &data );
    void errorOccurred( const QString &message );
    void streamConnected();

  private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onSocketError( QAbstractSocket::SocketError error );

  private:
    QTcpSocket mSocket;
    bool mHeadersSent = false;
};
