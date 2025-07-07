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

/**
 * \ingroup core
 */
struct PluginInformation
{
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

    bool enabled = false;
    bool configurable = false;

    PluginInformation( const QString &uuid = QString(), const QString &name = QString(), const QString &description = QString(), const QString &author = QString(), const QString &homepage = QString(), const QString &icon = QString() )
      : uuid( uuid )
      , name( name )
      , description( description )
      , author( author )
      , homepage( homepage )
      , icon( icon )
    {}
    ~PluginInformation() = default;
};

Q_DECLARE_METATYPE( PluginInformation )

/**
 * \ingroup core
 */
class PluginManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY( PluginModel *pluginModel READ pluginModel NOTIFY pluginModelChanged )

  public:
    /**
     * Constructs a PluginManager with the given \a engine.
     */
    explicit PluginManager( QQmlEngine *engine );

    /**
     * Loads a plugin from \a pluginPath and registers it under \a pluginName.
     * Optionally skips permission checks and marks it as a project plugin.
     */
    void loadPlugin( const QString &pluginPath, const QString &pluginName, bool skipPermissionCheck = false, bool isProjectPlugin = false );

    /**
     * Unloads the plugin loaded from \a pluginPath.
     */
    void unloadPlugin( const QString &pluginPath );

    /**
     * Unloads all currently loaded plugins.
     */
    void unloadPlugins();

    /**
     * Grants permission to the last plugin that requested it.
     * If \a permanent is true, saves this choice.
     */
    Q_INVOKABLE void grantRequestedPluginPermission( bool permanent = false );

    /**
     * Denies permission to the last plugin that requested it.
     * If \a permanent is true, saves this choice.
     */
    Q_INVOKABLE void denyRequestedPluginPermission( bool permanent = false );

    /**
     * Clears all saved plugin permissions except those explicitly enabled by the user.
     */
    Q_INVOKABLE void clearPluginPermissions();

    /**
     * Enables the application plugin identified by \a uuid.
     */
    Q_INVOKABLE void enableAppPlugin( const QString &uuid );

    /**
     * Disables the application plugin identified by \a uuid.
     */
    Q_INVOKABLE void disableAppPlugin( const QString &uuid );

    /**
     * Opens the configuration interface for the plugin identified by \a uuid.
     */
    Q_INVOKABLE void configureAppPlugin( const QString &uuid );

    /**
     * Returns true if the application plugin with \a uuid is currently enabled.
     */
    Q_INVOKABLE bool isAppPluginEnabled( const QString &uuid ) const;

    /**
     * Returns true if the application plugin with \a uuid is configurable.
     */
    Q_INVOKABLE bool isAppPluginConfigurable( const QString &uuid ) const;

    /**
     * Restores and loads previously enabled application plugins.
     */
    void restoreAppPlugins();

    /**
     * Installs a plugin from the given \a url.
     */
    Q_INVOKABLE void installFromUrl( const QString &url );

    /**
     * Uninstalls the application plugin identified by \a uuid.
     */
    Q_INVOKABLE void uninstall( const QString &uuid );

    /**
     * Finds and returns the plugin path corresponding to a project at \a projectPath.
     * Returns an empty string if none is found.
     */
    static QString findProjectPlugin( const QString &projectPath );

    /**
     * Returns the plugin model used by this manager.
     */
    PluginModel *pluginModel() const;


  signals:
    void pluginPermissionRequested( const QString &pluginName, bool isProjectPlugin );

    void appPluginEnabled( const QString &uuid );
    void appPluginDisabled( const QString &uuid );

    void installTriggered( const QString &name );
    void installProgress( double progress );
    void installEnded( const QString &uuid = QString(), const QString &error = QString() );


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
