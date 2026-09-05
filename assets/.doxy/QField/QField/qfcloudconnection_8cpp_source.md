

# File qfcloudconnection.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfcloudconnection.cpp**](qfcloudconnection_8cpp.md)

[Go to the documentation of this file](qfcloudconnection_8cpp.md)


```C++
/***************************************************************************
    qfcloudconnection.cpp
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

#include "qfcloudconnection.h"
#include "qfcloudutils.h"

#include <QDirIterator>
#include <QFile>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLockFile>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QSettings>
#include <QTextDocumentFragment>
#include <QTimer>
#include <QUrlQuery>
#include <qgsapplication.h>
#include <qgsauthmanager.h>
#include <qgsmessagelog.h>
#include <qgsnetworkaccessmanager.h>
#include <qgssettings.h>


QfCloudConnection::QfCloudConnection()
  : mUrl( QSettings().value( QStringLiteral( "/QFieldCloud/url" ), defaultUrl() ).toString() )
  , mUsername( QSettings().value( QStringLiteral( "/QFieldCloud/username" ) ).toString() )
  , mTokenConfigId( QSettings().value( QStringLiteral( "/QFieldCloud/tokenConfigId" ) ).toString() )
  , mProvider( QSettings().value( QStringLiteral( "/QFieldCloud/provider" ) ).toString() )
  , mProviderConfigId( QSettings().value( QStringLiteral( "/QFieldCloud/providerConfigId" ) ).toString() )
  , mServerInformation( QSettings().value( QStringLiteral( "/QFieldCloud/serverInformation" ) ).toMap() )
{
  if ( mUrl == defaultUrl() && mServerInformation.signupUrl.isEmpty() )
  {
    mServerInformation.signupUrl = QStringLiteral( "https://app.qfield.cloud/accounts/signup/" );
  }

  if ( !QgsApplication::authManager()->availableAuthMethodConfigs().contains( mProviderConfigId ) )
  {
    mProviderConfigId.clear();
    QSettings().remove( "/QFieldCloud/providerConfigId" );
  }

  if ( QgsApplication::authManager()->availableAuthMethodConfigs().contains( mTokenConfigId ) )
  {
    QgsAuthMethodConfig config;
    QgsApplication::authManager()->loadAuthenticationConfig( mTokenConfigId, config, true );
    mToken = config.config( "qfieldcloud-token" ).toLatin1();
  }
  else
  {
    mTokenConfigId.clear();
    QSettings().remove( "/QFieldCloud/tokenConfigId" );
  }

  QNetworkInformation::loadBackendByFeatures( QNetworkInformation::Feature::Reachability );
  mNetworkInformation = QNetworkInformation::instance();

  if ( mNetworkInformation )
  {
    connect( mNetworkInformation, &QNetworkInformation::reachabilityChanged, this,
             [this]( QNetworkInformation::Reachability ) {
               emit isReachableChanged();
               tryFlushQueuedProjectPushes();
             } );
  }

  restoreCookies();
}

QfCloudConnection::~QfCloudConnection()
{
  saveCookies();
}

void QfCloudConnection::queueProjectPush( const QString &projectId )
{
  if ( projectId.isEmpty() )
  {
    return;
  }

  mQueuedProjectPushes.insert( projectId );
  tryFlushQueuedProjectPushes();
}

void QfCloudConnection::tryFlushQueuedProjectPushes()
{
  if ( mIsFlushingQueuedProjectPushes )
  {
    return;
  }

  if ( mQueuedProjectPushes.isEmpty() )
  {
    return;
  }

  if ( status() != ConnectionStatus::LoggedIn )
  {
    return;
  }

  if ( !isReachable() )
  {
    return;
  }

  mIsFlushingQueuedProjectPushes = true;

  const QSet<QString> queued = mQueuedProjectPushes;
  mQueuedProjectPushes.clear();

  for ( const QString &id : queued )
  {
    emit queuedProjectPushRequested( id );
  }

  mIsFlushingQueuedProjectPushes = false;
}

bool QfCloudConnection::isReachable() const
{
  if ( !mNetworkInformation || !mNetworkInformation->supports( QNetworkInformation::Feature::Reachability ) )
  {
    // No backend or no reachability support, dont change behaviour
    return true;
  }

  switch ( mNetworkInformation->reachability() )
  {
    case QNetworkInformation::Reachability::Online:
    case QNetworkInformation::Reachability::Unknown:
      return true;

    case QNetworkInformation::Reachability::Disconnected:
    case QNetworkInformation::Reachability::Local:
    case QNetworkInformation::Reachability::Site:
      return false;
  }

  return true;
}

QMap<QString, QString> QfCloudConnection::sErrors = QMap<QString, QString>(
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

QString QfCloudConnection::errorString( QNetworkReply *reply )
{
  return CloudError( reply ).message();
}

QString QfCloudConnection::url() const
{
  return mUrl;
}

void QfCloudConnection::setUrl( const QString &url )
{
  if ( url == mUrl )
  {
    return;
  }

  mUrl = url;
  QSettings().setValue( QStringLiteral( "/QFieldCloud/url" ), url );

  if ( mServerInformation != QfCloudServerInformation() )
  {
    mServerInformation = QfCloudServerInformation();
    QSettings().remove( QStringLiteral( "/QFieldCloud/serverInformation" ) );
    emit serverInformationChanged();
  }

  if ( mStatus != ConnectionStatus::Disconnected )
  {
    // Disconnect from the previously used endpoint
    logout();
  }

  emit urlChanged();
}

QString QfCloudConnection::defaultUrl()
{
  return QStringLiteral( "https://app.qfield.cloud" );
}

QStringList QfCloudConnection::urls() const
{
  QStringList savedUrls = QSettings().value( QStringLiteral( "/QFieldCloud/urls" ) ).toStringList();
  if ( !savedUrls.contains( defaultUrl() ) )
  {
    savedUrls.prepend( defaultUrl() );
  }
  if ( !savedUrls.contains( mUrl ) )
  {
    savedUrls << mUrl;
  }
  return savedUrls;
}

QString QfCloudConnection::avatarUrl() const
{
  return mAvatarUrl;
}

QString QfCloudConnection::provider() const
{
  return mProvider;
}

void QfCloudConnection::setProvider( const QString &provider )
{
  if ( mProvider == provider )
  {
    return;
  }

  mProvider = provider;
  QSettings().setValue( QStringLiteral( "/QFieldCloud/provider" ), provider );

  emit providerChanged();
}

QString QfCloudConnection::username() const
{
  return mUsername;
}

void QfCloudConnection::setUsername( const QString &username )
{
  if ( mUsername == username )
  {
    return;
  }

  mQueuedProjectPushes.clear();
  mUsername = username;

  if ( mStatus != ConnectionStatus::Disconnected )
  {
    // Disconnect from the previously used username
    logout();
  }

  emit usernameChanged();
}

QString QfCloudConnection::password() const
{
  return mPassword;
}

void QfCloudConnection::setPassword( const QString &password )
{
  if ( password == mPassword )
  {
    return;
  }

  mPassword = password;
  emit passwordChanged();
}

QString QfCloudConnection::token() const
{
  return mToken;
}

QfCloudUserInformation QfCloudConnection::userInformation() const
{
  return mUserInformation;
}

bool QfCloudConnection::isFetchingAvailableProviders() const
{
  return mIsFetchingAvailableProviders;
}

QList<QfAuthenticationProvider> QfCloudConnection::availableProviders() const
{
  return mAvailableProviders.values();
}

void QfCloudConnection::getServerInformation()
{
  if ( !mAvailableProviders.isEmpty() )
  {
    mAvailableProviders.clear();
    emit availableProvidersChanged();
  }

  mIsFetchingAvailableProviders = true;
  emit isFetchingAvailableProvidersChanged();

  QNetworkRequest request;
  request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );
  QfNetworkReply *reply = get( request, "/api/v1/server/info/" );

  connect( reply, &QfNetworkReply::finished, this, [this, reply]() {
    QNetworkReply *rawReply = reply->currentRawReply();

    Q_ASSERT( reply->isFinished() );
    Q_ASSERT( rawReply );

    reply->deleteLater();
    rawReply->deleteLater();

    const int httpCode = rawReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      if ( httpCode == 404 )
      {
        fetchLegacyAuthenticationProviders();
        return;
      }

      mIsFetchingAvailableProviders = false;
      emit isFetchingAvailableProvidersChanged();
      return;
    }

    const QVariantMap payload = QJsonDocument::fromJson( rawReply->readAll() ).toVariant().toMap();

    const QfCloudServerInformation serverInformation( payload );
    if ( serverInformation != mServerInformation )
    {
      mServerInformation = serverInformation;
      QSettings().setValue( QStringLiteral( "/QFieldCloud/serverInformation" ), mServerInformation.toVariantMap() );
      emit serverInformationChanged();
    }

    const QVariantList providers = payload.value( QStringLiteral( "auth_providers" ) ).toList();
    for ( const QVariant &provider : providers )
    {
      const QVariantMap providerDetails = provider.toMap();
      const QString providerId = providerDetails.value( QStringLiteral( "id" ) ).toString();
      mAvailableProviders[providerId] = QfAuthenticationProvider( providerId, providerDetails.value( QStringLiteral( "name" ) ).toString(), providerDetails );
    }

    mIsFetchingAvailableProviders = false;
    emit isFetchingAvailableProvidersChanged();
    emit availableProvidersChanged();
  } );
}

void QfCloudConnection::fetchLegacyAuthenticationProviders()
{
  QNetworkRequest request;
  request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );
  QfNetworkReply *reply = get( request, "/api/v1/auth/providers/" );

  connect( reply, &QfNetworkReply::finished, this, [this, reply]() {
    QNetworkReply *rawReply = reply->currentRawReply();

    Q_ASSERT( reply->isFinished() );
    Q_ASSERT( rawReply );

    reply->deleteLater();
    rawReply->deleteLater();

    mIsFetchingAvailableProviders = false;
    emit isFetchingAvailableProvidersChanged();

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      return;
    }

    const QVariantList providers = QJsonDocument::fromJson( rawReply->readAll() ).toVariant().toList();
    for ( const QVariant &provider : providers )
    {
      const QVariantMap providerDetails = provider.toMap();
      const QString providerId = providerDetails.value( QStringLiteral( "id" ) ).toString();
      mAvailableProviders[providerId] = QfAuthenticationProvider( providerId, providerDetails.value( QStringLiteral( "name" ) ).toString(), providerDetails );
    }
    emit availableProvidersChanged();
  } );
}

void QfCloudConnection::login( const QString &password )
{
  if ( !mProvider.isEmpty() )
  {
    if ( mProviderConfigId.isEmpty() && !mAvailableProviders.contains( mProvider ) )
    {
      emit loginFailed( tr( "Authentication provider missing" ) );
      return;
    }
  }
  else
  {
    if ( mToken.isEmpty() && password.isEmpty() )
    {
      emit loginFailed( tr( "Password missing" ) );
      return;
    }
  }

  setPassword( password );
  setStatus( ConnectionStatus::Connecting );

  const bool loginUsingToken = !mProvider.isEmpty() || ( !mToken.isEmpty() && ( mPassword.isEmpty() || mUsername.isEmpty() ) );

  QfNetworkReply *reply = loginUsingToken
                            ? get( QStringLiteral( "/api/v1/auth/user/" ) )
                            : post( QStringLiteral( "/api/v1/auth/token/" ), QVariantMap(
                                                                               {
                                                                                 { "username", mUsername },
                                                                                 { "password", mPassword },
                                                                               } ) );

  // Handle login redirect as an error state
  connect( reply, &QfNetworkReply::redirected, this, [this, reply]() {
    QNetworkReply *rawReply = reply->currentRawReply();
    reply->deleteLater();
    rawReply->deleteLater();

    emit loginFailed( tr( "Login error due to unexpected redirect, please retry later" ) );

    setStatus( ConnectionStatus::Disconnected );
    return;
  } );

  connect( reply, &QfNetworkReply::finished, this, [this, reply, loginUsingToken]() {
    QNetworkReply *rawReply = reply->currentRawReply();

    Q_ASSERT( reply->isFinished() );
    Q_ASSERT( rawReply );

    reply->deleteLater();
    rawReply->deleteLater();

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      const int httpCode = rawReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();

      if ( rawReply->error() == QNetworkReply::HostNotFoundError )
      {
        emit loginFailed( tr( "Server not found, please check the server URL" ) );
      }
      else if ( rawReply->error() == QNetworkReply::TimeoutError )
      {
        emit loginFailed( tr( "Timeout error, please retry" ) );
      }
      else if ( httpCode == 400 || httpCode == 401 )
      {
        if ( !loginUsingToken )
        {
          emit loginFailed( tr( "Wrong username or password" ) );
        }
        else
        {
          emit loginFailed( tr( "Session expired" ) );
        }
      }
      else
      {
        QString message( errorString( rawReply ) );
        emit loginFailed( message );
      }

      if ( !mProvider.isEmpty() && !mProviderConfigId.isEmpty() )
      {
        QgsApplication::authManager()->removeAuthenticationConfig( mProviderConfigId );
        mProviderConfigId.clear();
        QSettings().remove( "/QFieldCloud/providerConfigId" );
        emit providerConfigurationChanged();
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

    QSettings settings;
    mUsername = resp.value( QStringLiteral( "username" ) ).toString();
    settings.setValue( QStringLiteral( "/QFieldCloud/username" ), mUsername );
    emit usernameChanged();

    mAvatarUrl = resp.value( QStringLiteral( "avatar_url" ) ).toString();
    emit avatarUrlChanged();
    mUserInformation = QfCloudUserInformation( mUsername, resp.value( QStringLiteral( "email" ) ).toString(), resp.value( QStringLiteral( "first_name" ) ).toString(), resp.value( QStringLiteral( "last_name" ) ).toString(), resp.value( QStringLiteral( "full_name" ) ).toString() );
    emit userInformationChanged();

    QStringList savedUrls = settings.value( QStringLiteral( "/QFieldCloud/urls" ), QStringList() << defaultUrl() ).toStringList();
    if ( !savedUrls.contains( mUrl ) )
    {
      savedUrls << mUrl;
      settings.setValue( QStringLiteral( "/QFieldCloud/urls" ), savedUrls );
      emit urlsChanged();
    }

    saveCookies();

    setStatus( ConnectionStatus::LoggedIn );
  } );
}

void QfCloudConnection::logout()
{
  QgsNetworkAccessManager *nam = QgsNetworkAccessManager::instance();
  QNetworkRequest request( mUrl + QStringLiteral( "/api/v1/auth/logout/" ) );
  request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
  setAuthenticationDetails( request );

  QNetworkReply *reply = nam->post( request, QByteArray() );

  connect( reply, &QNetworkReply::finished, this, [reply]() {
    reply->deleteLater();
  } );

  setPassword( QString() );
  invalidateToken();

  mAvatarUrl.clear();
  emit avatarUrlChanged();

  if ( !mProviderConfigId.isEmpty() )
  {
    QgsApplication::authManager()->removeAuthenticationConfig( mProviderConfigId );
    mProviderConfigId.clear();
    QSettings().remove( "/QFieldCloud/providerConfigId" );
    emit providerConfigurationChanged();
  }

  const QList<QNetworkCookie> cookies = QgsNetworkAccessManager::instance()->cookieJar()->cookiesForUrl( mUrl );
  for ( const QNetworkCookie &cookie : cookies )
  {
    QgsNetworkAccessManager::instance()->cookieJar()->deleteCookie( cookie );
  }
  saveCookies();

  setStatus( ConnectionStatus::Disconnected );
}

void QfCloudConnection::getUserOrganizations( const QString &user )
{
  if ( mStatus != ConnectionStatus::LoggedIn )
  {
    return;
  }

  QfNetworkReply *reply = get( QStringLiteral( "/api/v1/users/%1/organizations/" ).arg( user ) );

  connect( reply, &QfNetworkReply::finished, this, [this, reply]() {
    QNetworkReply *rawReply = reply->currentRawReply();
    reply->deleteLater();

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      QgsMessageLog::logMessage( QStringLiteral( "Failed to fetch user organizations: %1" ).arg( rawReply->errorString() ), QStringLiteral( "QFieldCloud" ) );
      return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson( rawReply->readAll() );
    const QJsonArray array = doc.array();

    QStringList organizations;
    for ( const auto &value : array )
    {
      const QString username = value.toObject().value( QStringLiteral( "username" ) ).toString();
      if ( !username.isEmpty() )
      {
        organizations.append( username );
      }
    }

    emit userOrganizationsReceived( organizations );
  } );
}

void QfCloudConnection::getSubscriptionInformation( const QString &user )
{
  if ( mStatus != ConnectionStatus::LoggedIn )
  {
    return;
  }

  QfNetworkReply *reply = get( QStringLiteral( "/api/v1/subscriptions/%1/current/" ).arg( user ) );

  connect( reply, &QfNetworkReply::finished, this, [this, reply]() {
    QNetworkReply *rawReply = reply->currentRawReply();
    reply->deleteLater();

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      QgsMessageLog::logMessage( QStringLiteral( "Failed to fetch subscription information: %1" ).arg( rawReply->errorString() ), QStringLiteral( "QFieldCloud" ) );
      return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson( rawReply->readAll() );
    const QJsonObject obj = doc.object();

    const QfCloudSubscriptionInformation subscriptionInformation( obj );
    emit subscriptionInformationReceived( subscriptionInformation );
  } );
}

QfCloudConnection::ConnectionStatus QfCloudConnection::status() const
{
  return mStatus;
}

QfCloudConnection::ConnectionState QfCloudConnection::state() const
{
  return mState;
}

QfNetworkReply *QfCloudConnection::post( const QString &endpoint, const QVariantMap &params, const QStringList &fileNames )
{
  QNetworkRequest request;
  return post( request, endpoint, params, fileNames );
}

QfNetworkReply *QfCloudConnection::post( QNetworkRequest &request, const QString &endpoint, const QVariantMap &params, const QStringList &fileNames )
{
  request.setUrl( mUrl + endpoint );

  QByteArray requestBody = QJsonDocument( QJsonObject::fromVariantMap( params ) ).toJson();
  setAuthenticationDetails( request );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );

  if ( fileNames.isEmpty() )
  {
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    return QfNetworkManager::post( request, requestBody );
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

  QfNetworkReply *reply = QfNetworkManager::post( request, multiPart );

  multiPart->setParent( reply );

  mPendingRequests++;
  setState( ConnectionState::Busy );
  connect( reply, &QfNetworkReply::finished, this, [this, reply]() {
    QNetworkReply *rawReply = reply->currentRawReply();
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

QfNetworkReply *QfCloudConnection::get( const QString &endpoint, const QVariantMap &params )
{
  QNetworkRequest request;

  request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );
  setAuthenticationDetails( request );

  return get( request, endpoint, params );
}


QfNetworkReply *QfCloudConnection::get( QNetworkRequest &request, const QString &endpoint, const QVariantMap &params )
{
  QUrl url( endpoint );

  if ( url.isRelative() )
  {
    url.setUrl( mUrl + endpoint );
  }

  return get( request, url, params );
}

QfNetworkReply *QfCloudConnection::get( QNetworkRequest &request, const QUrl &url, const QVariantMap &params )
{
  QUrlQuery urlQuery = QUrlQuery( url.query() );

  for ( auto [key, value] : params.asKeyValueRange() )
  {
    urlQuery.addQueryItem( key, value.toString() );
  }

  QUrl requestUrl = url;
  requestUrl.setQuery( urlQuery );

  request.setUrl( requestUrl );

  setClientHeaders( request );

  QfNetworkReply *reply = QfNetworkManager::get( request );

  mPendingRequests++;
  setState( ConnectionState::Busy );
  connect( reply, &QfNetworkReply::finished, this, [this, reply]() {
    QNetworkReply *rawReply = reply->currentRawReply();
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

  // assume all redirect will never emit "redirected"
  connect( reply, &QfNetworkReply::redirected, this, [this]() {
    if ( --mPendingRequests == 0 )
    {
      setState( ConnectionState::Idle );
    }
  } );

  return reply;
}

void QfCloudConnection::setToken( const QByteArray &token )
{
  if ( mToken == token )
  {
    return;
  }

  mToken = token;

  if ( !mToken.isEmpty() )
  {
    QgsAuthMethodConfig config;
    if ( QgsApplication::authManager()->availableAuthMethodConfigs().contains( mTokenConfigId ) )
    {
      QgsApplication::authManager()->loadAuthenticationConfig( mTokenConfigId, config, true );
    }
    else
    {
      config.setName( "qfieldcloud-credentials" );
      config.setMethod( "Basic" );
    }
    config.setConfig( "qfieldcloud-token", mToken );
    QgsApplication::authManager()->storeAuthenticationConfig( config, true );
    QSettings().setValue( "/QFieldCloud/tokenConfigId", config.id() );
  }
  else
  {
    if ( !mTokenConfigId.isEmpty() )
    {
      QgsApplication::authManager()->removeAuthenticationConfig( mTokenConfigId );
      mTokenConfigId.clear();
      QSettings().remove( "/QFieldCloud/tokenConfigId" );
    }
  }

  emit tokenChanged();
}

void QfCloudConnection::invalidateToken()
{
  mQueuedProjectPushes.clear();
  if ( mToken.isNull() )
  {
    return;
  }

  mToken = QByteArray();

  if ( !mTokenConfigId.isEmpty() )
  {
    QgsApplication::authManager()->removeAuthenticationConfig( mTokenConfigId );
    mTokenConfigId.clear();
    QSettings().remove( "/QFieldCloud/tokenConfigId" );
  }

  emit tokenChanged();
}

void QfCloudConnection::setStatus( ConnectionStatus status )
{
  if ( mStatus == status )
  {
    return;
  }

  mStatus = status;
  emit statusChanged();

  // If we just logged in and we have queued pushes waiting from offline mode,
  // try to flush them now (reachabilityChanged will also handle later changes).
  if ( mStatus == ConnectionStatus::LoggedIn )
  {
    tryFlushQueuedProjectPushes();
  }
}

void QfCloudConnection::setState( ConnectionState state )
{
  if ( mState == state )
  {
    return;
  }

  mState = state;
  emit stateChanged();
}

void QfCloudConnection::setAuthenticationDetails( QNetworkRequest &request )
{
  if ( !mToken.isNull() )
  {
    request.setRawHeader( "Authorization", "Token " + mToken );
  }

  if ( !mProvider.isEmpty() )
  {
    QString providerId;
    if ( mProviderConfigId.isEmpty() && mAvailableProviders.contains( mProvider ) )
    {
      const QVariantMap providerDetails = mAvailableProviders[mProvider].details();
      providerId = providerDetails.value( "id" ).toString();

      QVariantMap configMap;
      configMap["accessMethod"] = 0;
      configMap["clientId"] = providerDetails.value( "client_id" ).toString();
      configMap["clientSecret"] = providerDetails.value( "client_secret" ).toString();
      configMap["configType"] = 1;
      configMap["description"] = QString( "Connection details for QFieldCloud using %1 provider" ).arg( mProvider );
      configMap["extraTokens"] = providerDetails.value( "extra_tokens" ).toMap();
      configMap["grantFlow"] = providerDetails.value( "grant_flow" ).toInt();
      configMap["name"] = QString( "Autogenerated by QField" );
      configMap["persistToken"] = true;
      configMap["redirectHost"] = QString( "localhost" );
      configMap["redirectPort"] = 7070;
      configMap["refreshTokenUrl"] = providerDetails.value( "refresh_token_url" ).toString();
      configMap["requestTimeout"] = 30;
      configMap["requestUrl"] = providerDetails.value( "request_url" ).toString();
      configMap["scope"] = providerDetails.value( "scope" ).toString();
      configMap["tokenUrl"] = providerDetails.value( "token_url" ).toString();
      configMap["version"] = 1;
      QJsonDocument json = QJsonDocument::fromVariant( configMap );

      QgsAuthMethodConfig config;
      config.setName( "qfieldcloud-sso" );
      config.setMethod( "OAuth2" );
      config.setConfig( "oauth2config", json.toJson() );
      config.setConfig( "qfieldcloud-sso-id", providerId );
      QgsApplication::authManager()->storeAuthenticationConfig( config, true );

      mProviderConfigId = config.id();
      QSettings().setValue( QStringLiteral( "/QFieldCloud/providerConfigId" ), mProviderConfigId );
      emit providerConfigurationChanged();
    }
    else
    {
      QgsAuthMethodConfig config;
      QgsApplication::authManager()->loadAuthenticationConfig( mProviderConfigId, config, true );
      providerId = config.config( "qfieldcloud-sso-id" );
    }

    QgsApplication::authManager()->updateNetworkRequest( request, mProviderConfigId );
    request.setRawHeader( "X-QFC-IDP-ID", providerId.toLatin1() );

    const QList<QNetworkCookie> cookies = QgsNetworkAccessManager::instance()->cookieJar()->cookiesForUrl( mUrl );
    auto match = std::find_if( cookies.begin(), cookies.end(), []( const QNetworkCookie &cookie ) { return cookie.name() == QLatin1String( "csrftoken" ); } );
    if ( match != cookies.end() )
    {
      request.setRawHeader( "X-CSRFToken", match->value() );
      request.setRawHeader( "Referer", mUrl.toLatin1() );
    }
  }
}

void QfCloudConnection::setClientHeaders( QNetworkRequest &request )
{
  const QByteArray acceptLanguageHeader( "Accept-Language" );
  if ( !request.hasRawHeader( acceptLanguageHeader ) )
  {
    // the standard requires locales with dash instead of underscore
    request.setRawHeader( acceptLanguageHeader, QLocale::system().name().replace( QStringLiteral( "_" ), QStringLiteral( "-" ) ).toUtf8() );
  }
}

QfCloudConnection::CloudError::CloudError( QNetworkReply *reply )
{
  if ( !reply )
  {
    return;
  }

  QString errorMessage;

  mError = reply->error();
  mHttpCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();

  switch ( mError )
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
      mPayload = reply->readAll();
      QJsonParseError jsonError;
      mJson = QJsonDocument::fromJson( mPayload.toUtf8(), &jsonError );
      mJsonError = jsonError;

      QJsonObject doc = mJson.object();

      if ( jsonError.error == QJsonParseError::NoError )
      {
        if ( doc.contains( QStringLiteral( "code" ) ) )
        {
          mQfcCode = doc.value( QStringLiteral( "code" ) ).toString();
          errorMessage += QStringLiteral( "[QF/%1] " ).arg( mQfcCode );

          if ( sErrors.contains( mQfcCode ) )
          {
            errorMessage += sErrors.value( mQfcCode );
          }
          else
          {
            errorMessage += doc.value( QStringLiteral( "message" ) ).toString();
          }
        }
        else
        {
          errorMessage += QStringLiteral( "<no server details>" );
        }

        if ( errorMessage.isEmpty() )
        {
          errorMessage += QStringLiteral( "<empty server details>" );
        }
      }
      break;
  }

  errorMessage = QTextDocumentFragment::fromHtml( errorMessage ).toPlainText().trimmed();

  QString httpErrorMessage = QStringLiteral( "[HTTP/%1] %2 " ).arg( mHttpCode ).arg( reply->url().toString() );
  httpErrorMessage += ( mHttpCode >= 400 )
                        ? tr( "Server Error." )
                        : tr( "Network Error." );
  httpErrorMessage += mPayload.left( 200 );
  httpErrorMessage = QTextDocumentFragment::fromHtml( httpErrorMessage ).toPlainText().trimmed();
  QString payloadPlainText = QTextDocumentFragment::fromHtml( mPayload ).toPlainText().trimmed();

  if ( mPayload.size() > 200 )
  {
    errorMessage += QStringLiteral( "…" );
  }

  if ( errorMessage.isEmpty() )
  {
    errorMessage = httpErrorMessage;
    QgsMessageLog::logMessage( QStringLiteral( "%1\n%2\n%3" ).arg( errorMessage, payloadPlainText ).arg( reply->errorString() ) );
  }
  else
  {
    QgsMessageLog::logMessage( QStringLiteral( "%1\n%2\n%3\n%4" ).arg( errorMessage, httpErrorMessage, payloadPlainText ).arg( reply->errorString() ) );
  }

  // strip HTML tags
  errorMessage = QTextDocumentFragment::fromHtml( httpErrorMessage ).toPlainText();

  mMessage = errorMessage;
}

qsizetype QfCloudConnection::uploadPendingAttachments()
{
  if ( mUploadPendingCount > 0 )
  {
    return mUploadPendingCount;
  }

  QMultiMap<QString, QString> attachments = QfCloudUtils::getPendingAttachments( mUsername );
  if ( attachments.isEmpty() )
  {
    emit pendingAttachmentsUploadFinished();
    return 0;
  }

  mUploadPendingCount = attachments.size();
  mUploadDoneCount = 0;
  mUploadFailingCount = 0;
  processPendingAttachments();
  return mUploadPendingCount;
}

void QfCloudConnection::processPendingAttachments()
{
  QMultiMap<QString, QString> attachments = QfCloudUtils::getPendingAttachments( mUsername );
  mUploadPendingCount = attachments.size();

  QMultiMap<QString, QString>::const_iterator it = attachments.constBegin();
  while ( it != attachments.constEnd() )
  {
    if ( !QFileInfo( it.value() ).exists() )
    {
      // A pending attachment has been deleted from the local device, remove
      // This can happen when for e.g. users remove a cloud project from their devices
      QfCloudUtils::removePendingAttachment( mUsername, it.key(), it.value() );
      ++it;
      continue;
    }

    QFileInfo projectInfo( QfCloudUtils::localProjectFilePath( mUsername, it.key() ) );
    QDir projectDir( projectInfo.absolutePath() );
    const QString apiPath = projectDir.relativeFilePath( it.value() );
    QfNetworkReply *attachmentCloudReply = post( QStringLiteral( "/api/v1/files/%1/%2/" ).arg( it.key(), apiPath ), QVariantMap(), QStringList( { it.value() } ) );

    const QString projectId = it.key();
    const QString fileName = it.value();
    const QString statusName = QStringLiteral( "%1:%2" ).arg( QfCloudUtils::projectSetting( projectId, QStringLiteral( "name" ), QString() ).toString(), apiPath );
    emit pendingAttachmentsUploadStatus( statusName, 0.0, mUploadPendingCount - 1 );

    connect( attachmentCloudReply, &QfNetworkReply::uploadProgress, this, [this, statusName]( qint64 bytesSent, qint64 bytesTotal ) {
      emit pendingAttachmentsUploadStatus( statusName, bytesTotal > 0 ? static_cast<double>( bytesSent ) / bytesTotal : 0, mUploadPendingCount - 1 );
    } );

    connect( attachmentCloudReply, &QfNetworkReply::finished, this, [this, attachmentCloudReply, fileName, projectId]() {
      QNetworkReply *attachmentReply = attachmentCloudReply->currentRawReply();
      attachmentCloudReply->deleteLater();

      Q_ASSERT( attachmentCloudReply->isFinished() );
      Q_ASSERT( attachmentReply );

      // If there is an error, don't panic, we continue uploading. The files may be later manually synced.
      const int httpCode = attachmentReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
      if ( attachmentReply->error() != QNetworkReply::NoError )
      {
        QgsMessageLog::logMessage( tr( "Failed to upload attachment stored at `%1`, reason:\n%2" )
                                     .arg( fileName )
                                     .arg( QfCloudConnection::errorString( attachmentReply ) ) );

        // Retry uploading for non-404 errors
        if ( httpCode != 404 )
        {
          mUploadFailingCount++;

          if ( mUploadFailingCount < 5 )
          {
            // Retry the last attachment to upload after a brief delay
            QTimer::singleShot( std::pow( 5, mUploadFailingCount ) * 1000, this, [this] { processPendingAttachments(); } );
          }
          else
          {
            // Too many fails, bailing out for now
            emit pendingAttachmentsUploadFinished();
          }
          return;
        }
      }

      if ( httpCode != 201 && httpCode != 404 )
      {
        qDebug() << QStringLiteral( "Attachment project ID: %1 %2 %3" ).arg( projectId ).arg( fileName ).arg( httpCode );

        for ( const QByteArray &header : attachmentReply->rawHeaderList() )
        {
          qDebug() << QStringLiteral( "Attachment reply header: %1 => %2" ).arg( header ).arg( attachmentReply->rawHeader( header ) );
        }

        qDebug() << QStringLiteral( "Attachment reply content: %1" ).arg( attachmentReply->readAll() );
      }

      QfCloudUtils::removePendingAttachment( mUsername, projectId, fileName );
      mUploadPendingCount--;
      mUploadDoneCount++;
      mUploadFailingCount = 0;

      if ( mUploadPendingCount > 0 )
      {
        // Move onto the next attachment to upload
        processPendingAttachments();
      }
      else
      {
        emit pendingAttachmentsUploadFinished();
      }
    } );

    break;
  }
  return;
}

void QfCloudConnection::restoreCookies()
{
  QSettings settings;
  settings.beginGroup( "/QFieldCloud/cookies" );
  const QStringList cookieKeys = settings.childKeys();
  for ( const QString &cookieKey : cookieKeys )
  {
    QList<QNetworkCookie> cookies = QNetworkCookie::parseCookies( settings.value( cookieKey ).toByteArray() );
    if ( !cookies.isEmpty() )
    {
      if ( QDateTime::currentSecsSinceEpoch() < cookies[0].expirationDate().toSecsSinceEpoch() )
      {
        QgsNetworkAccessManager::instance()->cookieJar()->insertCookie( cookies[0] );
      }
    }
  }
}

void QfCloudConnection::saveCookies()
{
  QSettings settings;
  settings.remove( QStringLiteral( "/QFieldCloud/cookies" ) );
  const QList<QNetworkCookie> cookies = QgsNetworkAccessManager::instance()->cookieJar()->cookiesForUrl( mUrl );
  for ( int idx = 0; idx < cookies.count(); idx++ )
  {
    if ( !cookies[idx].isSessionCookie() && QDateTime::currentSecsSinceEpoch() < cookies[idx].expirationDate().toSecsSinceEpoch() )
    {
      settings.setValue( QStringLiteral( "/QFieldCloud/cookies/%1" ), cookies[idx].toRawForm() );
    }
  }
}
```


