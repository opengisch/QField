#pragma once

#include "abstractgnssreceiver.h"

#include <QTcpSocket>

class EgenioussReceiver : public AbstractGnssReceiver
{
    Q_OBJECT

  public:
    explicit EgenioussReceiver( const QString &address = QString(), const int port = 0, QObject *parent = nullptr );

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;
    QList<QPair<QString, QVariant>> details() override;

  private slots:
    void onReadyRead();
    void onErrorOccurred( QAbstractSocket::SocketError socketError );

  private:
    void processReceivedData();

  private:
    QTcpSocket *mTcpSocket = nullptr;
    QString mAddress;
    int mPort = 0;
    GnssPositionInformation mLastGnssPositionInformation;
    QByteArray mReceivedData;
    QByteArray payload;
};
