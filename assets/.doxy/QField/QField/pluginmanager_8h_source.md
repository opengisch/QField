

# File pluginmanager.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**pluginmanager.h**](pluginmanager_8h.md)

[Go to the documentation of this file](pluginmanager_8h.md)


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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "pluginmodel.h"

#include <QObject>
#include <QQmlEngine>

class PluginInformation
{
    Q_GADGET

    Q_PROPERTY( QString uuid MEMBER uuid )
    Q_PROPERTY( QString name MEMBER name )
    Q_PROPERTY( QString description MEMBER description )
    Q_PROPERTY( QString author MEMBER author )
    Q_PROPERTY( QString homepage MEMBER homepage )
    Q_PROPERTY( QString icon MEMBER icon )
    Q_PROPERTY( QString version MEMBER version )
    Q_PROPERTY( bool bundled MEMBER bundled )

  public:
    PluginInformation( const QString &uuid = QString(), const QString &name = QString(), const QString &description = QString(), const QString &author = QString(), const QString &homepage = QString(), const QString &icon = QString() )
      : uuid( uuid )
      , name( name )
      , description( description )
      , author( author )
      , homepage( homepage )
      , icon( icon )
    {}
    ~PluginInformation() = default;

    QString uuid;
    QString name;
    QString description;
    QString author;
    QString homepage;
    QString icon;

    bool locallyAvailable = false;
    QString path;
    QString version;

    bool remotelyAvailable = false;
    bool updateAvailable = false;
    QString downloadLink;
    QString remoteVersion;

    bool bundled = false;
    bool trusted = false;
    bool enabled = false;
    bool configurable = false;
};

Q_DECLARE_METATYPE( PluginInformation )


class PluginManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY( PluginModel *pluginModel READ pluginModel NOTIFY pluginModelChanged )
    Q_PROPERTY( QList<PluginInformation> availableAppPlugins READ availableAppPlugins NOTIFY availableAppPluginsChanged )

  public:
    explicit PluginManager( QQmlEngine *engine );

    QList<PluginInformation> availableAppPlugins() const;

    void loadPlugin( const QString &pluginPath, const QString &pluginName, bool skipPermissionCheck = false, bool isProjectPlugin = false );

    void unloadPlugin( const QString &pluginPath );

    void unloadPlugins();

    Q_INVOKABLE void grantRequestedPluginPermission( bool permanent = false );

    Q_INVOKABLE void denyRequestedPluginPermission( bool permanent = false );

    Q_INVOKABLE void clearPluginPermissions();

    Q_INVOKABLE void enableAppPlugin( const QString &uuid );

    Q_INVOKABLE void disableAppPlugin( const QString &uuid );

    Q_INVOKABLE void configureAppPlugin( const QString &uuid );

    Q_INVOKABLE bool isAppPluginEnabled( const QString &uuid ) const;

    Q_INVOKABLE bool isAppPluginConfigurable( const QString &uuid ) const;

    void restoreAppPlugins();

    Q_INVOKABLE void installFromUrl( const QString &url );

    Q_INVOKABLE void installFromRepository( const QString &uuid );

    Q_INVOKABLE void uninstall( const QString &uuid );

    static QString findProjectPlugin( const QString &projectPath );

    PluginModel *pluginModel() const;


  signals:
    void pluginPermissionRequested( const QString &pluginName, bool isProjectPlugin );

    void appPluginEnabled( const QString &uuid );
    void appPluginDisabled( const QString &uuid );

    void installTriggered( const QString &name );
    void installProgress( double progress );
    void installEnded( const QString &uuid = QString(), const QString &error = QString() );

    void availableAppPluginsChanged();
    void pluginModelChanged();

  private slots:
    void handleWarnings( const QList<QQmlError> &warnings );
    void callPluginMethod( const QString &uuid, const QString &methodName ) const;

  private:
    QQmlEngine *mEngine = nullptr;
    QMap<QString, QPointer<QObject>> mLoadedPlugins;

    QString mPermissionRequestPluginPath;

    PluginModel *mPluginModel = nullptr;
};

#endif // PLUGINMANAGER_H
```


