/***************************************************************************
qfieldxmlhttprequest.h - QFIELDXMLHTTPREQUEST

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

class NetworkReply;
class QHttpMultiPart;
class QNetworkReply;

/**
 * \brief QML-friendly XMLHttpRequest using QField's NetworkManager/NetworkReply.
 *
 * This class provides a small subset of the browser XMLHttpRequest (XHR) workflow,
 * but implemented on top of QField's networking stack:
 *
 *   - open(method, url)
 *   - setRequestHeader(name, value)
 *   - send(body)
 *   - abort()
 *
 * Observability:
 *   - readyState/status/statusText/responseText/response/responseType/responseUrl
 *   - callback properties: onreadystatechange, ondownloadprogress, onuploadprogress,
 *     onredirected, ontimeout, onaborted, onerror, onload, onloadend
 *
 * Payload handling:
 *   - QByteArray: sent as-is
 *   - QString: UTF-8 text payload (Content-Type inferred if not set)
 *   - QVariantMap/QVariantList: encoded as JSON payload (Content-Type inferred if not set)
 *   - Multipart upload:
 *       - enabled if body contains explicit "file://..." URLs OR Content-Type starts with "multipart/form-data"
 *
 * Security/safety:
 *   - local file uploads are intentionally restricted to:
 *       - current project directory
 *       - QFieldCloud local directory
 *
 * Notes:
 *   - The implementation aims to be minimal and predictable for QML usage, not a full browser XHR
 */
class QFieldXmlHttpRequest : public QObject
{
    Q_OBJECT

    Q_PROPERTY( ReadyState readyState READ readyState NOTIFY readyStateChanged )
    Q_PROPERTY( int status READ status NOTIFY responseChanged )
    Q_PROPERTY( QString statusText READ statusText NOTIFY responseChanged )

    // Response body decoded as UTF-8 text
    Q_PROPERTY( QString responseText READ responseText NOTIFY responseChanged )
    // Parsed response if JSON, otherwise equals responseText
    Q_PROPERTY( QVariant response READ response NOTIFY responseChanged )
    Q_PROPERTY( QString responseType READ responseType NOTIFY responseChanged )
    Q_PROPERTY( QUrl responseUrl READ responseUrl NOTIFY responseChanged )

    /**
   * \brief Timeout in milliseconds. 0 disables
   *    * Timeout is implemented as an external timer that aborts the request if it is still running
   */
    Q_PROPERTY( int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged )

