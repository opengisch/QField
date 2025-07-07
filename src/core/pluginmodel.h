/***************************************************************************
 pluginmodel.h - PluginModel

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

#ifndef PLUGINMODEL_H
#define PLUGINMODEL_H


#include <QAbstractListModel>
#include <QFileInfo>
#include <QSortFilterProxyModel>

class PluginManager;
class PluginInformation;

class PluginModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( bool isRefreshing READ isRefreshing NOTIFY isRefreshingChanged )

  public:
    enum PluginRoles
    {
      UuidRole = Qt::UserRole + 1,
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
      DownloadLinkRole
    };
    Q_ENUM( PluginRoles )

    /**
     * Constructs a PluginModel.
     */
    explicit PluginModel( PluginManager *manager, QObject *parent = nullptr );

    /**
     * Returns the number of plugins in the model.
     */
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    /**
     * Returns the data for \a index and \a role.
     */
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    /**
     * Returns the role names used by this model.
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * Sets the data for \a index and \a role to \a value.
     */
    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    /**
     * Returns true if the model contains a plugin with the given \a uuid.
     */
    bool hasPluginInformation( const QString &uuid ) const;

    /**
     * Returns the pluginInformation information for the pluginInformation identified by \a uuid.
     */
    PluginInformation pluginInformation( const QString &uuid ) const;

    /**
     * Loads local plugins, replacing the model contents entirely.
     * Any previously loaded plugins (remote or local) will be cleared.
     */
    void loadLocalPlugins();

    /**
     * Refreshes local plugins incrementally by scanning plugin directories.
     * Updates existing model items in-place, adds newly detected plugins,
     * and removes entries that no longer exist locally (unless still available remotely).
     */
    void refreshLocalPlugins();

    /**
     * Updates the enabled and configurable state of the plugin with \a uuid.
     */
    Q_INVOKABLE void updatePluginEnabledStateByUuid( const QString &uuid, bool enabled, bool configurable );

    /**
     * Fetches remote plugins from the QField plugin registry.
     */
    Q_INVOKABLE void fetchRemotePlugins();

    /**
     * Returns true if the model is currently fetching remote plugins.
     */
    bool isRefreshing() const;

  private:
    /**
     * Sets the list of plugins displayed by the model.
     */
    void setPlugins( const QList<PluginInformation> &plugins );

    /**
     * Scans the app data directories for plugins, reading metadata and preparing PluginInformation objects.
     *
     * @return A map of plugin UUIDs to PluginInformation objects representing all plugins found locally.
     */
    QMap<QString, PluginInformation> scanLocalPluginDirectories();
    /**
     * Reads the metadata (from metadata.txt) and prepares a PluginInformation for a given plugin directory.
     *
     * @param pluginDir The QFileInfo representing the plugin directory.
     * @return The filled PluginInformation structure.
     */
    PluginInformation readPluginMetadata( const QFileInfo &pluginDir );

  signals:
    void isRefreshingChanged();

  private:
    QList<PluginInformation> mPlugins;
    PluginManager *mManager = nullptr;
    bool mIsRefreshing = false;
};

/**
 * \ingroup core
 */
class PluginProxyModel : public QSortFilterProxyModel
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

    explicit PluginProxyModel( QObject *parent = nullptr );

    /**
     * Returns the current search term used to filter items.
     */
    QString searchTerm() const;

    /**
     * Sets the search term for filtering and updates the filter.
     */
    void setSearchTerm( const QString &searchTerm );

    /**
     * Returns the current plugin filter.
     */
    PluginFilter filter() const;

    /**
     * Sets the plugin \a filter.
     */
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

Q_DECLARE_METATYPE( PluginProxyModel::PluginFilter )


#endif // PLUGINMODEL_H
