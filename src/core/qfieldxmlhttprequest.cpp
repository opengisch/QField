/***************************************************************************
qfieldxmlhttprequest.cpp - XMLHTTPREQUEST

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

#include "qfieldxmlhttprequest.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QNetworkReply>
#include <QTimer>
#include <qfieldcloudutils.h>

#include <fileutils.h>
#include <networkmanager.h>
#include <networkreply.h>
#include <urlutils.h>

QFieldXmlHttpRequest::QFieldXmlHttpRequest( QObject *parent )
  : QObject( parent )
{
}

QFieldXmlHttpRequest::~QFieldXmlHttpRequest()
{
  // Destructor must be silent (no QML callbacks / no signals).
  cleanupReply( /*abortNetwork=*/true );
}

void QFieldXmlHttpRequest::setTimeout( int ms )
{
  mTimeoutMs = ms > 0 ? ms : 0;
}

bool QFieldXmlHttpRequest::containsNewlines( const QString &s )
{
  return s.contains( QLatin1Char( '\n' ) ) || s.contains( QLatin1Char( '\r' ) );
}

bool QFieldXmlHttpRequest::isOpen() const
{
  return !mMethod.isEmpty() && mUrl.isValid();
}

void QFieldXmlHttpRequest::clearResponse()
{
  mStatus = 0;
  mStatusText.clear();
  mResponse = {};
  mResponseText.clear();
  mResponseType.clear();
  mResponseUrl.clear();
}

void QFieldXmlHttpRequest::resetForNewRequest()
{
  clearResponse();
  mFinalized = false;
  mTimedOut = false;
  mAborted = false;
}

void QFieldXmlHttpRequest::cleanupReply( bool abortNetwork )
{
  // Raw reply hooks can outlive the wrapper when redirects/retries occur,
  // so disconnect them first to avoid late signals touching freed state.
  for ( const auto &c : std::as_const( mRawConnections ) )
    disconnect( c );
  mRawConnections.clear();

  if ( mRawReply )
  {
    disconnect( mRawReply, nullptr, this, nullptr );
    mRawReply = nullptr;
  }

  if ( mReply )
  {
    disconnect( mReply, nullptr, this, nullptr );

    if ( abortNetwork )
      mReply->abort();

    mReply->deleteLater();
    mReply = nullptr;
  }
}

void QFieldXmlHttpRequest::setReadyState( ReadyState state )
{
  if ( mReadyState == state )
    return;

  mReadyState = state;
  emit readyStateChanged();
  call( mOnReadyStateChanged );
}

void QFieldXmlHttpRequest::open( const QString &method, const QUrl &url )
{
  // open() cancels any in-flight request
  if ( mReply )
    abort();

  const QByteArray m = method.trimmed().toUtf8();
  if ( m.isEmpty() || !url.isValid() )
  {
    qWarning() << "QFieldXmlHttpRequest: invalid open() args:" << method << url;
    mMethod.clear();
    mUrl = QUrl();
    mRequest = QNetworkRequest();
    resetForNewRequest();
    setReadyState( Unsent );
    return;
  }

  mMethod = m;
  mUrl = url;
  mRequest = QNetworkRequest( mUrl );

  resetForNewRequest();
  setReadyState( Opened );
}

void QFieldXmlHttpRequest::setRequestHeader( const QString &headerName, const QString &headerValue )
{
  if ( !isOpen() || mReply )
    return;

  const QString h = headerName.trimmed();
  if ( h.isEmpty() )
    return;

  // Avoid CR/LF injection in raw headers. This is a minimal guard (not a full validator)
  if ( containsNewlines( h ) || containsNewlines( headerValue ) )
  {
    qWarning() << "QFieldXmlHttpRequest: blocked header with newline:" << headerName;
    return;
  }

  mRequest.setRawHeader( h.toUtf8(), headerValue.toUtf8() );
}

