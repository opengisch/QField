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

  public:
    PluginInformation( const QString &name, const QString &description, const QString &author, const QString &icon, const QString &path )
      : mName( name )
      , mDescription( description )
      , mAuthor( author )
      , mIcon( icon )
      , mPath( path )
    {}
    ~PluginInformation() = default;

    QString name() const { return mName; }
    QString description() const { return mDescription; }
    QString author() const { return mAuthor; }
    QString icon() const { return mIcon; }
    QString path() const { return mPath; }

  private:
    QString mName;
    QString mDescription;
    QString mAuthor;
    QString mIcon;
    QString mPath;
};

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

    void refreshAppPlugins();
    QList<PluginInformation> availableAppPlugins();

    static QString findProjectPlugin( const QString &projectPath );

  signals:
    void pluginPermissionRequested( const QString &pluginName );

  private slots:
    void handleWarnings( const QList<QQmlError> &warnings );

  private:
    QQmlEngine *mEngine = nullptr;
    QMap<QString, QPointer<QObject>> mLoadedPlugins;

    QString mPermissionRequestPluginPath;

    QList<PluginInformation> mAvailableAppPlugins;
};

#endif // PLUGINMANAGER_H
