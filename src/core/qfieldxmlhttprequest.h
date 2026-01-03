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

/**
 * This class exposes a lightweight, XMLHttpRequest-like API to QML for making HTTP requests in a predictable way.
 * It follows the familiar open(), setRequestHeader(), send(), and abort() workflow while exposing request state,
 * response data, and lifecycle callbacks as QML properties.
 *
 * Payload encoding is deterministic. QByteArray payloads are transmitted as-is. QString payloads are sent as UTF-8
 * text. QVariantMap and QVariantList payloads are encoded as JSON. When the caller does not explicitly set a
 * Content-Type header, an appropriate Content-Type is inferred from the payload type.
 *
 * Multipart uploads are supported when the body contains "file://..." URL strings, or when the caller sets a
 * Content-Type header starting with "multipart/form-data". Local file uploads are restricted to paths inside the
 * current project directory or inside the QFieldCloud local directory.
 *
 * Requests created for QML usage are expected to be short-lived. The object schedules deleteLater() automatically
 * once the request lifecycle ends (success, error, abort, or timeout).
 *
 * \note This is not a full browser XMLHttpRequest implementation. It focuses on a clean QML API and predictable
 * behavior within QField.
 *
 * \ingroup core
 */
class QFieldXmlHttpRequest : public QObject
{
    Q_OBJECT

    /**
     * The current request state aligned with browser XMLHttpRequest readyState values.
     */
    Q_PROPERTY( ReadyState readyState READ readyState NOTIFY readyStateChanged )

    /**
     * The HTTP status code of the final response. For network-level failures, this is 0.
     */
    Q_PROPERTY( int status READ status NOTIFY responseChanged )

    /**
     * The HTTP reason phrase of the final response when available.
     */
    Q_PROPERTY( QString statusText READ statusText NOTIFY responseChanged )

    /**
     * The response body decoded as UTF-8 text.
     */
    Q_PROPERTY( QString responseText READ responseText NOTIFY responseChanged )

    /**
     * The parsed response value when the response is JSON, otherwise this equals responseText.
     */
    Q_PROPERTY( QVariant response READ response NOTIFY responseChanged )

    /**
     * The response Content-Type value as provided by the server.
     */
    Q_PROPERTY( QString responseType READ responseType NOTIFY responseChanged )

    /**
     * The final response URL. This may differ from the requested URL when redirects were followed.
     */
    Q_PROPERTY( QUrl responseUrl READ responseUrl NOTIFY responseChanged )

    /**
     * The request timeout in milliseconds. A value of 0 disables the timeout.
     *
     * When the timeout triggers, the request is aborted and ontimeout and onloadend are called.
     */
    Q_PROPERTY( int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged )

    /**
     * Called whenever readyState changes.
     */
    Q_PROPERTY( QJSValue onreadystatechange READ onreadystatechange WRITE setOnreadystatechange )

    /**
     * Called during download progress as (receivedBytes, totalBytes).
     */
    Q_PROPERTY( QJSValue ondownloadprogress READ ondownloadprogress WRITE setOndownloadprogress )

    /**
     * Called during upload progress as (sentBytes, totalBytes).
     */
    Q_PROPERTY( QJSValue onuploadprogress READ onuploadprogress WRITE setOnuploadprogress )

    /**
     * Called when a redirect is reported by Qt as (urlString).
     */
    Q_PROPERTY( QJSValue onredirected READ onredirected WRITE setOnredirected )

    /**
     * Called when the timeout triggers.
     */
    Q_PROPERTY( QJSValue ontimeout READ ontimeout WRITE setOntimeout )

    /**
     * Called when the request is aborted.
     */
    Q_PROPERTY( QJSValue onaborted READ onaborted WRITE setOnaborted )

    /**
     * Called for network-level failures as (errorCode, errorMessage).
     */
    Q_PROPERTY( QJSValue onerror READ onerror WRITE setOnerror )

    /**
     * Called when the request completes and a status code is available.
     *
     * \note HTTP errors such as 404 or 500 still trigger onload. Callers should inspect status.
     */
    Q_PROPERTY( QJSValue onload READ onload WRITE setOnload )

    /**
     * Called at the end of the request lifecycle for success, error, abort, or timeout.
     */
    Q_PROPERTY( QJSValue onloadend READ onloadend WRITE setOnloadend )

  public:
    /**
     * ReadyState values aligned with browser XMLHttpRequest.
     */
    enum ReadyState : uchar
    {
      Unsent = 0,      //!< open() has not been called or open() args were invalid
      Opened,          //!< open() called successfully; request is prepared
      HeadersReceived, //!< headers are available (metaDataChanged)
      Loading,         //!< body is being received (readyRead)
      Done             //!< request finished (success or failure)
    };
    Q_ENUM( ReadyState )

    explicit QFieldXmlHttpRequest( QObject *parent = nullptr );
    ~QFieldXmlHttpRequest() override;

    /**
     * Creates a new request instance.
     */
    Q_INVOKABLE static QFieldXmlHttpRequest *newRequest( QObject *parent = nullptr );

    /**
     * Prepares the request. This must be called before send().
     *
     * Calling open() while a request is active aborts the active request. Supported methods are GET, POST,
     * PUT, and DELETE.
     */
    Q_INVOKABLE void open( const QString &method, const QUrl &url );

    /**
     * Sets a raw HTTP request header.
     *
     * Headers are only applied after open() and before send(). Newline protection is applied to prevent CR/LF
     * header injection.
     */
    Q_INVOKABLE void setRequestHeader( const QString &headerName, const QString &headerValue );