void QFieldXmlHttpRequest::send( const QVariant &body )
{
  if ( !isOpen() )
  {
    qWarning() << "QFieldXmlHttpRequest: call open() before send()";
    return;
  }

  // "Resend" should not behave like abort(): it should not produce an error callback chain
  if ( mReply )
    cleanupReply( /*abortNetwork=*/true );

  if ( mReadyState != Opened )
    setReadyState( Opened );

  resetForNewRequest();
  ++mRequestSerial; // separates this request from any previous timeout timers

  startRequest( body );
}

void QFieldXmlHttpRequest::startRequest( const QVariant &body )
{
  const QByteArray upper = mMethod.trimmed().toUpper();

  const bool isGet = ( upper == "GET" );
  const bool isPost = ( upper == "POST" );
  const bool isPut = ( upper == "PUT" );
  const bool isDelete = ( upper == "DELETE" );

  if ( !isGet && !isPost && !isPut && !isDelete )
  {
    qWarning() << "QFieldXmlHttpRequest: unsupported method:" << upper;
    finalizeAsError( QStringLiteral( "{\"detail\":\"Unsupported HTTP method\"}" ) );
    return;
  }

  const QString ct = mRequest.header( QNetworkRequest::ContentTypeHeader ).toString();
  const bool forceMultipart = ct.startsWith( QStringLiteral( "multipart/form-data" ), Qt::CaseInsensitive );
  const bool wantsMultipart = forceMultipart || bodyContainsFileUrls( body );

  if ( isGet )
  {
    mReply = NetworkManager::get( mRequest );
  }
  else if ( isDelete )
  {
    QByteArray payload;
    if ( body.isValid() && !body.isNull() )
      payload = bodyToBytes( body );

    mReply = NetworkManager::deleteResource( mRequest, payload );
  }
  else
  {
    if ( wantsMultipart )
    {
      // Important: if caller set "multipart/form-data" manually, it likely misses the required boundary.
      // Let QHttpMultiPart/QNetworkAccessManager generate the final Content-Type with boundary.
      if ( forceMultipart )
        mRequest.setHeader( QNetworkRequest::ContentTypeHeader, QVariant() );

      QHttpMultiPart *mp = buildMultipart( body );
      if ( !mp )
      {
        finalizeAsError( QStringLiteral( "{\"detail\":\"Invalid multipart body\"}" ) );
        return;
      }

      mReply = isPost ? NetworkManager::post( mRequest, mp )
                      : NetworkManager::put( mRequest, mp );

      // Keep multipart + devices alive for the duration of the request.
      if ( mReply )
        mp->setParent( mReply );
      else
        mp->deleteLater();
    }
    else
    {
      QString inferredContentType;
      QByteArray payload;

      if ( body.isValid() && !body.isNull() )
        payload = bodyToBytes( body, &inferredContentType );

      // Only infer Content-Type if caller didn't set one.
      if ( mRequest.header( QNetworkRequest::ContentTypeHeader ).isNull() && !inferredContentType.isEmpty() )
        mRequest.setHeader( QNetworkRequest::ContentTypeHeader, inferredContentType );

      mReply = isPost ? NetworkManager::post( mRequest, payload )
                      : NetworkManager::put( mRequest, payload );
    }
  }

  if ( !mReply )
  {
    finalizeAsError( QStringLiteral( "{\"detail\":\"Failed to start request\"}" ) );
    return;
  }

  hookReply( mReply );
}

void QFieldXmlHttpRequest::abort()
{
  if ( !mReply )
    return;

  mAborted = true;
  mReply->abort();

  call( mOnAborted );
  finalizeAsError( QStringLiteral( "{\"detail\":\"Operation aborted\"}" ) );
}

