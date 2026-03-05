

# File abstractgnssreceiver.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**abstractgnssreceiver.cpp**](abstractgnssreceiver_8cpp.md)

[Go to the documentation of this file](abstractgnssreceiver_8cpp.md)


```C++
/***************************************************************************
 abstractgnssreceiver.cpp - AbstractGnssReceiver

 ---------------------
 begin                : October 2024
 copyright            : (C) 2024 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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
  {
    return;
  }

  mSocketState = state;

  emit socketStateChanged( mSocketState );
  emit socketStateStringChanged( socketStateString() );
}
```


