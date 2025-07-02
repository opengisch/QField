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

#include "pluginmanager.h"

#include <QAbstractListModel>


class PluginModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( PluginManager *manager READ manager WRITE setManager NOTIFY managerChanged )

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
      AvailableRemotelyRole
    };
    Q_ENUM( PluginRoles )

    explicit PluginModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    void setPlugins( const QList<PluginInformation> &plugins );

    PluginManager *manager() const;
    void setManager( PluginManager *newManager );

    Q_INVOKABLE void clear();
    Q_INVOKABLE void refreshAppPluginsList();
    Q_INVOKABLE void updatePluginEnabledStateByUuid( const QString &uuid, bool enabled, bool configurable );

  signals:
    void managerChanged();

  private:
    QList<PluginInformation> mPlugins;
    PluginManager *mManager = nullptr;
};

#endif // PLUGINMODEL_H