void QFieldXmlHttpRequest::hookReply( NetworkReply *reply )
{
  // During redirects/retries, NetworkReply may swap out the underlying QNetworkReply instance.
  connect( reply, &NetworkReply::currentRawReplyChanged, this, [this]() {
    if ( !mReply )
      return;
    hookRawReply( mReply->currentRawReply() );
  } );

  connect( reply, &NetworkReply::downloadProgress, this, [this]( qint64 received, qint64 total ) {
    call( mOnDownloadProgress, { double( received ), double( total ) } );
  } );

  connect( reply, &NetworkReply::uploadProgress, this, [this]( qint64 sent, qint64 total ) {
    call( mOnUploadProgress, { double( sent ), double( total ) } );
  } );

  connect( reply, &NetworkReply::redirected, this, [this]( const QUrl &u ) {
    call( mOnRedirected, { u.toString() } );
  } );

  connect( reply, &NetworkReply::errorOccurred, this, [this]( QNetworkReply::NetworkError err ) {
    if ( mFinalized )
      return;

    // Timeout and cancel are handled as special events to match QML expectations.
    if ( err == QNetworkReply::TimeoutError )
    {
      if ( !mTimedOut )
      {
        mTimedOut = true;
        call( mOnTimeout );
      }
      return;
    }

    if ( err == QNetworkReply::OperationCanceledError )
    {
      if ( !mAborted )
      {
        mAborted = true;
        call( mOnAborted );
      }
      return;
    }

    const QString msg = ( mReply && mReply->currentRawReply() )
                          ? mReply->currentRawReply()->errorString()
                          : QString();
    call( mOnError, { int( err ), msg } );
  } );

  connect( reply, &NetworkReply::finished, this, [this]() {
    finalizeFromRawReply();
  } );

  // Hook current raw reply immediately so readyState transitions work even without redirects.
  hookRawReply( reply->currentRawReply() );

  if ( mTimeoutMs > 0 )
  {
    const quint64 serial = mRequestSerial;
    QTimer::singleShot( mTimeoutMs, this, [this, serial]() {
      // Prevent stale singleShot timers from aborting newer requests.
      if ( serial != mRequestSerial )
        return;

      if ( !mReply || mFinalized || mReply->isFinished() )
        return;

      mTimedOut = true;
      mReply->abort();
      call( mOnTimeout );
      finalizeAsError( QStringLiteral( "{\"detail\":\"Operation timed out\"}" ) );
    } );
  }
}

void QFieldXmlHttpRequest::hookRawReply( QNetworkReply *rawReply )
{
  if ( mRawReply == rawReply )
    return;

  for ( const auto &c : std::as_const( mRawConnections ) )
    disconnect( c );
  mRawConnections.clear();

  mRawReply = rawReply;
  if ( !mRawReply )
    return;

  // metaDataChanged -> headers are ready.
  mRawConnections << connect( mRawReply, &QNetworkReply::metaDataChanged, this, [this]() {
    if ( mReadyState < HeadersReceived )
      setReadyState( HeadersReceived );
  } );

  // readyRead -> body data is available.
  mRawConnections << connect( mRawReply, &QNetworkReply::readyRead, this, [this]() {
    if ( mReadyState < HeadersReceived )
      setReadyState( HeadersReceived );
    if ( mReadyState < Loading )
      setReadyState( Loading );
  } );
}

void QFieldXmlHttpRequest::finalizeFromRawReply()
{
  if ( mFinalized )
    return;

  mFinalized = true;

  QNetworkReply *raw = mReply ? mReply->currentRawReply() : nullptr;

  if ( raw )
  {
    mStatus = raw->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
    mStatusText = raw->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();

    mResponseUrl = raw->url();
    mResponseType = raw->header( QNetworkRequest::ContentTypeHeader ).toString();

    // Note: readAll() buffers the entire body in memory. This is acceptable for our intended usage.
    const QByteArray bytes = raw->readAll();
    mResponseText = QString::fromUtf8( bytes );

    // If response looks like JSON, parse to QVariantMap/QVariantList for convenience in QML.
    const QString lowerCt = mResponseType.toLower();
    if ( lowerCt.contains( QStringLiteral( "application/json" ) ) || lowerCt.contains( QStringLiteral( "+json" ) ) )
    {
      QJsonParseError pe;
      const QJsonDocument doc = QJsonDocument::fromJson( bytes, &pe );
      if ( pe.error == QJsonParseError::NoError )
      {
        if ( doc.isObject() )
          mResponse = doc.object().toVariantMap();
        else if ( doc.isArray() )
          mResponse = doc.array().toVariantList();
        else
          mResponse = mResponseText;
      }
      else
      {
        // Invalid JSON payload - keep text response.
        mResponse = mResponseText;
      }
    }
    else
    {
      mResponse = mResponseText;
    }
  }
  else
  {
    if ( mResponseText.isEmpty() )
      mResponseText = QStringLiteral( "{\"detail\":\"No response\"}" );
    mResponse = mResponseText;
  }

  cleanupReply( /*abortNetwork=*/false );

  setReadyState( Done );
  emit responseChanged();
}

