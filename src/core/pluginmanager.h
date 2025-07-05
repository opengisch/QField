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

/**
 * \ingroup core
 */
struct PluginInformation
{
    QString uuid;
    bool enabled;
    bool configurable;
    bool locallyAvailable;
    bool remotelyAvailable;
    QString name;
    QString description;
    QString author;
    QString homepage;
    QString icon;
    QString version;
    QString path;

    PluginInformation( const QString &uuid = QString(), const QString &name = QString(), const QString &description = QString(), const QString &author = QString(), const QString &homepage = QString(), const QString &icon = QString(), const QString &version = QString(), const QString &path = QString(), const bool locallyAvailable = false, const bool remotelyAvailable = false )
      : uuid( uuid )
      , name( name )
      , description( description )
      , author( author )
      , homepage( homepage )
      , icon( icon )
      , version( version )
      , path( path )
      , locallyAvailable( locallyAvailable )
      , remotelyAvailable( remotelyAvailable )
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

    Q_PROPERTY( QList<PluginInformation> availableAppPlugins READ availableAppPlugins NOTIFY availableAppPluginsChanged )

  public:
    explicit PluginManager( QQmlEngine *engine );
    ~PluginManager() override = default;

    void loadPlugin( const QString &pluginPath, const QString &pluginName, bool skipPermissionCheck = false, bool isProjectPlugin = false );
    void unloadPlugin( const QString &pluginPath );

    void unloadPlugins();

    Q_INVOKABLE void grantRequestedPluginPermission( bool permanent = false );
    Q_INVOKABLE void denyRequestedPluginPermission( bool permanent = false );

    Q_INVOKABLE void clearPluginPermissions();

    QList<PluginInformation> availableAppPlugins() const;

    Q_INVOKABLE void enableAppPlugin( const QString &uuid );
    Q_INVOKABLE void disableAppPlugin( const QString &uuid );
    Q_INVOKABLE void configureAppPlugin( const QString &uuid );

    Q_INVOKABLE bool isAppPluginEnabled( const QString &uuid ) const;
    Q_INVOKABLE bool isAppPluginConfigurable( const QString &uuid ) const;

    void refreshAppPlugins();
    void restoreAppPlugins();

    Q_INVOKABLE void installFromUrl( const QString &url );
    Q_INVOKABLE void uninstall( const QString &uuid );

    static QString findProjectPlugin( const QString &projectPath );

  signals:
    void pluginPermissionRequested( const QString &pluginName, bool isProjectPlugin );

    void availableAppPluginsChanged();
    void appPluginEnabled( const QString &uuid );
    void appPluginDisabled( const QString &uuid );

    void installTriggered( const QString &name );
    void installProgress( double progress );
    void installEnded( const QString &uuid = QString(), const QString &error = QString() );


  private slots:
    void handleWarnings( const QList<QQmlError> &warnings );
    void callPluginMethod( const QString &uuid, const QString &methodName ) const;

  private:
    QQmlEngine *mEngine = nullptr;
    QMap<QString, QPointer<QObject>> mLoadedPlugins;

    QString mPermissionRequestPluginPath;

    QMap<QString, PluginInformation> mAvailableAppPlugins;
};

#endif // PLUGINMANAGER_H
