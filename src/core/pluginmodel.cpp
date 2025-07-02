/***************************************************************************
 pluginmodel.cpp - PluginModel

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

#include "pluginmodel.h"

PluginModel::PluginModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

int PluginModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  return mPlugins.count();
}

QVariant PluginModel::data( const QModelIndex &index, int role ) const
{
  if ( !index.isValid() || index.row() >= mPlugins.count() )
    return QVariant();

  const PluginInformation &plugin = mPlugins[index.row()];

  switch ( role )
  {
    case UuidRole:
      return plugin.uuid;
    case EnabledRole:
      return plugin.enabled;
    case ConfigurableRole:
      return plugin.configurable;
    case NameRole:
      return plugin.name;
    case DescriptionRole:
      return plugin.description;
    case AuthorRole:
      return plugin.author;
    case HomepageRole:
      return plugin.homepage;
    case IconRole:
      return plugin.icon;
    case VersionRole:
      return plugin.version;
    case InstalledLocallyRole:
      return plugin.locallyAvailable;
    case AvailableRemotelyRole:
      return plugin.remotelyAvailable;
    default:
      return QVariant();
  }
}

QHash<int, QByteArray> PluginModel::roleNames() const
{
  return {
    { UuidRole, "Uuid" },
    { EnabledRole, "Enabled" },
    { ConfigurableRole, "Configurable" },
    { NameRole, "Name" },
    { DescriptionRole, "Description" },
    { AuthorRole, "Author" },
    { HomepageRole, "Homepage" },
    { IconRole, "Icon" },
    { VersionRole, "Version" },
    { InstalledLocallyRole, "InstalledLocallyRole" },
    { AvailableRemotelyRole, "AvailableRemotelyRole" } };
}

void PluginModel::setPlugins( const QList<PluginInformation> &plugins )
{
  beginResetModel();
  mPlugins = plugins;
  endResetModel();
}

PluginManager *PluginModel::manager() const
{
  return mManager;
}

void PluginModel::setManager( PluginManager *newManager )
{
  if ( mManager == newManager )
    return;
  mManager = newManager;

  refreshAppPluginsList();

  emit managerChanged();
}

void PluginModel::clear()
{
  beginResetModel();
  setPlugins( {} );
  endResetModel();
}

void PluginModel::refreshAppPluginsList()
{
  QList<PluginInformation> pluginEntries;

  auto isDuplicate = [&]( const QString &name ) {
    for ( PluginInformation &storedPlugin : pluginEntries )
    {
      if ( storedPlugin.name == name )
      { // we should check on uuid!
        return true;
      }
    }
    return false;
  };

  for ( PluginInformation &plugin : mManager->availableAppPlugins() + mManager->remotePlugins() )
  {
    if ( isDuplicate( plugin.name ) )
    {
      continue;
    }

    PluginInformation entry;
    entry.uuid = plugin.uuid;
    entry.name = plugin.name;
    entry.enabled = mManager->isAppPluginEnabled( plugin.uuid );
    entry.configurable = mManager->isAppPluginConfigurable( plugin.uuid );
    entry.locallyAvailable = mManager->isAppPluginLocallyAvailable( plugin.name );
    entry.remotelyAvailable = mManager->isAppPluginRemotelyAvailable( plugin.name );
    entry.description = plugin.description;
    entry.author = plugin.author;
    entry.homepage = plugin.homepage;
    entry.icon = plugin.icon;
    entry.version = plugin.version;
    pluginEntries.append( entry );
  }

  setPlugins( pluginEntries );
}

bool PluginModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( !index.isValid() || index.row() >= mPlugins.size() )
    return false;

  PluginInformation &plugin = mPlugins[index.row()];

  switch ( role )
  {
    case EnabledRole:
      plugin.enabled = value.toBool();
      emit dataChanged( index, index, { EnabledRole } );
      return true;
    case ConfigurableRole:
      plugin.configurable = value.toBool();
      emit dataChanged( index, index, { ConfigurableRole } );
      return true;
    default:
      return false;
  }
}

void PluginModel::updatePluginEnabledStateByUuid( const QString &uuid, bool enabled, bool configurable )
{
  for ( int i = 0; i < mPlugins.size(); ++i )
  {
    if ( mPlugins[i].uuid == uuid )
    {
      mPlugins[i].enabled = enabled;
      mPlugins[i].configurable = configurable;
      emit dataChanged( index( i ), index( i ), { EnabledRole, ConfigurableRole } );
      break;
    }
  }
}
