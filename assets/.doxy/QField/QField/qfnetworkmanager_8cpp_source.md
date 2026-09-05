

# File qfnetworkmanager.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfnetworkmanager.cpp**](qfnetworkmanager_8cpp.md)

[Go to the documentation of this file](qfnetworkmanager_8cpp.md)


```C++
/***************************************************************************
    qfnetworkmanager.cpp
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

#include "qfnetworkmanager.h"
#include "qfnetworkreply.h"

#include <qgsnetworkaccessmanager.h>


QfNetworkReply *QfNetworkManager::get( const QNetworkRequest &request )
{
  return new QfNetworkReply( QNetworkAccessManager::GetOperation, request, QByteArray() );
}


QfNetworkReply *QfNetworkManager::post( const QNetworkRequest &request, const QByteArray &payload )
{
  return new QfNetworkReply( QNetworkAccessManager::PostOperation, request, payload );
}


QfNetworkReply *QfNetworkManager::post( const QNetworkRequest &request, QHttpMultiPart *payload )
{
  return new QfNetworkReply( QNetworkAccessManager::PostOperation, request, payload );
}


QfNetworkReply *QfNetworkManager::put( const QNetworkRequest &request, const QByteArray &payload )
{
  return new QfNetworkReply( QNetworkAccessManager::PutOperation, request, payload );
}


QfNetworkReply *QfNetworkManager::put( const QNetworkRequest &request, QHttpMultiPart *payload )
{
  return new QfNetworkReply( QNetworkAccessManager::PutOperation, request, payload );
}


QfNetworkReply *QfNetworkManager::deleteResource( const QNetworkRequest &request, const QByteArray &payload )
{
  return new QfNetworkReply( QNetworkAccessManager::DeleteOperation, request, payload );
}
```


