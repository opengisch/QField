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

  mFetchPendingPaths << QStringLiteral( "/" );
  emit isFetchingAvailablePathsChanged();

  mWebdavDirParser.listDirectory( &mWebdavConnection, mFetchPendingPaths.first() );
}

void WebdavConnection::processDirParserFinished()
{
  const QList<QWebdavItem> list = mWebdavDirParser.getList();
  for ( const QWebdavItem &item : list )
  {
    if ( item.isDir() )
    {
      mFetchPendingPaths << item.path();
    }
  }

  mAvailablePaths << mFetchPendingPaths.takeFirst();
  if ( !mFetchPendingPaths.isEmpty() )
  {
    mWebdavDirParser.listDirectory( &mWebdavConnection, mFetchPendingPaths.first() );
  }
  else
  {
    emit isFetchingAvailablePathsChanged();

    mAvailablePaths.sort();
    emit availablePathsChanged();
  }
}
