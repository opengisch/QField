

# File qfpluginmodel.cpp

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qfpluginmodel.cpp**](qfpluginmodel_8cpp.md)

[Go to the documentation of this file](qfpluginmodel_8cpp.md)


```C++
/***************************************************************************
 qfpluginmodel.cpp - QfPluginModel

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

#include "qfplatformutilities.h"
#include "qfpluginmanager.h"
#include "qfpluginmodel.h"

#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include <qgsnetworkaccessmanager.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

#define REMOTE_PLUGINS_URL "https://qfield.org/plugins.json"

QfPluginModel::QfPluginModel( QfPluginManager *manager, QObject *parent )
  : mManager( manager ), QAbstractListModel( parent )
{
}

int QfPluginModel::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mPlugins.count() ) : 0;
}

QVariant QfPluginModel::data( const QModelIndex &index, int role ) const
{
  if ( !index.isValid() || index.row() >= mPlugins.count() )
    return QVariant();

  const QfPluginInformation &plugin = mPlugins[index.row()];

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

QHash<int, QByteArray> QfPluginModel::roleNames() const
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

bool QfPluginModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( !index.isValid() || index.row() >= mPlugins.size() )
    return false;

  QfPluginInformation &plugin = mPlugins[index.row()];

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

QList<QfPluginInformation> QfPluginModel::availableAppPlugins() const
{
  QList<QfPluginInformation> availableAppPlugins;
  for ( const QfPluginInformation &pluginInformation : mPlugins )
  {
    if ( pluginInformation.locallyAvailable )
    {
      availableAppPlugins << pluginInformation;
    }
  }
  return availableAppPlugins;
}

void QfPluginModel::updatePluginEnabledStateByUuid( const QString &uuid, bool enabled, bool configurable )
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

void QfPluginModel::insertPluginsInformation( QMap<QString, QfPluginInformation> &pluginsInformation, bool isLocal )
{
  for ( int i = 0; i < mPlugins.size(); )
  {
    QfPluginInformation &plugin = mPlugins[i];
    if ( pluginsInformation.contains( plugin.uuid ) )
    {
      // Plugin found, update its information
      const QfPluginInformation &pluginInformation = pluginsInformation[plugin.uuid];
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

  for ( const QfPluginInformation &newPluginInformation : pluginsInformation )
  {
    const int index = static_cast<int>( mPlugins.size() );
    beginInsertRows( QModelIndex(), index, index );
    mPlugins.append( newPluginInformation );
    endInsertRows();
  }
}

void QfPluginModel::refresh( bool fetchRemote )
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

void QfPluginModel::fetchRemotePlugins()
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

void QfPluginModel::populateRemotePlugins()
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

    QMap<QString, QfPluginInformation> foundRemotePlugins;
    const QJsonArray jsonArray = jsonDoc.array();
    for ( const QJsonValueConstRef &value : jsonArray )
    {
      if ( !value.isObject() )
        continue;

      const QJsonObject obj = value.toObject();

      QfPluginInformation info;
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

void QfPluginModel::populateLocalPlugins()
{
  QMap<QString, QfPluginInformation> foundLocalPlugins;
  const QStringList dirs = QStringList() << QStringLiteral( "%1/qfield" ).arg( QfPlatformUtilities::instance()->systemSharedDataLocation() ) << QfPlatformUtilities::instance()->appDataDirs();
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
          const QfPluginInformation plugin = readPluginMetadata( candidate );
          foundLocalPlugins[plugin.uuid] = plugin;
        }
      }
    }
  }
  insertPluginsInformation( foundLocalPlugins, true );
}

QfPluginInformation QfPluginModel::readPluginMetadata( const QFileInfo &pluginDir )
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

  QfPluginInformation plugin( pluginDir.fileName(), name, description, author, homepage, icon );
  plugin.bundled = path.startsWith( QfPlatformUtilities::instance()->systemSharedDataLocation() );
  plugin.version = version;
  plugin.path = path;
  plugin.locallyAvailable = true;
  plugin.enabled = mManager->isAppPluginEnabled( plugin.uuid );
  plugin.configurable = mManager->isAppPluginConfigurable( plugin.uuid );

  return plugin;
}

bool QfPluginModel::hasPluginInformation( const QString &uuid ) const
{
  return !mPlugins.isEmpty() ? std::any_of( mPlugins.begin(), mPlugins.end(), [&]( const QfPluginInformation &plugin ) { return plugin.uuid == uuid; } ) : false;
}

QfPluginInformation QfPluginModel::pluginInformation( const QString &uuid ) const
{
  const QList<QfPluginInformation>::const_iterator foundPluginIt = std::find_if( mPlugins.begin(), mPlugins.end(), [&]( const QfPluginInformation &plugin ) { return plugin.uuid == uuid; } );
  if ( foundPluginIt != mPlugins.end() )
    return *foundPluginIt;
  else
    return QfPluginInformation();
}

bool QfPluginModel::isRefreshing() const
{
  return mIsRefreshing;
}

QfPluginProxyModel::QfPluginProxyModel( QObject *parent )
  : QSortFilterProxyModel( parent )
{
  setFilterCaseSensitivity( Qt::CaseInsensitive );
  setFilterRole( QfPluginModel::PluginRoles::NameRole );
  sort( 0 );
}

QString QfPluginProxyModel::searchTerm() const
{
  return mSearchTerm;
}

void QfPluginProxyModel::setSearchTerm( const QString &searchTerm )
{
  if ( mSearchTerm != searchTerm )
  {
    beginFilterChange();
    mSearchTerm = searchTerm;
    endFilterChange( QSortFilterProxyModel::Direction::Rows );
    emit searchTermChanged();
  }
}

bool QfPluginProxyModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  bool matchesPluginType = false;
  const QModelIndex currentRowIndex = sourceModel()->index( sourceRow, 0, sourceParent );

  switch ( mFilter )
  {
    case LocalPlugin:
      matchesPluginType = sourceModel()->data( currentRowIndex, QfPluginModel::InstalledLocallyRole ).toBool();
      break;
    case RemotePlugin:
      matchesPluginType = sourceModel()->data( currentRowIndex, QfPluginModel::AvailableRemotelyRole ).toBool();
      break;
  }

  const QModelIndex index = sourceModel()->index( sourceRow, 0, sourceParent );
  const QVariant data = sourceModel()->data( index, QfPluginModel::PluginRoles::NameRole );
  const bool matchesTextFilter = mSearchTerm.isEmpty() || data.toString().contains( mSearchTerm, Qt::CaseInsensitive );

  return matchesTextFilter && matchesPluginType;
}


void QfPluginProxyModel::setFilter( PluginFilter filter )
{
  if ( mFilter == filter )
    return;

  beginFilterChange();
  mFilter = filter;
  endFilterChange( QSortFilterProxyModel::Direction::Rows );

  emit filterChanged();

  sort( 0 );
}

bool QfPluginProxyModel::lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const
{
  const QString leftName = sourceModel()->data( sourceLeft, QfPluginModel::PluginRoles::NameRole ).toString().toLower();
  const QString rightName = sourceModel()->data( sourceRight, QfPluginModel::PluginRoles::NameRole ).toString().toLower();

  return leftName < rightName;
}

QfPluginProxyModel::PluginFilter QfPluginProxyModel::filter() const
{
  return mFilter;
}
```


