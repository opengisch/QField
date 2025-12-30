/***************************************************************************
qfieldxmlhttprequest.h - QFieldXmlHttpRequest

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

/**
 * This class provides a small, predictable
 * subset of browser XMLHttpRequest semantics:
 *
 *  - open(method, url)
 *  - setRequestHeader(name, value)
 *  - send(body)
 *  - abort()
 *
 * It exposes common XHR state and event callbacks as QML properties.
 *
 * Payload handling:
 *  - QByteArray is sent as-is.
 *  - QString is sent as UTF-8 text (Content-Type is inferred if not set).
 *  - QVariantMap / QVariantList is encoded as JSON (Content-Type is inferred if not set).
 *  - Multipart upload is enabled if the body contains "file://..." URLs or if the caller sets
 *    a Content-Type header starting with "multipart/form-data".
 *
 * Security:
 *  - Local file uploads are restricted to:
 *    - the current project directory
 *    - the QFieldCloud local directory
 *
 * \note This is not a full browser XHR implementation. It focuses on a clean QML API and
 * predictable behavior in QField.
 */
class QFieldXmlHttpRequest : public QObject
{
    Q_OBJECT

    //! The current request state, aligned with browser XMLHttpRequest readyState values.
    Q_PROPERTY( ReadyState readyState READ readyState NOTIFY readyStateChanged )

    //! The HTTP status code of the final response. For network-level failures, this is 0.
    Q_PROPERTY( int status READ status NOTIFY responseChanged )

    //! The HTTP reason phrase of the final response (when available).
    Q_PROPERTY( QString statusText READ statusText NOTIFY responseChanged )

    //! The response body decoded as UTF-8 text.
    Q_PROPERTY( QString responseText READ responseText NOTIFY responseChanged )

    //! The parsed response value for JSON payloads; otherwise it equals responseText.
    Q_PROPERTY( QVariant response READ response NOTIFY responseChanged )

    //! The response Content-Type string (as provided by the server).
    Q_PROPERTY( QString responseType READ responseType NOTIFY responseChanged )

    //! The final response URL (after redirects, when redirects are followed).
    Q_PROPERTY( QUrl responseUrl READ responseUrl NOTIFY responseChanged )

    //! The request timeout in milliseconds. A value of 0 disables the timeout.
    //!
    //! When the timeout triggers, the request is aborted and ontimeout/onloadend are called.
    Q_PROPERTY( int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged )

    //! If enabled, the object schedules deleteLater() after onload/onerror/onloadend completes.
    Q_PROPERTY( bool autoDelete READ autoDelete WRITE setAutoDelete NOTIFY autoDeleteChanged )

    //! Called whenever readyState changes.
    Q_PROPERTY( QJSValue onreadystatechange READ onreadystatechange WRITE setOnreadystatechange )

    //! Called during download progress as (receivedBytes, totalBytes).
    Q_PROPERTY( QJSValue ondownloadprogress READ ondownloadprogress WRITE setOndownloadprogress )

    //! Called during upload progress as (sentBytes, totalBytes).
    Q_PROPERTY( QJSValue onuploadprogress READ onuploadprogress WRITE setOnuploadprogress )

    //! Called when a redirect is reported by Qt as (urlString).
    Q_PROPERTY( QJSValue onredirected READ onredirected WRITE setOnredirected )

    //! Called when the timeout triggers.
    Q_PROPERTY( QJSValue ontimeout READ ontimeout WRITE setOntimeout )

    //! Called when the request is aborted.
    Q_PROPERTY( QJSValue onaborted READ onaborted WRITE setOnaborted )

    //! Called for network-level failures as (errorCode, errorMessage).
    Q_PROPERTY( QJSValue onerror READ onerror WRITE setOnerror )

    //! Called when the request completes and status is available.
    //!
    //! \note HTTP errors like 404/500 still trigger onload. Developers/Users should inspect status.
    Q_PROPERTY( QJSValue onload READ onload WRITE setOnload )

    //! Called at the end of the request lifecycle (success, error, abort, timeout).
    Q_PROPERTY( QJSValue onloadend READ onloadend WRITE setOnloadend )

  public:
    //! ReadyState values aligned with browser XMLHttpRequest.
    enum ReadyState : uchar
    {
      Unsent = 0,      // open() has not been called or open() args were invalid
      Opened,          // open() called successfully; request is prepared
      HeadersReceived, // headers are available (metaDataChanged)
      Loading,         // body is being received (readyRead)
      Done             // request finished (success or failure)
    };
    Q_ENUM( ReadyState )

    explicit QFieldXmlHttpRequest( QObject *parent = nullptr );
    ~QFieldXmlHttpRequest() override;

    //! Creates a new request instance.
    Q_INVOKABLE static QFieldXmlHttpRequest *newRequest( QObject *parent = nullptr );

