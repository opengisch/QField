/***************************************************************************
 pluginmanager.h - PluginManager

 ---------------------
 begin                : 14.05.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "platformutilities.h"
#include "pluginmanager.h"
#include "qgsziputils.h"

#include <QDir>
#include <QFileInfo>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QRegularExpression>
#include <QSettings>
#include <qgsmessagelog.h>
#include <qgsnetworkaccessmanager.h>

PluginManager::PluginManager( QQmlEngine *engine )
  : QObject( engine )
  , mEngine( engine )
{
  connect( mEngine, &QQmlEngine::warnings, this, &PluginManager::handleWarnings );
  refreshAppPlugins();
}

void PluginManager::loadPlugin( const QString &pluginPath, const QString &pluginName, bool skipPermissionCheck )
{
  QSettings settings;
  QString pluginKey = pluginPath;
  pluginKey.replace( QChar( '/' ), QChar( '_' ) );
  settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginKey ) );
  const QString pluginUuid = settings.value( QStringLiteral( "uuid" ) ).toString();
  if ( !skipPermissionCheck )
  {
    const QStringList keys = settings.childKeys();
    if ( keys.contains( QStringLiteral( "permissionGranted" ) ) )
    {
      if ( !settings.value( QStringLiteral( "permissionGranted" ) ).toBool() )
      {
        return;
      }
    }
    else
    {
      mPermissionRequestPluginPath = pluginPath;
      emit pluginPermissionRequested( pluginName );
      return;
    }
  }
  settings.endGroup();

  if ( mLoadedPlugins.contains( pluginPath ) )
  {
    unloadPlugin( pluginPath );
  }

  // Bypass caching to insure updated QML content is loaded
  QUrl url = QUrl::fromLocalFile( pluginPath );
  url.setQuery( QStringLiteral( "t=%1" ).arg( QDateTime::currentSecsSinceEpoch() ) );

  QQmlComponent component( mEngine, url, this );
  if ( component.status() == QQmlComponent::Status::Error )
  {
    for ( const QQmlError &error : component.errors() )
    {
      QgsMessageLog::logMessage( error.toString(), QStringLiteral( "Plugin Manager" ), Qgis::MessageLevel::Critical );
    }
    return;
  }

  QObject *object = component.create( mEngine->rootContext() );
  mLoadedPlugins.insert( pluginPath, QPointer<QObject>( object ) );

  if ( !pluginUuid.isEmpty() )
  {
    emit appPluginEnabled( pluginUuid );
  }
}

void PluginManager::unloadPlugin( const QString &pluginPath )
{
  if ( mLoadedPlugins.contains( pluginPath ) )
  {
    QSettings settings;
    QString pluginKey = pluginPath;
    pluginKey.replace( QChar( '/' ), QChar( '_' ) );
    settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginKey ) );
    const QString pluginUuid = settings.value( QStringLiteral( "uuid" ) ).toString();
    settings.endGroup();

    if ( mLoadedPlugins[pluginPath] )
    {
      mLoadedPlugins[pluginPath]->deleteLater();
    }
    mLoadedPlugins.remove( pluginPath );

    if ( !pluginUuid.isEmpty() )
    {
      emit appPluginDisabled( pluginUuid );
    }
  }
}

void PluginManager::unloadPlugins()
{
  const QStringList loadedPluginPaths = mLoadedPlugins.keys();
  for ( const QString &loadedPluginPath : loadedPluginPaths )
  {
    unloadPlugin( loadedPluginPath );
  }
}

void PluginManager::handleWarnings( const QList<QQmlError> &warnings )
{
  for ( const QQmlError &warning : warnings )
  {
    if ( warning.url().isLocalFile() )
    {
      if ( mLoadedPlugins.keys().contains( warning.url().toLocalFile() ) )
      {
        QgsMessageLog::logMessage( warning.toString(), QStringLiteral( "Plugin Manager" ), Qgis::MessageLevel::Warning );
      }
    }
  }
}

void PluginManager::grantRequestedPluginPermission( bool permanent )
{
  QSettings settings;
  QString pluginKey = mPermissionRequestPluginPath;
  pluginKey.replace( QChar( '/' ), QChar( '_' ) );
  settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginKey ) );
  const QString uuid = settings.value( QStringLiteral( "uuid" ) ).toString();

  if ( permanent )
  {
    settings.setValue( QStringLiteral( "permissionGranted" ), true );
    if ( !uuid.isEmpty() )
    {
      settings.setValue( QStringLiteral( "userEnabled" ), true );
    }
  }

  settings.endGroup();
  loadPlugin( mPermissionRequestPluginPath, QString(), true );
  mPermissionRequestPluginPath.clear();
  if ( !uuid.isEmpty() )
  {
    callPluginMethod( uuid, "appWideEnabled" );
  }
}

void PluginManager::denyRequestedPluginPermission( bool permanent )
{
  QSettings settings;
  QString pluginKey = mPermissionRequestPluginPath;
  pluginKey.replace( QChar( '/' ), QChar( '_' ) );
  settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginKey ) );
  const QString pluginUuid = settings.value( QStringLiteral( "uuid" ) ).toString();
  if ( permanent )
  {
    settings.setValue( QStringLiteral( "permissionGranted" ), false );
  }
  settings.endGroup();

  if ( !pluginUuid.isEmpty() )
  {
    emit appPluginDisabled( pluginUuid );
  }

  mPermissionRequestPluginPath.clear();
}

void PluginManager::clearPluginPermissions()
{
  QSettings settings;
  settings.beginGroup( QStringLiteral( "/qfield/plugins/" ) );
  const QStringList pluginKeys = settings.childGroups();
  for ( const QString &pluginKey : pluginKeys )
  {
    if ( !settings.value( QStringLiteral( "%1/userEnabled" ).arg( pluginKey ), false ).toBool() )
    {
      settings.remove( QStringLiteral( "%1/permissionGranted" ).arg( pluginKey ) );
    }
  }
  settings.endGroup();
}

void PluginManager::restoreAppPlugins()
{
  QSettings settings;
  settings.beginGroup( QStringLiteral( "/qfield/plugins/" ) );
  const QStringList pluginKeys = settings.childGroups();
  for ( const QString &pluginKey : pluginKeys )
  {
    if ( settings.value( QStringLiteral( "%1/userEnabled" ).arg( pluginKey ), false ).toBool() )
    {
      const QString uuid = settings.value( QStringLiteral( "%1/uuid" ).arg( pluginKey ) ).toString();
      if ( mAvailableAppPlugins.contains( uuid ) )
      {
        loadPlugin( mAvailableAppPlugins[uuid].path(), mAvailableAppPlugins[uuid].name() );
      }
    }
  }
  settings.endGroup();
}

void PluginManager::refreshAppPlugins()
{
  mAvailableAppPlugins.clear();

  const QStringList dataDirs = PlatformUtilities::instance()->appDataDirs();
  for ( QString dataDir : dataDirs )
  {
    QDir pluginsDir( dataDir += QStringLiteral( "plugins" ) );
    const QList<QFileInfo> candidates = pluginsDir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot );
    for ( const QFileInfo &candidate : candidates )
    {
      const QString path = QStringLiteral( "%1/main.qml" ).arg( candidate.absoluteFilePath() );
      if ( QFileInfo::exists( path ) )
      {
        QString name = candidate.fileName();
        QString description;
        QString author;
        QString homepage;
        QString icon;
        QString version;

        const QString metadataPath = QStringLiteral( "%1/metadata.txt" ).arg( candidate.absoluteFilePath() );
        if ( QFileInfo::exists( metadataPath ) )
        {
          QSettings metadata( metadataPath, QSettings::IniFormat );
          name = metadata.value( "name", candidate.fileName() ).toString();
          description = metadata.value( "description" ).toString();
          author = metadata.value( "author" ).toString();
          homepage = metadata.value( "homepage" ).toString();
          if ( !homepage.isEmpty() )
          {
            // Only tolerate http(s) URLs
            const QUrl url( homepage );
            if ( !url.scheme().startsWith( QStringLiteral( "http" ) ) )
            {
              homepage.clear();
            }
          }
          if ( !metadata.value( "icon" ).toString().isEmpty() )
          {
            icon = QStringLiteral( "%1/%2" ).arg( candidate.absoluteFilePath(), metadata.value( "icon" ).toString() );
          }
          version = metadata.value( "version" ).toString();
        }
        mAvailableAppPlugins.insert( candidate.fileName(), PluginInformation( candidate.fileName(), name, description, author, homepage, icon, version, path ) );
      }
    }
  }

  emit availableAppPluginsChanged();
}

QList<PluginInformation> PluginManager::availableAppPlugins() const
{
  return mAvailableAppPlugins.values();
}

void PluginManager::enableAppPlugin( const QString &uuid )
{
  if ( mAvailableAppPlugins.contains( uuid ) )
  {
    const QString pluginPath = mAvailableAppPlugins[uuid].path();
    if ( !mLoadedPlugins.contains( pluginPath ) )
    {
      QSettings settings;
      QString pluginKey = pluginPath;
      pluginKey.replace( QChar( '/' ), QChar( '_' ) );
      settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginKey ) );
      settings.setValue( QStringLiteral( "uuid" ), uuid );
      if ( settings.value( QStringLiteral( "permissionGranted" ), false ).toBool() )
      {
        settings.setValue( QStringLiteral( "userEnabled" ), true );
      }
      settings.endGroup();

      loadPlugin( pluginPath, mAvailableAppPlugins[uuid].name() );

      if ( mLoadedPlugins.contains( pluginPath ) )
      {
        callPluginMethod( uuid, "appWideEnabled" );
      }
    }
  }
}

void PluginManager::disableAppPlugin( const QString &uuid )
{
  callPluginMethod( uuid, "appWideDisabled" );
  if ( mAvailableAppPlugins.contains( uuid ) )
  {
    if ( mLoadedPlugins.contains( mAvailableAppPlugins[uuid].path() ) )
    {
      QSettings settings;
      QString pluginKey = mAvailableAppPlugins[uuid].path();
      pluginKey.replace( QChar( '/' ), QChar( '_' ) );
      settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginKey ) );
      settings.setValue( QStringLiteral( "userEnabled" ), false );
      settings.endGroup();

      unloadPlugin( mAvailableAppPlugins[uuid].path() );
    }
  }
}

void PluginManager::configureAppPlugin( const QString &uuid )
{
  callPluginMethod( uuid, QStringLiteral( "configure" ) );
}

bool PluginManager::isAppPluginEnabled( const QString &uuid ) const
{
  return mAvailableAppPlugins.contains( uuid ) && mLoadedPlugins.contains( mAvailableAppPlugins[uuid].path() );
}

bool PluginManager::isAppPluginConfigurable( const QString &uuid ) const
{
  if ( mAvailableAppPlugins.contains( uuid ) && mLoadedPlugins.contains( mAvailableAppPlugins[uuid].path() ) )
  {
    const char *normalizedSignature = QMetaObject::normalizedSignature( "configure()" );
    const int idx = mLoadedPlugins[mAvailableAppPlugins[uuid].path()]->metaObject()->indexOfSlot( normalizedSignature );
    return idx >= 0;
  }

  return false;
}

void PluginManager::installFromUrl( const QString &url )
{
  QString sanitizedUrl = url.trimmed();
  if ( sanitizedUrl.isEmpty() )
    return;

  if ( !sanitizedUrl.contains( QRegularExpression( "^([a-z][a-z0-9+\\-.]*):" ) ) )
  {
    // Prepend HTTPS when the URL scheme is missing instead of assured failure
    sanitizedUrl = QStringLiteral( "https://%1" ).arg( sanitizedUrl );
  }

  QgsNetworkAccessManager *manager = QgsNetworkAccessManager::instance();
  QNetworkRequest request( ( QUrl( sanitizedUrl ) ) );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy );

  emit installTriggered( request.url().fileName() );

  QNetworkReply *reply = manager->get( request );
  connect( reply, &QNetworkReply::downloadProgress, this, [=]( int bytesReceived, int bytesTotal ) {
    if ( bytesTotal != 0 )
    {
      emit installProgress( static_cast<double>( bytesReceived ) / bytesTotal );
    }
  } );

  connect( reply, &QNetworkReply::finished, this, [=]() {
    const QString dataDir = PlatformUtilities::instance()->appDataDirs().at( 0 );
    QString error;
    if ( !dataDir.isEmpty() && reply->error() == QNetworkReply::NoError )
    {
      QString fileName = reply->url().fileName();
      QString contentDisposition = reply->header( QNetworkRequest::ContentDispositionHeader ).toString();
      if ( !contentDisposition.isEmpty() )
      {
        QRegularExpression rx( QStringLiteral( "filename=\"?([^\";]*)\"?" ) );
        QRegularExpressionMatch match = rx.match( contentDisposition );
        if ( match.hasMatch() )
        {
          fileName = match.captured( 1 );
        }
      }

      QFileInfo fileInfo = QFileInfo( fileName );
      const QString fileSuffix = fileInfo.suffix().toLower();
      if ( fileSuffix == QLatin1String( "zip" ) )
      {
        QString filePath = QStringLiteral( "%1/plugins/%2" ).arg( dataDir, fileName );
        QDir( QFileInfo( filePath ).absolutePath() ).mkpath( "." );
        QFile file( filePath );
        if ( file.open( QIODevice::WriteOnly ) )
        {
          const QByteArray data = reply->readAll();
          file.write( data );
          file.close();

          QStringList zipFiles = QgsZipUtils::files( filePath );
          if ( zipFiles.contains( QStringLiteral( "main.qml" ) ) )
          {
            // Insure no previous version is running
            disableAppPlugin( fileInfo.completeBaseName() );

            // Remove the .zip suffix as well as version information (e.g. myplugin-v1.0.zip becomes myplugin)
            const QString pluginDirectoryName = fileName.replace( QRegularExpression( "(-v?\\d+(\\.\\d+)*)?.zIP$", QRegularExpression::CaseInsensitiveOption ), QString() );
            QDir pluginDirectory = QStringLiteral( "%1/plugins/%2" ).arg( dataDir, pluginDirectoryName );
            if ( pluginDirectory.exists() )
            {
              pluginDirectory.removeRecursively();
            }
            pluginDirectory.mkpath( "." );
            if ( QgsZipUtils::unzip( filePath, pluginDirectory.absolutePath(), zipFiles, false ) )
            {
              file.remove();

              refreshAppPlugins();
              emit installEnded( pluginDirectory.dirName() );

              return;
            }
            else
            {
              pluginDirectory.removeRecursively();
              error = tr( "The downloaded zip file could not be decompressed" );
            }
          }
          else
          {
            error = tr( "The downloaded zip file does not contain the required main.qml plugin file" );
          }
          file.remove();
        }
        else
        {
          error = tr( "Can't save the downloaded file" );
        }
      }
      else
      {
        error = tr( "Download file is not an zipped plugin" );
      }
    }
    else
    {
      error = tr( "Network error" );
    }

    emit installEnded( QString(), error );
  } );
}

void PluginManager::uninstall( const QString &uuid )
{
  if ( mAvailableAppPlugins.contains( uuid ) )
  {
    disableAppPlugin( uuid );

    QFileInfo fi( mAvailableAppPlugins[uuid].path() );
    fi.absoluteDir().removeRecursively();

    refreshAppPlugins();
  }
}

QString PluginManager::findProjectPlugin( const QString &projectPath )
{
  const QFileInfo fi( projectPath );
  const QString completeBaseName = fi.completeBaseName();
  QStringList possiblePluginPaths = QStringList() << QStringLiteral( "%1/%2.qml" ).arg( fi.absolutePath(), completeBaseName );
  // Cloud-served projects come with a _cloud suffix, take that into account
  if ( completeBaseName.endsWith( "_qfield" ) )
  {
    possiblePluginPaths << QStringLiteral( "%1/%2.qml" ).arg( fi.absolutePath(), fi.completeBaseName().mid( 0, completeBaseName.size() - 7 ) );
  }
  for ( QString &possiblePluginPath : possiblePluginPaths )
  {
    if ( QFileInfo::exists( possiblePluginPath ) )
    {
      return possiblePluginPath;
    }
  }
  return QString();
}

void PluginManager::callPluginMethod( const QString &uuid, const QString &methodName ) const
{
  if ( !mAvailableAppPlugins.contains( uuid ) )
  {
    return;
  }

  const QString pluginPath = mAvailableAppPlugins[uuid].path();
  if ( !mLoadedPlugins.contains( pluginPath ) )
  {
    return;
  }

  const QPointer<QObject> object = mLoadedPlugins[pluginPath];

  const char *normalizedSignature = QMetaObject::normalizedSignature( ( methodName + "()" ).toStdString().c_str() );
  const int methodIndex = object->metaObject()->indexOfSlot( normalizedSignature );

  if ( methodIndex != -1 )
  {
    QMetaObject::invokeMethod( object.data(), methodName.toStdString().c_str() );
  }
}
