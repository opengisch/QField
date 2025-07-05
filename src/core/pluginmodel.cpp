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
#include <QFileInfo>
#include <QSettings>
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
}

void PluginModel::clear()
{
  beginResetModel();
  setPlugins( {} );
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
  mLoading = true;
  emit loadingChanged();

  const QUrl url( "https://qfield.org/plugins.json" );

  QNetworkRequest request( url );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy );

  QgsNetworkAccessManager *manager = new QgsNetworkAccessManager( this );

  QNetworkReply *reply = manager->get( request );

  connect( reply, &QNetworkReply::finished, this, [=]() {
    reply->deleteLater();
    manager->deleteLater();

    mLoading = false;
    emit loadingChanged();

    if ( reply->error() != QNetworkReply::NoError )
    {
      qWarning() << "Failed to fetch remote plugins:" << reply->errorString();
      return;
    }

    const QByteArray responseData = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson( responseData, &parseError );

    if ( parseError.error != QJsonParseError::NoError )
    {
      qWarning() << "JSON parse error:" << parseError.errorString();
      return;
    }

    if ( !jsonDoc.isArray() )
    {
      qWarning() << "Expected JSON array!";
      return;
    }

    QList<PluginInformation> remotePlugins;
    const QJsonArray jsonArray = jsonDoc.array();
    for ( const QJsonValue &value : jsonArray )
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
      info.uuid = obj.value( "key" ).toString();
      info.remotelyAvailable = true;
      info.locallyAvailable = false;

      remotePlugins.append( info );
    }

    // Merge with local plugins
    QList<PluginInformation> mergedPlugins = mPlugins;

    for ( const PluginInformation &remote : remotePlugins )
    {
      bool existsLocally = false;
      for ( PluginInformation &local : mergedPlugins )
      {
        if ( local.uuid == remote.uuid )
        {
          local.remotelyAvailable = true;
          existsLocally = true;
          break;
        }
      }
      if ( !existsLocally )
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

void PluginModel::loadLocalPlugins()
{
  QList<PluginInformation> plugins;

  const QStringList dataDirs = PlatformUtilities::instance()->appDataDirs();
  for ( QString dataDir : dataDirs )
  {
    QDir pluginsDir( dataDir += QStringLiteral( "plugins" ) );
    const QList<QFileInfo> candidates = pluginsDir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot );
    for ( const QFileInfo &candidate : candidates )
    {
      const QString path = QStringLiteral( "%1/main.qml" ).arg( candidate.absoluteFilePath() );
      if ( QFileInfo::exists( path ) )
      {
        QString name = candidate.fileName();
        QString description, author, homepage, icon, version;

        const QString metadataPath = QStringLiteral( "%1/metadata.txt" ).arg( candidate.absoluteFilePath() );
        if ( QFileInfo::exists( metadataPath ) )
        {
          QSettings metadata( metadataPath, QSettings::IniFormat );
          name = metadata.value( "name", candidate.fileName() ).toString();
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
            icon = QStringLiteral( "%1/%2" ).arg( candidate.absoluteFilePath(), metadata.value( "icon" ).toString() );
          version = metadata.value( "version" ).toString();
        }

        PluginInformation plugin( candidate.fileName(), name, description, author, homepage, icon, version, path, true, false );
        plugin.enabled = mManager->isAppPluginEnabled( plugin.uuid );
        plugin.configurable = mManager->isAppPluginConfigurable( plugin.uuid );
        plugins.append( plugin );
      }
    }
  }

  std::sort( plugins.begin(), plugins.end(), []( const PluginInformation &plugin1, const PluginInformation &plugin2 ) {
    return plugin1.name.toLower() < plugin2.name.toLower();
  } );

  setPlugins( plugins );
}

bool PluginModel::hasPlugin( const QString &uuid ) const
{
  for ( const PluginInformation &plugin : mPlugins )
  {
    if ( plugin.uuid == uuid )
      return true;
  }
  return false;
}

PluginInformation PluginModel::plugin( const QString &uuid ) const
{
  for ( const PluginInformation &plugin : mPlugins )
  {
    if ( plugin.uuid == uuid )
      return plugin;
  }
  return PluginInformation();
}

bool PluginModel::loading() const
{
  return mLoading;
}
