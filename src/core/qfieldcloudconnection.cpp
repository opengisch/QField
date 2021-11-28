/***************************************************************************
    qfieldcloudconnection.cpp
    ---------------------
    begin                : January 2020
    copyright            : (C) 2020 by Matthias Kuhn
    email                : matthias at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfield.h"
#include "qfieldcloudconnection.h"

#include <QFile>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QSettings>
#include <QTimer>
#include <QUrlQuery>
#include <qgsapplication.h>
#include <qgsmessagelog.h>
#include <qgsnetworkaccessmanager.h>
#include <qgssettings.h>


QFieldCloudConnection::QFieldCloudConnection()
  : mUrl( QSettings().value( QStringLiteral( "/QFieldCloud/url" ), defaultUrl() ).toString() )
  , mUsername( QSettings().value( QStringLiteral( "/QFieldCloud/username" ) ).toString() )
  , mToken( QSettings().value( QStringLiteral( "/QFieldCloud/token" ) ).toByteArray() )
{
  QgsNetworkAccessManager::instance()->setTimeout( 60 * 60 * 1000 );
#if QT_VERSION >= QT_VERSION_CHECK( 5, 15, 0 )
  QgsNetworkAccessManager::instance()->setTransferTimeout( 5 * 60 * 1000 );
#endif
  // we cannot use "/" as separator, since QGIS puts a suffix QGIS/31700 anyway
  const QString userAgent = QStringLiteral( "qfield|%1|%2|%3|" ).arg( qfield::appVersion, qfield::appVersionStr, qfield::gitRev );
  QgsSettings().setValue( QStringLiteral( "/qgis/networkAndProxy/userAgent" ), userAgent );
}

QMap<QString, QString> QFieldCloudConnection::sErrors = QMap<QString, QString>(
{
{ "unknown_error", QObject::tr( "QFieldCloud Unknown Error" ) },
{ "status_not_ok", QObject::tr( "Status not ok" ) },
{ "empty_content", QObject::tr( "Empty content" ) },
{ "object_not_found", QObject::tr( "Object not found" ) },
{ "api_error", QObject::tr( "API Error" ) },
{ "validation_error", QObject::tr( "Validation Error" ) },
{ "multiple_projects", QObject::tr( "Multiple Projects" ) },
{ "invalid_deltafile", QObject::tr( "Invalid delta file" ) },
{ "no_qgis_project", QObject::tr( "The project does not contain a valid QGIS project file" ) },
{ "invalid_job", QObject::tr( "Invalid job" ) },
{ "qgis_export_error", QObject::tr( "QGIS export failed" ) },
{ "qgis_cannot_open_project", QObject::tr( "QGIS is unable to open the QGIS project" ) },
} );

QString QFieldCloudConnection::errorString( QNetworkReply *reply )
{
  if ( !reply )
    return QString();

  QString errorMessage;
  QString payload;
  switch ( reply->error() )
  {
    case QNetworkReply::NoError:
      break;
    case QNetworkReply::TimeoutError:
      errorMessage += tr( "[timeout] The request took too long to finish, please retry." );
      break;
    case QNetworkReply::OperationCanceledError:
      errorMessage += tr( "[aborted] The request has been aborted." );
      break;
    default:
      payload = reply->readAll();
      QJsonParseError jsonError;
      const QJsonObject doc = QJsonDocument::fromJson( payload.toUtf8(), &jsonError ).object();

      if ( jsonError.error == QJsonParseError::NoError )
      {
        if ( doc.contains( QStringLiteral( "code" ) ) )
        {
          QString code = doc.value( QStringLiteral( "code" ) ).toString();
          errorMessage += QStringLiteral( "[QF/%1] " ).arg( code );

          if ( sErrors.contains( code ) )
            errorMessage += sErrors.value( code );
          else
            errorMessage += doc.value( QStringLiteral( "message" ) ).toString();
        }
        else
          errorMessage += QStringLiteral( "<no server details>" );

        if ( errorMessage.isEmpty() )
          errorMessage += QStringLiteral( "<empty server details>" );
      }
      break;
  }

  const int httpCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
  QString httpErrorMessage = QStringLiteral( "[HTTP/%1] %2 " ).arg( httpCode ).arg( reply->url().toString() );
  httpErrorMessage += ( httpCode >= 400 )
                      ? tr( "Server Error." )
                      : tr( "Network Error." );
  httpErrorMessage += payload.left( 200 );

  if ( payload.size() > 200 )
    errorMessage += QStringLiteral( "…" );

  if ( errorMessage.isEmpty() )
  {
    errorMessage = httpErrorMessage;
    QgsMessageLog::logMessage( QStringLiteral( "%1\n%2\n%3" ).arg( errorMessage, payload ).arg( reply->errorString() ) );
  }
  else
  {
    QgsMessageLog::logMessage( QStringLiteral( "%1\n%2\n%3\n%4" ).arg( errorMessage, httpErrorMessage, payload ).arg( reply->errorString() ) );
  }

  return errorMessage;
}

QString QFieldCloudConnection::url() const
{
  return mUrl;
}

void QFieldCloudConnection::setUrl( const QString &url )
{
  if ( url == mUrl )
    return;

  mUrl = url;

  QSettings().setValue( QStringLiteral( "/QFieldCloud/url" ), url );

  emit urlChanged();
}

QString QFieldCloudConnection::defaultUrl()
{
  return QStringLiteral( "https://app.qfield.cloud" );
}

QString QFieldCloudConnection::username() const
{
  return mUsername;
}

QString QFieldCloudConnection::avatarUrl() const
{
  return mAvatarUrl;
}

void QFieldCloudConnection::setUsername( const QString &username )
{
  if ( mUsername == username )
    return;

  mUsername = username;
  invalidateToken();

  emit usernameChanged();
}

QString QFieldCloudConnection::password() const
{
  return mPassword;
}

void QFieldCloudConnection::setPassword( const QString &password )
{
  if ( password == mPassword )
    return;

  mPassword = password;
  emit passwordChanged();
}

CloudUserInformation QFieldCloudConnection::userInformation() const
{
  return mUserInformation;
}

void QFieldCloudConnection::login()
{
  NetworkReply *reply = ( !mToken.isEmpty() && ( mPassword.isEmpty() || mUsername.isEmpty() ) )
                        ? get( QStringLiteral( "/api/v1/auth/user/" ) )
                        : post( QStringLiteral( "/api/v1/auth/token/" ), QVariantMap(
                                                                         {
                                                                         { "username", mUsername },
                                                                         { "password", mPassword },
                                                                         } ) );

  setStatus( ConnectionStatus::Connecting );

  connect( reply, &NetworkReply::finished, this, [=]() {
    QNetworkReply *rawReply = reply->reply();

    Q_ASSERT( reply->isFinished() );
    Q_ASSERT( rawReply );

    reply->deleteLater();
    rawReply->deleteLater();

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      int httpCode = rawReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();

      if ( rawReply->error() == QNetworkReply::HostNotFoundError )
      {
        emit loginFailed( tr( "Server not found, please check the server URL" ) );
      }
      else if ( rawReply->error() == QNetworkReply::TimeoutError )
      {
        emit loginFailed( tr( "Timeout error, please retry" ) );
      }
      else if ( httpCode == 400 )
      {
        emit loginFailed( tr( "Wrong username or password" ) );
      }
      else if ( httpCode == 401 )
      {
        emit loginFailed( tr( "Session expired" ) );
        invalidateToken();
      }
      else
      {
        QString message( errorString( rawReply ) );
        emit loginFailed( message );
      }

      setStatus( ConnectionStatus::Disconnected );
      return;
    }

    QJsonObject resp = QJsonDocument::fromJson( rawReply->readAll() ).object();

    if ( resp.isEmpty() )
    {
      emit loginFailed( tr( "Login temporary unavailable" ) );
      setStatus( ConnectionStatus::Disconnected );
      return;
    }

    QByteArray token = resp.value( QStringLiteral( "token" ) ).toString().toUtf8();

    if ( !token.isEmpty() )
    {
      setToken( token );
    }

    mUsername = resp.value( QStringLiteral( "username" ) ).toString();
    QSettings().setValue( "/QFieldCloud/username", mUsername );
    emit usernameChanged();

    mAvatarUrl = resp.value( QStringLiteral( "avatar_url" ) ).toString();
    emit avatarUrlChanged();
    mUserInformation = CloudUserInformation( mUsername, resp.value( QStringLiteral( "email" ) ).toString() );
    emit userInformationChanged();

    setStatus( ConnectionStatus::LoggedIn );
  } );
}

void QFieldCloudConnection::logout()
{
  QgsNetworkAccessManager *nam = QgsNetworkAccessManager::instance();
  QNetworkRequest request( mUrl + QStringLiteral( "/api/v1/auth/logout/" ) );
  request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
  setAuthenticationToken( request );

  QNetworkReply *reply = nam->post( request, QByteArray() );

  connect( reply, &QNetworkReply::finished, this, [reply]() {
    reply->deleteLater();
  } );

  mPassword.clear();
  invalidateToken();

  mAvatarUrl.clear();
  emit avatarUrlChanged();

  setStatus( ConnectionStatus::Disconnected );
}

QFieldCloudConnection::ConnectionStatus QFieldCloudConnection::status() const
{
  return mStatus;
}

QFieldCloudConnection::ConnectionState QFieldCloudConnection::state() const
{
  return mState;
}

NetworkReply *QFieldCloudConnection::post( const QString &endpoint, const QVariantMap &params, const QStringList &fileNames )
{
  QNetworkRequest request( mUrl + endpoint );
  QByteArray requestBody = QJsonDocument( QJsonObject::fromVariantMap( params ) ).toJson();
  setAuthenticationToken( request );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );

  if ( fileNames.isEmpty() )
  {
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    return NetworkManager::post( request, requestBody );
  }

  QHttpMultiPart *multiPart = new QHttpMultiPart( QHttpMultiPart::FormDataType );
  QHttpPart textPart;

  QJsonDocument doc( QJsonObject::fromVariantMap( params ) );
  textPart.setHeader( QNetworkRequest::ContentTypeHeader, QVariant( "application/json" ) );
  textPart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant( "form-data; name=\"text\"" ) );
  textPart.setBody( doc.toJson() );

  multiPart->append( textPart );

  for ( const QString &fileName : fileNames )
  {
    QHttpPart filePart;
    QFile *file = new QFile( fileName, multiPart );

    if ( !file->open( QIODevice::ReadOnly ) )
      return nullptr;

    const QString header = QStringLiteral( "form-data; name=\"file\"; filename=\"%1\"" ).arg( fileName );
    filePart.setHeader( QNetworkRequest::ContentTypeHeader, QVariant( "application/json" ) );
    filePart.setHeader( QNetworkRequest::ContentDispositionHeader, header );
    filePart.setBodyDevice( file );

    multiPart->append( filePart );
  }

  setClientHeaders( request );

  NetworkReply *reply = NetworkManager::post( request, multiPart );

  multiPart->setParent( reply );

  mPendingRequests++;
  setState( ConnectionState::Busy );
  connect( reply, &NetworkReply::finished, this, [=]() {
    QNetworkReply *rawReply = reply->reply();
    if ( --mPendingRequests == 0 )
    {
      if ( rawReply->error() != QNetworkReply::NoError )
      {
        int httpCode = rawReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
        if ( httpCode == 401 )
        {
          // Access token has been invalidated remotely
          invalidateToken();
          setStatus( ConnectionStatus::Disconnected );
        }
      }
      setState( ConnectionState::Idle );
    }
  } );

  return reply;
}

NetworkReply *QFieldCloudConnection::get( const QString &endpoint, const QVariantMap &params )
{
  QNetworkRequest request;

  request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );
  setAuthenticationToken( request );

  return get( request, endpoint, params );
}


NetworkReply *QFieldCloudConnection::get( QNetworkRequest &request, const QString &endpoint, const QVariantMap &params )
{
  QUrl url( endpoint );

  if ( url.isRelative() )
    url.setUrl( mUrl + endpoint );

  return get( request, url, params );
}

NetworkReply *QFieldCloudConnection::get( QNetworkRequest &request, const QUrl &url, const QVariantMap &params )
{
  QUrlQuery urlQuery = QUrlQuery( url.query() );

  for ( auto [key, value] : qfield::asKeyValueRange( params ) )
    urlQuery.addQueryItem( key, value.toString() );

  QUrl requestUrl = url;
  requestUrl.setQuery( urlQuery );

  request.setUrl( requestUrl );

  setClientHeaders( request );

  NetworkReply *reply = NetworkManager::get( request );

  mPendingRequests++;
  setState( ConnectionState::Busy );
  connect( reply, &NetworkReply::finished, this, [=]() {
    QNetworkReply *rawReply = reply->reply();
    if ( --mPendingRequests == 0 )
    {
      if ( rawReply->error() != QNetworkReply::NoError )
      {
        int httpCode = rawReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
        if ( httpCode == 401 )
        {
          // Access token has been invalidated remotely
          invalidateToken();
          setStatus( ConnectionStatus::Disconnected );
        }
      }
      setState( ConnectionState::Idle );
    }
  } );

  return reply;
}

void QFieldCloudConnection::setToken( const QByteArray &token )
{
  if ( mToken == token )
    return;

  mToken = token;
  QSettings().setValue( "/QFieldCloud/token", token );

  emit tokenChanged();
}

void QFieldCloudConnection::invalidateToken()
{
  if ( mToken.isNull() )
    return;

  mToken = QByteArray();
  QSettings().remove( "/QFieldCloud/token" );

  emit tokenChanged();
}

void QFieldCloudConnection::setStatus( ConnectionStatus status )
{
  if ( mStatus == status )
    return;

  mStatus = status;
  emit statusChanged();
}

void QFieldCloudConnection::setState( ConnectionState state )
{
  if ( mState == state )
    return;

  mState = state;
  emit stateChanged();
}

void QFieldCloudConnection::setAuthenticationToken( QNetworkRequest &request )
{
  if ( !mToken.isNull() )
  {
    request.setRawHeader( "Authorization", "Token " + mToken );
  }
}

void QFieldCloudConnection::setClientHeaders( QNetworkRequest &request )
{
  const QByteArray acceptLanguageHeader( "Accept-Language" );
  if ( !request.hasRawHeader( acceptLanguageHeader ) )
  {
    // the standard requires locales with dash instead of underscore
    request.setRawHeader( acceptLanguageHeader, QLocale::system().name().replace( QStringLiteral( "_" ), QStringLiteral( "-" ) ).toUtf8() );
  }
}
