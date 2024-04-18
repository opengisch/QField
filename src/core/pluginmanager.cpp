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

#include <QDir>
#include <QFileInfo>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QSettings>
#include <qgsmessagelog.h>

PluginManager::PluginManager( QQmlEngine *engine )
  : QObject( engine )
  , mEngine( engine )
{
  connect( mEngine, &QQmlEngine::warnings, this, &PluginManager::handleWarnings );
  refreshAppPlugins();
}

void PluginManager::loadPlugin( const QString &pluginPath, const QString &pluginName, bool skipPermissionCheck )
{
  if ( !skipPermissionCheck )
  {
    QSettings settings;
    QString pluginKey = pluginPath;
    pluginKey.replace( QChar( '/' ), QChar( '_' ) );
    settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginKey ) );
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

  if ( mLoadedPlugins.contains( pluginPath ) )
  {
    unloadPlugin( pluginPath );
  }

  QQmlComponent component( mEngine, pluginPath, this );
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
}

void PluginManager::unloadPlugin( const QString &pluginPath )
{
  if ( mLoadedPlugins.contains( pluginPath ) )
  {
    if ( mLoadedPlugins[pluginPath] )
    {
      mLoadedPlugins[pluginPath]->deleteLater();
      mLoadedPlugins.remove( pluginPath );
    }
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
  if ( permanent )
  {
    QSettings settings;
    QString pluginKey = mPermissionRequestPluginPath;
    pluginKey.replace( QChar( '/' ), QChar( '_' ) );
    settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginKey ) );
    settings.setValue( QStringLiteral( "permissionGranted" ), true );
    settings.endGroup();
  }

  loadPlugin( mPermissionRequestPluginPath, QString(), true );
  mPermissionRequestPluginPath.clear();
}

void PluginManager::denyRequestedPluginPermission( bool permanent )
{
  if ( permanent )
  {
    QSettings settings;
    QString pluginKey = mPermissionRequestPluginPath;
    pluginKey.replace( QChar( '/' ), QChar( '_' ) );
    settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginKey ) );
    settings.setValue( QStringLiteral( "permissionGranted" ), false );
    settings.endGroup();
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
    settings.remove( QStringLiteral( "%1/permissionGranted" ).arg( pluginKey ) );
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
        QString icon;

        const QString metadataPath = QStringLiteral( "%1/metadata.txt" ).arg( candidate.absoluteFilePath() );
        if ( QFileInfo::exists( metadataPath ) )
        {
          QSettings metadata( metadataPath, QSettings::IniFormat );
          name = metadata.value( "name", candidate.fileName() ).toString();
          description = metadata.value( "description" ).toString();
          author = metadata.value( "author" ).toString();
          if ( !metadata.value( "icon" ).toString().isEmpty() )
          {
            icon = QStringLiteral( "%1/%2" ).arg( candidate.absoluteFilePath(), metadata.value( "icon" ).toString() );
          }
        }
        mAvailableAppPlugins.insert( candidate.fileName(), PluginInformation( candidate.fileName(), name, description, author, icon, path ) );
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
    if ( !mLoadedPlugins.contains( mAvailableAppPlugins[uuid].path() ) )
    {
      QSettings settings;
      QString pluginKey = mAvailableAppPlugins[uuid].path();
      pluginKey.replace( QChar( '/' ), QChar( '_' ) );
      settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginKey ) );
      settings.setValue( QStringLiteral( "permissionGranted" ), true );
      settings.setValue( QStringLiteral( "userEnabled" ), true );
      settings.setValue( QStringLiteral( "uuid" ), uuid );
      settings.endGroup();

      loadPlugin( mAvailableAppPlugins[uuid].path(), mAvailableAppPlugins[uuid].name() );
    }
  }
}

void PluginManager::disableAppPlugin( const QString &uuid )
{
  if ( mAvailableAppPlugins.contains( uuid ) )
  {
    if ( mLoadedPlugins.contains( mAvailableAppPlugins[uuid].path() ) )
    {
      QSettings settings;
      QString pluginKey = mAvailableAppPlugins[uuid].path();
      pluginKey.replace( QChar( '/' ), QChar( '_' ) );
      settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginKey ) );
      settings.setValue( QStringLiteral( "permissionGranted" ), false );
      settings.setValue( QStringLiteral( "userEnabled" ), false );
      settings.endGroup();

      unloadPlugin( mAvailableAppPlugins[uuid].path() );
    }
  }
}

bool PluginManager::isAppPluginEnabled( const QString &uuid ) const
{
  return mAvailableAppPlugins.contains( uuid ) && mLoadedPlugins.contains( mAvailableAppPlugins[uuid].path() );
}

QString PluginManager::findProjectPlugin( const QString &projectPath )
{
  const QFileInfo fi( projectPath );
  const QString pluginPath = QStringLiteral( "%1/%2.qml" ).arg( fi.absolutePath(), fi.completeBaseName() );
  if ( QFileInfo::exists( pluginPath ) )
  {
    return pluginPath;
  }
  return QString();
}
