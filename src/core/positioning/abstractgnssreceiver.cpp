#include "abstractgnssreceiver.h"

AbstractGnssReceiver::AbstractGnssReceiver( QObject *parent )
  : QObject( parent )
{
}

QString AbstractGnssReceiver::socketStateString()
{
  switch ( mSocketState )
  {
    case QAbstractSocket::ConnectingState:
    case QAbstractSocket::HostLookupState:
      return tr( "Connecting…" );
    case QAbstractSocket::ConnectedState:
    case QAbstractSocket::BoundState:
      return tr( "Successfully connected" );
    case QAbstractSocket::UnconnectedState:
      return tr( "Disconnected" );
    default:
      return tr( "Socket state %1" ).arg( static_cast<int>( socketState() ) );
  }
}

void AbstractGnssReceiver::setSocketState( const QAbstractSocket::SocketState &state )
{
  if ( mSocketState == state )
    return;
  mSocketState = state;
  emit socketStateChanged( mSocketState );
  emit socketStateStringChanged( socketStateString() );
}
