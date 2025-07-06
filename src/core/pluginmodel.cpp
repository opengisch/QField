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

#include "platformutilities.h"
#include "pluginmanager.h"
#include "pluginmodel.h"
#include "qgsnetworkaccessmanager.h"

#include <QDir>
#include <QSettings>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

#define REMOTE_PLUGINS_URL "https://qfield.org/plugins.json"

PluginModel::PluginModel( PluginManager *manager, QObject *parent )
  : mManager( manager ), QAbstractListModel( parent )
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
    case DownloadLinkRole:
      return plugin.downloadLink;
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
    { InstalledLocallyRole, "InstalledLocally" },
    { AvailableRemotelyRole, "AvailableRemotely" },
    { DownloadLinkRole, "DownloadLink" } };
}

void PluginModel::setPlugins( const QList<PluginInformation> &plugins )
{
  beginResetModel();
  mPlugins = plugins;
  endResetModel();
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
  mIsRefreshing = true;
  emit isRefreshingChanged();

  const QUrl url( REMOTE_PLUGINS_URL );

  QNetworkRequest request( url );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy );

  QgsNetworkAccessManager *networkManager = new QgsNetworkAccessManager( this );

  QNetworkReply *reply = networkManager->get( request );

  connect( reply, &QNetworkReply::finished, this, [=]() {
    reply->deleteLater();
    networkManager->deleteLater();

    mIsRefreshing = false;
    emit isRefreshingChanged();

    if ( reply->error() != QNetworkReply::NoError )
    {
      qDebug() << "Failed to fetch remote plugins:" << reply->errorString();
      return;
    }

    const QByteArray responseData = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson( responseData, &parseError );

    if ( parseError.error != QJsonParseError::NoError )
    {
      qDebug() << "JSON parse error:" << parseError.errorString();
      return;
    }

    if ( !jsonDoc.isArray() )
    {
      qDebug() << "Expected JSON array!";
      return;
    }

    QList<PluginInformation> remotePlugins;
    const QJsonArray jsonArray = jsonDoc.array();
    for ( const QJsonValueConstRef &value : jsonArray )
    {
      if ( !value.isObject() )
        continue;

      const QJsonObject obj = value.toObject();

      PluginInformation info;
      info.uuid = obj.value( "key" ).toString();
      info.name = obj.value( "name" ).toString();
      info.description = obj.value( "description" ).toString();
      info.author = obj.value( "author" ).toString();
      info.homepage = obj.value( "homepage" ).toString();
      info.icon = obj.value( "icon" ).toString();
      info.version = obj.value( "version" ).toString();
      info.downloadLink = obj.value( "download" ).toString();
      info.remotelyAvailable = true;
      info.locallyAvailable = false;

      remotePlugins.append( info );
    }

    // Merge with local plugins
    QList<PluginInformation> mergedPlugins = mPlugins;

    for ( const PluginInformation &remote : remotePlugins )
    {
      QList<PluginInformation>::iterator existingPluginIt = std::find_if( mergedPlugins.begin(), mergedPlugins.end(), [&]( const PluginInformation &local ) { return local.uuid == remote.uuid; } );
      if ( existingPluginIt != mergedPlugins.end() )
      {
        existingPluginIt->remotelyAvailable = true;
        existingPluginIt->icon = remote.icon;
        existingPluginIt->downloadLink = remote.downloadLink;
      }
      else
      {
        PluginInformation entry = remote;
        entry.enabled = mManager->isAppPluginEnabled( entry.uuid );
        entry.configurable = mManager->isAppPluginConfigurable( entry.uuid );
        mergedPlugins.append( entry );
      }
    }

    setPlugins( mergedPlugins );
  } );
}

QMap<QString, PluginInformation> PluginModel::scanLocalPluginDirectories()
{
  QMap<QString, PluginInformation> plugins;

  const QStringList dataDirs = PlatformUtilities::instance()->appDataDirs();
  for ( QString dataDir : dataDirs )
  {
    const QDir pluginsDir( dataDir += QStringLiteral( "plugins" ) );
    const QList<QFileInfo> candidates = pluginsDir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot );
    for ( const QFileInfo &candidate : candidates )
    {
      const QString path = QStringLiteral( "%1/main.qml" ).arg( candidate.absoluteFilePath() );
      if ( QFileInfo::exists( path ) )
      {
        const PluginInformation plugin = readPluginMetadata( candidate );
        plugins.insert( plugin.uuid, plugin );
      }
    }
  }

  return plugins;
}

void PluginModel::loadLocalPlugins()
{
  setPlugins( scanLocalPluginDirectories().values() );
}

