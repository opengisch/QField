/***************************************************************************
  localfilesmodel.cpp

 ---------------------
 begin                : 17.05.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "localfilesmodel.h"
#include "platformutilities.h"
#include "qfield.h"
#include "qfieldcloudutils.h"
#include "webdavconnection.h"

#include <QDir>
#include <QFile>
#include <QImageReader>

LocalFilesModel::LocalFilesModel( QObject *parent )
  : QAbstractListModel( parent )
{
  QSettings settings;
  const bool favoritesInitialized = settings.value( QStringLiteral( "qfieldFavoritesInitialized" ), false ).toBool();
  if ( !favoritesInitialized )
  {
    const QString applicationDirectory = PlatformUtilities::instance()->applicationDirectory();
    if ( !applicationDirectory.isEmpty() )
    {
      mFavorites << QStringLiteral( "%1/Imported Projects" ).arg( applicationDirectory )
                 << QStringLiteral( "%1/Imported Datasets" ).arg( applicationDirectory );
    }
    const QString sampleProjectPath = PlatformUtilities::instance()->systemLocalDataLocation( QLatin1String( "sample_projects" ) );
    mFavorites << sampleProjectPath;
    settings.setValue( QStringLiteral( "qfieldFavorites" ), mFavorites );
    settings.setValue( QStringLiteral( "qfieldFavoritesInitialized" ), true );
  }
  else
  {
    mFavorites = settings.value( QStringLiteral( "qfieldFavorites" ), QStringList() ).toStringList();
  }
  resetToRoot();
}

QHash<int, QByteArray> LocalFilesModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[ItemMetaTypeRole] = "ItemMetaType";
  roles[ItemTypeRole] = "ItemType";
  roles[ItemTitleRole] = "ItemTitle";
  roles[ItemFormatRole] = "ItemFormat";
  roles[ItemPathRole] = "ItemPath";
  roles[ItemSizeRole] = "ItemSize";
  roles[ItemHasThumbnailRole] = "ItemHasThumbnail";
  roles[ItemIsFavoriteRole] = "ItemIsFavorite";
  roles[ItemHasWebdavConfigurationRole] = "ItemHasWebdavConfiguration";
  return roles;
}

void LocalFilesModel::resetToRoot()
{
  mHistory.clear();
  mHistory << QLatin1String( "root" );

  emit currentPathChanged();

  reloadModel();
}

void LocalFilesModel::resetToPath( const QString &path )
{
  mHistory.clear();
  setCurrentPath( path );

  if ( mHistory.isEmpty() )
  {
    resetToRoot();
  }
}

bool LocalFilesModel::isPathFavoriteEditable( const QString &path )
{
  const QString sampleProjectPath = PlatformUtilities::instance()->systemLocalDataLocation( QLatin1String( "sample_projects" ) );
  return path != sampleProjectPath;
}

void LocalFilesModel::addToFavorites( const QString &path )
{
  if ( !mFavorites.contains( path ) )
  {
    mFavorites << path;
    QSettings().setValue( QStringLiteral( "qfieldFavorites" ), mFavorites );
    reloadModel();
  }
}

void LocalFilesModel::removeFromFavorites( const QString &path )
{
  if ( mFavorites.removeAll( path ) )
  {
    QSettings().setValue( QStringLiteral( "qfieldFavorites" ), mFavorites );
    reloadModel();
  }
}

QString LocalFilesModel::currentTitle() const
{
  return getCurrentTitleFromPath( currentPath() );
}

const QString LocalFilesModel::getCurrentTitleFromPath( const QString &path ) const
{
  if ( path == QLatin1String( "root" ) )
  {
    return QStringLiteral( "Home" );
  }
  else if ( path == PlatformUtilities::instance()->applicationDirectory() )
  {
    return tr( "QField files directory" );
  }
  else if ( path == PlatformUtilities::instance()->applicationDirectory() + QStringLiteral( "/Imported Projects" ) )
  {
    return tr( "Imported projects" );
  }
  else if ( path == PlatformUtilities::instance()->applicationDirectory() + QStringLiteral( "/Imported Datasets" ) )
  {
    return tr( "Imported datasets" );
  }
  else if ( PlatformUtilities::instance()->additionalApplicationDirectories().contains( path ) )
  {
    return tr( "Additional files directory" );
  }
  else if ( path == PlatformUtilities::instance()->systemLocalDataLocation( QLatin1String( "sample_projects" ) ) )
  {
    return tr( "Sample projects" );
  }
  else
  {
    const QString cloudProjectId = QFieldCloudUtils::getProjectId( path );
    if ( !cloudProjectId.isEmpty() )
    {
      return QFieldCloudUtils::projectSetting( cloudProjectId, QStringLiteral( "name" ), QString() ).toString();
    }
  }

  return QFileInfo( path ).fileName();
}

void LocalFilesModel::setCurrentPath( const QString &path )
{
  QFileInfo fi( path );
  if ( fi.exists() )
  {
    const QString newPath = fi.isDir() ? path : fi.absolutePath();
    if ( newPath != currentPath() )
    {
      mHistory << ( fi.isDir() ? path : fi.absolutePath() );

      emit currentPathChanged();
    }

    reloadModel();
  }
}

QString LocalFilesModel::currentPath() const
{
  return ( !mHistory.isEmpty() ? mHistory.last() : QString() );
}

bool LocalFilesModel::isDeletedAllowedInCurrentPath() const
{
  const QString path = currentPath();
  const QString applicationDirectory = PlatformUtilities::instance()->applicationDirectory();
  if ( !applicationDirectory.isEmpty() && path.startsWith( applicationDirectory ) )
  {
    return true;
  }
  else
  {
    const QStringList additionalApplicationDirectories = PlatformUtilities::instance()->additionalApplicationDirectories();
    if ( std::any_of( additionalApplicationDirectories.begin(), additionalApplicationDirectories.end(), [&path]( const QString &directory ) { return ( !directory.isEmpty() && path.startsWith( directory ) ); } ) )
    {
      return true;
    }
  }

  return false;
}

void LocalFilesModel::moveUp()
{
  mHistory.removeLast();

  emit currentPathChanged();

  reloadModel();
}

void LocalFilesModel::reloadModel()
{
  beginResetModel();
  mItems.clear();

  const QString path = currentPath();
  if ( path == QLatin1String( "root" ) )
  {
    const QString applicationDirectory = PlatformUtilities::instance()->applicationDirectory();
    if ( !applicationDirectory.isEmpty() )
    {
      mItems << Item( ItemMetaType::Folder, ItemType::ApplicationFolder, tr( "QField files directory" ), QString(), applicationDirectory );
    }

    const QStringList additionalApplicationDirectories = PlatformUtilities::instance()->additionalApplicationDirectories();
    for ( const QString &item : additionalApplicationDirectories )
    {
      QFileInfo fi( item );
      if ( fi.exists() )
      {
        mItems << Item( ItemMetaType::Folder, ItemType::ExternalStorage, tr( "Additional files directory" ), QString(), fi.absoluteFilePath() );
      }
    }

    const QStringList rootDirectories = PlatformUtilities::instance()->rootDirectories();
    for ( const QString &item : rootDirectories )
    {
      QFileInfo fi( item );
      if ( fi.exists() )
      {
        mItems << Item( ItemMetaType::Folder, ItemType::SimpleFolder, fi.absoluteFilePath(), QString(), fi.absoluteFilePath() );
      }
    }

    const QStringList favorites = QSettings().value( QStringLiteral( "qfieldFavorites" ), QStringList() ).toStringList();
    QList<Item> favoriteItems;
    for ( const QString &item : favorites )
    {
      if ( QFileInfo::exists( item ) )
      {
        favoriteItems << Item( ItemMetaType::Favorite, ItemType::SimpleFolder, getCurrentTitleFromPath( item ), QString(), item );
      }
    }

    std::sort( favoriteItems.begin(), favoriteItems.end(), []( const Item &a, const Item &b ) { return a.title < b.title; } );
    mItems.append( favoriteItems );
  }
  else
  {
    QDir dir( path );
    if ( dir.exists() )
    {
      const QStringList items = dir.entryList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::IgnoreCase );
      QList<Item> folders;
      QList<Item> files;
      QList<Item> projects;
      QList<Item> datasets;
      for ( const QString &item : items )
      {
        QFileInfo fi( path + QDir::separator() + item );
        if ( fi.isDir() )
        {
          folders << Item( ItemMetaType::Folder, ItemType::SimpleFolder, fi.fileName(), QString(), fi.absoluteFilePath() );
        }
        else
        {
          const QString suffix = fi.suffix().toLower();
          if ( ( suffix == QStringLiteral( "png" ) || suffix == QStringLiteral( "jpg" ) ) && items.contains( fi.completeBaseName() ) )
          {
            // Skip project preview images
            continue;
          }
          else if ( item == QStringLiteral( "qfield_webdav_configuration.json" ) )
          {
            // Skip QField WebDAV configuration file
            continue;
          }

          if ( SUPPORTED_PROJECT_EXTENSIONS.contains( suffix ) )
          {
            projects << Item( ItemMetaType::Project, ItemType::ProjectFile, fi.completeBaseName(), suffix, fi.absoluteFilePath(), fi.size() );
          }
          else if ( SUPPORTED_VECTOR_EXTENSIONS.contains( suffix ) && suffix != QStringLiteral( "pdf" ) )
          {
            datasets << Item( ItemMetaType::Dataset, ItemType::VectorDataset, fi.completeBaseName(), suffix, fi.absoluteFilePath(), fi.size() );
          }
          else if ( SUPPORTED_RASTER_EXTENSIONS.contains( suffix ) )
          {
            datasets << Item( ItemMetaType::Dataset, ItemType::RasterDataset, fi.completeBaseName(), suffix, fi.absoluteFilePath(), fi.size() );
          }
          else if ( SUPPORTED_FILE_EXTENSIONS.contains( suffix ) )
          {
            files << Item( ItemMetaType::File, ItemType::OtherFile, fi.completeBaseName(), suffix, fi.absoluteFilePath(), fi.size() );
          }
        }
      }
      mItems << folders << projects << datasets << files;
    }
  }

  endResetModel();
}

int LocalFilesModel::rowCount( const QModelIndex &parent ) const
{
  if ( !parent.isValid() )
    return static_cast<int>( mItems.size() );
  else
    return 0;
}

QVariant LocalFilesModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mItems.size() || index.row() < 0 )
    return QVariant();

  switch ( static_cast<Role>( role ) )
  {
    case ItemMetaTypeRole:
      return mItems[index.row()].metaType;

    case ItemTypeRole:
      return mItems[index.row()].type;

    case ItemTitleRole:
      return mItems[index.row()].title;

    case ItemFormatRole:
      return mItems[index.row()].format;

    case ItemPathRole:
      return mItems[index.row()].path;

    case ItemSizeRole:
      return mItems[index.row()].size;

    case ItemHasThumbnailRole:
      return mItems[index.row()].size < 25000000
             && SUPPORTED_DATASET_THUMBNAIL.contains( mItems[index.row()].format );

    case ItemIsFavoriteRole:
      return mFavorites.contains( mItems[index.row()].path );

    case ItemHasWebdavConfigurationRole:
    {
      return WebdavConnection::hasWebdavConfiguration( mItems[index.row()].path );
    }
  }

  return QVariant();
}
