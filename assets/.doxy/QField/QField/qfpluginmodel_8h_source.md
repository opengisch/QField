

# File qfpluginmodel.h

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qfpluginmodel.h**](qfpluginmodel_8h.md)

[Go to the documentation of this file](qfpluginmodel_8h.md)


```C++
/***************************************************************************
 qfpluginmodel.h - QfPluginModel

 ---------------------
 begin                : June 2025
 copyright            : (C) 2025 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFPLUGINMODEL_H
#define QFPLUGINMODEL_H


#include <QAbstractListModel>
#include <QFileInfo>
#include <QSortFilterProxyModel>

class QfPluginManager;
class QfPluginInformation;

class QfPluginModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( bool isRefreshing READ isRefreshing NOTIFY isRefreshingChanged )

  public:
    enum PluginRoles
    {
      UuidRole = Qt::UserRole + 1,
      TrustedRole,
      BundledRole,
      EnabledRole,
      ConfigurableRole,
      NameRole,
      DescriptionRole,
      AuthorRole,
      HomepageRole,
      IconRole,
      VersionRole,
      InstalledLocallyRole,
      AvailableRemotelyRole,
      AvailableUpdateRole,
    };
    Q_ENUM( PluginRoles )

    
    explicit QfPluginModel( QfPluginManager *manager, QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    QHash<int, QByteArray> roleNames() const override;

    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    QList<QfPluginInformation> availableAppPlugins() const;

    bool hasPluginInformation( const QString &uuid ) const;

    QfPluginInformation pluginInformation( const QString &uuid ) const;

    Q_INVOKABLE void updatePluginEnabledStateByUuid( const QString &uuid, bool enabled, bool configurable );

    Q_INVOKABLE void refresh( bool fetchRemote = false );

    bool isRefreshing() const;

  signals:
    void isRefreshingChanged();
    void remoteFetched();

  private:
    void fetchRemotePlugins();

    void populateLocalPlugins();
    void populateRemotePlugins();

    QfPluginInformation readPluginMetadata( const QFileInfo &pluginDir );

    void insertPluginsInformation( QMap<QString, QfPluginInformation> &pluginsInformation, bool isLocal );

    QList<QfPluginInformation> mPlugins;
    QfPluginManager *mManager = nullptr;
    bool mIsRefreshing = false;
};

class QfPluginProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY( QString searchTerm READ searchTerm WRITE setSearchTerm NOTIFY searchTermChanged )
    Q_PROPERTY( PluginFilter filter READ filter WRITE setFilter NOTIFY filterChanged )

  public:
    enum PluginFilter
    {
      LocalPlugin,
      RemotePlugin,
    };
    Q_ENUM( PluginFilter )

    explicit QfPluginProxyModel( QObject *parent = nullptr );

    QString searchTerm() const;

    void setSearchTerm( const QString &searchTerm );

    PluginFilter filter() const;

    void setFilter( PluginFilter filter );

  protected:
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;
    bool lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const override;

  signals:
    void searchTermChanged();
    void filterChanged();

  private:
    QString mSearchTerm;
    PluginFilter mFilter = LocalPlugin;
};

Q_DECLARE_METATYPE( QfPluginProxyModel::PluginFilter )


#endif // QFPLUGINMODEL_H
```


