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

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickWindow>

PluginManager::PluginManager( QQmlEngine *engine )
  : QObject( engine )
  , mEngine( engine )
{
}

void PluginManager::loadPlugin( const QString &pluginPath )
{
  if ( mLoadedPlugins.contains( pluginPath ) )
  {
    unloadPlugin( pluginPath );
  }

  QQmlComponent component( mEngine, pluginPath, this );
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
