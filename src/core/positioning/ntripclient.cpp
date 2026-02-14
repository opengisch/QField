#include "ntripclient.h"
#include "ntripsocketclient.h"

#include <QDebug>

NtripClient::NtripClient( QObject *parent )
  : QObject( parent )
{
}

NtripClient::~NtripClient()
{
  stop();
}

void NtripClient::start( const QString &ntripHost, const quint16 &port, const QString &mountpoint, const QString &username, const QString &password, int version )
{
  if ( mSocketClient )
  {
    return;
  }

  mBytesSent = 0;
  mBytesReceived = 0;

  mSocketClient = new NtripSocketClient( this );

  connect( mSocketClient, &NtripSocketClient::correctionDataReceived, [this]( const QByteArray &data ) {
    mBytesReceived += data.size();

    emit correctionDataReceived( data );
    emit bytesCountersChanged();
  } );

  connect( mSocketClient, &NtripSocketClient::errorOccurred, [this]( const QString &msg, bool isPermanent ) {
    qWarning() << msg;
    emit errorOccurred( msg, isPermanent );
  } );

  connect( mSocketClient, &NtripSocketClient::streamConnected, [this]() {
    emit streamConnected();
  } );

  connect( mSocketClient, &NtripSocketClient::streamDisconnected, [this]() {
    emit streamDisconnected();
  } );

  mBytesSent = mSocketClient->start(
    ntripHost,
    port,
    "/" + mountpoint,
    username,
    password,
    version );

  // Emit immediately to show sent bytes
  emit bytesCountersChanged();
}

void NtripClient::stop()
{
  if ( mSocketClient )
  {
    mSocketClient->stop();
    mSocketClient->deleteLater();
    mSocketClient = nullptr;
  }
}

void NtripClient::sendNmeaSentence( const QString &sentence )
{
  if ( !mSocketClient )
  {
    return;
  }

  const qint64 bytesWritten = mSocketClient->sendNmeaSentence( sentence.toUtf8() );
  if ( bytesWritten > 0 )
  {
    mBytesSent += bytesWritten;
    emit bytesCountersChanged();
  }
}