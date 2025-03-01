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

#include <QDirIterator>
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
  if ( mUsername == username )
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
  if ( mPassword == password )
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
    mAvailablePaths.sort();

    mIsFetchingAvailablePaths = false;
    emit isFetchingAvailablePathsChanged();
    emit availablePathsChanged();
  }
  else if ( mIsImportingPath || mIsDownloadingPath )
  {
    if ( !list.isEmpty() )
    {
      applyStoredPassword();

      QDir localDir( mProcessLocalPath );
      for ( const QWebdavItem &item : list )
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
              mWebdavItems << item;
              mBytesTotal += item.size();
            }
          }
          else
          {
            mWebdavItems << item;
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

      mIsUploadingPath = false;
      emit isUploadingPathChanged();
    }
    else
    {
      // Filter files to  upload
      applyStoredPassword();

      QStringList remoteDirs;
      for ( const QWebdavItem &item : list )
      {
        if ( item.isDir() )
        {
          remoteDirs << item.path();
        }
        else
        {
          QFileInfo fileInfo( mProcessLocalPath + item.path().mid( mProcessRemotePath.size() ) );
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

      mWebdavMkDirs.clear();
      for ( const QFileInfo &fileInfo : mLocalItems )
      {
        // Insure the path exists remotely
        QString remoteDir = mProcessRemotePath + fileInfo.absolutePath().mid( mProcessLocalPath.size() ).replace( QDir::separator(), "/" );
        if ( !remoteDirs.contains( remoteDir ) && !mWebdavMkDirs.contains( remoteDir ) )
        {
          const QStringList remoteDirParts = remoteDir.mid( mProcessRemotePath.size() ).split( "/", Qt::SkipEmptyParts );
          remoteDir = mProcessRemotePath;
          for ( const QString &part : remoteDirParts )
          {
            remoteDir += part + "/";
            if ( !remoteDirs.contains( remoteDir ) && !mWebdavMkDirs.contains( remoteDir ) )
            {
              mWebdavMkDirs << remoteDir;
            }
          }
        }

        mBytesTotal += fileInfo.size();
      }
      emit progressChanged();

      putLocalItems();
    }
  }
}

