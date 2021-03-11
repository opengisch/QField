/***************************************************************************
    networkreply.cpp
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

#include "networkreply.h"

#include <QTimer>


NetworkReply::NetworkReply( const QNetworkAccessManager::Operation operation, const QNetworkRequest &request, const QByteArray &payloadByteArray = QByteArray() ):
  mOperation( operation ),
  mIsMultiPartPayload( false ),
  mRequest( request ),
  mPayloadByteArray( payloadByteArray )
{
  initiateRequest();
};


NetworkReply::NetworkReply( const QNetworkAccessManager::Operation operation, const QNetworkRequest &request, QHttpMultiPart *payloadMultiPart ):
  mOperation( operation ),
  mIsMultiPartPayload( true ),
  mRequest( request ),
  mPayloadMultiPart( payloadMultiPart )
{
  initiateRequest();
};


void NetworkReply::abort()
{
  mIsFinished = true;
  mReply->abort();
}


QNetworkReply *NetworkReply::reply() const
{
  if ( mIsFinished )
    return mReply;

  return nullptr;
}


void NetworkReply::ignoreSslErrors( const QList<QSslError> &errors )
{
  mExpectedSslErrors = errors;
}


bool NetworkReply::isFinished() const
{
  return mIsFinished;
}


void NetworkReply::initiateRequest()
{
  switch ( mOperation )
  {
    case QNetworkAccessManager::HeadOperation:
      mReply = mNetworkAccessManager->head( mRequest );
      break;
    case QNetworkAccessManager::GetOperation:
      mReply = mNetworkAccessManager->get( mRequest );
      break;
    case QNetworkAccessManager::PutOperation:
      if ( mIsMultiPartPayload )
        mReply = mNetworkAccessManager->put( mRequest, mPayloadMultiPart );
      else
        mReply = mNetworkAccessManager->put( mRequest, mPayloadByteArray );
      break;
    case QNetworkAccessManager::PostOperation:
      if ( mIsMultiPartPayload )
        mReply = mNetworkAccessManager->post( mRequest, mPayloadMultiPart );
      else
        mReply = mNetworkAccessManager->post( mRequest, mPayloadByteArray );
      break;
    case QNetworkAccessManager::DeleteOperation:
      mReply = mNetworkAccessManager->deleteResource( mRequest );
      break;
    case QNetworkAccessManager::CustomOperation:
      throw QStringLiteral( "Not implemented!" );
    case QNetworkAccessManager::UnknownOperation:
      throw QStringLiteral( "Not implemented!" );
  }

  mReply->ignoreSslErrors( mExpectedSslErrors );

  connect( mReply, &QNetworkReply::finished, this, &NetworkReply::onFinished );
  connect( mReply, &QNetworkReply::encrypted, this, &NetworkReply::encrypted );
  connect( mReply, &QNetworkReply::downloadProgress, this, &NetworkReply::downloadProgress );
  connect( mReply, &QNetworkReply::uploadProgress, this, &NetworkReply::uploadProgress );

  // TODO remove this!!! temporary SSL workaround
  connect( mReply, &QNetworkReply::sslErrors, this, [ = ]( const QList<QSslError> &errors )
  {
    for ( const QSslError &error : errors )
      qDebug() << "SSL: " << error;

    mReply->ignoreSslErrors( errors );
  } );
}


void NetworkReply::onFinished()
{
  bool canRetry = false;
  QNetworkReply::NetworkError error = mReply->error();

  switch ( error )
  {
    case QNetworkReply::NoError:
      mIsFinished = true;
      emit finished();
      return;
    case QNetworkReply::RemoteHostClosedError:
    case QNetworkReply::TimeoutError:
    case QNetworkReply::TemporaryNetworkFailureError:
    case QNetworkReply::NetworkSessionFailedError:
    case QNetworkReply::ProxyTimeoutError:
    case QNetworkReply::InternalServerError:
    case QNetworkReply::ContentReSendError:
    case QNetworkReply::ServiceUnavailableError:
      canRetry = true;
      break;
    case QNetworkReply::ConnectionRefusedError:
    case QNetworkReply::HostNotFoundError:
    case QNetworkReply::OperationCanceledError:
    case QNetworkReply::SslHandshakeFailedError:
    case QNetworkReply::BackgroundRequestNotAllowedError:
    case QNetworkReply::TooManyRedirectsError:
    case QNetworkReply::InsecureRedirectError:
    case QNetworkReply::ProxyConnectionRefusedError:
    case QNetworkReply::ProxyConnectionClosedError:
    case QNetworkReply::ProxyNotFoundError:
    case QNetworkReply::ProxyAuthenticationRequiredError:
    case QNetworkReply::ContentAccessDenied:
    case QNetworkReply::ContentOperationNotPermittedError:
    case QNetworkReply::ContentNotFoundError:
    case QNetworkReply::AuthenticationRequiredError:
    case QNetworkReply::ContentGoneError:
    case QNetworkReply::ContentConflictError:
    case QNetworkReply::OperationNotImplementedError:
    case QNetworkReply::ProtocolUnknownError:
    case QNetworkReply::ProtocolInvalidOperationError:
    case QNetworkReply::UnknownNetworkError:
    case QNetworkReply::UnknownProxyError:
    case QNetworkReply::UnknownContentError:
    case QNetworkReply::ProtocolFailure:
    case QNetworkReply::UnknownServerError:
      canRetry = false;
      break;
  }

  if ( ! canRetry || mRetriesLeft == 0 )
  {
    mIsFinished = true;

    emit errorOccurred( error );
    emit finished();

    return;
  }

  emit temporaryErrorOccurred( error );

  // wait random time before the retry is sent
//  QTimer::singleShot( mRNG.bounded( sMaxTimeoutBetweenRetriesMs ), this, [ = ]()
  QTimer::singleShot( 100, this, [ = ]()
  {
    emit retry();

    mRetriesLeft--;

    initiateRequest();
  } );
}
