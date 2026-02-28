

# File qfieldxmlhttprequest.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldxmlhttprequest.h**](qfieldxmlhttprequest_8h.md)

[Go to the documentation of this file](qfieldxmlhttprequest_8h.md)


```C++
/***************************************************************************
qfieldxmlhttprequest.h - QFieldXmlHttpRequest

---------------------
begin                : 15.12.2025
copyright            : (C) 2025 by Kaustuv Pokharel
email                : kaustuv (at) opengis.ch
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef QFIELDXMLHTTPREQUEST_H
#define QFIELDXMLHTTPREQUEST_H

#include <QJSValue>
#include <QList>
#include <QMap>
#include <QMetaObject>
#include <QMetaType>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QPointer>
#include <QUrl>
#include <QVariant>

class QHttpMultiPart;

class QFieldXmlHttpRequest : public QObject
{
    Q_OBJECT

    Q_PROPERTY( ReadyState readyState READ readyState NOTIFY readyStateChanged )

    
    Q_PROPERTY( int status READ status NOTIFY responseChanged )

    Q_PROPERTY( QString statusText READ statusText NOTIFY responseChanged )

    Q_PROPERTY( QString responseText READ responseText NOTIFY responseChanged )

    Q_PROPERTY( QVariant response READ response NOTIFY responseChanged )

    Q_PROPERTY( QString responseType READ responseType NOTIFY responseChanged )

    Q_PROPERTY( QUrl responseUrl READ responseUrl NOTIFY responseChanged )

    Q_PROPERTY( int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged )

    Q_PROPERTY( QJSValue onreadystatechange READ onreadystatechange WRITE setOnreadystatechange )

    Q_PROPERTY( QJSValue ondownloadprogress READ ondownloadprogress WRITE setOndownloadprogress )

    Q_PROPERTY( QJSValue onuploadprogress READ onuploadprogress WRITE setOnuploadprogress )

    Q_PROPERTY( QJSValue onredirected READ onredirected WRITE setOnredirected )

    Q_PROPERTY( QJSValue ontimeout READ ontimeout WRITE setOntimeout )

    Q_PROPERTY( QJSValue onaborted READ onaborted WRITE setOnaborted )

    Q_PROPERTY( QJSValue onerror READ onerror WRITE setOnerror )

    Q_PROPERTY( QJSValue onload READ onload WRITE setOnload )

    Q_PROPERTY( QJSValue onloadend READ onloadend WRITE setOnloadend )

  public:
    enum ReadyState : uchar
    {
      Unsent = 0,      
      Opened,          
      HeadersReceived, 
      Loading,         
      Done             
    };
    Q_ENUM( ReadyState )

    explicit QFieldXmlHttpRequest( QObject *parent = nullptr );
    ~QFieldXmlHttpRequest() override;

    Q_INVOKABLE static QFieldXmlHttpRequest *newRequest( QObject *parent = nullptr );

    Q_INVOKABLE void open( const QString &method, const QUrl &url );

    Q_INVOKABLE void setRequestHeader( const QString &headerName, const QString &headerValue );

    Q_INVOKABLE void send( const QVariant &body = QVariant() );

    Q_INVOKABLE void abort();

    Q_INVOKABLE QString getResponseHeader( const QString &name ) const;

    Q_INVOKABLE QString getAllResponseHeaders() const;

    ReadyState readyState() const { return mReadyState; }

    int status() const { return mStatus; }

    QString statusText() const { return mStatusText; }

    QString responseText() const { return mResponseText; }

    QVariant response() const { return mResponse; }

    QString responseType() const { return mResponseType; }

    QUrl responseUrl() const { return mResponseUrl; }

    void setTimeout( int milliseconds );

    int timeout() const { return mTimeoutMs; }

    QJSValue onreadystatechange() const { return mOnReadyStateChanged; }

    void setOnreadystatechange( const QJSValue &callback ) { mOnReadyStateChanged = callback; }

    QJSValue ondownloadprogress() const { return mOnDownloadProgress; }

    void setOndownloadprogress( const QJSValue &callback ) { mOnDownloadProgress = callback; }

    QJSValue onuploadprogress() const { return mOnUploadProgress; }

    void setOnuploadprogress( const QJSValue &callback ) { mOnUploadProgress = callback; }

    QJSValue onredirected() const { return mOnRedirected; }

    void setOnredirected( const QJSValue &callback ) { mOnRedirected = callback; }

    QJSValue ontimeout() const { return mOnTimeout; }

    void setOntimeout( const QJSValue &callback ) { mOnTimeout = callback; }

    QJSValue onaborted() const { return mOnAborted; }

    void setOnaborted( const QJSValue &callback ) { mOnAborted = callback; }

    QJSValue onerror() const { return mOnError; }

    void setOnerror( const QJSValue &callback ) { mOnError = callback; }

    QJSValue onload() const { return mOnLoad; }

    void setOnload( const QJSValue &callback ) { mOnLoad = callback; }

    QJSValue onloadend() const { return mOnLoadEnd; }

    void setOnloadend( const QJSValue &callback ) { mOnLoadEnd = callback; }

  signals:
    void readyStateChanged();

    void responseChanged();

    void timeoutChanged();

  private:
    bool isOpen() const;
    void setReadyState( ReadyState state );
    void clearResponse();
    void resetForNewRequest();

    void cleanupReply( bool abortNetwork );

    void startRequest( const QVariant &body );
    void hookReply( QNetworkReply *networkReply );
    void finalizeReply( QNetworkReply *finishedReply );
    void finalizeAsError( const QString &detail, bool shouldEmitResponseChanged = true );

    QByteArray bodyToBytes( const QVariant &body, QString *outContentType = nullptr ) const;
    bool bodyContainsFileUrls( const QVariant &body ) const;
    QHttpMultiPart *buildMultipart( const QVariant &body ) const;
    bool isAllowedLocalUploadPath( const QString &localPath ) const;

    void call( const QJSValue &callback, const QJSValueList &arguments = {} );
    static bool containsNewlines( const QString &stringValue );

    void collectResponseHeaders( QNetworkReply *networkReply );
    static QString normalizeHeaderName( const QString &stringValue );

    void scheduleAutoDelete();

  private:
    QPointer<QNetworkReply> mReply;
    QList<QMetaObject::Connection> mConnections;

    QNetworkRequest mRequest;
    QByteArray mMethod;
    QUrl mUrl;

    ReadyState mReadyState = Unsent;

    int mTimeoutMs = 0;

    bool mFinalized = false;
    bool mTimedOut = false;
    bool mAborted = false;

    quint64 mRequestSerial = 0;

    int mStatus = 0;
    QString mStatusText;
    QVariant mResponse;
    QString mResponseText;
    QString mResponseType;
    QUrl mResponseUrl;

    QMap<QString, QString> mResponseHeaders;

    QNetworkReply::NetworkError mLastError = QNetworkReply::NoError;
    QString mLastErrorString;

    QJSValue mOnReadyStateChanged;
    QJSValue mOnDownloadProgress;
    QJSValue mOnUploadProgress;
    QJSValue mOnRedirected;
    QJSValue mOnTimeout;
    QJSValue mOnAborted;
    QJSValue mOnError;
    QJSValue mOnLoad;
    QJSValue mOnLoadEnd;
};

#endif // QFIELDXMLHTTPREQUEST_H
```


