/***************************************************************************
                          webdavconnection.cpp
                             -------------------
  begin                : January 2025
  copyright            : (C) 2025 by Mathieu Pellerin
  email                : mathieu@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "webdavconnection.h"

#include <QSettings>
#include <QtWebDAV/qwebdavitem.h>
#include <qgsapplication.h>
#include <qgsauthmanager.h>

WebdavConnection::WebdavConnection( QObject *parent )
  : QObject( parent )
{
  connect( &mWebdavConnection, &QWebdav::errorChanged, this, &WebdavConnection::processConnectionError );
  connect( &mWebdavDirParser, &QWebdavDirParser::errorChanged, this, &WebdavConnection::processDirParserError );
  connect( &mWebdavDirParser, &QWebdavDirParser::finished, this, &WebdavConnection::processDirParserFinished );
}

void WebdavConnection::setUrl( const QString &url )
{
  if ( mUrl == url.trimmed() )
    return;

  mUrl = url.trimmed();
  emit urlChanged();

  if ( !mAvailablePaths.isEmpty() )
  {
    mAvailablePaths.clear();
    emit availablePathsChanged();
  }

  checkStoredPassword();
}

void WebdavConnection::setUsername( const QString &username )
{
  if ( mUrl == username )
    return;

  mUsername = username;
  emit usernameChanged();

  if ( !mAvailablePaths.isEmpty() )
  {
    mAvailablePaths.clear();
    emit availablePathsChanged();
  }

  mWebdavConnection.clearAccessCache();
  checkStoredPassword();
}

void WebdavConnection::setPassword( const QString &password )
{
  if ( mUrl == password )
    return;

  mPassword = password;
  emit passwordChanged();

  if ( !mAvailablePaths.isEmpty() )
  {
    mAvailablePaths.clear();
    emit availablePathsChanged();
  }

  mWebdavConnection.clearAccessCache();
}

void WebdavConnection::setStorePassword( bool storePassword )
{
  if ( mStorePassword == storePassword )
    return;

  mStorePassword = storePassword;
  emit storePasswordChanged();
}

void WebdavConnection::checkStoredPassword()
{
  mStoredPassword.clear();

  if ( !mUrl.isEmpty() && !mUsername.isEmpty() )
  {
    QgsAuthManager *authManager = QgsApplication::instance()->authManager();
    QgsAuthMethodConfigsMap configs = authManager->availableAuthMethodConfigs();
    for ( QgsAuthMethodConfig &config : configs )
    {
      if ( config.uri() == mUrl )
      {
        authManager->loadAuthenticationConfig( config.id(), config, true );
        if ( config.config( QStringLiteral( "username" ) ) == mUsername )
        {
          mStoredPassword = config.config( QStringLiteral( "password" ) );
        }
      }
    }
  }

  emit isPasswordStoredChanged();
}

void WebdavConnection::applyStoredPassword()
{
  QgsAuthManager *authManager = QgsApplication::instance()->authManager();
  QgsAuthMethodConfigsMap configs = authManager->availableAuthMethodConfigs();
  if ( mStorePassword )
  {
    if ( !mPassword.isEmpty() )
    {
      bool found = false;
      for ( QgsAuthMethodConfig &config : configs )
      {
        if ( config.uri() == mUrl )
        {
          authManager->loadAuthenticationConfig( config.id(), config, true );
          if ( config.config( QStringLiteral( "username" ) ) == mUsername )
          {
            if ( config.config( QStringLiteral( "password" ) ) != mPassword )
            {
              config.setConfig( "password", mPassword );
              authManager->updateAuthenticationConfig( config );

              mStoredPassword = mPassword;
              emit isPasswordStoredChanged();
            }

            found = true;
            break;
          }
        }
      }

      if ( !found )
      {
        QgsAuthMethodConfig config( QStringLiteral( "Basic" ) );
        config.setName( QStringLiteral( "WebDAV created on %1" ).arg( QDateTime::currentDateTime().toString() ) );
        config.setUri( mUrl );
        config.setConfig( "username", mUsername );
        config.setConfig( "password", mPassword );
        authManager->storeAuthenticationConfig( config );

        mStoredPassword = mPassword;
        emit isPasswordStoredChanged();
      }
    }
  }
  else
  {
    for ( const QgsAuthMethodConfig &config : configs )
    {
      if ( config.uri() == mUrl && config.config( QStringLiteral( "username" ) ) == mUsername )
      {
        authManager->removeAuthenticationConfig( config.id() );
      }
    }

    if ( !mStoredPassword.isEmpty() )
    {
      mStoredPassword = mPassword;
      emit isPasswordStoredChanged();
    }
  }
}

void WebdavConnection::setupConnection()
{
  QUrl connectionUrl( mUrl );
  bool isHttps = connectionUrl.scheme() == QStringLiteral( "https" );
  mWebdavConnection.setConnectionSettings( isHttps ? QWebdav::HTTPS : QWebdav::HTTP, connectionUrl.host(), connectionUrl.path( QUrl::FullyEncoded ), mUsername, !mPassword.isEmpty() ? mPassword : mStoredPassword );
}

void WebdavConnection::fetchAvailablePaths()
{
  if ( mUrl.isEmpty() || mUsername.isEmpty() || ( mPassword.isEmpty() && mStoredPassword.isEmpty() ) )
    return;

  mAvailablePaths.clear();
  emit availablePathsChanged();

  setupConnection();

  mIsFetchingAvailablePaths = true;
  emit isFetchingAvailablePathsChanged();

  mWebdavDirParser.listDirectory( &mWebdavConnection, QStringLiteral( "/" ), true );
}

void WebdavConnection::processDirParserFinished()
{
  const QList<QWebdavItem> list = mWebdavDirParser.getList();
  if ( mIsFetchingAvailablePaths )
  {
    if ( !list.isEmpty() )
    {
      applyStoredPassword();

      mAvailablePaths << QStringLiteral( "/" );
      for ( const QWebdavItem &item : list )
      {
        if ( item.isDir() )
        {
          mAvailablePaths << item.path();
        }
      }
    }

    mIsFetchingAvailablePaths = false;
    emit isFetchingAvailablePathsChanged();

    mAvailablePaths.sort();
    emit availablePathsChanged();
  }
  else if ( mIsImportingPath )
  {
    if ( !list.isEmpty() )
    {
      applyStoredPassword();

      QDir importLocalDir( mImportLocalPath );
      for ( const QWebdavItem &item : list )
      {
        if ( item.isDir() )
        {
          importLocalDir.mkpath( item.path().mid( mImportRemotePath.size() ) );
        }
        else
        {
          mImportItems << item;
          mImportingBytesTotal += item.size();
        }
      }
      emit progressChanged();
    }

    processImportItems();
  }
}

void WebdavConnection::processImportItems()
{
  if ( !mImportItems.isEmpty() )
  {
    const QString itemPath = mImportItems.first().path();
    const QDateTime itemLastModified = mImportItems.first().lastModified();
    QNetworkReply *reply = mWebdavConnection.get( itemPath );
    QTemporaryFile *temporaryFile = new QTemporaryFile( reply );
    temporaryFile->setFileTemplate( QStringLiteral( "%1%2.XXXXXXXXXXXX" ).arg( mImportLocalPath, itemPath.mid( mImportRemotePath.size() ) ) );
    temporaryFile->open();
    connect( reply, &QNetworkReply::downloadProgress, this, [=]( int bytesReceived, int bytesTotal ) {
      mImportingCurrentBytesReceived = bytesReceived;
      emit progressChanged();

      temporaryFile->write( reply->readAll() );
    } );
    connect( reply, &QNetworkReply::finished, this, [=]() {
      mImportingBytesReceived += mImportingCurrentBytesReceived;
      mImportingCurrentBytesReceived = 0;
      if ( reply->error() == QNetworkReply::NoError )
      {
        temporaryFile->write( reply->readAll() );
        temporaryFile->setAutoRemove( false );
        temporaryFile->rename( mImportLocalPath + itemPath.mid( mImportRemotePath.size() ) );
        temporaryFile->close();
        delete temporaryFile;

        // Attach last modified date value coming from the server (cannot be done via QTemporaryFile)
        QFile file( QStringLiteral( "%1%2" ).arg( mImportLocalPath, itemPath.mid( mImportRemotePath.size() ) ) );
        file.open( QFile::Append );
        file.setFileTime( itemLastModified, QFileDevice::FileModificationTime );
        file.setFileTime( itemLastModified, QFileDevice::FileAccessTime );
        file.close();

        QFileInfo fi( file );
        qDebug() << itemLastModified << fi.fileTime( QFileDevice::FileBirthTime ) << fi.fileTime( QFileDevice::FileModificationTime ) << fi.fileTime( QFileDevice::FileAccessTime );
      }
      else
      {
        mLastError = tr( "Failed to download file %1 due to network error (%1)" ).arg( reply->error() );
      }

      mImportItems.removeFirst();
      processImportItems();
      reply->deleteLater();
    } );
  }
  else
  {
    QVariantMap webdavConfiguration;
    webdavConfiguration[QStringLiteral( "url" )] = mUrl;
    webdavConfiguration[QStringLiteral( "username" )] = mUsername;
    webdavConfiguration[QStringLiteral( "remote_path" )] = mImportRemotePath;

    QJsonDocument jsonDocument = QJsonDocument::fromVariant( webdavConfiguration );
    QFile jsonFile( QStringLiteral( "%1qfield_webdav_configuration.json" ).arg( mImportLocalPath ) );
    jsonFile.open( QFile::WriteOnly );
    jsonFile.write( jsonDocument.toJson() );
    jsonFile.close();

    mIsImportingPath = false;
    emit isImportingPathChanged();
  }
}

void WebdavConnection::importPath( const QString &remotePath, const QString &localPath )
{
  if ( mUrl.isEmpty() || mUsername.isEmpty() || ( mPassword.isEmpty() && mStoredPassword.isEmpty() ) )
    return;

  setupConnection();

  QString localFolder = QStringLiteral( "%1 - %2 - %3" ).arg( mWebdavConnection.hostname(), mWebdavConnection.username(), remotePath );
  localFolder.replace( QRegularExpression( "[\\\\\\/\\<\\>\\:\\|\\?\\*\\\"]" ), QString( "_" ) );

  QDir localDir( localPath );
  localDir.mkpath( localFolder );

  mImportRemotePath = remotePath;
  mImportLocalPath = QDir::cleanPath( localPath + QDir::separator() + localFolder ) + QDir::separator();

  mImportItems.clear();

  mImportingBytesReceived = 0;
  mImportingBytesTotal = 0;
  emit progressChanged();

  mIsImportingPath = true;
  emit isImportingPathChanged();

  mWebdavDirParser.listDirectory( &mWebdavConnection, mImportRemotePath, true );
}

double WebdavConnection::progress() const
{
  if ( mIsImportingPath && mImportingBytesTotal > 0 )
  {
    return static_cast<double>( mImportingBytesReceived + mImportingCurrentBytesReceived ) / mImportingBytesTotal;
  }

  return 0;
}

void WebdavConnection::processConnectionError( const QString &error )
{
  mLastError = error;
  emit lastErrorChanged();
}

void WebdavConnection::processDirParserError( const QString &error )
{
  mLastError = error;
  emit lastErrorChanged();
}