    //! Prepares the request. This must be called before send().
    //!
    //! Calling open() while a request is active aborts the active request.
    //! Supported methods are GET, POST, PUT, and DELETE.
    Q_INVOKABLE void open( const QString &method, const QUrl &url );

    //! Sets a raw HTTP request header.
    //!
    //! Headers are only applied after open() and before send(). Basic newline protection
    //! is applied to prevent CR/LF header injection.
    Q_INVOKABLE void setRequestHeader( const QString &headerName, const QString &headerValue );

    //! Starts the request.
    //!
    //! The body may be:
    //!  - QByteArray (sent as-is)
    //!  - QString (UTF-8 text)
    //!  - QVariantMap / QVariantList (JSON encoding)
    //!  - QVariantMap with "file://..." strings for multipart uploads (restricted local paths)
    Q_INVOKABLE void send( const QVariant &body = QVariant() );

    //! Aborts the current request (if any).
    //!
    //! This triggers onaborted/onloadend (if set) and transitions readyState to Done.
    Q_INVOKABLE void abort();

    //! Returns a response header value by name (final response only).
    Q_INVOKABLE QString getResponseHeader( const QString &name ) const;

    //! Returns all response headers in HTTP-style format (final response only).
    Q_INVOKABLE QString getAllResponseHeaders() const;

    ReadyState readyState() const { return mReadyState; }

    int status() const { return mStatus; }
    QString statusText() const { return mStatusText; }

    QString responseText() const { return mResponseText; }
    QVariant response() const { return mResponse; }
    QString responseType() const { return mResponseType; }
    QUrl responseUrl() const { return mResponseUrl; }

    //! Sets the request timeout in milliseconds. A value of 0 disables the timeout.
    void setTimeout( int milliseconds );
    int timeout() const { return mTimeoutMs; }

    bool autoDelete() const { return mAutoDelete; }
    void setAutoDelete( bool enabled );

    QJSValue onreadystatechange() const { return mOnReadyStateChanged; }
    void setOnreadystatechange( const QJSValue &cb ) { mOnReadyStateChanged = cb; }

    QJSValue ondownloadprogress() const { return mOnDownloadProgress; }
    void setOndownloadprogress( const QJSValue &cb ) { mOnDownloadProgress = cb; }

    QJSValue onuploadprogress() const { return mOnUploadProgress; }
    void setOnuploadprogress( const QJSValue &cb ) { mOnUploadProgress = cb; }

    QJSValue onredirected() const { return mOnRedirected; }
    void setOnredirected( const QJSValue &cb ) { mOnRedirected = cb; }

    QJSValue ontimeout() const { return mOnTimeout; }
    void setOntimeout( const QJSValue &cb ) { mOnTimeout = cb; }

    QJSValue onaborted() const { return mOnAborted; }
    void setOnaborted( const QJSValue &cb ) { mOnAborted = cb; }

    QJSValue onerror() const { return mOnError; }
    void setOnerror( const QJSValue &cb ) { mOnError = cb; }

    QJSValue onload() const { return mOnLoad; }
    void setOnload( const QJSValue &cb ) { mOnLoad = cb; }

    QJSValue onloadend() const { return mOnLoadEnd; }
    void setOnloadend( const QJSValue &cb ) { mOnLoadEnd = cb; }

  signals:
    void readyStateChanged();
    void responseChanged();
    void timeoutChanged();
    void autoDeleteChanged();

  private:
    bool isOpen() const;
    void setReadyState( ReadyState state );
    void clearResponse();
    void resetForNewRequest();

    // Disconnects signals and disposes the current reply.
    //
    // This method must never call QML callbacks.
    void cleanupReply( bool abortNetwork );

    void startRequest( const QVariant &body );
    void hookReply( QNetworkReply *networkReply );
    void finalizeFromRawReply();
    void finalizeAsError( const QString &msg, bool emitResponse = true );

    QByteArray bodyToBytes( const QVariant &body, QString *outContentType = nullptr ) const;
    bool bodyContainsFileUrls( const QVariant &body ) const;
    QHttpMultiPart *buildMultipart( const QVariant &body ) const;
    bool isAllowedLocalUploadPath( const QString &localPath ) const;

    void call( const QJSValue &cb, const QJSValueList &args = {} );
    static bool containsNewlines( const QString &stringValue );

    void collectResponseHeaders( QNetworkReply *networkReply );
    static QString normalizeHeaderName( const QString &stringValue );

    void maybeAutoDelete();

  private:
    QPointer<QNetworkReply> mReply;
    QList<QMetaObject::Connection> mConnections;

    QNetworkRequest mRequest;
    QByteArray mMethod;
    QUrl mUrl;

    ReadyState mReadyState = Unsent;

    int mTimeoutMs = 0;
    bool mAutoDelete = false;

    bool mFinalized = false;
    bool mTimedOut = false;
    bool mAborted = false;

    // Increments on each send() call. Used to prevent stale timeout timers from aborting newer requests.
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
