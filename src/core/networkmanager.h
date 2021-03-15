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


/**
 * NetworkManager is used to make reliable and retriable network requests. It wraps around `QgsNetworkAccessManager`.
 *
 * Note: After the request has finished, it is the responsibility of the user to delete the NetworkReply object at an appropriate time.
 * Do not directly delete it inside the slot connected to finished(). You can use the deleteLater() function.
 */
class NetworkManager
{

  public:

    /**
     * makes HTTP GET \a request and returns a reply.
     */
    static NetworkReply *get( const QNetworkRequest &request );


    /**
     * makes HTTP POST \a request with an optional \a payload and returns a reply
     */
    static NetworkReply *post( const QNetworkRequest &request, const QByteArray &payload = QByteArray() );


    /**
     * makes HTTP POST \a request with a multipart \a payload and returns a reply
     */
    static NetworkReply *post( const QNetworkRequest &request, QHttpMultiPart *payload );


    /**
     * makes HTTP PUT \a request with an optional \a payload and returns a reply
     */
    static NetworkReply *put( const QNetworkRequest &request, const QByteArray &payload = QByteArray() );


    /**
     * makes HTTP PUT \a request with a multipart \a payload and returns a reply
     */
    static NetworkReply *put( const QNetworkRequest &request, QHttpMultiPart *payload );


    /**
     * makes HTTP DELETE \a request with an optional \a payload and returns a reply
     */
    static NetworkReply *deleteResource( const QNetworkRequest &request, const QByteArray &payload = QByteArray() );

};


#endif // NETWORKMANAGER_H
