

# File networkmanager.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**networkmanager.h**](networkmanager_8h.md)

[Go to the documentation of this file](networkmanager_8h.md)


```C++
/***************************************************************************
    networkmanager.h
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

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>


class QNetworkRequest;
class NetworkReply;
class QHttpMultiPart;
class QgsNetworkAccessManager;


class NetworkManager
{
  public:
    static NetworkReply *get( const QNetworkRequest &request );


    static NetworkReply *post( const QNetworkRequest &request, const QByteArray &payload = QByteArray() );


    static NetworkReply *post( const QNetworkRequest &request, QHttpMultiPart *payload );


    static NetworkReply *put( const QNetworkRequest &request, const QByteArray &payload = QByteArray() );


    static NetworkReply *put( const QNetworkRequest &request, QHttpMultiPart *payload );


    static NetworkReply *deleteResource( const QNetworkRequest &request, const QByteArray &payload = QByteArray() );
};


#endif // NETWORKMANAGER_H
```