    Q_PROPERTY( bool autoDelete READ autoDelete WRITE setAutoDelete NOTIFY autoDeleteChanged )

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
      Unsent = 0,      // open() has not been called or open() args were invalid
      Opened,          //open() called successfully; request is prepared
      HeadersReceived, //headers are available (metaDataChanged)
      Loading,         //body is being received (readyRead)
      Done             //request finished (success or failure)
    };
    Q_ENUM( ReadyState )

    explicit QFieldXmlHttpRequest( QObject *parent = nullptr );
    ~QFieldXmlHttpRequest() override;

    Q_INVOKABLE static QFieldXmlHttpRequest *newRequest( QObject *parent = nullptr );

    /**
   * \brief Prepares the request. Must be called before send().
   *    * behavior: calling open() while a request is active will abort it.
   *    * \param method HTTP method (GET/POST/PUT/DELETE supported)
   * \param url Request URL
   */
    Q_INVOKABLE void open( const QString &method, const QUrl &url );

    /**
   * \brief Sets a raw HTTP request header.
   *    * Only effective after open() and before send().
   * Basic newline protection is applied to avoid CR/LF header injection.
   */
    Q_INVOKABLE void setRequestHeader( const QString &headerName, const QString &headerValue );

    /**
   * \brief Starts the request.
   *    * Body can be:
   *  - QByteArray (sent as-is)
   *  - QString (UTF-8 text)
   *  - QVariantMap / QVariantList (JSON encoding)
   *  - multipart: QVariantMap containing "file://..." strings (restricted local paths)
   */
    Q_INVOKABLE void send( const QVariant &body = QVariant() );

    /**
   * \brief Aborts the current request (if any).
   *    * This will trigger onaborted (if set) and finalize the request state as Done.
   */
    Q_INVOKABLE void abort();

    /**
   * \brief XHR-like response header accessors (final response only).
   */
    Q_INVOKABLE QString getResponseHeader( const QString &name ) const;
    Q_INVOKABLE QString getAllResponseHeaders() const;

    ReadyState readyState() const { return mReadyState; }

    int status() const { return mStatus; }
    QString statusText() const { return mStatusText; }

    QString responseText() const { return mResponseText; }
    QVariant response() const { return mResponse; }
    QString responseType() const { return mResponseType; }
    QUrl responseUrl() const { return mResponseUrl; }

    /**
   * \brief Sets request timeout in ms. 0 disables.
   */
    void setTimeout( int ms );
    int timeout() const { return mTimeoutMs; }

    bool autoDelete() const { return mAutoDelete; }
    void setAutoDelete( bool v );

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
    // Returns true if open() was called with valid method+URL.
    bool isOpen() const;

    // Transitions readyState and emits onreadystatechange.
    void setReadyState( ReadyState state );

    // Clears response-related members (status/text/body/type/url).
    void clearResponse();

    // Resets per-request flags (finalized/aborted/timedOut) and clears response.
    void resetForNewRequest();

    /**
   * \brief Disconnects signals and disposes current reply objects.
   * \param abortNetwork If true, aborts the network operation before deleting the reply.
   *    * This method is used both for resend and destruction. It must never call JS callbacks.
   */
    void cleanupReply( bool abortNetwork );

    // Creates and starts the underlying NetworkReply based on method/body.
    void startRequest( const QVariant &body );

    // Hooks high-level NetworkReply signals (progress/errors/finished/raw reply changes).
    void hookReply( NetworkReply *reply );

    // Hooks current raw QNetworkReply to derive readyState transitions.
    void hookRawReply( QNetworkReply *rawReply );

    // Finalizes response members from raw reply data.
    void finalizeFromRawReply();

    // Finalizes as an error message payload (stored in responseText/response).
    void finalizeAsError( const QString &msg, bool emitResponse = true );

    // Serializes body into raw bytes (with optional inferred Content-Type).
    QByteArray bodyToBytes( const QVariant &body, QString *outContentType = nullptr ) const;

    // Detects explicit "file://..." strings to decide multipart behavior.
    bool bodyContainsFileUrls( const QVariant &body ) const;

    // Builds multipart/form-data body; returns nullptr on invalid structure.
    QHttpMultiPart *buildMultipart( const QVariant &body ) const;

    // Returns true if a local file path is allowed to be uploaded.
    bool isAllowedLocalUploadPath( const QString &localPath ) const;

    // Calls a QML callback safely
    void call( const QJSValue &cb, const QJSValueList &args = {} );

    // Basic newline detection helper for header protection.
    static bool containsNewlines( const QString &s );

    // Collect final response headers from raw reply
    void collectResponseHeaders( QNetworkReply *rawReply );
    static QString normalizeHeaderName( const QString &s );

    // Schedules deleteLater() if autoDelete is enabled
    void maybeAutoDelete();

  private:
    QPointer<NetworkReply> mReply;
    QPointer<QNetworkReply> mRawReply;
    QList<QMetaObject::Connection> mRawConnections;

    QNetworkRequest mRequest;
    QByteArray mMethod;
    QUrl mUrl;

    ReadyState mReadyState = Unsent;

    int mTimeoutMs = 0;
    bool mAutoDelete = false;

    bool mFinalized = false;
    bool mTimedOut = false;
    bool mAborted = false;

    /**
   * \brief Increments on each send() call.
   *    * Used to prevent stale timeout singleShot timers from aborting newer requests.
   */
    quint64 mRequestSerial = 0;

    int mStatus = 0;
    QString mStatusText;
    QVariant mResponse;
    QString mResponseText;
    QString mResponseType;
    QUrl mResponseUrl;

    // Final response headers
    QMap<QString, QString> mResponseHeaders;

    // Track last Qt network error (for status==0 network failures)
    QNetworkReply::NetworkError mLastError = QNetworkReply::NoError;
    QString mLastErrorString;

    // Callback properties
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
