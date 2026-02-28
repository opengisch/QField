

# File pluginmanager.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**pluginmanager.cpp**](pluginmanager_8cpp.md)

[Go to the documentation of this file](pluginmanager_8cpp.md)


```C++
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
  , mPluginModel( new PluginModel( this ) )
{
  connect( mEngine, &QQmlEngine::warnings, this, &PluginManager::handleWarnings );

  mPluginModel->refresh( false );

  connect( mPluginModel, &QAbstractItemModel::dataChanged, this, &PluginManager::availableAppPluginsChanged );
  connect( mPluginModel, &QAbstractItemModel::rowsInserted, this, &PluginManager::availableAppPluginsChanged );
  connect( mPluginModel, &QAbstractItemModel::rowsRemoved, this, &PluginManager::availableAppPluginsChanged );
}

QList<PluginInformation> PluginManager::availableAppPlugins() const
{
  return mPluginModel->availableAppPlugins();
}

void PluginManager::loadPlugin( const QString &pluginPath, const QString &pluginName, bool skipPermissionCheck, bool isProjectPlugin )
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
      emit pluginPermissionRequested( pluginName, isProjectPlugin );
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

    // Clear QML components cache of dynamically loaded items
    mEngine->clearComponentCache();
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
  QStringList checkedPluginUUids;
  QSettings settings;
  settings.beginGroup( QStringLiteral( "/qfield/plugins/" ) );
  const QStringList pluginKeys = settings.childGroups();
  for ( const QString &pluginKey : pluginKeys )
  {
    const QString uuid = settings.value( QStringLiteral( "%1/uuid" ).arg( pluginKey ) ).toString();
    checkedPluginUUids << uuid;
    const PluginInformation pluginInformation = mPluginModel->pluginInformation( uuid );
    if ( settings.value( QStringLiteral( "%1/userEnabled" ).arg( pluginKey ), false ).toBool() )
    {
      if ( mPluginModel->hasPluginInformation( uuid ) )
      {
        loadPlugin( mPluginModel->pluginInformation( uuid ).path, mPluginModel->pluginInformation( uuid ).name, pluginInformation.bundled || pluginInformation.remotelyAvailable );
      }
    }
  }
  settings.endGroup();

  const QList<PluginInformation> plugins = availableAppPlugins();
  for ( const PluginInformation &plugin : plugins )
  {
    if ( plugin.bundled && !checkedPluginUUids.contains( plugin.uuid ) )
    {
      loadPlugin( plugin.path, plugin.name, true );
    }
  }
}

void PluginManager::enableAppPlugin( const QString &uuid )
{
  if ( mPluginModel->hasPluginInformation( uuid ) )
  {
    const PluginInformation pluginInformation = mPluginModel->pluginInformation( uuid );
    if ( !mLoadedPlugins.contains( pluginInformation.path ) )
    {
      QSettings settings;
      QString pluginKey = pluginInformation.path;
      pluginKey.replace( QChar( '/' ), QChar( '_' ) );
      settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginKey ) );
      settings.setValue( QStringLiteral( "uuid" ), uuid );
      if ( settings.value( QStringLiteral( "permissionGranted" ), false ).toBool() || pluginInformation.bundled || pluginInformation.remotelyAvailable )
      {
        settings.setValue( QStringLiteral( "userEnabled" ), true );
      }
      settings.endGroup();

      loadPlugin( pluginInformation.path, pluginInformation.name, pluginInformation.bundled || pluginInformation.remotelyAvailable );

      if ( mLoadedPlugins.contains( pluginInformation.path ) )
      {
        callPluginMethod( uuid, "appWideEnabled" );
      }
    }
  }
}

void PluginManager::disableAppPlugin( const QString &uuid )
{
  callPluginMethod( uuid, "appWideDisabled" );
  if ( mPluginModel->hasPluginInformation( uuid ) )
  {
    if ( mLoadedPlugins.contains( mPluginModel->pluginInformation( uuid ).path ) )
    {
      QString pluginKey = mPluginModel->pluginInformation( uuid ).path;
      pluginKey.replace( QChar( '/' ), QChar( '_' ) );

      QSettings settings;
      settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginKey ) );
      settings.setValue( QStringLiteral( "uuid" ), uuid );
      settings.setValue( QStringLiteral( "userEnabled" ), false );
      settings.endGroup();

      unloadPlugin( mPluginModel->pluginInformation( uuid ).path );
    }
  }
}

void PluginManager::configureAppPlugin( const QString &uuid )
{
  callPluginMethod( uuid, QStringLiteral( "configure" ) );
}

bool PluginManager::isAppPluginEnabled( const QString &uuid ) const
{
  return mPluginModel->hasPluginInformation( uuid ) && mLoadedPlugins.contains( mPluginModel->pluginInformation( uuid ).path );
}

bool PluginManager::isAppPluginConfigurable( const QString &uuid ) const
{
  if ( mPluginModel->hasPluginInformation( uuid ) && mLoadedPlugins.contains( mPluginModel->pluginInformation( uuid ).path ) )
  {
    QByteArray normalizedSignature = QMetaObject::normalizedSignature( "configure()" );
    const int idx = mLoadedPlugins[mPluginModel->pluginInformation( uuid ).path]->metaObject()->indexOfSlot( normalizedSignature.constData() );
    return idx >= 0;
  }

  return false;
}

void PluginManager::installFromRepository( const QString &uuid )
{
  if ( mPluginModel->hasPluginInformation( uuid ) )
  {
    PluginInformation pluginInformation = mPluginModel->pluginInformation( uuid );
    if ( pluginInformation.remotelyAvailable && !pluginInformation.downloadLink.isEmpty() )
    {
      installFromUrl( pluginInformation.downloadLink );
    }
  }
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
  connect( reply, &QNetworkReply::downloadProgress, this, [this]( qint64 bytesReceived, qint64 bytesTotal ) {
    if ( bytesTotal != 0 )
    {
      emit installProgress( static_cast<double>( bytesReceived ) / bytesTotal );
    }
  } );

  connect( reply, &QNetworkReply::finished, this, [this, reply, url]() {
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

          bool pluginDirectoryWithinZip = false;
          QString pluginDirectoryName;
          QStringList zipFiles = QgsZipUtils::files( filePath );
          if ( !zipFiles.isEmpty() && !zipFiles.contains( "main.qml" ) && zipFiles.at( 0 ).indexOf( '/' ) >= 0 )
          {
            pluginDirectoryName = zipFiles.at( 0 ).mid( 0, zipFiles.at( 0 ).indexOf( '/' ) );
            pluginDirectoryWithinZip = true;
          }
          if ( zipFiles.contains( QStringLiteral( "%1main.qml" ).arg( !pluginDirectoryName.isEmpty() ? pluginDirectoryName + "/" : QString() ) ) )
          {
            // Insure no previous version is running
            disableAppPlugin( fileInfo.completeBaseName() );

            // Remove the .zip suffix as well as version information (e.g. myplugin-v1.0.zip becomes myplugin)
            if ( !pluginDirectoryWithinZip )
            {
              pluginDirectoryName = fileName.replace( QRegularExpression( "(-v?\\d+(\\.\\d+)*)?.zIP$", QRegularExpression::CaseInsensitiveOption ), QString() );
            }

            if ( mPluginModel->hasPluginInformation( pluginDirectoryName ) )
            {
              PluginInformation pluginInformation = mPluginModel->pluginInformation( pluginDirectoryName );
              if ( pluginInformation.remotelyAvailable && pluginInformation.downloadLink != url )
              {
                error = tr( "The requested plugin URL is present in the available plugins list, please install via its download button" );
                emit installEnded( QString(), error );
                return;
              }
            }

            QDir pluginDirectory = QStringLiteral( "%1/plugins/%2" ).arg( dataDir, pluginDirectoryName );
            if ( pluginDirectory.exists() )
            {
              pluginDirectory.removeRecursively();
            }
            pluginDirectory.mkpath( "." );
            if ( pluginDirectoryWithinZip )
            {
              pluginDirectory.cdUp();
            }
            if ( QgsZipUtils::unzip( filePath, pluginDirectory.absolutePath(), zipFiles, false ) )
            {
              file.remove();

              mPluginModel->refresh( false );
              emit installEnded( pluginDirectoryName );

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
  if ( mPluginModel->hasPluginInformation( uuid ) )
  {
    disableAppPlugin( uuid );

    QFileInfo fi( mPluginModel->pluginInformation( uuid ).path );
    fi.absoluteDir().removeRecursively();

    mPluginModel->refresh( false );
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
  if ( !mPluginModel->hasPluginInformation( uuid ) )
  {
    return;
  }

  const QString pluginPath = mPluginModel->pluginInformation( uuid ).path;
  if ( !mLoadedPlugins.contains( pluginPath ) )
  {
    return;
  }

  const QPointer<QObject> object = mLoadedPlugins[pluginPath];

  QByteArray normalizedSignature = QMetaObject::normalizedSignature( ( methodName + "()" ).toStdString().c_str() );
  const int methodIndex = object->metaObject()->indexOfSlot( normalizedSignature.constData() );

  if ( methodIndex != -1 )
  {
    QMetaObject::invokeMethod( object.data(), methodName.toStdString().c_str() );
  }
}

PluginModel *PluginManager::pluginModel() const
{
  return mPluginModel;
}
```


