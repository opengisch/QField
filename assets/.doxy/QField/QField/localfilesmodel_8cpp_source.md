

# File localfilesmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**localfilesmodel.cpp**](localfilesmodel_8cpp.md)

[Go to the documentation of this file](localfilesmodel_8cpp.md)


```C++
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
  mFavorites = settings.value( QStringLiteral( "qfieldFavorites" ), QStringList() ).toStringList();

  const QString applicationDirectory = PlatformUtilities::instance()->applicationDirectory();
  mCreatedProjectsPath = QDir::cleanPath( QStringLiteral( "%1/Created Projects" ).arg( applicationDirectory ) );
  mImportedProjectsPath = QDir::cleanPath( QStringLiteral( "%1/Imported Projects" ).arg( applicationDirectory ) );
  mImportedDatasetsPath = QDir::cleanPath( QStringLiteral( "%1/Imported Datasets" ).arg( applicationDirectory ) );
  mSampleProjectsPath = QDir::cleanPath( PlatformUtilities::instance()->systemLocalDataLocation( QLatin1String( "sample_projects" ) ) );

  const bool favoritesInitialized = settings.value( QStringLiteral( "qfieldFavoritesInitialized3" ), false ).toBool();
  if ( !favoritesInitialized )
  {
    mFavorites.clear();

    if ( !applicationDirectory.isEmpty() )
    {
      if ( !mFavorites.contains( mCreatedProjectsPath ) )
      {
        mFavorites << mCreatedProjectsPath;
      }
      if ( !mFavorites.contains( mImportedProjectsPath ) )
      {
        mFavorites << mImportedProjectsPath;
      }
      if ( !mFavorites.contains( mImportedDatasetsPath ) )
      {
        mFavorites << mImportedDatasetsPath;
      }
    }

    if ( !mFavorites.contains( mSampleProjectsPath ) )
    {
      mFavorites << mSampleProjectsPath;
    }

    settings.setValue( QStringLiteral( "qfieldFavorites" ), mFavorites );
    settings.setValue( QStringLiteral( "qfieldFavoritesInitialized3" ), true );
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
  roles[ItemCheckedRole] = "ItemChecked";
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
    return tr( "Home" );
  }
  else if ( path == PlatformUtilities::instance()->applicationDirectory() )
  {
    return tr( "QField files directory" );
  }
  else if ( path == mCreatedProjectsPath )
  {
    return tr( "Created projects" );
  }
  else if ( path == mImportedProjectsPath )
  {
    return tr( "Imported projects" );
  }
  else if ( path == mImportedDatasetsPath )
  {
    return tr( "Imported datasets" );
  }
  else if ( path == mSampleProjectsPath )
  {
    return tr( "Sample projects" );
  }
  else if ( PlatformUtilities::instance()->additionalApplicationDirectories().contains( path ) )
  {
    return tr( "Additional files directory" );
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
    const QStringList favorites = QSettings().value( QStringLiteral( "qfieldFavorites" ), QStringList() ).toStringList();
    QList<LocalFileItem> favoriteItems;
    for ( const QString &item : favorites )
    {
      if ( QFileInfo::exists( item ) )
      {
        favoriteItems << LocalFileItem( ItemMetaType::Favorite, ItemType::SimpleFolder, getCurrentTitleFromPath( item ), QString(), item );
      }
    }

    std::sort( favoriteItems.begin(), favoriteItems.end(), []( const LocalFileItem &a, const LocalFileItem &b ) { return a.title() < b.title(); } );
    mItems.append( favoriteItems );

    const QString applicationDirectory = PlatformUtilities::instance()->applicationDirectory();
    if ( !applicationDirectory.isEmpty() )
    {
      mItems << LocalFileItem( ItemMetaType::Folder, ItemType::ApplicationFolder, tr( "QField files directory" ), QString(), applicationDirectory );
    }

    const QStringList additionalApplicationDirectories = PlatformUtilities::instance()->additionalApplicationDirectories();
    for ( const QString &item : additionalApplicationDirectories )
    {
      QFileInfo fi( item );
      if ( fi.exists() )
      {
        mItems << LocalFileItem( ItemMetaType::Folder, ItemType::ExternalStorage, tr( "Additional files directory" ), QString(), fi.absoluteFilePath() );
      }
    }

    const QStringList rootDirectories = PlatformUtilities::instance()->rootDirectories();
    for ( const QString &item : rootDirectories )
    {
      QFileInfo fi( item );
      if ( fi.exists() )
      {
        mItems << LocalFileItem( ItemMetaType::Folder, ItemType::SimpleFolder, fi.absoluteFilePath(), QString(), fi.absoluteFilePath() );
      }
    }
  }
  else
  {
    QDir dir( path );
    if ( dir.exists() )
    {
      const QStringList items = dir.entryList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::IgnoreCase );
      QList<LocalFileItem> folders;
      QList<LocalFileItem> files;
      QList<LocalFileItem> projects;
      QList<LocalFileItem> datasets;
      for ( const QString &item : items )
      {
        QFileInfo fi( path + QDir::separator() + item );
        if ( fi.isDir() )
        {
          folders << LocalFileItem( ItemMetaType::Folder, ItemType::SimpleFolder, fi.fileName(), QString(), fi.absoluteFilePath() );
        }
        else
        {
          const QString suffix = fi.suffix().toLower();
          if ( ( suffix == QStringLiteral( "png" ) || suffix == QStringLiteral( "jpg" ) ) && items.contains( fi.completeBaseName() ) )
          {
            // Skip project preview images
            continue;
          }
          else if ( suffix == QStringLiteral( "qgs" ) || suffix == QStringLiteral( "qgz" ) )
          {
            QRegularExpression re( QStringLiteral( "(.*)_[A-Za-z]{2}" ) );
            QRegularExpressionMatch match = re.match( fi.completeBaseName() );
            if ( match.hasMatch() )
            {
              if ( items.contains( QStringLiteral( "%1.qgs" ).arg( match.captured( 1 ) ), Qt::CaseInsensitive ) || items.contains( QStringLiteral( "%1.qgz" ).arg( match.captured( 1 ) ), Qt::CaseInsensitive ) )
              {
                // Skip translated project, users should always use the original project
                continue;
              }
            }
          }
          else if ( suffix == QStringLiteral( "zip" ) )
          {
            if ( item.endsWith( QStringLiteral( "_attachments.zip" ), Qt::CaseInsensitive ) )
            {
              const QString reducedItemName = item.mid( 0, item.size() - 16 );
              if ( items.contains( QStringLiteral( "%1.qgs" ).arg( reducedItemName ), Qt::CaseInsensitive ) || items.contains( QStringLiteral( "%1.qgz" ).arg( reducedItemName ), Qt::CaseInsensitive ) )
              {
                // Skip project attachments sidecar file
                continue;
              }
            }
          }
          else if ( item == QStringLiteral( "qfield_webdav_configuration.json" ) )
          {
            // Skip QField WebDAV configuration file
            continue;
          }

          if ( SUPPORTED_PROJECT_EXTENSIONS.contains( suffix ) )
          {
            projects << LocalFileItem( ItemMetaType::Project, ItemType::ProjectFile, fi.completeBaseName(), suffix, fi.absoluteFilePath(), fi.size() );
          }
          else if ( SUPPORTED_VECTOR_EXTENSIONS.contains( suffix ) && suffix != QStringLiteral( "pdf" ) )
          {
            datasets << LocalFileItem( ItemMetaType::Dataset, ItemType::VectorDataset, fi.completeBaseName(), suffix, fi.absoluteFilePath(), fi.size() );
          }
          else if ( SUPPORTED_RASTER_EXTENSIONS.contains( suffix ) )
          {
            datasets << LocalFileItem( ItemMetaType::Dataset, ItemType::RasterDataset, fi.completeBaseName(), suffix, fi.absoluteFilePath(), fi.size() );
          }
          else if ( SUPPORTED_FILE_EXTENSIONS.contains( suffix ) )
          {
            files << LocalFileItem( ItemMetaType::File, ItemType::OtherFile, fi.completeBaseName(), suffix, fi.absoluteFilePath(), fi.size() );
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

  const LocalFileItem &item = mItems[index.row()];

  switch ( static_cast<Role>( role ) )
  {
    case ItemMetaTypeRole:
      return item.metaType();

    case ItemTypeRole:
      return item.type();

    case ItemTitleRole:
      return item.title();

    case ItemFormatRole:
      return item.format();

    case ItemPathRole:
      return item.path();

    case ItemSizeRole:
      return item.size();

    case ItemHasThumbnailRole:
      return item.size() < 25000000 && SUPPORTED_DATASET_THUMBNAIL.contains( item.format() );

    case ItemIsFavoriteRole:
      return mFavorites.contains( item.path() );

    case ItemHasWebdavConfigurationRole:
      return WebdavConnection::hasWebdavConfiguration( item.path() );

    case ItemCheckedRole:
      return item.checked();
  }

  return QVariant();
}

LocalFileItem LocalFilesModel::get( int index ) const
{
  if ( index < 0 || index >= mItems.size() )
    return LocalFileItem();

  return mItems[index];
}

bool LocalFilesModel::inSelectionMode()
{
  if ( currentTitle() == tr( "Home" ) )
    return false;

  return std::any_of( mItems.begin(), mItems.end(), []( const LocalFileItem &item ) { return item.checked(); } );
}

void LocalFilesModel::setChecked( const int &mIdx, const bool &checked )
{
  if ( mIdx < 0 || mIdx >= mItems.size() )
  {
    return;
  }

  const bool hadSelection = inSelectionMode();
  if ( mItems[mIdx].checked() != checked )
  {
    mItems[mIdx].setChecked( checked );

    const QModelIndex changedIndex = index( mIdx, 0, QModelIndex() );
    emit dataChanged( changedIndex, changedIndex, { ItemCheckedRole } );

    const bool hasSelection = inSelectionMode();
    if ( hadSelection != hasSelection )
    {
      emit inSelectionModeChanged();
    }
  }
}

void LocalFilesModel::clearSelection()
{
  for ( LocalFileItem &item : mItems )
  {
    item.setChecked( false );
  }
  emit inSelectionModeChanged();
  emit dataChanged( index( 0, 0, QModelIndex() ), index( static_cast<int>( mItems.size() ) - 1, 0, QModelIndex() ), { ItemCheckedRole } );
}
```


