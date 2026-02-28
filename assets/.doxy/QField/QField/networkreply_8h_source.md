

# File networkreply.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**networkreply.h**](networkreply_8h.md)

[Go to the documentation of this file](networkreply_8h.md)


```C++
/***************************************************************************
    networkreply.h
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


#ifndef NETWORKREPLY_H
#define NETWORKREPLY_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <qgsnetworkaccessmanager.h>
//#include <QRandomGenerator>


class NetworkReply : public QObject
{
    Q_OBJECT

  public:
    NetworkReply( const QNetworkAccessManager::Operation operation, const QNetworkRequest &request, const QByteArray &payloadByteArray );


    NetworkReply( const QNetworkAccessManager::Operation operation, const QNetworkRequest &request, QHttpMultiPart *payloadMultiPart );


    void abort();


    QNetworkReply *currentRawReply() const;


    void ignoreSslErrors( const QList<QSslError> &errors );


    bool isFinished() const;


  signals:

    void downloadProgress( qint64 bytesReceived, qint64 bytesTotal );


    void uploadProgress( qint64 bytesSent, qint64 bytesTotal );


    void encrypted();


    void finished();


    void redirected( const QUrl &url );


    void redirectAllowed();


    // /////////////////
    // more than QNetworkReply signals
    // /////////////////
    void retry();


    void errorOccurred( QNetworkReply::NetworkError code );


    void temporaryErrorOccurred( QNetworkReply::NetworkError code );

    void currentRawReplyChanged();

  private:
    static const int sMaxTimeoutBetweenRetriesMs = 2000;


    QNetworkAccessManager::Operation mOperation;


    bool mIsFinished = false;

    bool mIsRedirected = false;

    bool mIsMultiPartPayload = false;


    int mRetriesLeft = 0;


    QList<QSslError> mExpectedSslErrors;


    //    QRandomGenerator mRNG;


    QgsNetworkAccessManager *mNetworkAccessManager = QgsNetworkAccessManager::instance();


    QNetworkRequest mRequest;


    const QByteArray mPayloadByteArray;


    QHttpMultiPart *mPayloadMultiPart = nullptr;


    QNetworkReply *mReply = nullptr;


    void initiateRequest();


    void onFinished();


    void onRedirected( const QUrl &url );
};


#endif // NETWORKREPLY_H
```


