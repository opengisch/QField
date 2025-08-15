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

void NtripClient::start( const QString &ntripHost, const quint16 &port, const QString &mountpoint, const QString &username, const QString &password )
{
  if ( mReply )
  {
    qWarning() << "NtripClient already running";
    return;
  }

  mBytesSent = 0;
  mBytesReceived = 0;

  NtripSocketClient *client = new NtripSocketClient( this );

  connect( client, &NtripSocketClient::correctionDataReceived, [this]( const QByteArray &data ) {
    mBytesReceived += data.size();

    quint8 firstByte = quint8( data.at( 0 ) );
    if ( firstByte == 0xD3 )
    {
      qDebug() << "RTCM chunk:";
    }
    else if ( firstByte == 0x73 )
    {
      qDebug() << "SPARTN chunk:";
    }
    else
    {
      qDebug() << "UNKNOWN chunk:";
    }

    qDebug() << data.size() << "bytes";
    // send to your GNSS device
    emit correctionDataReceived( data );
    emit bytesCountersChanged();
  } );

  connect( client, &NtripSocketClient::errorOccurred, [this]( const QString &msg ) {
    qWarning() << msg;
    emit errorOccurred( msg );
  } );

  connect( client, &NtripSocketClient::streamConnected, [this]() {
    emit streamConnected();
  } );

  mBytesSent = client->start(
    ntripHost,
    port,
    "/" + mountpoint,
    username,
    password );

  // Emit immediately to show sent bytes
  emit bytesCountersChanged();


  //connect(mReply, &QNetworkReply::readyRead, this, &NtripClient::onReadyRead);
  //connect(mReply, &QNetworkReply::finished, this, &NtripClient::onFinished);
  //connect(mReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
  //        this, &NtripClient::onError);
}

void NtripClient::stop()
{
  if ( mReply )
  {
    disconnect( mReply, nullptr, this, nullptr ); // ✅ Disconnect all signals

    if ( mReply->isRunning() )
    {
      mReply->abort(); // ✅ Cancel the request
    }
    mReply->deleteLater();
    mReply = nullptr;
  }
}

/*
void NtripClient::onReadyRead()
{
    QByteArray data = mReply->readAll();
    qInfo() << data + "\n";
    emit correctionDataReceived(data);
}
*/

void NtripClient::onFinished()
{
  if ( mReply )
  {
    emit errorOccurred( "NTRIP connection closed" );
  }
  // Schedule cleanup after Qt finishes emitting signals
  QMetaObject::invokeMethod( this, "stop", Qt::QueuedConnection );
}

void NtripClient::onError( QNetworkReply::NetworkError code )
{
  int status = mReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
  QString reason = mReply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();

  qWarning() << "HTTP status during error:" << status << reason;
  qWarning() << "Network error code:" << code;

  emit errorOccurred(
    QStringLiteral( "Network error %1, HTTP %2 %3" )
      .arg( code )
      .arg( status )
      .arg( reason ) );
  //emit errorOccurred(QStringLiteral("NTRIP error: %1").arg(code));
  // Schedule cleanup after Qt finishes emitting signals
  QMetaObject::invokeMethod( this, "stop", Qt::QueuedConnection );
}
