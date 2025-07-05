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

class PluginManager;
class PluginInformation;

class PluginModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( bool loading READ loading NOTIFY loadingChanged )

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
    explicit PluginModel( QObject *parent = nullptr );

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
     * Sets the list of plugins displayed by the model.
     */
    void setPlugins( const QList<PluginInformation> &plugins );

    /**
     * Returns the plugin manager used by this model.
     */
    PluginManager *manager() const;

    /**
     * Sets the plugin manager to \a newManager.
     */
    void setManager( PluginManager *newManager );

    /**
     * Returns true if the model contains a plugin with the given \a uuid.
     */
    bool hasPlugin( const QString &uuid ) const;

    /**
     * Returns the plugin information for the plugin identified by \a uuid.
     */
    PluginInformation plugin( const QString &uuid ) const;

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
     * Clears the list of plugins in the model.
     */
    Q_INVOKABLE void clear();

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
    bool loading() const;

  private:
    /**
     * Scans the app data directories for plugins, reading metadata and preparing PluginInformation objects.
     *
     * @return A list of PluginInformation objects representing all plugins found locally.
     */
    QList<PluginInformation> scanLocalPluginDirectories();

    /**
     * Reads the metadata (from metadata.txt) and prepares a PluginInformation for a given plugin directory.
     *
     * @param pluginDir The QFileInfo representing the plugin directory.
     * @return The filled PluginInformation structure.
     */
    PluginInformation readPluginMetadata( const QFileInfo &pluginDir );

  signals:
    void loadingChanged();

  private:
    QList<PluginInformation> mPlugins;
    PluginManager *mManager = nullptr;
    bool mLoading = false;
};

#endif // PLUGINMODEL_H
