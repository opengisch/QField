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


#include "platformutilities.h"
#include "webdavconnection.h"

#include <QCryptographicHash>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QLockFile>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QSettings>
#include <QTemporaryFile>
#include <QUrl>
#include <QtWebDAV/qwebdavitem.h>
#include <qgsapplication.h>
#include <qgsauthmanager.h>

#include <algorithm>

namespace
{
  const QString kWebdavConfigFile = QStringLiteral( "qfield_webdav_configuration.json" );
  const QString kWebdavLockFile = QStringLiteral( "qfield_webdav_configuration.lock" );

  bool isInHiddenDotFolder( const QString &relativePath )
  {
    return relativePath.startsWith( QLatin1Char( '.' ) ) || relativePath.contains( QStringLiteral( "/." ) );
  }

  QString findWebdavRootForPath( const QString &path )
  {
    QFileInfo fi( QDir::cleanPath( path ) );
    QDir dir( fi.isFile() ? fi.absolutePath() : fi.absoluteFilePath() );

    while ( !dir.exists( kWebdavConfigFile ) )
    {
      if ( !dir.cdUp() )
      {
        return QString();
      }
    }

    return dir.absolutePath();
  }

  bool readWebdavConfig( const QString &rootPath, QVariantMap &outConfig, QString &outError )
  {
    QFile f( rootPath + QDir::separator() + kWebdavConfigFile );
    if ( !f.open( QFile::ReadOnly ) )
    {
      outError = QObject::tr( "Failed to read WebDAV config file." );
      return false;
    }

    const QByteArray raw = f.readAll();
    QJsonParseError e;
    const QJsonDocument doc = QJsonDocument::fromJson( raw, &e );

    if ( e.error != QJsonParseError::NoError || doc.isNull() )
    {
      outError = QObject::tr( "Invalid WebDAV config JSON." );
      return false;
    }

    outConfig = doc.toVariant().toMap();
    return true;
  }

  QString ensureTrailingSlash( QString path )
  {
    if ( !path.endsWith( QLatin1Char( '/' ) ) )
    {
      path += QLatin1Char( '/' );
    }
    return path;
  }

  QByteArray computeLocalSignature( const QString &rootPath )
  {
    if ( rootPath.isEmpty() )
    {
      return QByteArray();
    }

    const QDir base( rootPath );

    QList<QByteArray> entries;
    QDirIterator it( rootPath, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories );
    while ( it.hasNext() )
    {
      it.next();
      const QFileInfo fi = it.fileInfo();

      const QString name = fi.fileName();
      if ( name == kWebdavConfigFile || name == kWebdavLockFile )
      {
        continue;
      }

      const QString rel = base.relativeFilePath( fi.absoluteFilePath() );
      if ( isInHiddenDotFolder( rel ) )
      {
        continue;
      }

      const qint64 size = fi.size();
      const qint64 mtime = fi.fileTime( QFileDevice::FileModificationTime ).toMSecsSinceEpoch();
      // one deterministic record per file
      entries << ( rel + "\n" + QString::number( size ) + "|" + QString::number( mtime ) + "\n" ).toUtf8();
    }

    std::sort( entries.begin(), entries.end() );

    QCryptographicHash hash( QCryptographicHash::Sha256 );
    for ( const QByteArray &e : entries )
    {
      hash.addData( e );
    }

    return hash.result();
  }
} // namespace

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
  {
    return;
  }

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
  if ( mUsername == username )
  {
    return;
  }

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
  if ( mPassword == password )
  {
    return;
  }

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
  {
    return;
  }

  mStorePassword = storePassword;
  emit storePasswordChanged();
}

