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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QQmlEngine>

class PluginManager : public QObject
{
    Q_OBJECT

  public:
    explicit PluginManager( QQmlEngine *engine );
    ~PluginManager() override = default;

    void loadPlugin( const QString &pluginPath, const QString &pluginName, bool skipPermissionCheck = false );
    void unloadPlugin( const QString &pluginPath );

    Q_INVOKABLE void grantRequestedPluginPermission( bool permanent = false );
    Q_INVOKABLE void denyRequestedPluginPermission( bool permanent = false );

    static QString findProjectPlugin( const QString &projectPath );

  signals:
    void pluginPermissionRequested( const QString &pluginName );

  private:
    QQmlEngine *mEngine = nullptr;
    QMap<QString, QPointer<QObject>> mLoadedPlugins;

    QString mPermissionRequestPluginPath;
};

#endif // PLUGINMANAGER_H
