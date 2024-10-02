#ifndef EGENIOUSSRECEIVER_H
#define EGENIOUSSRECEIVER_H

#include "abstractgnssreceiver.h"

#include <QJsonObject>
#include <QTcpSocket>

class EgenioussReceiver : public AbstractGnssReceiver
{
    Q_OBJECT

  public:
    explicit EgenioussReceiver( QObject *parent = nullptr );
    ~EgenioussReceiver();

  public slots:
    QAbstractSocket::SocketState socketState() override;

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;
    QList<QPair<QString, QVariant>> details() override;

  private slots:
    void onReadyRead();
    void handleError( QAbstractSocket::SocketError error );
    void setSocketState( const QAbstractSocket::SocketState socketState );

  private:
    void processReceivedData();

  private:
    QTcpSocket *mTcpSocket = nullptr;
    QJsonObject mPayload;
    const QHostAddress::SpecialAddress mAddress = QHostAddress::LocalHost;
    const int mPort = 1235;
};

#endif // EGENIOUSSRECEIVER_H
