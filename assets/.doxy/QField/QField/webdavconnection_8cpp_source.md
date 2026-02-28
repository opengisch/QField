

# File webdavconnection.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**webdavconnection.cpp**](webdavconnection_8cpp.md)

[Go to the documentation of this file](webdavconnection_8cpp.md)


```C++
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

#define WEBDAV_CONFIGURATION_FILENAME QStringLiteral( "qfield_webdav_configuration.json" )
#define WEBDAV_LOCK_FILENAME QStringLiteral( "qfield_webdav_configuration.lock" )

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
  {
    return;
  }

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
          const QString normalizedRemote = ensureTrailingSlash( mProcessRemotePath );
          const QString rel = item.path().mid( normalizedRemote.size() ); // no leading '/'
          localDir.mkpath( rel );
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
      for ( const QWebdavItem &item : mWebdavItems )
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
      for ( const QWebdavItem &item : mWebdavItems )
      {
        if ( item.isDir() )
        {
          remoteDirs << ensureTrailingSlash( item.path() );
        }
        else
        {
          const QString rel = item.path().mid( normalizedRemotePath.size() );
          if ( isInHiddenDotFolder( rel ) )
          {
            continue;
          }

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

        // Insure the path exists remotely
        if ( !remoteDirs.contains( remoteDir ) && !mWebdavMkDirs.contains( remoteDir ) )
        {
          const QStringList parts = remoteDir.mid( normalizedRemotePath.size() ).split( "/", Qt::SkipEmptyParts );
          QString currentPath = normalizedRemotePath;
          for ( const QString &part : parts )
          {
            currentPath += part + "/";
            if ( !remoteDirs.contains( currentPath ) && !mWebdavMkDirs.contains( currentPath ) )
            {
              mWebdavMkDirs << currentPath;
            }
          }
        }

        mBytesTotal += fileInfo.size();
      }

      std::sort( mWebdavMkDirs.begin(), mWebdavMkDirs.end(), []( const QString &first, const QString &second ) {
        const qsizetype dfirst = first.count( QLatin1Char( '/' ) );
        const qsizetype dsecond = second.count( QLatin1Char( '/' ) );
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
          // Remove pre-existing file
          file.remove();
        }

        temporaryFile->write( reply->readAll() );
        temporaryFile->setAutoRemove( false );
        temporaryFile->rename( mProcessLocalPath + itemPath.mid( mProcessRemotePath.size() ) );
        temporaryFile->close();
        delete temporaryFile;

        // Attach last modified date value coming from the server (cannot be done via QTemporaryFile)
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
      QFile jsonFile( QDir( mProcessLocalPath ).filePath( WEBDAV_CONFIGURATION_FILENAME ) );
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
  QDir importedProjectsDir( QStringLiteral( "%1/Imported Projects/" ).arg( PlatformUtilities::instance()->applicationDirectory() ) );
  const QStringList projectFolders = findWebdavProjectFolders( importedProjectsDir.absolutePath() );

  for ( const QString &projectFolder : projectFolders )
  {
    QFile webdavConfigurationFile( projectFolder + QDir::separator() + WEBDAV_CONFIGURATION_FILENAME );
    if ( !webdavConfigurationFile.open( QFile::ReadOnly ) )
    {
      continue;
    }

    const QJsonDocument jsonDocument = QJsonDocument::fromJson( webdavConfigurationFile.readAll() );
    if ( jsonDocument.isEmpty() )
    {
      continue;
    }

    const QVariantMap webdavConfiguration = jsonDocument.toVariant().toMap();
    const QString importedFolderKey = QStringLiteral( "%1 - %2" ).arg( webdavConfiguration.value( "url" ).toString(), webdavConfiguration.value( "username" ).toString() );

    importedFolders[importedFolderKey][webdavConfiguration.value( "remote_path" ).toString()] = importedProjectsDir.relativeFilePath( projectFolder );
  }

  // Collect saved imports
  QVariantMap history;

  QSettings settings;
  settings.beginGroup( QStringLiteral( "/qfield/webdavImports" ) );
  const QStringList urls = settings.childGroups();
  settings.endGroup();

  QDateTime lastUrlImportTime( QDate( 1900, 1, 1 ), QTime( 0, 0 ) );
  QString lastUrl;

  QVariantMap urlsDetails;

  for ( const QString &url : urls )
  {
    const QString decodedUrl = QUrl::fromPercentEncoding( url.toLatin1() );

    settings.beginGroup( QStringLiteral( "/qfield/webdavImports/%1/users" ).arg( url ) );
    const QStringList users = settings.childGroups();
    settings.endGroup();

    QDateTime lastUserImportTime( QDate( 1900, 1, 1 ), QTime( 0, 0 ) );
    QString lastUser;

    QVariantMap usersDetails;

    for ( const QString &user : users )
    {
      const QString decodedUser = QUrl::fromPercentEncoding( user.toLatin1() );

      settings.beginGroup( QStringLiteral( "/qfield/webdavImports/%1/users/%2" ).arg( url, user ) );

      QVariantMap details;
      details[QStringLiteral( "lastImportPath" )] = settings.value( QStringLiteral( "lastImportPath" ) ).toString();

      const QString importedKey = QStringLiteral( "%1 - %2" ).arg( decodedUrl, decodedUser );
      details[QStringLiteral( "importPaths" )] = importedFolders.contains( importedKey ) ? importedFolders[importedKey] : QVariantMap();

      const QDateTime importTime = settings.value( QStringLiteral( "lastImportTime" ) ).toDateTime();

      if ( lastUserImportTime < importTime )
      {
        lastUserImportTime = importTime;
        lastUser = decodedUser;
      }

      if ( lastUrlImportTime < importTime )
      {
        lastUrlImportTime = importTime;
        lastUrl = decodedUrl;
      }

      usersDetails[decodedUser] = details;

      settings.endGroup();
    }

    QVariantMap urlDetails;
    urlDetails[QStringLiteral( "users" )] = usersDetails;
    urlDetails[QStringLiteral( "lastUser" )] = lastUser;

    urlsDetails[decodedUrl] = urlDetails;
  }

  history[QStringLiteral( "urls" )] = urlsDetails;
  history[QStringLiteral( "lastUrl" )] = lastUrl;

  return history;
}

QStringList WebdavConnection::findWebdavProjectFolders( const QString &basePath )
{
  QStringList projects;
  if ( basePath.isEmpty() )
  {
    return projects;
  }

  QDirIterator it( basePath,
                   QStringList() << WEBDAV_CONFIGURATION_FILENAME,
                   QDir::Files,
                   QDirIterator::Subdirectories );

  while ( it.hasNext() )
  {
    it.next();
    projects << it.fileInfo().absolutePath();
  }
  return projects;
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

    mCurrentUploadFileSize = mLocalItems.first().size();
    mCurrentUploadBytesSentMax = 0;
    mCurrentBytesProcessed = 0;
    emit progressChanged();

    QFile *file = new QFile( itemPath );
    file->open( QFile::ReadOnly );
    QNetworkReply *reply = mWebdavConnection.put( remoteItemPath, file );
    file->setParent( reply );

    connect( reply, &QNetworkReply::uploadProgress, this, [this]( qint64 bytesSent, qint64 bytesTotal ) {
      // Prefer bytesTotal if provided, else fall back to QFileInfo size
      const qint64 total = ( bytesTotal > 0 ) ? bytesTotal : mCurrentUploadFileSize;

      qint64 sent = bytesSent;
      if ( total > 0 )
      {
        sent = std::min( sent, total );
      }

      if ( sent > mCurrentUploadBytesSentMax )
      {
        mCurrentUploadBytesSentMax = sent;
      }

      mCurrentBytesProcessed = mCurrentUploadBytesSentMax;
      emit progressChanged();
    } );

    connect( reply, &QNetworkReply::finished, this, [this, reply, remoteItemPath]() {
      const qint64 completed = ( mCurrentUploadFileSize > 0 )
                                 ? mCurrentUploadFileSize
                                 : mCurrentUploadBytesSentMax;

      mBytesProcessed += completed;

      mCurrentBytesProcessed = 0;
      mCurrentUploadFileSize = 0;
      mCurrentUploadBytesSentMax = 0;
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

  const bool ok = success && mLastError.isEmpty();
  const QString msg = ok ? tr( "Upload finished." ) : ( mLastError.isEmpty() ? errorMessage : mLastError );

  if ( mAutoUploadActive )
  {
    if ( ok )
    {
      mAutoUploadSignature = computeLocalSignature( mAutoUploadRoot );
      mAutoUploadSignatureRoot = mAutoUploadRoot;
    }

    mAutoUploadActive = false;
    mAutoUploadRoot.clear();
    emit uploadFinished( ok, msg );
  }

  unlockUpload();
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
  bool webdavConfigurationExists = dir.exists( WEBDAV_CONFIGURATION_FILENAME );
  QStringList remoteChildrenPath;
  while ( !webdavConfigurationExists )
  {
    remoteChildrenPath.prepend( dir.dirName() );
    if ( !dir.cdUp() )
    {
      break;
    }

    webdavConfigurationExists = dir.exists( WEBDAV_CONFIGURATION_FILENAME );
  }

  if ( webdavConfigurationExists )
  {
    QFile webdavConfigurationFile( dir.absoluteFilePath( WEBDAV_CONFIGURATION_FILENAME ) );
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

bool WebdavConnection::uploadPathsInternal( const QStringList &localPaths, bool requireConfirmation, bool autoUpload, bool force, QString *errorMessage )
{
  auto fail = [&]( const QString &msg ) -> bool {
    if ( errorMessage )
    {
      *errorMessage = msg;
    }
    unlockUpload();
    return false;
  };

  if ( localPaths.isEmpty() )
  {
    return fail( tr( "No local paths provided." ) );
  }

  if ( mIsFetchingAvailablePaths || mIsImportingPath || mIsDownloadingPath || mIsUploadingPath )
  {
    return fail( tr( "WebDAV is busy." ) );
  }

  mLocalItems.clear();

  // Find config root from first path (and ensure all paths match it)
  QString configRoot;
  QVariantMap webdavConfiguration;
  QString webdavConfigurationErr;

  bool webdavConfigurationLoaded = false;

  for ( const QString &localPath : localPaths )
  {
    QFileInfo fi( QDir::cleanPath( localPath ) );
    const QString fileLocalPath = fi.isFile() ? fi.absolutePath() : fi.absoluteFilePath();

    QDir dir( fileLocalPath );
    QStringList remoteChildrenPath;

    while ( !dir.exists( WEBDAV_CONFIGURATION_FILENAME ) )
    {
      remoteChildrenPath.prepend( dir.dirName() );
      if ( !dir.cdUp() )
      {
        break;
      }
    }

    if ( !webdavConfigurationLoaded )
    {
      if ( !dir.exists( WEBDAV_CONFIGURATION_FILENAME ) )
      {
        return fail( tr( "Not a WebDAV imported project." ) );
      }

      configRoot = dir.absolutePath();

      // lock applies to BOTH auto upload and 3-dot upload
      if ( !tryLockUpload( configRoot, errorMessage ) )
      {
        return false;
      }

      if ( !readWebdavConfig( configRoot, webdavConfiguration, webdavConfigurationErr ) )
      {
        return fail( webdavConfigurationErr );
      }

      const QString webdavConfigurationUrl = webdavConfiguration.value( QStringLiteral( "url" ) ).toString();
      const QString webdavConfigurationUser = webdavConfiguration.value( QStringLiteral( "username" ) ).toString();
      QString webdavConfigurationRemote = webdavConfiguration.value( QStringLiteral( "remote_path" ) ).toString();

      if ( webdavConfigurationUrl.isEmpty() || webdavConfigurationUser.isEmpty() || webdavConfigurationRemote.isEmpty() )
      {
        return fail( tr( "WebDAV config is missing required fields." ) );
      }

      webdavConfigurationRemote = ensureTrailingSlash( webdavConfigurationRemote );

      setUrl( webdavConfigurationUrl );
      setUsername( webdavConfigurationUser );
      setStorePassword( isPasswordStored() );

      // auto-upload must not pop UI for password
      if ( autoUpload && mPassword.isEmpty() && mStoredPassword.isEmpty() )
      {
        return fail( tr( "No stored password available." ) );
      }

      // compute final remote base for this selection
      if ( !remoteChildrenPath.isEmpty() )
      {
        webdavConfigurationRemote = ensureTrailingSlash( webdavConfigurationRemote + remoteChildrenPath.join( "/" ) + "/" );
      }

      mProcessRemotePath = webdavConfigurationRemote;
      mProcessLocalPath = QDir::cleanPath( fileLocalPath ) + QDir::separator();

      webdavConfigurationLoaded = true;
    }
    else
    {
      // enforce same config root across all selected paths
      if ( dir.absolutePath() != configRoot )
      {
        return fail( tr( "Selected items belong to different WebDAV projects." ) );
      }

      QString newRemotePath = ensureTrailingSlash( webdavConfiguration.value( QStringLiteral( "remote_path" ) ).toString() );
      if ( !remoteChildrenPath.isEmpty() )
      {
        newRemotePath = ensureTrailingSlash( newRemotePath + remoteChildrenPath.join( "/" ) );
      }

      mProcessRemotePath = getCommonPath( newRemotePath, mProcessRemotePath );
      mProcessLocalPath = QDir::cleanPath( getCommonPath( fileLocalPath, mProcessLocalPath ) ) + QDir::separator();
    }

    const QDir base( configRoot );

    if ( fi.isDir() )
    {
      QDirIterator it( fileLocalPath, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories );
      while ( it.hasNext() )
      {
        it.next();
        const QFileInfo itemFi = it.fileInfo();

        const QString name = itemFi.fileName();
        if ( name == WEBDAV_CONFIGURATION_FILENAME || name == WEBDAV_LOCK_FILENAME )
        {
          continue;
        }

        const QString rel = base.relativeFilePath( itemFi.absoluteFilePath() );
        if ( isInHiddenDotFolder( rel ) )
        {
          continue;
        }

        mLocalItems << itemFi;
      }
    }
    else
    {
      const QString name = fi.fileName();
      if ( name != WEBDAV_CONFIGURATION_FILENAME && name != WEBDAV_LOCK_FILENAME )
      {
        const QString rel = base.relativeFilePath( fi.absoluteFilePath() );
        if ( !isInHiddenDotFolder( rel ) )
        {
          mLocalItems << fi;
        }
      }
    }
  }

  if ( mLocalItems.isEmpty() )
  {
    return fail( tr( "Nothing to upload." ) );
  }

  // auto-upload->unchanged check (only here, so both paths share the same implementation)
  if ( autoUpload )
  {
    const QByteArray sig = computeLocalSignature( configRoot );
    if ( !force && !mAutoUploadSignature.isEmpty() && mAutoUploadSignatureRoot == configRoot && sig == mAutoUploadSignature )
    {
      return fail( tr( "No local changes detected." ) );
    }

    mAutoUploadActive = true;
    mAutoUploadRoot = configRoot;
    mLastError.clear();
  }

  beginUpload( requireConfirmation );
  return true;
}

void WebdavConnection::uploadPaths( const QStringList &localPaths )
{
  QString err;
  if ( !uploadPathsInternal( localPaths, true, false, false, &err ) )
  {
    if ( !err.isEmpty() )
    {
      mLastError = err;
      emit lastErrorChanged();
    }
  }
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
      mAutoUploadActive = false;
      mAutoUploadRoot.clear();
      emit uploadFinished( false, tr( "Upload cancelled." ) );
    }

    unlockUpload();
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
  const QString root = findWebdavRootForPath( projectPath );
  if ( root.isEmpty() )
  {
    emit uploadSkipped( tr( "Not a WebDAV imported project." ) );
    return;
  }

  QString reason;
  if ( !uploadPathsInternal( QStringList() << root, false, true, force, &reason ) )
  {
    emit uploadSkipped( reason.isEmpty() ? tr( "Upload skipped." ) : reason );
  }
}

bool WebdavConnection::hasWebdavConfiguration( const QString &path )
{
  return !findWebdavRootForPath( path ).isEmpty();
}

bool WebdavConnection::tryLockUpload( const QString &root, QString *errorMessage )
{
  unlockUpload();

  mUploadLock.reset( new QLockFile( root + QDir::separator() + WEBDAV_LOCK_FILENAME ) );
  mUploadLock->setStaleLockTime( 0 );

  if ( !mUploadLock->tryLock( 0 ) )
  {
    if ( errorMessage )
    {
      *errorMessage = tr( "Upload is locked by another process." );
    }
    mUploadLock.reset();
    return false;
  }

  return true;
}

void WebdavConnection::unlockUpload()
{
  if ( mUploadLock )
  {
    mUploadLock->unlock();
    mUploadLock.reset();
  }
}

void WebdavConnection::beginUpload( bool requireConfirmation )
{
  mBytesProcessed = 0;
  mBytesTotal = 0;
  mCurrentBytesProcessed = 0;
  emit progressChanged();

  mIsUploadingPath = true;
  emit isUploadingPathChanged();

  if ( requireConfirmation )
  {
    const QUrl url( mUrl );
    emit confirmationRequested( url.host(), mUsername );
  }
  else
  {
    confirmRequest();
  }
}

bool WebdavConnection::isInHiddenDotFolder( const QString &relativePath ) const
{
  return relativePath.startsWith( QLatin1Char( '.' ) ) || relativePath.contains( QStringLiteral( "/." ) );
}

QString WebdavConnection::ensureTrailingSlash( QString path ) const
{
  if ( !path.endsWith( QLatin1Char( '/' ) ) )
  {
    path += QLatin1Char( '/' );
  }
  return path;
}

QString WebdavConnection::findWebdavRootForPath( const QString &path )
{
  QFileInfo fi( QDir::cleanPath( path ) );
  QDir dir( fi.isFile() ? fi.absolutePath() : fi.absoluteFilePath() );

  while ( !dir.exists( WEBDAV_CONFIGURATION_FILENAME ) )
  {
    if ( !dir.cdUp() )
    {
      return QString();
    }
  }

  return dir.absolutePath();
}

bool WebdavConnection::readWebdavConfig( const QString &rootPath, QVariantMap &outConfig, QString &errorMessage ) const
{
  QFile f( rootPath + QDir::separator() + WEBDAV_CONFIGURATION_FILENAME );
  if ( !f.open( QFile::ReadOnly ) )
  {
    errorMessage = tr( "Failed to read WebDAV config file." );
    return false;
  }

  const QByteArray raw = f.readAll();
  QJsonParseError e;
  const QJsonDocument doc = QJsonDocument::fromJson( raw, &e );

  if ( e.error != QJsonParseError::NoError || doc.isNull() )
  {
    errorMessage = tr( "Invalid WebDAV config JSON." );
    return false;
  }

  outConfig = doc.toVariant().toMap();
  return true;
}

QByteArray WebdavConnection::computeLocalSignature( const QString &rootPath ) const
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
    if ( name == WEBDAV_CONFIGURATION_FILENAME || name == WEBDAV_LOCK_FILENAME )
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

#undef WEBDAV_CONFIGURATION_FILENAME
#undef WEBDAV_LOCK_FILENAME
```


