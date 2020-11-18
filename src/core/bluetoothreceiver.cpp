#include "bluetoothreceiver.h"
#include <QDebug>

BluetoothReceiver::BluetoothReceiver(QObject *parent)
{
    startDeviceDiscovery();
}
void BluetoothReceiver::startDeviceDiscovery()
{
    // Create a discovery agent and connect to its signals
    QBluetoothDeviceDiscoveryAgent *deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    deviceDiscoveryAgent->start();

    QList discoveredDevices = deviceDiscoveryAgent->discoveredDevices();

    for( QBluetoothDeviceInfo device: discoveredDevices )
    {
        //qDebug() << "Device:" << device.name() << '(' << device.address().toString() << ')';
        if( device.name() == "reach")
        {
            qDebug() << "Check service for: " << device.name() << '(' << device.address().toString() << ')';

            QBluetoothServiceDiscoveryAgent *serviceDiscoveryAgent = new QBluetoothServiceDiscoveryAgent(device.address(), this);

            connect(serviceDiscoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
                    this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));

        }
    }
}

void BluetoothReceiver::serviceDiscovered(const QBluetoothServiceInfo &service)
{
    qDebug() << "Service:" << service.serviceName() << '(' << service.device().address().toString() << ')';
    connectService( service );
}

void BluetoothReceiver::connectService(const QBluetoothServiceInfo &service)
{
    socket = new QBluetoothSocket( QBluetoothServiceInfo::RfcommProtocol );

     connect(socket, &QBluetoothSocket::connected,
         [=](){
        qDebug() << "SOCKET CONNECTED";
        readSocket();
     });
     connect(socket, &QBluetoothSocket::disconnected,
         [=](){
        qDebug() << "SOCKET DISCONNECTED";
     });
     connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
         [=](QBluetoothSocket::SocketError error){
        qDebug() << "SOCKET ERROR: " <<error;
     });

    socket->connectToService(service.device().address(), service.serviceUuid());
}

void BluetoothReceiver::readSocket()
{
    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        qDebug() << socket->peerName() << "sais ["<<line.length()<<"]: "<< QString::fromUtf8(line.constData());
    }
}