void WebdavConnection::checkStoredPassword()
{
  mStoredPassword.clear();

  if ( !mUrl.isEmpty() && !mUsername.isEmpty() )
  {
    QgsAuthManager *authManager = QgsApplication::authManager();
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
  QgsAuthManager *authManager = QgsApplication::authManager();
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
    for ( QgsAuthMethodConfig &config : configs )
    {
      if ( config.uri() == mUrl )
      {
        authManager->loadAuthenticationConfig( config.id(), config, true );
        if ( config.config( QStringLiteral( "username" ) ) == mUsername )
        {
          authManager->removeAuthenticationConfig( config.id() );
        }
      }
    }

    if ( !mStoredPassword.isEmpty() )
    {
      mStoredPassword.clear();
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

void WebdavConnection::fetchAvailablePaths( const QString &remotePath )
{
  if ( mUrl.isEmpty() || mUsername.isEmpty() || ( mPassword.isEmpty() && mStoredPassword.isEmpty() ) )
    return;

  setupConnection();

  mIsFetchingAvailablePaths = true;
  emit isFetchingAvailablePathsChanged();

  QString path = remotePath;
  if ( path.isEmpty() )
  {
    // Reset any previously fetched details
    path = QStringLiteral( "/" );
    mAvailablePaths.clear();
    mCheckedPaths.clear();
    mAvailablePaths << QStringLiteral( "/" );
  }
  else
  {
    // Assume this is building on a valid paths tree, but reset any children to that remote path
    mCheckedPaths.erase( std::remove_if( mCheckedPaths.begin(), mCheckedPaths.end(), [path]( const QString &checkedPath ) { return checkedPath.startsWith( path ); } ), mCheckedPaths.end() );
  }

  mCheckedPaths << path;
  mWebdavDirParser.listDirectory( &mWebdavConnection, path, false );
}

void WebdavConnection::processDirParserFinished()
{
  const QList<QWebdavItem> list = mWebdavDirParser.getList();
  if ( !list.isEmpty() )
  {
    applyStoredPassword();

    for ( const QWebdavItem &item : list )
    {
      mWebdavItems << item;
      if ( item.isDir() )
      {
        if ( !mAvailablePaths.contains( item.path() ) )
        {
          mAvailablePaths << item.path();
        }
      }
    }

    if ( !mIsFetchingAvailablePaths && !mAvailablePaths.isEmpty() )
    {
      // When downloading or uploading folders, we must fetch all possible paths
      auto match = std::find_if( mAvailablePaths.begin(), mAvailablePaths.end(), [this]( const QString &availablePath ) { return !mCheckedPaths.contains( availablePath ); } );
      if ( match != mAvailablePaths.end() )
      {
        mCheckedPaths << *match;
        mWebdavDirParser.listDirectory( &mWebdavConnection, *match, false );
        return;
      }
    }
  }

  if ( mIsFetchingAvailablePaths )
  {
    mAvailablePaths.sort();
    mIsFetchingAvailablePaths = false;
    emit checkedPathsChanged();
    emit isFetchingAvailablePathsChanged();
    emit availablePathsChanged();
  }
  else if ( mIsImportingPath || mIsDownloadingPath )
  {
    if ( !mWebdavItems.isEmpty() )
    {
      QDir localDir( mProcessLocalPath );
      for ( const QWebdavItem &item : mWebdavItems )
      {
        if ( item.isDir() )
        {
          localDir.mkpath( item.path().mid( mProcessRemotePath.size() ) );
        }
        else
        {
          if ( mIsDownloadingPath )
          {
            QFileInfo fileInfo( mProcessLocalPath + item.path().mid( mProcessRemotePath.size() ) );
            if ( !fileInfo.exists() || ( fileInfo.fileTime( QFileDevice::FileModificationTime ) != item.lastModified() ) )
            {
              mBytesTotal += item.size();
            }
          }
          else
          {
            mBytesTotal += item.size();
          }
        }
      }
      emit progressChanged();
    }

    getWebdavItems();
  }
  else if ( mIsUploadingPath )
  {
    if ( !mWebdavLastModified.isEmpty() )
    {
      // Adjust modified date to match upload files
      for ( const QWebdavItem &item : list )
      {
        if ( mWebdavLastModified.contains( item.path() ) )
        {
          QFile file( mProcessLocalPath + item.path().mid( mProcessRemotePath.size() ) );
          if ( file.exists() )
          {
            // The local file should always exist at this stage, just playing safe
            file.open( QFile::Append );
            file.setFileTime( item.lastModified(), QFileDevice::FileModificationTime );
            file.setFileTime( item.lastModified(), QFileDevice::FileAccessTime );
            file.close();
          }
        }
      }

      mWebdavLastModified.clear();

      finishUpload( true, QString() );
    }
    else
    {
      applyStoredPassword();

      const QString normalizedRemotePath = ensureTrailingSlash( mProcessRemotePath );

      QStringList remoteDirs;
      for ( const QWebdavItem &item : list )
      {
        if ( item.isDir() )
        {
          remoteDirs << ensureTrailingSlash( item.path() );
        }
        else
        {
          const QString rel = item.path().mid( normalizedRemotePath.size() );
          if ( isInHiddenDotFolder( rel ) )
            continue;

          QFileInfo fileInfo( mProcessLocalPath + item.path().mid( normalizedRemotePath.size() ) );
          if ( fileInfo.exists() )
          {
            auto localFileInfo = std::find_if( mLocalItems.begin(), mLocalItems.end(), [&fileInfo]( const QFileInfo &entry ) {
              return entry.absoluteFilePath() == fileInfo.absoluteFilePath();
            } );

            if ( localFileInfo != mLocalItems.end() )
            {
              if ( localFileInfo->fileTime( QFileDevice::FileModificationTime ) == item.lastModified() )
              {
                mLocalItems.remove( localFileInfo - mLocalItems.begin(), 1 );
              }
            }
          }
        }
      }

      remoteDirs << normalizedRemotePath;
      remoteDirs.removeDuplicates();

      mWebdavMkDirs.clear();

      const QDir base( mProcessLocalPath );
      for ( const QFileInfo &fileInfo : mLocalItems )
      {
        const QString relFile = base.relativeFilePath( fileInfo.absoluteFilePath() );

        if ( relFile.isEmpty() || relFile == QStringLiteral( "." ) || isInHiddenDotFolder( relFile ) )
        {
          continue;
        }

        QString relDir = QFileInfo( relFile ).path();
        QString remoteDir = normalizedRemotePath;

        if ( relDir != QStringLiteral( "." ) && !relDir.isEmpty() )
        {
          remoteDir += relDir.replace( QDir::separator(), QLatin1Char( '/' ) ) + QLatin1Char( '/' );
        }

        remoteDir = ensureTrailingSlash( remoteDir );

        if ( !remoteDirs.contains( remoteDir ) && !mWebdavMkDirs.contains( remoteDir ) )
        {
          const QStringList parts = remoteDir.mid( normalizedRemotePath.size() ).split( "/", Qt::SkipEmptyParts );
          QString cur = normalizedRemotePath;
          for ( const QString &p : parts )
          {
            cur += p + "/";
            if ( !remoteDirs.contains( cur ) && !mWebdavMkDirs.contains( cur ) )
            {
              mWebdavMkDirs << cur;
            }
          }
        }

        mBytesTotal += fileInfo.size();
      }

      std::sort( mWebdavMkDirs.begin(), mWebdavMkDirs.end(), []( const QString &first, const QString &second ) {
        const int dfirst = first.count( QLatin1Char( '/' ) );
        const int dsecond = second.count( QLatin1Char( '/' ) );
        return dfirst == dsecond ? first < second : dfirst < dsecond;
      } );
      mWebdavMkDirs.removeDuplicates();

      emit progressChanged();
      putLocalItems();
    }
  }
}

void WebdavConnection::getWebdavItems()
{
  if ( !mWebdavItems.isEmpty() )
  {
    if ( mWebdavItems.first().isDir() )
    {
      mWebdavItems.removeFirst();
      getWebdavItems();
      return;
    }

    const QString itemPath = mWebdavItems.first().path();
    const QDateTime itemLastModified = mWebdavItems.first().lastModified();
    QNetworkReply *reply = mWebdavConnection.get( itemPath );
    QTemporaryFile *temporaryFile = new QTemporaryFile( reply );
    temporaryFile->setFileTemplate( QStringLiteral( "%1%2.XXXXXXXXXXXX" ).arg( mProcessLocalPath, itemPath.mid( mProcessRemotePath.size() ) ) );
    temporaryFile->open();

    connect( reply, &QNetworkReply::downloadProgress, this, [this, reply, temporaryFile]( qint64 bytesReceived, qint64 bytesTotal ) {
      mCurrentBytesProcessed = bytesReceived;
      emit progressChanged();

      temporaryFile->write( reply->readAll() );
    } );

    connect( reply, &QNetworkReply::finished, this, [this, reply, temporaryFile, itemPath, itemLastModified]() {
      mBytesProcessed += mCurrentBytesProcessed;
      mCurrentBytesProcessed = 0;
      if ( reply->error() == QNetworkReply::NoError )
      {
        QFile file( mProcessLocalPath + itemPath.mid( mProcessRemotePath.size() ) );
        if ( file.exists() )
        {
          file.remove();
        }

        temporaryFile->write( reply->readAll() );
        temporaryFile->setAutoRemove( false );
        temporaryFile->rename( mProcessLocalPath + itemPath.mid( mProcessRemotePath.size() ) );
        temporaryFile->close();
        delete temporaryFile;

        file.open( QFile::Append );
        file.setFileTime( itemLastModified, QFileDevice::FileModificationTime );
        file.setFileTime( itemLastModified, QFileDevice::FileAccessTime );
        file.close();
      }
      else
      {
        mLastError = tr( "Failed to download file %1 due to network error (%2)" ).arg( itemPath ).arg( reply->error() );
      }

      mWebdavItems.removeFirst();
      getWebdavItems();
      reply->deleteLater();
    } );
  }
  else
  {
    if ( mIsImportingPath )
    {
      QVariantMap webdavConfiguration;
      webdavConfiguration[QStringLiteral( "url" )] = mUrl;
      webdavConfiguration[QStringLiteral( "username" )] = mUsername;
      webdavConfiguration[QStringLiteral( "remote_path" )] = mProcessRemotePath;

      QJsonDocument jsonDocument = QJsonDocument::fromVariant( webdavConfiguration );
      QFile jsonFile( QStringLiteral( "%1qfield_webdav_configuration.json" ).arg( mProcessLocalPath ) );
      jsonFile.open( QFile::WriteOnly );
      jsonFile.write( jsonDocument.toJson() );
      jsonFile.close();

      QSettings settings;
      settings.beginGroup( QStringLiteral( "/qfield/webdavImports/%1/users/%2" ).arg( QUrl::toPercentEncoding( mUrl ), QUrl::toPercentEncoding( mUsername ) ) );
      settings.setValue( QStringLiteral( "lastImportPath" ), mProcessRemotePath );
      settings.setValue( QStringLiteral( "lastImportTime" ), QDateTime::currentDateTime() );
      settings.endGroup();

      mIsImportingPath = false;
      emit isImportingPathChanged();
      emit importSuccessful( mProcessLocalPath );
    }
    else if ( mIsDownloadingPath )
    {
      mIsDownloadingPath = false;
      emit isDownloadingPathChanged();
    }
  }
}

void WebdavConnection::forgetHistory( const QString &url, const QString &username )
{
  QgsAuthManager *authManager = QgsApplication::authManager();
  QgsAuthMethodConfigsMap configs = authManager->availableAuthMethodConfigs();
  QSettings settings;
  if ( !username.isEmpty() )
  {
    // Add a dummy value into the root of the server to avoid it being deleted due to empty group
    settings.setValue( QStringLiteral( "/qfield/webdavImports/%1/dummy" ).arg( QUrl::toPercentEncoding( url ) ), 1 );
    settings.beginGroup( QStringLiteral( "/qfield/webdavImports/%1/users/%2" ).arg( QUrl::toPercentEncoding( url ), QUrl::toPercentEncoding( username ) ) );
    settings.remove( "" );
    settings.endGroup();

    for ( QgsAuthMethodConfig &config : configs )
    {
      if ( config.uri() == url )
      {
        authManager->loadAuthenticationConfig( config.id(), config, true );
        if ( config.config( QStringLiteral( "username" ) ) == username )
        {
          authManager->removeAuthenticationConfig( config.id() );
        }
      }
    }
  }
  else if ( !url.isEmpty() )
  {
    settings.beginGroup( QStringLiteral( "/qfield/webdavImports/%1" ).arg( QUrl::toPercentEncoding( url ) ) );
    settings.remove( "" );
    settings.endGroup();

    for ( QgsAuthMethodConfig &config : configs )
    {
      if ( config.uri() == url )
      {
        authManager->removeAuthenticationConfig( config.id() );
      }
    }
  }
  else
  {
    settings.beginGroup( QStringLiteral( "/qfield/webdavImports" ) );
    const QStringList urls = settings.allKeys();
    settings.remove( "" );
    settings.endGroup();

    for ( QgsAuthMethodConfig &config : configs )
    {
      if ( urls.contains( config.uri() ) )
      {
        authManager->removeAuthenticationConfig( config.id() );
      }
    }
  }
}

QVariantMap WebdavConnection::importHistory()
{
  // Collect imported folders
  QMap<QString, QVariantMap> importedFolders;
  QDir importedProjectsDir( QStringLiteral( "%1/Imported Projects/" ).arg( PlatformUtilities::instance()->applicationDirectory() ), { QStringLiteral( "qfield_webdav_configuration.json" ) } );
  QDirIterator it( importedProjectsDir.absolutePath(), QDir::Filter::Files, QDirIterator::Subdirectories );
  while ( it.hasNext() )
  {
    QFile webdavConfigurationFile( it.next() );
    webdavConfigurationFile.open( QFile::ReadOnly );
    QJsonDocument jsonDocument = QJsonDocument::fromJson( webdavConfigurationFile.readAll() );
    if ( !jsonDocument.isEmpty() )
    {
      QVariantMap webdavConfiguration = jsonDocument.toVariant().toMap();
      const QString importedFolderKey = QStringLiteral( "%1 - %2" ).arg( webdavConfiguration["url"].toString(), webdavConfiguration["username"].toString() );
      importedFolders[importedFolderKey][webdavConfiguration["remote_path"].toString()] = importedProjectsDir.relativeFilePath( it.fileInfo().path() );
    }
  }
  // Collect saved imports
  QVariantMap history;

  QSettings settings;
  settings.beginGroup( QStringLiteral( "/qfield/webdavImports" ) );
  const QStringList urls = settings.childGroups();
  settings.endGroup();

  QDateTime lastUrlImportTime( QDate( 1900, 0, 0 ), QTime( 0, 0, 0, 0 ) );
  QString lastUrl;
  QVariantMap urlsDetails;
  for ( const QString &url : urls )
  {
    const QString decodedUrl = QUrl::fromPercentEncoding( url.toLatin1() );
    settings.beginGroup( QStringLiteral( "/qfield/webdavImports/%1/users" ).arg( url ) );
    const QStringList users = settings.childGroups();
    settings.endGroup();

    QDateTime lastUserImportTime( QDate( 1900, 0, 0 ), QTime( 0, 0, 0, 0 ) );
    QString lastUser;
    QVariantMap usersDetails;
    for ( const QString &user : users )
    {
      const QString decodedUser = QUrl::fromPercentEncoding( user.toLatin1() );
      settings.beginGroup( QStringLiteral( "/qfield/webdavImports/%1/users/%2" ).arg( url, user ) );

      QVariantMap details;
      details["lastImportPath"] = settings.value( "lastImportPath" ).toString();
      details["importPaths"] = importedFolders.contains( QStringLiteral( "%1 - %2" ).arg( decodedUrl, decodedUser ) ) ? importedFolders[QStringLiteral( "%1 - %2" ).arg( decodedUrl, decodedUser )] : QVariantMap();
      usersDetails[decodedUser] = details;

      if ( lastUserImportTime < settings.value( "lastImportTime" ).toDateTime() )
      {
        lastUserImportTime = settings.value( "lastImportTime" ).toDateTime();
        lastUser = decodedUser;
      }
      if ( lastUrlImportTime < settings.value( "lastImportTime" ).toDateTime() )
      {
        lastUrlImportTime = settings.value( "lastImportTime" ).toDateTime();
        lastUrl = decodedUrl;
      }
    }

    QVariantMap details;
    details["users"] = usersDetails;
    details["lastUser"] = lastUser;
    urlsDetails[decodedUrl] = details;
  }

  history["urls"] = urlsDetails;
  history["lastUrl"] = lastUrl;

  return history;
}

void WebdavConnection::putLocalItems()
{
  if ( !mWebdavMkDirs.isEmpty() )
  {
    const QString dirPath = ensureTrailingSlash( mWebdavMkDirs.first() );

    QNetworkReply *reply = mWebdavConnection.mkdir( dirPath );

    connect( reply, &QNetworkReply::finished, this, [this, reply, dirPath]() {
      mBytesProcessed += mCurrentBytesProcessed;
      mCurrentBytesProcessed = 0;
      emit progressChanged();

      // WebDAV MKCOL:
      // - 2xx => created
      // - 405 => already exists (common on some servers) -> treat as success
      const int httpStatus = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
      bool ok = ( reply->error() == QNetworkReply::NoError );
      if ( !ok )
      {
        ok = ( httpStatus >= 200 && httpStatus < 300 ) || httpStatus == 405;
      }

      if ( !ok )
      {
        mLastError = tr( "Failed to upload file %1 due to network error (%2)" ).arg( dirPath ).arg( httpStatus );
      }

      mWebdavMkDirs.removeFirst();
      reply->deleteLater();
      putLocalItems();
    } );
  }
  else if ( !mLocalItems.isEmpty() )
  {
    const QString itemPath = mLocalItems.first().absoluteFilePath();

    const QDir base( mProcessLocalPath );
    const QString relFile = base.relativeFilePath( itemPath );

    if ( relFile.isEmpty() || relFile == QStringLiteral( "." ) || isInHiddenDotFolder( relFile ) )
    {
      mLocalItems.removeFirst();
      putLocalItems();
      return;
    }

    const QString remoteItemPath = ensureTrailingSlash( mProcessRemotePath ) + QString( relFile ).replace( QDir::separator(), QLatin1Char( '/' ) );

    QFile *file = new QFile( itemPath );
    file->open( QFile::ReadOnly );
    QNetworkReply *reply = mWebdavConnection.put( remoteItemPath, file );
    file->setParent( reply );

    connect( reply, &QNetworkReply::uploadProgress, this, [this]( qint64 bytesSent, qint64 ) {
      mCurrentBytesProcessed = bytesSent;
      emit progressChanged();
    } );

    connect( reply, &QNetworkReply::finished, this, [this, reply, remoteItemPath]() {
      mBytesProcessed += mCurrentBytesProcessed;
      mCurrentBytesProcessed = 0;
      emit progressChanged();

      // HTTP 2xx codes are success for WebDAV PUT
      const int httpStatus = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
      if ( reply->error() != QNetworkReply::NoError && ( httpStatus < 200 || httpStatus >= 300 ) )
      {
        mLastError = tr( "Failed to upload file %1 due to network error (%2)" ).arg( remoteItemPath ).arg( httpStatus );
      }

      mWebdavLastModified << remoteItemPath;

      mLocalItems.removeFirst();
      reply->deleteLater();
      putLocalItems();
    } );
  }
  else
  {
    if ( mIsUploadingPath )
    {
      if ( !mWebdavLastModified.isEmpty() )
      {
        mCheckedPaths.clear();
        mCheckedPaths << mProcessRemotePath;
        mAvailablePaths.clear();
        mWebdavDirParser.listDirectory( &mWebdavConnection, mProcessRemotePath, false );
      }
      else
      {
        finishUpload( true, QString() );
      }
    }
  }
}

void WebdavConnection::finishUpload( bool success, const QString &errorMessage )
{
  mIsUploadingPath = false;
  emit isUploadingPathChanged();

  if ( mAutoUploadActive )
  {
    const bool ok = success && mLastError.isEmpty();
    const QString msg = ok ? tr( "Upload finished." ) : ( mLastError.isEmpty() ? errorMessage : mLastError );

    if ( ok )
      mAutoUploadSignature = computeLocalSignature( mAutoUploadRoot );

    if ( mAutoUploadLock )
    {
      mAutoUploadLock->unlock();
      mAutoUploadLock.reset();
    }

    mAutoUploadActive = false;
    mAutoUploadRoot.clear();

    emit uploadFinished( ok, msg );
  }
}

void WebdavConnection::importPath( const QString &remotePath, const QString &localPath, QString localFolder )
{
  if ( mUrl.isEmpty() || mUsername.isEmpty() || ( mPassword.isEmpty() && mStoredPassword.isEmpty() ) )
    return;

  setupConnection();

  if ( localFolder.isEmpty() )
  {
    localFolder = QStringLiteral( "%1 - %2" ).arg( remotePath, mWebdavConnection.username() );
  }
  localFolder.replace( QRegularExpression( "[\\\\\\/\\<\\>\\:\\|\\?\\*\\\"]" ), QString( "_" ) );

  QDir localDir( localPath );
  QString localFolderCheck = localFolder;
  int folderSuffix = 0;
  while ( localDir.exists( localFolderCheck ) )
  {
    localFolderCheck = QStringLiteral( "%1 - %2" ).arg( localFolder, QString::number( ++folderSuffix ) );
  }
  localFolder = localFolderCheck;
  ( void ) localDir.mkdir( localFolder );

  mProcessRemotePath = remotePath;
  mProcessLocalPath = QDir::cleanPath( localPath + QDir::separator() + localFolder ) + QDir::separator();

  mWebdavItems.clear();
  mBytesProcessed = 0;
  mBytesTotal = 0;
  emit progressChanged();

  mIsImportingPath = true;
  emit isImportingPathChanged();

  mCheckedPaths.clear();
  mCheckedPaths << mProcessRemotePath;
  mAvailablePaths.clear();
  mWebdavDirParser.listDirectory( &mWebdavConnection, mProcessRemotePath, false );
}

void WebdavConnection::downloadPath( const QString &localPath )
{
  QDir dir( localPath );
  bool webdavConfigurationExists = dir.exists( "qfield_webdav_configuration.json" );
  QStringList remoteChildrenPath;
  while ( !webdavConfigurationExists )
  {
    remoteChildrenPath.prepend( dir.dirName() );
    if ( !dir.cdUp() )
      break;

    webdavConfigurationExists = dir.exists( "qfield_webdav_configuration.json" );
  }

  if ( webdavConfigurationExists )
  {
    QFile webdavConfigurationFile( dir.absolutePath() + QDir::separator() + QStringLiteral( "qfield_webdav_configuration.json" ) );
    webdavConfigurationFile.open( QFile::ReadOnly );
    QJsonDocument jsonDocument = QJsonDocument::fromJson( webdavConfigurationFile.readAll() );
    if ( !jsonDocument.isEmpty() )
    {
      QVariantMap webdavConfiguration = jsonDocument.toVariant().toMap();
      setUrl( webdavConfiguration["url"].toString() );
      setUsername( webdavConfiguration["username"].toString() );
      setStorePassword( isPasswordStored() );

      mProcessRemotePath = webdavConfiguration["remote_path"].toString();
      if ( !remoteChildrenPath.isEmpty() )
      {
        mProcessRemotePath = mProcessRemotePath + remoteChildrenPath.join( "/" ) + QStringLiteral( "/" );
      }
      mProcessLocalPath = QDir::cleanPath( localPath ) + QDir::separator();

      mWebdavItems.clear();
      mBytesProcessed = 0;
      mBytesTotal = 0;
      emit progressChanged();

      mIsDownloadingPath = true;
      emit isDownloadingPathChanged();
      const QUrl url( mUrl );
      emit confirmationRequested( url.host(), mUsername );
    }
  }
}

void WebdavConnection::uploadPaths( const QStringList &localPaths )
{
  mLocalItems.clear();
  bool webdavConfigurationExists = false;
  QJsonDocument webdavJson;
  QVariantMap webdavConfiguration;

  for ( const QString &localPath : localPaths )
  {
    QFileInfo fi( QDir::cleanPath( localPath ) );
    const QString fileLocalPath = fi.isFile() ? fi.absolutePath() : fi.absoluteFilePath();
    QDir dir( fileLocalPath );
    QStringList remoteChildrenPath;

    while ( !dir.exists( "qfield_webdav_configuration.json" ) )
    {
      remoteChildrenPath.prepend( dir.dirName() );
      if ( !dir.cdUp() )
        break;
    }

    if ( !webdavConfigurationExists )
    {
      webdavConfigurationExists = dir.exists( "qfield_webdav_configuration.json" );

      if ( webdavConfigurationExists )
      {
        const QString webdavConfigurationPath = dir.absolutePath() + QDir::separator() + QStringLiteral( "qfield_webdav_configuration.json" );
        QFile webdavConfigurationFile( webdavConfigurationPath );
        webdavConfigurationFile.open( QFile::ReadOnly );
        webdavJson = QJsonDocument::fromJson( webdavConfigurationFile.readAll() );

        if ( !webdavJson.isEmpty() )
        {
          webdavConfiguration = webdavJson.toVariant().toMap();
          setUrl( webdavConfiguration["url"].toString() );
          setUsername( webdavConfiguration["username"].toString() );
          setStorePassword( isPasswordStored() );
          mProcessLocalPath = fi.isFile() ? fi.absolutePath() : fi.absoluteFilePath();
          mProcessRemotePath = webdavConfiguration["remote_path"].toString();

          if ( !remoteChildrenPath.isEmpty() )
          {
            mProcessRemotePath = mProcessRemotePath + remoteChildrenPath.join( "/" ) + QStringLiteral( "/" );
          }
        }
      }
    }
    else if ( !webdavJson.isEmpty() )
    {
      QString newRemotePath = webdavConfiguration["remote_path"].toString();
      if ( !remoteChildrenPath.isEmpty() )
      {
        newRemotePath = newRemotePath + remoteChildrenPath.join( "/" ) + QStringLiteral( "/" );
      }
      mProcessRemotePath = getCommonPath( newRemotePath, mProcessRemotePath );
    }

    if ( !webdavJson.isEmpty() )
    {
      if ( fi.isDir() )
      {
        QDirIterator it( fileLocalPath, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories );
        while ( it.hasNext() )
        {
          it.next();
          if ( it.fileName() != QStringLiteral( "qfield_webdav_configuration.json" ) )
          {
            mLocalItems << it.fileInfo();
          }
        }
      }
      else
      {
        mLocalItems << fi;
      }

      mProcessLocalPath = getCommonPath( fileLocalPath, mProcessLocalPath );
    }
  }

  mBytesProcessed = 0;
  mBytesTotal = 0;
  emit progressChanged();

  mIsUploadingPath = true;
  emit isUploadingPathChanged();
  const QUrl url( mUrl );
  emit confirmationRequested( url.host(), mUsername );
}

QString WebdavConnection::getCommonPath( const QString &addressA, const QString &addressB )
{
  const QStringList pathComponentsA = addressA.split( "/" );
  const QStringList pathComponentsB = addressB.split( "/" );
  const qsizetype minLength = std::min( pathComponentsA.size(), pathComponentsB.size() );

  QString commonPath = QStringLiteral( "/" );

  for ( qsizetype i = 0; i < minLength; ++i )
  {
    if ( pathComponentsA[i] == pathComponentsB[i] )
    {
      if ( i > 0 )
      {
        commonPath += pathComponentsA[i] + QStringLiteral( "/" );
      }
    }
    else
    {
      break;
    }
  }

  while ( !commonPath.isEmpty() && commonPath.endsWith( "/" ) )
  {
    commonPath.chop( 1 );
  }

  commonPath += QStringLiteral( "/" );

  return commonPath;
}

void WebdavConnection::confirmRequest()
{
  if ( mIsDownloadingPath || mIsUploadingPath )
  {
    mWebdavItems.clear();
    mLastError.clear();
    setupConnection();
    mCheckedPaths.clear();
    mCheckedPaths << mProcessRemotePath;
    mAvailablePaths.clear();
    mWebdavDirParser.listDirectory( &mWebdavConnection, mProcessRemotePath, false );
  }
}

void WebdavConnection::cancelRequest()
{
  if ( mIsDownloadingPath )
  {
    mIsDownloadingPath = false;
    emit isDownloadingPathChanged();
  }
  else if ( mIsUploadingPath )
  {
    mIsUploadingPath = false;
    emit isUploadingPathChanged();

    if ( mAutoUploadActive )
    {
      if ( mAutoUploadLock )
      {
        mAutoUploadLock->unlock();
        mAutoUploadLock.reset();
      }
      mAutoUploadActive = false;
      mAutoUploadRoot.clear();

      emit uploadFinished( false, tr( "Upload cancelled." ) );
    }
  }
}

double WebdavConnection::progress() const
{
  if ( ( mIsImportingPath || mIsDownloadingPath || mIsUploadingPath ) && mBytesTotal > 0 )
  {
    return static_cast<double>( mBytesProcessed + mCurrentBytesProcessed ) / mBytesTotal;
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

void WebdavConnection::requestUpload( const QString &projectPath, bool force )
{
  auto skip = [this]( const QString &reason ) {
    if ( mAutoUploadLock )
    {
      mAutoUploadLock->unlock();
      mAutoUploadLock.reset();
    }
    mAutoUploadActive = false;
    mAutoUploadRoot.clear();
    emit uploadSkipped( reason );
  };

  if ( projectPath.isEmpty() )
  {
    return skip( tr( "No project path provided." ) );
  }

  if ( mIsFetchingAvailablePaths || mIsImportingPath || mIsDownloadingPath || mIsUploadingPath )
  {
    return skip( tr( "WebDAV is busy." ) );
  }

  const QString root = findWebdavRootForPath( projectPath );
  if ( root.isEmpty() )
  {
    return skip( tr( "Not a WebDAV imported project." ) );
  }

  mAutoUploadLock.reset( new QLockFile( root + QDir::separator() + kWebdavLockFile ) );
  mAutoUploadLock->setStaleLockTime( 0 );
  if ( !mAutoUploadLock->tryLock( 0 ) )
  {
    return skip( tr( "Upload is locked by another process." ) );
  }

  QVariantMap cfg;
  QString cfgErr;
  if ( !readWebdavConfig( root, cfg, cfgErr ) )
  {
    return skip( cfgErr );
  }

  const QString cfgUrl = cfg.value( QStringLiteral( "url" ) ).toString();
  const QString cfgUser = cfg.value( QStringLiteral( "username" ) ).toString();
  QString cfgRemote = cfg.value( QStringLiteral( "remote_path" ) ).toString();

  if ( cfgUrl.isEmpty() || cfgUser.isEmpty() || cfgRemote.isEmpty() )
  {
    return skip( tr( "WebDAV config is missing required fields." ) );
  }

  cfgRemote = ensureTrailingSlash( cfgRemote );

  setUrl( cfgUrl );
  setUsername( cfgUser );
  setStorePassword( isPasswordStored() );

  if ( mPassword.isEmpty() && mStoredPassword.isEmpty() )
  {
    return skip( tr( "No stored password available." ) );
  }

  const QByteArray sig = computeLocalSignature( root );
  if ( !force && !mAutoUploadSignature.isEmpty() && sig == mAutoUploadSignature )
  {
    return skip( tr( "No local changes detected." ) );
  }

  mAutoUploadActive = true;
  mAutoUploadRoot = root;
  mLastError.clear();

  mLocalItems.clear();
  QDirIterator it( root, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories );
  while ( it.hasNext() )
  {
    it.next();
    const QString fileName = it.fileName();
    if ( fileName == kWebdavConfigFile || fileName == kWebdavLockFile )
    {
      continue;
    }

    const QDir base( root );
    const QString rel = base.relativeFilePath( it.fileInfo().absoluteFilePath() );
    if ( isInHiddenDotFolder( rel ) )
    {
      continue;
    }

    mLocalItems << it.fileInfo();
  }

  if ( mLocalItems.isEmpty() )
  {
    return skip( tr( "Nothing to upload." ) );
  }

  mProcessLocalPath = QDir::cleanPath( root ) + QDir::separator();
  mProcessRemotePath = cfgRemote;

  mBytesProcessed = 0;
  mBytesTotal = 0;
  mCurrentBytesProcessed = 0;
  emit progressChanged();

  mIsUploadingPath = true;
  emit isUploadingPathChanged();

  confirmRequest();
}

bool WebdavConnection::hasWebdavConfiguration( const QString &path )
{
  const QFileInfo fileInfo( path );
  QDir dir( fileInfo.isFile() ? fileInfo.absolutePath() : fileInfo.absoluteFilePath() );
  bool webdavConfigurationExists = dir.exists( "qfield_webdav_configuration.json" );
  while ( !webdavConfigurationExists && dir.cdUp() )
  {
    webdavConfigurationExists = dir.exists( "qfield_webdav_configuration.json" );
  }
  return webdavConfigurationExists;
}
