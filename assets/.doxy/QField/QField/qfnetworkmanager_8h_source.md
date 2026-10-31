

# File qfnetworkmanager.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfnetworkmanager.h**](qfnetworkmanager_8h.md)

[Go to the documentation of this file](qfnetworkmanager_8h.md)


```C++
/***************************************************************************
    qfnetworkmanager.h
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

#ifndef QFNETWORKMANAGER_H
#define QFNETWORKMANAGER_H

#include <QObject>


class QNetworkRequest;
class QfNetworkReply;
class QHttpMultiPart;
class QgsNetworkAccessManager;


class QfNetworkManager
{
  public:
    static QfNetworkReply *get( const QNetworkRequest &request );


    static QfNetworkReply *post( const QNetworkRequest &request, const QByteArray &payload = QByteArray() );


    static QfNetworkReply *post( const QNetworkRequest &request, QHttpMultiPart *payload );


    static QfNetworkReply *put( const QNetworkRequest &request, const QByteArray &payload = QByteArray() );


    static QfNetworkReply *put( const QNetworkRequest &request, QHttpMultiPart *payload );


    static QfNetworkReply *deleteResource( const QNetworkRequest &request, const QByteArray &payload = QByteArray() );
};


#endif // QFNETWORKMANAGER_H
```


