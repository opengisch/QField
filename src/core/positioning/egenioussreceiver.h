#pragma once

#include "abstractgnssreceiver.h"

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

  private:
    QTcpSocket *mTcpSocket = nullptr;
    GnssPositionInformation mLastGnssPositionInformation;
    QByteArray mReceivedData;
    QByteArray payload;
};
