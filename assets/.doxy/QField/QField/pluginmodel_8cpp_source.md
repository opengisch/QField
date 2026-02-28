

# File pluginmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**pluginmodel.cpp**](pluginmodel_8cpp.md)

[Go to the documentation of this file](pluginmodel_8cpp.md)


```C++
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
#include <QStandardPaths>
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
  return !parent.isValid() ? static_cast<int>( mPlugins.count() ) : 0;
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
    case BundledRole:
      return plugin.bundled;
    case TrustedRole:
      return plugin.trusted;
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
    case AvailableUpdateRole:
      return plugin.updateAvailable;
    default:
      return QVariant();
  }
}

QHash<int, QByteArray> PluginModel::roleNames() const
{
  return {
    { UuidRole, "Uuid" },
    { BundledRole, "Bundled" },
    { TrustedRole, "Trusted" },
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
    { AvailableUpdateRole, "AvailableUpdate" } };
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

QList<PluginInformation> PluginModel::availableAppPlugins() const
{
  QList<PluginInformation> availableAppPlugins;
  for ( const PluginInformation &pluginInformation : mPlugins )
  {
    if ( pluginInformation.locallyAvailable )
    {
      availableAppPlugins << pluginInformation;
    }
  }
  return availableAppPlugins;
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

void PluginModel::insertPluginsInformation( QMap<QString, PluginInformation> &pluginsInformation, bool isLocal )
{
  for ( int i = 0; i < mPlugins.size(); )
  {
    PluginInformation &plugin = mPlugins[i];
    if ( pluginsInformation.contains( plugin.uuid ) )
    {
      // Plugin found, update its information
      const PluginInformation &pluginInformation = pluginsInformation[plugin.uuid];
      plugin.name = pluginInformation.name;
      plugin.description = pluginInformation.description;
      plugin.author = pluginInformation.author;
      plugin.homepage = pluginInformation.homepage;
      plugin.icon = pluginInformation.icon;

      if ( isLocal )
      {
        plugin.version = pluginInformation.version;
        plugin.path = pluginInformation.path;
        plugin.locallyAvailable = true;
        plugin.enabled = pluginInformation.enabled;
        plugin.configurable = pluginInformation.configurable;
      }
      else
      {
        plugin.trusted = pluginInformation.trusted;
        plugin.remoteVersion = pluginInformation.remoteVersion;
        plugin.downloadLink = pluginInformation.downloadLink;
        plugin.remotelyAvailable = true;
      }
      plugin.updateAvailable = !plugin.version.isEmpty() && !plugin.remoteVersion.isEmpty() && plugin.version != plugin.remoteVersion;

      emit dataChanged( index( i ), index( i ) );
      pluginsInformation.remove( plugin.uuid );
      ++i;
    }
    else
    {
      if ( isLocal && plugin.remotelyAvailable )
      {
        // Plugin still remotely available
        if ( plugin.locallyAvailable )
        {
          plugin.path = QString();
          plugin.locallyAvailable = false;
          emit dataChanged( index( i ), index( i ), { InstalledLocallyRole, AvailableUpdateRole } );
        }
        ++i;
      }
      else if ( !isLocal && plugin.locallyAvailable )
      {
        // Plugin still locally available
        if ( plugin.remotelyAvailable )
        {
          plugin.trusted = false;
          plugin.remoteVersion = QString();
          plugin.downloadLink = QString();
          plugin.remotelyAvailable = false;
          emit dataChanged( index( i ), index( i ), { AvailableRemotelyRole, AvailableUpdateRole, TrustedRole } );
        }
        ++i;
      }
      else
      {
        // Plugin disappeared, remove
        beginRemoveRows( QModelIndex(), i, i );
        mPlugins.removeAt( i );
        endRemoveRows();
      }
    }
  }

  for ( const PluginInformation &newPluginInformation : pluginsInformation )
  {
    const int index = static_cast<int>( mPlugins.size() );
    beginInsertRows( QModelIndex(), index, index );
    mPlugins.append( newPluginInformation );
    endInsertRows();
  }
}

void PluginModel::refresh( bool fetchRemote )
{
  populateLocalPlugins();

  if ( fetchRemote )
  {
    fetchRemotePlugins();
  }
  else
  {
    populateRemotePlugins();
  }
}

void PluginModel::fetchRemotePlugins()
{
  mIsRefreshing = true;
  emit isRefreshingChanged();

  const QUrl url( REMOTE_PLUGINS_URL );

  QNetworkRequest request( url );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy );
  QNetworkReply *reply = QgsNetworkAccessManager::instance()->get( request );

  connect( reply, &QNetworkReply::finished, this, [this, reply]() {
    reply->deleteLater();

    mIsRefreshing = false;
    emit isRefreshingChanged();

    if ( reply->error() != QNetworkReply::NoError )
    {
      qDebug() << "Failed to fetch remote plugins: " << reply->errorString();
      return;
    }

    const QByteArray responseData = reply->readAll();
    QDir cacheDir( QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation ) );
    cacheDir.mkpath( QStringLiteral( "plugins" ) );
    QFile cacheFile( QStringLiteral( "%1/plugins/cache.json" ).arg( QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation ) ) );
    if ( cacheFile.open( QIODeviceBase::WriteOnly ) )
    {
      cacheFile.write( responseData );
    }
    cacheFile.close();

    populateRemotePlugins();

    emit remoteFetched();
  } );
}

void PluginModel::populateRemotePlugins()
{
  QFile cacheFile( QStringLiteral( "%1/plugins/cache.json" ).arg( QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation ) ) );
  if ( cacheFile.exists() && cacheFile.open( QIODeviceBase::ReadOnly ) )
  {
    const QByteArray cacheData = cacheFile.readAll();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson( cacheData, &parseError );
    if ( parseError.error != QJsonParseError::NoError )
    {
      qDebug() << "JSON parse error when parsing remote plugins: " << parseError.errorString();
      return;
    }
    else if ( !jsonDoc.isArray() )
    {
      qDebug() << "Expected JSON array when parsing remote plugins";
      return;
    }

    QMap<QString, PluginInformation> foundRemotePlugins;
    const QJsonArray jsonArray = jsonDoc.array();
    for ( const QJsonValueConstRef &value : jsonArray )
    {
      if ( !value.isObject() )
        continue;

      const QJsonObject obj = value.toObject();

      PluginInformation info;
      info.trusted = true;
      info.uuid = obj.value( "uuid" ).toString();
      info.name = obj.value( "name" ).toString();
      info.description = obj.value( "description" ).toString();
      info.author = obj.value( "author" ).toString();
      info.homepage = obj.value( "homepage" ).toString();
      info.icon = obj.value( "icon" ).toString();
      info.remoteVersion = obj.value( "version" ).toString();
      info.downloadLink = obj.value( "download" ).toString();
      info.remotelyAvailable = true;

      if ( !info.uuid.isEmpty() )
      {
        foundRemotePlugins[info.uuid] = info;
      }
    }

    insertPluginsInformation( foundRemotePlugins, false );
  }
}

void PluginModel::populateLocalPlugins()
{
  QMap<QString, PluginInformation> foundLocalPlugins;
  const QStringList dirs = QStringList() << QStringLiteral( "%1/qfield" ).arg( PlatformUtilities::instance()->systemSharedDataLocation() ) << PlatformUtilities::instance()->appDataDirs();
  for ( const QString &dir : dirs )
  {
    QDir pluginsDir( dir );
    if ( pluginsDir.cd( QStringLiteral( "plugins" ) ) )
    {
      const QList<QFileInfo> candidates = pluginsDir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot );
      for ( const QFileInfo &candidate : candidates )
      {
        const QString path = QStringLiteral( "%1/main.qml" ).arg( candidate.absoluteFilePath() );
        if ( QFileInfo::exists( path ) )
        {
          const PluginInformation plugin = readPluginMetadata( candidate );
          foundLocalPlugins[plugin.uuid] = plugin;
        }
      }
    }
  }
  insertPluginsInformation( foundLocalPlugins, true );
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

  PluginInformation plugin( pluginDir.fileName(), name, description, author, homepage, icon );
  plugin.bundled = path.startsWith( PlatformUtilities::instance()->systemSharedDataLocation() );
  plugin.version = version;
  plugin.path = path;
  plugin.locallyAvailable = true;
  plugin.enabled = mManager->isAppPluginEnabled( plugin.uuid );
  plugin.configurable = mManager->isAppPluginConfigurable( plugin.uuid );

  return plugin;
}

bool PluginModel::hasPluginInformation( const QString &uuid ) const
{
  return !mPlugins.isEmpty() ? std::any_of( mPlugins.begin(), mPlugins.end(), [&]( const PluginInformation &plugin ) { return plugin.uuid == uuid; } ) : false;
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
```