void PluginModel::refreshLocalPlugins()
{
  QMap<QString, PluginInformation> foundLocalPlugins = scanLocalPluginDirectories();

  for ( int i = 0; i < mPlugins.size(); )
  {
    const QString &uuid = mPlugins[i].uuid;
    if ( foundLocalPlugins.contains( uuid ) )
    {
      const PluginInformation &local = foundLocalPlugins[uuid];
      mPlugins[i].name = local.name;
      mPlugins[i].description = local.description;
      mPlugins[i].author = local.author;
      mPlugins[i].homepage = local.homepage;
      mPlugins[i].version = local.version;
      mPlugins[i].path = local.path;
      mPlugins[i].locallyAvailable = true;
      mPlugins[i].enabled = local.enabled;
      mPlugins[i].configurable = local.configurable;

      emit dataChanged( index( i ), index( i ) );
      foundLocalPlugins.remove( uuid );
      ++i; // only increment if not removing
    }
    else if ( !mPlugins[i].remotelyAvailable )
    {
      // Local plugin directory removed, no remote fallback — remove from model.
      beginRemoveRows( QModelIndex(), i, i );
      mPlugins.removeAt( i );
      endRemoveRows();
    }
    else
    {
      // still available remotely, just mark as not local
      if ( mPlugins[i].locallyAvailable )
      {
        mPlugins[i].locallyAvailable = false;
        emit dataChanged( index( i ), index( i ), { InstalledLocallyRole } );
      }
      ++i;
    }
  }

  for ( const PluginInformation &newPlugin : foundLocalPlugins )
  {
    beginInsertRows( QModelIndex(), mPlugins.size(), mPlugins.size() );
    mPlugins.append( newPlugin );
    endInsertRows();
  }
}

PluginInformation PluginModel::readPluginMetadata( const QFileInfo &pluginDir )
{
  QString name = pluginDir.fileName();
  QString description, author, homepage, icon, version;
  const QString path = QStringLiteral( "%1/main.qml" ).arg( pluginDir.absoluteFilePath() );

  const QString metadataPath = QStringLiteral( "%1/metadata.txt" ).arg( pluginDir.absoluteFilePath() );
  if ( QFileInfo::exists( metadataPath ) )
  {
    QSettings metadata( metadataPath, QSettings::IniFormat );
    name = metadata.value( "name", pluginDir.fileName() ).toString();
    description = metadata.value( "description" ).toString();
    author = metadata.value( "author" ).toString();
    homepage = metadata.value( "homepage" ).toString();
    if ( !homepage.isEmpty() )
    {
      QUrl url( homepage );
      if ( !url.scheme().startsWith( "http" ) )
        homepage.clear();
    }
    if ( !metadata.value( "icon" ).toString().isEmpty() )
      icon = QStringLiteral( "%1/%2" ).arg( pluginDir.absoluteFilePath(), metadata.value( "icon" ).toString() );
    version = metadata.value( "version" ).toString();
  }

  PluginInformation plugin( pluginDir.fileName(), name, description, author, homepage, icon, version, path, "", true, false, false, false );
  plugin.enabled = mManager->isAppPluginEnabled( plugin.uuid );
  plugin.configurable = mManager->isAppPluginConfigurable( plugin.uuid );

  return plugin;
}

bool PluginModel::hasPluginInformation( const QString &uuid ) const
{
  return std::any_of( mPlugins.begin(), mPlugins.end(), [&]( const PluginInformation &plugin ) { return plugin.uuid == uuid; } );
}

PluginInformation PluginModel::pluginInformation( const QString &uuid ) const
{
  const QList<PluginInformation>::const_iterator foundPluginIt = std::find_if( mPlugins.begin(), mPlugins.end(), [&]( const PluginInformation &plugin ) { return plugin.uuid == uuid; } );
  if ( foundPluginIt != mPlugins.end() )
    return *foundPluginIt;
  else
    return PluginInformation();
}

bool PluginModel::isRefreshing() const
{
  return mIsRefreshing;
}

PluginProxyModel::PluginProxyModel( QObject *parent )
  : QSortFilterProxyModel( parent )
{
  setFilterCaseSensitivity( Qt::CaseInsensitive );
  setFilterRole( PluginModel::PluginRoles::NameRole );
  sort( 0 );
}

QString PluginProxyModel::searchTerm() const
{
  return mSearchTerm;
}

void PluginProxyModel::setSearchTerm( const QString &searchTerm )
{
  if ( mSearchTerm != searchTerm )
  {
    mSearchTerm = searchTerm;
    emit searchTermChanged();
    invalidateFilter();
  }
}

bool PluginProxyModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  bool matchesPluginType = false;
  const QModelIndex currentRowIndex = sourceModel()->index( sourceRow, 0, sourceParent );

  switch ( mFilter )
  {
    case LocalPlugin:
      matchesPluginType = sourceModel()->data( currentRowIndex, PluginModel::InstalledLocallyRole ).toBool();
      break;
    case RemotePlugin:
      matchesPluginType = sourceModel()->data( currentRowIndex, PluginModel::AvailableRemotelyRole ).toBool();
      break;
  }

  const QModelIndex index = sourceModel()->index( sourceRow, 0, sourceParent );
  const QVariant data = sourceModel()->data( index, PluginModel::PluginRoles::NameRole );
  const bool matchesTextFilter = mSearchTerm.isEmpty() || data.toString().contains( mSearchTerm, Qt::CaseInsensitive );

  return matchesTextFilter && matchesPluginType;
}


void PluginProxyModel::setFilter( PluginFilter filter )
{
  if ( mFilter == filter )
    return;

  mFilter = filter;
  invalidateFilter();

  emit filterChanged();

  sort( 0 );
}

bool PluginProxyModel::lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const
{
  const QString leftName = sourceModel()->data( sourceLeft, PluginModel::PluginRoles::NameRole ).toString().toLower();
  const QString rightName = sourceModel()->data( sourceRight, PluginModel::PluginRoles::NameRole ).toString().toLower();

  return leftName < rightName;
}

PluginProxyModel::PluginFilter PluginProxyModel::filter() const
{
  return mFilter;
}
