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
}

void PluginManager::loadPlugin( const QString &pluginPath, const QString &pluginName, bool skipPermissionCheck )
{
  if ( !skipPermissionCheck )
  {
    QSettings settings;
    settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( pluginPath ) );
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
    settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( mPermissionRequestPluginPath ) );
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
    settings.beginGroup( QStringLiteral( "/qfield/plugins/%1" ).arg( mPermissionRequestPluginPath ) );
    settings.setValue( QStringLiteral( "permissionGranted" ), false );
    settings.endGroup();
  }

  mPermissionRequestPluginPath.clear();
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
