#pragma once

#include "abstractgnssreceiver.h"

#include <QJsonObject>
#include <QTcpSocket>

class EgenioussReceiver : public AbstractGnssReceiver
{
    Q_OBJECT

  public:
    explicit EgenioussReceiver( QObject *parent = nullptr );

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;
    QList<QPair<QString, QVariant>> details() override;

  private slots:
    void onReadyRead();
    void onErrorOccurred( QAbstractSocket::SocketError socketError );

  private:
    void processReceivedData();
    void connected();
    void disconnected();

  private:
    QTcpSocket *mTcpSocket = nullptr;
    QJsonObject mPayload;
    const QHostAddress::SpecialAddress mAddress = QHostAddress::LocalHost;
    const int mPort = 1235;
};