    /**
     * Starts the request.
     *
     * The body can be provided as QByteArray, QString, QVariantMap, or QVariantList. QByteArray is sent as-is,
     * QString is encoded as UTF-8 text, and QVariantMap/QVariantList are JSON-encoded. If multipart upload is
     * desired, the caller can pass a QVariantMap containing "file://..." string values, or can set a Content-Type
     * header starting with "multipart/form-data". Local file URLs are accepted only when they resolve inside the
     * current project directory or the QFieldCloud local directory.
     */
    Q_INVOKABLE void send( const QVariant &body = QVariant() );

    /**
     * Aborts the current request if one is active.
     *
     * \note This triggers onaborted and onloadend if set and transitions readyState to Done.
     */
    Q_INVOKABLE void abort();

    /**
     * Returns a response header value by name. This is available only after the request finishes.
     */
    Q_INVOKABLE QString getResponseHeader( const QString &name ) const;

    /**
     * Returns all response headers in HTTP-style format. This is available only after the request finishes.
     */
    Q_INVOKABLE QString getAllResponseHeaders() const;

    //! \copydoc QFieldXmlHttpRequest::readyState
    ReadyState readyState() const { return mReadyState; }

    //! \copydoc QFieldXmlHttpRequest::status
    int status() const { return mStatus; }

    //! \copydoc QFieldXmlHttpRequest::statusText
    QString statusText() const { return mStatusText; }

    //! \copydoc QFieldXmlHttpRequest::responseText
    QString responseText() const { return mResponseText; }

    //! \copydoc QFieldXmlHttpRequest::response
    QVariant response() const { return mResponse; }

    //! \copydoc QFieldXmlHttpRequest::responseType
    QString responseType() const { return mResponseType; }

    //! \copydoc QFieldXmlHttpRequest::responseUrl
    QUrl responseUrl() const { return mResponseUrl; }

    //! \copydoc QFieldXmlHttpRequest::timeout
    void setTimeout( int milliseconds );

    //! \copydoc QFieldXmlHttpRequest::timeout
    int timeout() const { return mTimeoutMs; }

    //! \copydoc QFieldXmlHttpRequest::onreadystatechange
    QJSValue onreadystatechange() const { return mOnReadyStateChanged; }

    //! \copydoc QFieldXmlHttpRequest::onreadystatechange
    void setOnreadystatechange( const QJSValue &callback ) { mOnReadyStateChanged = callback; }

    //! \copydoc QFieldXmlHttpRequest::ondownloadprogress
    QJSValue ondownloadprogress() const { return mOnDownloadProgress; }

    //! \copydoc QFieldXmlHttpRequest::ondownloadprogress
    void setOndownloadprogress( const QJSValue &callback ) { mOnDownloadProgress = callback; }

    //! \copydoc QFieldXmlHttpRequest::onuploadprogress
    QJSValue onuploadprogress() const { return mOnUploadProgress; }

    //! \copydoc QFieldXmlHttpRequest::onuploadprogress
    void setOnuploadprogress( const QJSValue &callback ) { mOnUploadProgress = callback; }

    //! \copydoc QFieldXmlHttpRequest::onredirected
    QJSValue onredirected() const { return mOnRedirected; }

    //! \copydoc QFieldXmlHttpRequest::onredirected
    void setOnredirected( const QJSValue &callback ) { mOnRedirected = callback; }

    //! \copydoc QFieldXmlHttpRequest::ontimeout
    QJSValue ontimeout() const { return mOnTimeout; }

    //! \copydoc QFieldXmlHttpRequest::ontimeout
    void setOntimeout( const QJSValue &callback ) { mOnTimeout = callback; }

    //! \copydoc QFieldXmlHttpRequest::onaborted
    QJSValue onaborted() const { return mOnAborted; }

    //! \copydoc QFieldXmlHttpRequest::onaborted
    void setOnaborted( const QJSValue &callback ) { mOnAborted = callback; }

    //! \copydoc QFieldXmlHttpRequest::onerror
    QJSValue onerror() const { return mOnError; }

    //! \copydoc QFieldXmlHttpRequest::onerror
    void setOnerror( const QJSValue &callback ) { mOnError = callback; }

    //! \copydoc QFieldXmlHttpRequest::onload
    QJSValue onload() const { return mOnLoad; }

    //! \copydoc QFieldXmlHttpRequest::onload
    void setOnload( const QJSValue &callback ) { mOnLoad = callback; }

    //! \copydoc QFieldXmlHttpRequest::onloadend
    QJSValue onloadend() const { return mOnLoadEnd; }

    //! \copydoc QFieldXmlHttpRequest::onloadend
    void setOnloadend( const QJSValue &callback ) { mOnLoadEnd = callback; }

  signals:
    //! \copydoc QFieldXmlHttpRequest::readyState
    void readyStateChanged();

    //! Emitted whenever response-related properties change.
    void responseChanged();

    //! \copydoc QFieldXmlHttpRequest::timeout
    void timeoutChanged();

  private:
    bool isOpen() const;
    void setReadyState( ReadyState state );
    void clearResponse();
    void resetForNewRequest();

    /**
     * Disconnects signals and disposes the current reply.
     *
     * This method must never call QML callbacks.
     */
    void cleanupReply( bool abortNetwork );

    void startRequest( const QVariant &body );
    void hookReply( QNetworkReply *networkReply );
    void finalizeFromRawReply();
    void finalizeAsError( const QString &message, bool shouldEmitResponseChanged = true );

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
