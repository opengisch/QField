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
  connect( &mWebdavConnection, &QWebdav::errorChanged, this, [=]( const QString &error ) {
    qDebug() << "connection error:" << error;
  } );
  connect( &mWebdavDirParser, &QWebdavDirParser::errorChanged, this, [=]( const QString &error ) {
    qDebug() << "parser error:" << error;
  } );
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

  checkStoredPassword();
}

void WebdavConnection::setPassword( const QString &password )
{
  if ( mUrl == password )
    return;

  mPassword = password;
  emit passwordChanged();
}

void WebdavConnection::checkStoredPassword()
{
  mStoredPassword.clear();

  if ( !mUrl.isEmpty() && !mUsername.isEmpty() )
  {
    QgsAuthManager *authManager = QgsApplication::instance()->authManager();
    const QgsAuthMethodConfigsMap configs = authManager->availableAuthMethodConfigs();
    for ( const QgsAuthMethodConfig &config : configs )
    {
      if ( config.uri() == mUrl && config.config( QStringLiteral( "username" ) ) == mUsername )
      {
        mStoredPassword = config.config( QStringLiteral( "password" ) );
      }
    }
  }

  emit isPasswordStoredChanged();
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
    mAvailablePaths << QStringLiteral( "/" );
    for ( const QWebdavItem &item : list )
    {
      if ( item.isDir() )
      {
        mAvailablePaths << item.path();
      }
    }

    mIsFetchingAvailablePaths = false;
    emit isFetchingAvailablePathsChanged();

    mAvailablePaths.sort();
    emit availablePathsChanged();
  }
  else if ( mIsImportingPath )
  {
    QDir importLocalDir( mImportLocalPath );
    for ( const QWebdavItem &item : list )
    {
      if ( item.isDir() )
      {
        importLocalDir.mkpath( item.path().mid( mImportRemotePath.size() ) );
      }
      else
      {
        mImportItems << item.path();
        mImportingBytesTotal += item.size();
      }
    }
    emit progressChanged();

    processImportItems();
  }
}

void WebdavConnection::processImportItems()
{
  if ( !mImportItems.isEmpty() )
  {
    const QString itemPath = mImportItems.first();
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
        temporaryFile->close();
        temporaryFile->rename( mImportLocalPath + itemPath.mid( mImportRemotePath.size() ) );
      }
      else
      {
        //TODO
        qDebug() << reply->error() << reply->errorString();
      }

      mImportItems.removeFirst();
      processImportItems();
      reply->deleteLater();
    } );
  }
  else
  {
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
