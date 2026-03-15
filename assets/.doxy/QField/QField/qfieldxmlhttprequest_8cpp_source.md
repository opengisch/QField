

# File qfieldxmlhttprequest.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldxmlhttprequest.cpp**](qfieldxmlhttprequest_8cpp.md)

[Go to the documentation of this file](qfieldxmlhttprequest_8cpp.md)


```C++
/***************************************************************************
qfieldxmlhttprequest.cpp - QFieldXmlHttpRequest

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
#include <qgsnetworkaccessmanager.h>

#include <algorithm>
#include <fileutils.h>
#include <urlutils.h>

namespace
{
  bool isFileUrlString( const QString &stringValue )
  {
    return stringValue.trimmed().startsWith( QStringLiteral( "file://" ), Qt::CaseInsensitive );
  }
} // namespace

QFieldXmlHttpRequest::QFieldXmlHttpRequest( QObject *parent )
  : QObject( parent )
{
}

QFieldXmlHttpRequest::~QFieldXmlHttpRequest()
{
  // Destructor must be silent (no QML callbacks / no signals)
  cleanupReply( true );
}

QFieldXmlHttpRequest *QFieldXmlHttpRequest::newRequest( QObject *parent )
{
  return new QFieldXmlHttpRequest( parent );
}

void QFieldXmlHttpRequest::setTimeout( int milliseconds )
{
  const int timeoutMilliseconds = milliseconds > 0 ? milliseconds : 0;
  if ( mTimeoutMs == timeoutMilliseconds )
  {
    return;
  }

  mTimeoutMs = timeoutMilliseconds;
  emit timeoutChanged();
}

void QFieldXmlHttpRequest::scheduleAutoDelete()
{
  // Defer so QML callbacks finish first.
  QTimer::singleShot( 0, this, &QObject::deleteLater );
}

bool QFieldXmlHttpRequest::containsNewlines( const QString &stringValue )
{
  return stringValue.contains( QLatin1Char( '\n' ) ) || stringValue.contains( QLatin1Char( '\r' ) );
}

bool QFieldXmlHttpRequest::isOpen() const
{
  return !mMethod.isEmpty() && mUrl.isValid();
}

QString QFieldXmlHttpRequest::normalizeHeaderName( const QString &stringValue )
{
  return stringValue.trimmed().toLower();
}

void QFieldXmlHttpRequest::collectResponseHeaders( QNetworkReply *networkReply )
{
  if ( !networkReply )
  {
    return;
  }

  const QList<QNetworkReply::RawHeaderPair> &headerPairs = networkReply->rawHeaderPairs();
  for ( const QNetworkReply::RawHeaderPair &headerPair : headerPairs )
  {
    const QString headerNameLowercase = QString::fromUtf8( headerPair.first ).trimmed().toLower();
    const QString headerValue = QString::fromUtf8( headerPair.second ).trimmed();
    if ( !headerNameLowercase.isEmpty() )
    {
      mResponseHeaders.insert( headerNameLowercase, headerValue );
    }
  }
}

QString QFieldXmlHttpRequest::getResponseHeader( const QString &name ) const
{
  return mResponseHeaders.value( normalizeHeaderName( name ) );
}

QString QFieldXmlHttpRequest::getAllResponseHeaders() const
{
  QString allHeaders;

  QMap<QString, QString>::ConstIterator headerIterator = mResponseHeaders.constBegin();
  for ( ; headerIterator != mResponseHeaders.constEnd(); ++headerIterator )
  {
    allHeaders += headerIterator.key() + QStringLiteral( ": " ) + headerIterator.value() + QStringLiteral( "\r\n" );
  }

  return allHeaders;
}

void QFieldXmlHttpRequest::clearResponse()
{
  mStatus = 0;
  mStatusText.clear();
  mResponse = {};
  mResponseText.clear();
  mResponseType.clear();
  mResponseUrl.clear();

  mResponseHeaders.clear();
  mLastError = QNetworkReply::NoError;
  mLastErrorString.clear();
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
  for ( const QMetaObject::Connection &connection : mConnections )
  {
    disconnect( connection );
  }
  mConnections.clear();

  if ( mReply )
  {
    disconnect( mReply, nullptr, this, nullptr );

    if ( abortNetwork )
    {
      mReply->abort();
    }

    mReply->deleteLater();
    mReply = nullptr;
  }
}

void QFieldXmlHttpRequest::setReadyState( ReadyState state )
{
  if ( mReadyState == state )
  {
    return;
  }

  mReadyState = state;
  emit readyStateChanged();
  call( mOnReadyStateChanged );
}

void QFieldXmlHttpRequest::open( const QString &method, const QUrl &url )
{
  // open() cancels any in-flight request
  if ( mReply )
  {
    cleanupReply( true );
  }

  const QByteArray methodBytes = method.trimmed().toUtf8();
  if ( methodBytes.isEmpty() || !url.isValid() )
  {
    qWarning() << "QFieldXmlHttpRequest: invalid open() args:" << method << url;
    mMethod.clear();
    mUrl = QUrl();
    mRequest = QNetworkRequest();
    resetForNewRequest();
    setReadyState( Unsent );
    return;
  }

  mMethod = methodBytes;
  mUrl = url;
  mRequest = QNetworkRequest( mUrl );
  mRequest.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy );

  resetForNewRequest();
  setReadyState( Opened );
}

void QFieldXmlHttpRequest::setRequestHeader( const QString &headerName, const QString &headerValue )
{
  if ( !isOpen() || mReply )
  {
    return;
  }

  const QString headerKey = headerName.trimmed();
  if ( headerKey.isEmpty() )
  {
    return;
  }

  if ( containsNewlines( headerKey ) || containsNewlines( headerValue ) )
  {
    qWarning() << "QFieldXmlHttpRequest: blocked header with newline:" << headerName;
    return;
  }

  const QString headerKeyLower = normalizeHeaderName( headerKey );

  // IMPORTANT: keep Content-Type in the typed header too, because startRequest()
  // reads it via mRequest.header(QNetworkRequest::ContentTypeHeader)
  if ( headerKeyLower == QStringLiteral( "content-type" ) )
  {
    mRequest.setHeader( QNetworkRequest::ContentTypeHeader, headerValue.trimmed() );
    return;
  }

  mRequest.setRawHeader( headerKey.toUtf8(), headerValue.toUtf8() );
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
  {
    cleanupReply( true );
  }

  if ( mReadyState != Opened )
  {
    setReadyState( Opened );
  }

  resetForNewRequest();
  ++mRequestSerial;

  startRequest( body );
}

void QFieldXmlHttpRequest::startRequest( const QVariant &body )
{
  const QByteArray upperCaseMethod = mMethod.trimmed().toUpper();

  // Security: only allow http(s) network requests.
  // Do this at send-time to avoid breaking existing open()->setHeader()->send() flows.
  const QString urlScheme = mUrl.scheme().toLower();
  if ( urlScheme != QStringLiteral( "http" ) && urlScheme != QStringLiteral( "https" ) )
  {
    qWarning() << "QFieldXmlHttpRequest: blocked non-http(s) URL:" << mUrl;
    finalizeAsError( QStringLiteral( "Blocked non-http(s) URL" ) );
    return;
  }

  const bool isGetMethod = ( upperCaseMethod == "GET" );
  const bool isPostMethod = ( upperCaseMethod == "POST" );
  const bool isPutMethod = ( upperCaseMethod == "PUT" );
  const bool isDeleteMethod = ( upperCaseMethod == "DELETE" );

  if ( !isGetMethod && !isPostMethod && !isPutMethod && !isDeleteMethod )
  {
    qWarning() << "QFieldXmlHttpRequest: unsupported method:" << upperCaseMethod;
    finalizeAsError( QStringLiteral( "Unsupported HTTP method" ) );
    return;
  }

  const QString requestContentTypeHeader = mRequest.header( QNetworkRequest::ContentTypeHeader ).toString();
  const bool isMultipartForcedByHeader = requestContentTypeHeader.startsWith( QStringLiteral( "multipart/form-data" ), Qt::CaseInsensitive );

  // Multipart can only be built from a map (see buildMultipart()).
  const bool canBuildMultipart = body.canConvert<QVariantMap>();

  // Only detect file URLs when we can actually build a multipart body.
  const bool bodyHasFileUrls = canBuildMultipart && bodyContainsFileUrls( body );

  const bool shouldUseMultipart = ( isMultipartForcedByHeader && canBuildMultipart ) || bodyHasFileUrls;

  // If the caller forces multipart via header but doesn't provide a map body, fail fast with a clear error.
  if ( isMultipartForcedByHeader && !canBuildMultipart )
  {
    qWarning() << "QFieldXmlHttpRequest: multipart/form-data requires a QVariantMap body";
    finalizeAsError( QStringLiteral( "multipart/form-data requires a map body" ) );
    return;
  }

  QNetworkAccessManager *networkAccessManager = QgsNetworkAccessManager::instance();
  if ( !networkAccessManager )
  {
    finalizeAsError( QStringLiteral( "Network manager unavailable" ) );
    return;
  }

  QNetworkReply *networkReply = nullptr;

  if ( isGetMethod )
  {
    networkReply = networkAccessManager->get( mRequest );
  }
  else if ( isDeleteMethod )
  {
    QString inferredContentTypeHeader;
    QByteArray requestPayload;

    if ( body.isValid() && !body.isNull() )
    {
      requestPayload = bodyToBytes( body, &inferredContentTypeHeader );
    }

    if ( mRequest.header( QNetworkRequest::ContentTypeHeader ).isNull() && !inferredContentTypeHeader.isEmpty() )
    {
      mRequest.setHeader( QNetworkRequest::ContentTypeHeader, inferredContentTypeHeader );
    }

    if ( requestPayload.isEmpty() )
    {
      networkReply = networkAccessManager->deleteResource( mRequest );
    }

    else
    {
      networkReply = networkAccessManager->sendCustomRequest( mRequest, QByteArrayLiteral( "DELETE" ), requestPayload );
    }
  }
  else
  {
    if ( shouldUseMultipart )
    {
      // Do NOT keep a preset Content-Type, because QHttpMultiPart/QNetworkAccessManager
      // will generate the correct Content-Type including the boundary.
      if ( isMultipartForcedByHeader )
      {
        mRequest.setHeader( QNetworkRequest::ContentTypeHeader, QVariant() );
      }

      QHttpMultiPart *multipartBody = buildMultipart( body );
      if ( !multipartBody )
      {
        finalizeAsError( QStringLiteral( "Invalid multipart body" ) );
        return;
      }

      networkReply = isPostMethod ? networkAccessManager->post( mRequest, multipartBody )
                                  : networkAccessManager->put( mRequest, multipartBody );

      if ( networkReply )
      {
        multipartBody->setParent( networkReply );
      }
      else
      {
        multipartBody->deleteLater();
      }
    }
    else
    {
      QString inferredContentTypeHeader;
      QByteArray requestPayload;

      if ( body.isValid() && !body.isNull() )
      {
        requestPayload = bodyToBytes( body, &inferredContentTypeHeader );
      }

      if ( mRequest.header( QNetworkRequest::ContentTypeHeader ).isNull() && !inferredContentTypeHeader.isEmpty() )
      {
        mRequest.setHeader( QNetworkRequest::ContentTypeHeader, inferredContentTypeHeader );
      }

      networkReply = isPostMethod ? networkAccessManager->post( mRequest, requestPayload )
                                  : networkAccessManager->put( mRequest, requestPayload );
    }
  }

  if ( !networkReply )
  {
    finalizeAsError( QStringLiteral( "Failed to start request" ) );
    return;
  }

  mReply = networkReply;
  hookReply( mReply );
}

void QFieldXmlHttpRequest::abort()
{
  if ( mFinalized )
  {
    return;
  }

  if ( !mReply )
  {
    return;
  }

  mAborted = true;

  cleanupReply( true );
  clearResponse();

  setReadyState( Done );
  emit responseChanged();

  call( mOnAborted );
  call( mOnLoadEnd );
  scheduleAutoDelete();
}

void QFieldXmlHttpRequest::hookReply( QNetworkReply *networkReply )
{
  if ( !networkReply )
  {
    return;
  }

  mConnections << connect( networkReply, &QNetworkReply::metaDataChanged, this, [this, networkReply]() {
    if ( mReadyState < HeadersReceived )
    {
      setReadyState( HeadersReceived );
    }

    collectResponseHeaders( networkReply );
  } );

  mConnections << connect( networkReply, &QNetworkReply::readyRead, this, [this]() {
    if ( mReadyState < HeadersReceived )
    {
      setReadyState( HeadersReceived );
    }
    if ( mReadyState < Loading )
    {
      setReadyState( Loading );
    }
  } );

  mConnections << connect( networkReply, &QNetworkReply::downloadProgress, this, [this]( qint64 receivedBytes, qint64 totalBytes ) {
    call( mOnDownloadProgress, { double( receivedBytes ), double( totalBytes ) } );
  } );

  mConnections << connect( networkReply, &QNetworkReply::uploadProgress, this, [this]( qint64 sentBytes, qint64 totalBytes ) {
    call( mOnUploadProgress, { double( sentBytes ), double( totalBytes ) } );
  } );

  mConnections << connect( networkReply, &QNetworkReply::redirected, this, [this]( const QUrl &redirectUrl ) {
    call( mOnRedirected, { redirectUrl.toString() } );
  } );

  mConnections << connect( networkReply, &QNetworkReply::errorOccurred, this, [this]( QNetworkReply::NetworkError networkError ) {
    if ( mFinalized )
    {
      return;
    }

    if ( mAborted )
    {
      return;
    }

    if ( networkError == QNetworkReply::TimeoutError )
    {
      if ( mTimedOut )
      {
        return;
      }

      mTimedOut = true;

      cleanupReply( true );
      clearResponse();

      setReadyState( Done );
      emit responseChanged();

      call( mOnTimeout );
      call( mOnLoadEnd );
      scheduleAutoDelete();
      return;
    }

    if ( networkError == QNetworkReply::OperationCanceledError )
    {
      return;
    }

    mLastError = networkError;
    mLastErrorString = mReply ? mReply->errorString() : QString();
  } );

  mConnections << connect( networkReply, &QNetworkReply::finished, this, [this, networkReply]() {
    finalizeReply( networkReply );
  } );

  if ( mTimeoutMs > 0 )
  {
    const quint64 requestSerialSnapshot = mRequestSerial;
    QTimer::singleShot( mTimeoutMs, this, [this, requestSerialSnapshot]() {
      if ( requestSerialSnapshot != mRequestSerial )
      {
        return;
      }

      if ( mFinalized || mAborted || mTimedOut )
      {
        return;
      }

      if ( !mReply || mReply->isFinished() )
      {
        return;
      }

      mTimedOut = true;

      cleanupReply( true );
      clearResponse();

      setReadyState( Done );
      emit responseChanged();

      call( mOnTimeout );
      call( mOnLoadEnd );
      scheduleAutoDelete();
    } );
  }
}

void QFieldXmlHttpRequest::finalizeReply( QNetworkReply *finishedReply )
{
  if ( mFinalized )
  {
    return;
  }

  // Ignore stale finished signals (example; resend raced with previous reply)
  if ( !finishedReply || finishedReply != mReply )
  {
    return;
  }

  mFinalized = true;

  mStatus = mReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
  mStatusText = mReply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();

  mResponseUrl = mReply->url();
  mResponseType = mReply->header( QNetworkRequest::ContentTypeHeader ).toString();

  collectResponseHeaders( mReply );

  const QByteArray responseBytes = mReply->readAll();
  mResponseText = QString::fromUtf8( responseBytes );

  const QString responseContentTypeLowercase = mResponseType.toLower();
  if ( responseContentTypeLowercase.contains( QStringLiteral( "application/json" ) ) || responseContentTypeLowercase.contains( QStringLiteral( "+json" ) ) )
  {
    QJsonParseError jsonParseError;
    const QJsonDocument jsonDocument = QJsonDocument::fromJson( responseBytes, &jsonParseError );
    if ( jsonParseError.error == QJsonParseError::NoError )
    {
      if ( jsonDocument.isObject() )
      {
        mResponse = jsonDocument.object().toVariantMap();
      }
      else if ( jsonDocument.isArray() )
      {
        mResponse = jsonDocument.array().toVariantList();
      }
      else
      {
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
    mResponse = mResponseText;
  }

  cleanupReply( false );

  setReadyState( Done );
  emit responseChanged();

  if ( mStatus == 0 )
  {
    const int errorCode = ( mLastError != QNetworkReply::NoError ) ? int( mLastError ) : int( QNetworkReply::UnknownNetworkError );
    const QString errorMessage = !mLastErrorString.isEmpty() ? mLastErrorString : QStringLiteral( "Network error" );
    call( mOnError, { errorCode, errorMessage } );
  }
  else
  {
    call( mOnLoad );
  }

  call( mOnLoadEnd );
  scheduleAutoDelete();
}

void QFieldXmlHttpRequest::finalizeAsError( const QString &detail, bool shouldEmitResponseChanged )
{
  if ( mFinalized )
  {
    return;
  }

  clearResponse();

  const QJsonObject errorObj { { QStringLiteral( "detail" ), detail } };
  mResponseText = QString::fromUtf8( QJsonDocument( errorObj ).toJson( QJsonDocument::Compact ) );
  mResponse = errorObj.toVariantMap();

  mLastError = QNetworkReply::UnknownNetworkError;
  mLastErrorString = detail;

  mFinalized = true;

  cleanupReply( false );

  setReadyState( Done );
  if ( shouldEmitResponseChanged )
  {
    emit responseChanged();
  }

  call( mOnError, { int( QNetworkReply::UnknownNetworkError ), detail } );
  call( mOnLoadEnd );
  scheduleAutoDelete();
}

QByteArray QFieldXmlHttpRequest::bodyToBytes( const QVariant &body, QString *outContentType ) const
{
  if ( outContentType )
  {
    outContentType->clear();
  }

  if ( body.typeId() == QMetaType::QByteArray )
  {
    return body.toByteArray();
  }

  if ( body.typeId() == QMetaType::QString )
  {
    if ( outContentType )
    {
      *outContentType = QStringLiteral( "text/plain; charset=utf-8" );
    }
    return body.toString().toUtf8();
  }

  const QJsonValue jsonValue = QJsonValue::fromVariant( body );
  QJsonDocument jsonDocument = jsonValue.isObject()  ? QJsonDocument( jsonValue.toObject() )
                               : jsonValue.isArray() ? QJsonDocument( jsonValue.toArray() )
                                                     : QJsonDocument( QJsonObject { { QStringLiteral( "value" ), jsonValue } } );

  if ( outContentType )
  {
    *outContentType = QStringLiteral( "application/json" );
  }

  return jsonDocument.toJson( QJsonDocument::Compact );
}

bool QFieldXmlHttpRequest::bodyContainsFileUrls( const QVariant &body ) const
{
  if ( body.typeId() == QMetaType::QString )
  {
    return isFileUrlString( body.toString() );
  }

  if ( body.canConvert<QVariantList>() )
  {
    const QVariantList variantList = body.toList();
    return std::any_of( variantList.cbegin(), variantList.cend(), []( const QVariant &listValue ) {
      return listValue.typeId() == QMetaType::QString && isFileUrlString( listValue.toString() );
    } );
  }

  if ( body.canConvert<QVariantMap>() )
  {
    const QVariantMap variantMap = body.toMap();
    return std::any_of( variantMap.cbegin(), variantMap.cend(), []( const QVariant &mapValue ) {
      if ( mapValue.typeId() == QMetaType::QString && isFileUrlString( mapValue.toString() ) )
      {
        return true;
      }

      if ( mapValue.canConvert<QVariantList>() )
      {
        const QVariantList nestedVariantList = mapValue.toList();
        return std::any_of( nestedVariantList.cbegin(), nestedVariantList.cend(), []( const QVariant &nestedListValue ) {
          return nestedListValue.typeId() == QMetaType::QString && isFileUrlString( nestedListValue.toString() );
        } );
      }
      return false;
    } );
  }

  return false;
}

bool QFieldXmlHttpRequest::isAllowedLocalUploadPath( const QString &localPath ) const
{
  if ( localPath.isEmpty() )
  {
    return false;
  }

  const QString canonical = QFileInfo( localPath ).canonicalFilePath();
  if ( canonical.isEmpty() )
  {
    return false;
  }

  if ( FileUtils::isWithinProjectDirectory( canonical ) )
  {
    return true;
  }

  const QString cloudRoot = QFieldCloudUtils::localCloudDirectory();
  if ( !cloudRoot.isEmpty() )
  {
    const QString cloudCanonical = QFileInfo( cloudRoot ).canonicalFilePath();
    if ( !cloudCanonical.isEmpty() )
    {
      const QString cloudRootPrefix = QDir::cleanPath( cloudCanonical ) + QDir::separator();
      if ( QDir::cleanPath( canonical ).startsWith( cloudRootPrefix ) )
      {
        return true;
      }
    }
  }

  return false;
}

QHttpMultiPart *QFieldXmlHttpRequest::buildMultipart( const QVariant &body ) const
{
  if ( !body.canConvert<QVariantMap>() )
  {
    return nullptr;
  }

  const QVariantMap bodyMap = body.toMap();
  QHttpMultiPart *multipartBody = new QHttpMultiPart( QHttpMultiPart::FormDataType );

  const auto appendTextPart = [multipartBody]( const QString &fieldName, const QString &fieldValueText ) {
    if ( fieldName.isEmpty() )
    {
      return;
    }

    QHttpPart httpPart;
    httpPart.setHeader( QNetworkRequest::ContentDispositionHeader,
                        QStringLiteral( "form-data; name=\"%1\"" ).arg( fieldName ) );
    httpPart.setBody( fieldValueText.toUtf8() );
    multipartBody->append( httpPart );
  };

  const auto appendFilePart = [this, multipartBody]( const QString &fieldName, const QUrl &fileUrl ) -> bool {
    if ( fieldName.isEmpty() || !fileUrl.isValid() || !fileUrl.isLocalFile() )
    {
      return false;
    }

    const QString filePath = fileUrl.toLocalFile();
    if ( !isAllowedLocalUploadPath( filePath ) )
    {
      qWarning() << "QFieldXmlHttpRequest: blocked file upload path:" << filePath;
      return false;
    }

    QFile *fileDevice = new QFile( filePath );
    if ( !fileDevice->exists() || !fileDevice->open( QFile::ReadOnly ) )
    {
      qWarning() << "QFieldXmlHttpRequest: cannot open file:" << filePath;
      delete fileDevice;
      return false;
    }

    const QString mimeTypeName = FileUtils::mimeTypeName( filePath );

    QHttpPart httpPart;
    httpPart.setHeader( QNetworkRequest::ContentTypeHeader, mimeTypeName );
    httpPart.setHeader( QNetworkRequest::ContentDispositionHeader,
                        QStringLiteral( "form-data; name=\"%1\"; filename=\"%2\"" )
                          .arg( fieldName, QFileInfo( filePath ).fileName() ) );
    httpPart.setBodyDevice( fileDevice );

    fileDevice->setParent( multipartBody );
    multipartBody->append( httpPart );
    return true;
  };

  QVariantMap::ConstIterator fieldIterator = bodyMap.constBegin();
  for ( ; fieldIterator != bodyMap.constEnd(); ++fieldIterator )
  {
    const QString &fieldName = fieldIterator.key();
    const QVariant &fieldValue = fieldIterator.value();

    if ( fieldValue.canConvert<QVariantList>() )
    {
      const QVariantList listValues = fieldValue.toList();
      for ( const QVariant &listValue : listValues )
      {
        if ( listValue.typeId() == QMetaType::QString )
        {
          const QString stringValue = listValue.toString();
          if ( isFileUrlString( stringValue ) )
          {
            const QUrl fileUrl = UrlUtils::fromString( stringValue );
            if ( fileUrl.isLocalFile() && appendFilePart( fieldName, fileUrl ) )
            {
              continue;
            }
          }
        }
        const QJsonValue value = QJsonValue::fromVariant( listValue );
        if ( value.isObject() || value.isArray() )
        {
          const QJsonDocument document = value.isObject() ? QJsonDocument( value.toObject() ) : QJsonDocument( value.toArray() );
          appendTextPart( fieldName, QString::fromUtf8( document.toJson( QJsonDocument::Compact ) ) );
        }
        else
        {
          appendTextPart( fieldName, value.toVariant().toString() );
        }
      }
      continue;
    }

    if ( fieldValue.typeId() == QMetaType::QString )
    {
      const QString stringValue = fieldValue.toString();
      if ( isFileUrlString( stringValue ) )
      {
        const QUrl fileUrl = UrlUtils::fromString( stringValue );
        if ( appendFilePart( fieldName, fileUrl ) )
        {
          continue;
        }
      }
    }

    const QJsonValue jsonValue = QJsonValue::fromVariant( fieldValue );
    if ( jsonValue.isObject() || jsonValue.isArray() )
    {
      const QJsonDocument jsonDocument = jsonValue.isObject() ? QJsonDocument( jsonValue.toObject() ) : QJsonDocument( jsonValue.toArray() );
      appendTextPart( fieldName, QString::fromUtf8( jsonDocument.toJson( QJsonDocument::Compact ) ) );
      continue;
    }

    appendTextPart( fieldName, jsonValue.toVariant().toString() );
  }

  return multipartBody;
}

void QFieldXmlHttpRequest::call( const QJSValue &callback, const QJSValueList &arguments )
{
  if ( !callback.isCallable() )
  {
    return;
  }

  const QJSValue result = callback.call( arguments );
  if ( result.isError() )
  {
    qWarning() << "QFieldXmlHttpRequest callback error:"
               << result.property( "fileName" ).toString()
               << result.property( "lineNumber" ).toInt()
               << result.toString();
  }
}
```


