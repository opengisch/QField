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

#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

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
  setLocalAppPlugins();
  emit managerChanged();
}

void PluginModel::clear()
{
  beginResetModel();
  setPlugins( {} );
  endResetModel();
}

void PluginModel::setLocalAppPlugins()
{
  QList<PluginInformation> pluginEntries;

  for ( const PluginInformation &plugin : mManager->availableAppPlugins() )
  {
    PluginInformation entry;
    entry.uuid = plugin.uuid;
    entry.name = plugin.name;
    entry.enabled = mManager->isAppPluginEnabled( plugin.uuid );
    entry.configurable = mManager->isAppPluginConfigurable( plugin.uuid );
    entry.locallyAvailable = true;
    entry.remotelyAvailable = false;
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
    case InstalledLocallyRole:
      plugin.locallyAvailable = value.toBool();
      emit dataChanged( index, index, { InstalledLocallyRole } );
      return true;
    case AvailableRemotelyRole:
      plugin.remotelyAvailable = value.toBool();
      emit dataChanged( index, index, { AvailableRemotelyRole } );
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

void PluginModel::fetchRemotePlugins()
{
  QList<PluginInformation> mRemotePlugins = {};

  QString jsonString = R"(
    [
      {
        "name": "OSRM Routing",
        "description": "Provides routing visualization within QField",
        "icon": "https://raw.githubusercontent.com/opengisch/qfield-osrm/refs/heads/main/icon.svg",
        "version": "1.0",
        "download": "https://github.com/opengisch/qfield-osrm/releases/download/v1.0/qfield-osrm-routing.zip",
        "minimum_version": "3.6.0",
        "homepage": "https://github.com/opengisch/qfield-osrm",
        "author": "OPENGIS.ch"
      },
      {
        "name": "OpenStreetMap Nominatim Search",
        "description": "Integrates OpenStreetMap Nominatim results into the search bar through the osm prefix",
        "icon": "https://raw.githubusercontent.com/opengisch/qfield-nominatim-locator/refs/heads/main/icon.svg",
        "version": "1.3",
        "download": "https://github.com/opengisch/qfield-nominatim-locator/releases/download/v1.3/qfield-nominatim-locator-v1.3.zip",
        "minimum_version": "3.6.0",
        "homepage": "https://github.com/opengisch/qfield-nominatim-locator",
        "author": "OPENGIS.ch"
      }
    ])";

  QJsonParseError parseError;
  QJsonDocument jsonDoc = QJsonDocument::fromJson( jsonString.toUtf8(), &parseError );

  if ( parseError.error != QJsonParseError::NoError )
  {
    qWarning() << "JSON parse error:" << parseError.errorString();
    return;
  }

  if ( !jsonDoc.isArray() )
  {
    qWarning() << "Expected a JSON array!";
    return;
  }

  const QJsonArray jsonArray = jsonDoc.array();
  for ( const QJsonValueConstRef &value : jsonArray )
  {
    if ( !value.isObject() )
      continue;

    const QJsonObject obj = value.toObject();

    PluginInformation info;
    info.name = obj.value( "name" ).toString();
    info.description = obj.value( "description" ).toString();
    info.author = obj.value( "author" ).toString();
    info.homepage = obj.value( "homepage" ).toString();
    info.icon = obj.value( "icon" ).toString();
    info.version = obj.value( "version" ).toString();
    info.path = obj.value( "download" ).toString();
    info.uuid = info.name; // "WE_NEED_SECURE_UUID";
    info.remotelyAvailable = true;
    info.locallyAvailable = false;
    mRemotePlugins.append( info );
  }

  QList<PluginInformation> pluginEntries = mPlugins;

  for ( const PluginInformation &plugin : mRemotePlugins )
  {
    bool locallyExists = false;
    for ( int i = 0; i < mPlugins.size(); ++i )
    {
      if ( pluginEntries[i].name == plugin.name )
      {
        pluginEntries[i].remotelyAvailable = true;
        locallyExists = true;
        break;
      }
    }
    if ( !locallyExists )
    {
      PluginInformation entry;
      entry.uuid = plugin.uuid;
      entry.name = plugin.name;
      entry.enabled = mManager->isAppPluginEnabled( plugin.uuid );
      entry.configurable = mManager->isAppPluginConfigurable( plugin.uuid );
      entry.locallyAvailable = false;
      entry.remotelyAvailable = true;
      entry.description = plugin.description;
      entry.author = plugin.author;
      entry.homepage = plugin.homepage;
      entry.icon = plugin.icon;
      entry.version = plugin.version;
      pluginEntries.append( entry );
    }
  }

  setPlugins( pluginEntries );
}