void WebdavConnection::getWebdavItems()
{
  if ( !mWebdavItems.isEmpty() )
  {
    const QString itemPath = mWebdavItems.first().path();
    const QDateTime itemLastModified = mWebdavItems.first().lastModified();
    QNetworkReply *reply = mWebdavConnection.get( itemPath );
    QTemporaryFile *temporaryFile = new QTemporaryFile( reply );
    temporaryFile->setFileTemplate( QStringLiteral( "%1%2.XXXXXXXXXXXX" ).arg( mProcessLocalPath, itemPath.mid( mProcessRemotePath.size() ) ) );
    temporaryFile->open();

    connect( reply, &QNetworkReply::downloadProgress, this, [=]( int bytesReceived, int bytesTotal ) {
      mCurrentBytesProcessed = bytesReceived;
      emit progressChanged();

      temporaryFile->write( reply->readAll() );
    } );

    connect( reply, &QNetworkReply::finished, this, [=]() {
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

QVariantMap WebdavConnection::importHistory()
{
  // Collect imported folders
  QMap<QString, QStringList> importedFolders;
  QDirIterator it( QStringLiteral( "%1/Imported Projects/" ).arg( PlatformUtilities::instance()->applicationDirectory() ), { QStringLiteral( "qfield_webdav_configuration.json" ) }, QDir::Filter::Files, QDirIterator::Subdirectories );
  while ( it.hasNext() )
  {
    QFile webdavConfigurationFile( it.next() );
    webdavConfigurationFile.open( QFile::ReadOnly );
    QJsonDocument jsonDocument = QJsonDocument::fromJson( webdavConfigurationFile.readAll() );
    if ( !jsonDocument.isEmpty() )
    {
      QVariantMap webdavConfiguration = jsonDocument.toVariant().toMap();
      importedFolders[QStringLiteral( "%1 - %2" ).arg( webdavConfiguration["url"].toString(), webdavConfiguration["username"].toString() )] << webdavConfiguration["remote_path"].toString();
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
      details["importPaths"] = importedFolders.contains( QStringLiteral( "%1 - %2" ).arg( decodedUrl, decodedUser ) ) ? importedFolders[QStringLiteral( "%1 - %2" ).arg( decodedUrl, decodedUser )] : QStringList();
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
    const QString dirPath = mWebdavMkDirs.first();

    QNetworkReply *reply = mWebdavConnection.mkdir( dirPath );

    connect( reply, &QNetworkReply::finished, this, [=]() {
      mBytesProcessed += mCurrentBytesProcessed;
      mCurrentBytesProcessed = 0;
      emit progressChanged();
      if ( reply->error() != QNetworkReply::NoError )
      {
        mLastError = tr( "Failed to upload file %1 due to network error (%2)" ).arg( dirPath ).arg( reply->error() );
      }

      mWebdavMkDirs.removeFirst();
      putLocalItems();
    } );
  }
  else if ( !mLocalItems.isEmpty() )
  {
    const QString itemPath = mLocalItems.first().absoluteFilePath();
    const QString remoteItemPath = mProcessRemotePath + itemPath.mid( mProcessLocalPath.size() ).replace( QDir::separator(), "/" );

    QFile *file = new QFile( itemPath );
    file->open( QFile::ReadOnly );
    QNetworkReply *reply = mWebdavConnection.put( remoteItemPath, file );
    file->setParent( reply );

    connect( reply, &QNetworkReply::uploadProgress, this, [=]( int bytesSent, int bytesTotal ) {
      mCurrentBytesProcessed = bytesSent;
      emit progressChanged();
    } );

    connect( reply, &QNetworkReply::finished, this, [=]() {
      mBytesProcessed += mCurrentBytesProcessed;
      mCurrentBytesProcessed = 0;
      emit progressChanged();
      if ( reply->error() != QNetworkReply::NoError )
      {
        mLastError = tr( "Failed to upload file %1 due to network error (%2)" ).arg( remoteItemPath ).arg( reply->error() );
      }

      mWebdavLastModified << remoteItemPath;

      mLocalItems.removeFirst();
      putLocalItems();
      reply->deleteLater();
    } );
  }
  else
  {
    if ( mIsUploadingPath )
    {
      if ( !mWebdavLastModified.isEmpty() )
      {
        mWebdavDirParser.listDirectory( &mWebdavConnection, mProcessRemotePath, true );
      }
      else
      {
        mIsUploadingPath = false;
        emit isUploadingPathChanged();
      }
    }
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

  mProcessRemotePath = remotePath;
  mProcessLocalPath = QDir::cleanPath( localPath + QDir::separator() + localFolder ) + QDir::separator();

  mWebdavItems.clear();
  mBytesProcessed = 0;
  mBytesTotal = 0;
  emit progressChanged();

  mIsImportingPath = true;
  emit isImportingPathChanged();

  mWebdavDirParser.listDirectory( &mWebdavConnection, mProcessRemotePath, true );
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
  for ( const QString &localPath : localPaths )
  {
    QFileInfo fi( QDir::cleanPath( localPath ) );
    QDir dir( fi.isFile() ? fi.absolutePath() : fi.absoluteFilePath() );

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

        mProcessLocalPath = QDir::cleanPath( fi.isFile() ? fi.absolutePath() : fi.absoluteFilePath() ) + QDir::separator();

        mWebdavLastModified.clear();

        if ( fi.isDir() )
        {
          mLocalItems.clear();
          QDirIterator it( mProcessLocalPath, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories );
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

        mBytesProcessed = 0;
        mBytesTotal = 0;
        emit progressChanged();

        mIsUploadingPath = true;
        emit isUploadingPathChanged();
        const QUrl url( mUrl );
        emit confirmationRequested( url.host(), mUsername );
      }
    }
  }
}

void WebdavConnection::confirmRequest()
{
  if ( mIsDownloadingPath || mIsUploadingPath )
  {
    setupConnection();
    mWebdavDirParser.listDirectory( &mWebdavConnection, mProcessRemotePath, true );
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
