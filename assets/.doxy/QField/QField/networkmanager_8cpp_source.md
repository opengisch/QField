

# File networkmanager.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**networkmanager.cpp**](networkmanager_8cpp.md)

[Go to the documentation of this file](networkmanager_8cpp.md)


```C++
/***************************************************************************
    networkmanager.cpp
    ---------------------
    begin                : June 2020
    copyright            : (C) 2020 by Ivan Ivanov
    email                : ivan at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "networkmanager.h"
#include "networkreply.h"

#include <qgsnetworkaccessmanager.h>


NetworkReply *NetworkManager::get( const QNetworkRequest &request )
{
  return new NetworkReply( QNetworkAccessManager::GetOperation, request, QByteArray() );
}


NetworkReply *NetworkManager::post( const QNetworkRequest &request, const QByteArray &payload )
{
  return new NetworkReply( QNetworkAccessManager::PostOperation, request, payload );
}


NetworkReply *NetworkManager::post( const QNetworkRequest &request, QHttpMultiPart *payload )
{
  return new NetworkReply( QNetworkAccessManager::PostOperation, request, payload );
}


NetworkReply *NetworkManager::put( const QNetworkRequest &request, const QByteArray &payload )
{
  return new NetworkReply( QNetworkAccessManager::PutOperation, request, payload );
}


NetworkReply *NetworkManager::put( const QNetworkRequest &request, QHttpMultiPart *payload )
{
  return new NetworkReply( QNetworkAccessManager::PutOperation, request, payload );
}


NetworkReply *NetworkManager::deleteResource( const QNetworkRequest &request, const QByteArray &payload )
{
  return new NetworkReply( QNetworkAccessManager::DeleteOperation, request, payload );
}
```