void QFieldXmlHttpRequest::finalizeAsError( const QString &msg, bool emitResponse )
{
  if ( mFinalized )
    return;

  clearResponse();
  mResponseText = msg;
  mResponse = mResponseText;

  mFinalized = true;

  cleanupReply( /*abortNetwork=*/false );

  setReadyState( Done );
  if ( emitResponse )
    emit responseChanged();
}

QByteArray QFieldXmlHttpRequest::bodyToBytes( const QVariant &body, QString *outContentType ) const
{
  if ( outContentType )
    outContentType->clear();

  if ( body.typeId() == QMetaType::QByteArray )
    return body.toByteArray();

  if ( body.typeId() == QMetaType::QString )
  {
    if ( outContentType )
      *outContentType = QStringLiteral( "text/plain; charset=utf-8" );
    return body.toString().toUtf8();
  }

  // JSON-encode other variants (map/list/primitive wrapped).
  const QJsonValue jv = QJsonValue::fromVariant( body );
  QJsonDocument doc = jv.isObject()  ? QJsonDocument( jv.toObject() )
                      : jv.isArray() ? QJsonDocument( jv.toArray() )
                                     : QJsonDocument( QJsonObject { { QStringLiteral( "value" ), jv } } );

  if ( outContentType )
    *outContentType = QStringLiteral( "application/json" );

  return doc.toJson( QJsonDocument::Compact );
}

bool QFieldXmlHttpRequest::bodyContainsFileUrls( const QVariant &body ) const
{
  auto isFileUrlString = []( const QString &s ) -> bool {
    return s.trimmed().startsWith( QStringLiteral( "file://" ), Qt::CaseInsensitive );
  };

  if ( body.typeId() == QMetaType::QString )
    return isFileUrlString( body.toString() );

  if ( body.canConvert<QVariantList>() )
  {
    const QVariantList lst = body.toList();
    for ( const QVariant &v : lst )
      if ( v.typeId() == QMetaType::QString && isFileUrlString( v.toString() ) )
        return true;
  }

  if ( body.canConvert<QVariantMap>() )
  {
    const QVariantMap m = body.toMap();
    for ( auto it = m.constBegin(); it != m.constEnd(); ++it )
    {
      const QVariant v = it.value();
      if ( v.typeId() == QMetaType::QString && isFileUrlString( v.toString() ) )
        return true;

      if ( v.canConvert<QVariantList>() )
      {
        const QVariantList lst = v.toList();
        for ( const QVariant &lv : lst )
          if ( lv.typeId() == QMetaType::QString && isFileUrlString( lv.toString() ) )
            return true;
      }
    }
  }

  return false;
}

bool QFieldXmlHttpRequest::isAllowedLocalUploadPath( const QString &localPath ) const
{
  if ( localPath.isEmpty() )
    return false;

  // Canonical path prevents "../" tricks and resolves symlinks for safer containment checks.
  const QString canonical = QFileInfo( localPath ).canonicalFilePath();
  if ( canonical.isEmpty() )
    return false;

  if ( FileUtils::isWithinProjectDirectory( canonical ) )
    return true;

  const QString cloudRoot = QFieldCloudUtils::localCloudDirectory();
  if ( !cloudRoot.isEmpty() )
  {
    const QString cloudCanonical = QFileInfo( cloudRoot ).canonicalFilePath();
    if ( !cloudCanonical.isEmpty() )
    {
      const QString prefix = QDir::cleanPath( cloudCanonical ) + QDir::separator();
      if ( QDir::cleanPath( canonical ).startsWith( prefix ) )
        return true;
    }
  }

  return false;
}

