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

class PluginInformation
{
    Q_GADGET

    Q_PROPERTY( QString uuid READ uuid )
    Q_PROPERTY( QString name READ name )
    Q_PROPERTY( QString description READ description )
    Q_PROPERTY( QString author READ author )
    Q_PROPERTY( QString icon READ icon )

  public:
    PluginInformation( const QString &uuid = QString(), const QString &name = QString(), const QString &description = QString(), const QString &author = QString(), const QString &icon = QString(), const QString &path = QString() )
      : mUuid( uuid )
      , mName( name )
      , mDescription( description )
      , mAuthor( author )
      , mIcon( icon )
      , mPath( path )
    {}
    ~PluginInformation() = default;

    QString uuid() const { return mUuid; }
    QString name() const { return mName; }
    QString description() const { return mDescription; }
    QString author() const { return mAuthor; }
    QString icon() const { return mIcon; }
    QString path() const { return mPath; }

  private:
    QString mUuid;
    QString mName;
    QString mDescription;
    QString mAuthor;
    QString mIcon;
    QString mPath;
};

Q_DECLARE_METATYPE( PluginInformation )

class PluginManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QList<PluginInformation> availableAppPlugins READ availableAppPlugins NOTIFY availableAppPluginsChanged )

  public:
    explicit PluginManager( QQmlEngine *engine );
    ~PluginManager() override = default;

    void loadPlugin( const QString &pluginPath, const QString &pluginName, bool skipPermissionCheck = false );
    void unloadPlugin( const QString &pluginPath );

    Q_INVOKABLE void grantRequestedPluginPermission( bool permanent = false );
    Q_INVOKABLE void denyRequestedPluginPermission( bool permanent = false );

    Q_INVOKABLE void clearProjectPluginPermissions();

    QList<PluginInformation> availableAppPlugins() const;

    Q_INVOKABLE void enableAppPlugin( const QString &uuid );
    Q_INVOKABLE void disableAppPlugin( const QString &uuid );
    Q_INVOKABLE bool isAppPluginEnabled( const QString &uuid ) const;

    void refreshAppPlugins();
    void restoreAppPlugins();

    Q_INVOKABLE void installFromUrl( const QString &url );

    static QString findProjectPlugin( const QString &projectPath );

  signals:
    void pluginPermissionRequested( const QString &pluginName );
    void availableAppPluginsChanged();

    void installTriggered( const QString &name );
    void installProgress( double progress );
    void installEnded( const QString &uuid = QString(), const QString &error = QString() );

  private slots:
    void handleWarnings( const QList<QQmlError> &warnings );

  private:
    QQmlEngine *mEngine = nullptr;
    QMap<QString, QPointer<QObject>> mLoadedPlugins;

    QString mPermissionRequestPluginPath;

    QMap<QString, PluginInformation> mAvailableAppPlugins;
};

#endif // PLUGINMANAGER_H