QHttpMultiPart *QFieldXmlHttpRequest::buildMultipart( const QVariant &body ) const
{
  if ( !body.canConvert<QVariantMap>() )
    return nullptr;

  const QVariantMap m = body.toMap();
  auto *mp = new QHttpMultiPart( QHttpMultiPart::FormDataType );

  auto appendTextPart = [mp]( const QString &name, const QString &value ) {
    if ( name.isEmpty() )
      return;

    QHttpPart part;
    part.setHeader( QNetworkRequest::ContentDispositionHeader,
                    QStringLiteral( "form-data; name=\"%1\"" ).arg( name ) );
    part.setBody( value.toUtf8() );
    mp->append( part );
  };

  auto appendFilePart = [this, mp]( const QString &name, const QUrl &fileUrl ) -> bool {
    if ( name.isEmpty() || !fileUrl.isValid() || !fileUrl.isLocalFile() )
      return false;

    const QString path = fileUrl.toLocalFile();
    if ( !isAllowedLocalUploadPath( path ) )
    {
      qWarning() << "QFieldXmlHttpRequest: blocked file upload path:" << path;
      return false;
    }

    auto *file = new QFile( path );
    if ( !file->exists() || !file->open( QFile::ReadOnly ) )
    {
      qWarning() << "QFieldXmlHttpRequest: cannot open file:" << path;
      delete file;
      return false;
    }

    const QString mime = FileUtils::mimeTypeName( path );

    QHttpPart part;
    part.setHeader( QNetworkRequest::ContentTypeHeader, mime );
    part.setHeader( QNetworkRequest::ContentDispositionHeader,
                    QStringLiteral( "form-data; name=\"%1\"; filename=\"%2\"" )
                      .arg( name, QFileInfo( path ).fileName() ) );
    part.setBodyDevice( file );

    // mp owns the QFile via QObject parenting (ensures device lifetime while upload runs).
    file->setParent( mp );
    mp->append( part );
    return true;
  };

  for ( auto it = m.constBegin(); it != m.constEnd(); ++it )
  {
    const QString key = it.key();
    const QVariant val = it.value();

    if ( val.canConvert<QVariantList>() )
    {
      const QVariantList lst = val.toList();
      for ( const QVariant &v : lst )
      {
        if ( v.typeId() == QMetaType::QString )
        {
          const QString s = v.toString();
          if ( s.trimmed().startsWith( QStringLiteral( "file://" ), Qt::CaseInsensitive ) )
          {
            const QUrl u = UrlUtils::fromString( s );
            if ( u.isLocalFile() && appendFilePart( key, u ) )
              continue;
          }
        }
        appendTextPart( key, QJsonValue::fromVariant( v ).toVariant().toString() );
      }
      continue;
    }

    if ( val.typeId() == QMetaType::QString )
    {
      const QString s = val.toString();
      if ( s.trimmed().startsWith( QStringLiteral( "file://" ), Qt::CaseInsensitive ) )
      {
        const QUrl u = UrlUtils::fromString( s );
        if ( appendFilePart( key, u ) )
          continue;
      }
    }

    const QJsonValue jv = QJsonValue::fromVariant( val );
    if ( jv.isObject() || jv.isArray() )
    {
      const QJsonDocument doc = jv.isObject() ? QJsonDocument( jv.toObject() ) : QJsonDocument( jv.toArray() );
      appendTextPart( key, QString::fromUtf8( doc.toJson( QJsonDocument::Compact ) ) );
      continue;
    }

    appendTextPart( key, jv.toVariant().toString() );
  }

  return mp;
}

void QFieldXmlHttpRequest::call( QJSValue cb, const QJSValueList &args )
{
  if ( !cb.isCallable() )
    return;

  const QJSValue r = cb.call( args );
  if ( r.isError() )
  {
    qWarning() << "QFieldXmlHttpRequest callback error:"
               << r.property( "fileName" ).toString()
               << r.property( "lineNumber" ).toInt()
               << r.toString();
  }
}
