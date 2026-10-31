

# File qflocalfilesmodel.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qflocalfilesmodel.cpp**](qflocalfilesmodel_8cpp.md)

[Go to the documentation of this file](qflocalfilesmodel_8cpp.md)


```C++
/***************************************************************************
  qflocalfilesmodel.cpp

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

#include "qfcloudutils.h"
#include "qffileutils.h"
#include "qfield.h"
#include "qflocalfilesmodel.h"
#include "qfnetworkreply.h"
#include "qfplatformutilities.h"
#include "qfwebdavconnection.h"

#include <QDir>
#include <QFile>
#include <QImageReader>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtConcurrent>

#include <algorithm>

QfLocalFilesModel::QfLocalFilesModel( QObject *parent )
  : QAbstractListModel( parent )
{
  QSettings settings;
  mFavorites = settings.value( QStringLiteral( "qfieldFavorites" ), QStringList() ).toStringList();

  const QString applicationDirectory = QfPlatformUtilities::instance()->applicationDirectory();
  mCreatedProjectsPath = QDir::cleanPath( QStringLiteral( "%1/Created Projects" ).arg( applicationDirectory ) );
  mImportedProjectsPath = QDir::cleanPath( QStringLiteral( "%1/Imported Projects" ).arg( applicationDirectory ) );
  mImportedDatasetsPath = QDir::cleanPath( QStringLiteral( "%1/Imported Datasets" ).arg( applicationDirectory ) );

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

    settings.setValue( QStringLiteral( "qfieldFavorites" ), mFavorites );
    settings.setValue( QStringLiteral( "qfieldFavoritesInitialized3" ), true );
  }

  // Remove obsolete sample projects path
  const QString sampleProjectsPath = QDir::cleanPath( QfPlatformUtilities::instance()->systemLocalDataLocation( QLatin1String( "sample_projects" ) ) );
  if ( mFavorites.contains( sampleProjectsPath ) )
  {
    removeFromFavorites( sampleProjectsPath );
  }

  resetToRoot();
}

QHash<int, QByteArray> QfLocalFilesModel::roleNames() const
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
  roles[ItemCloudStatusRole] = "ItemCloudStatus";
  return roles;
}

void QfLocalFilesModel::resetToRoot()
{
  clearCloudFiles();
  mHistory.clear();
  mHistory << QLatin1String( "root" );

  emit currentPathChanged();

  reloadModel();
}

void QfLocalFilesModel::resetToPath( const QString &path )
{
  clearCloudFiles();
  mHistory.clear();
  setCurrentPath( path );

  if ( mHistory.isEmpty() )
  {
    resetToRoot();
  }
}

bool QfLocalFilesModel::isPathFavoriteEditable( const QString & )
{
  return true;
}

void QfLocalFilesModel::addToFavorites( const QString &path )
{
  if ( !mFavorites.contains( path ) )
  {
    mFavorites << path;
    QSettings().setValue( QStringLiteral( "qfieldFavorites" ), mFavorites );
    reloadModel();
  }
}

void QfLocalFilesModel::removeFromFavorites( const QString &path )
{
  if ( mFavorites.removeAll( path ) )
  {
    QSettings().setValue( QStringLiteral( "qfieldFavorites" ), mFavorites );
    reloadModel();
  }
}

QString QfLocalFilesModel::currentTitle() const
{
  return getCurrentTitleFromPath( currentPath() );
}

const QString QfLocalFilesModel::getCurrentTitleFromPath( const QString &path ) const
{
  if ( path == QLatin1String( "root" ) )
  {
    return tr( "Home" );
  }
  else if ( path == QfPlatformUtilities::instance()->applicationDirectory() )
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
  else if ( QfPlatformUtilities::instance()->additionalApplicationDirectories().contains( path ) )
  {
    return tr( "Additional files directory" );
  }
  else
  {
    const QString cloudProjectId = QfCloudUtils::getProjectId( path );
    if ( !cloudProjectId.isEmpty() )
    {
      const QFileInfo fi( path );
      if ( fi.fileName() == cloudProjectId )
      {
        return QfCloudUtils::projectSetting( cloudProjectId, QStringLiteral( "name" ), QString() ).toString();
      }
    }
  }

  return QFileInfo( path ).fileName();
}

void QfLocalFilesModel::setCurrentPath( const QString &path )
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

QString QfLocalFilesModel::currentPath() const
{
  return ( !mHistory.isEmpty() ? mHistory.last() : QString() );
}

bool QfLocalFilesModel::isDeletedAllowedInCurrentPath() const
{
  const QString path = currentPath();
  const QString applicationDirectory = QfPlatformUtilities::instance()->applicationDirectory();
  if ( !applicationDirectory.isEmpty() && path.startsWith( applicationDirectory ) )
  {
    return true;
  }
  else
  {
    const QStringList additionalApplicationDirectories = QfPlatformUtilities::instance()->additionalApplicationDirectories();
    if ( std::any_of( additionalApplicationDirectories.begin(), additionalApplicationDirectories.end(), [&path]( const QString &directory ) { return ( !directory.isEmpty() && path.startsWith( directory ) ); } ) )
    {
      return true;
    }
  }

  return false;
}

void QfLocalFilesModel::moveUp()
{
  mHistory.removeLast();

  emit currentPathChanged();

  reloadModel();
}

void QfLocalFilesModel::reloadModel()
{
  beginResetModel();
  mItems.clear();

  const QString path = currentPath();
  if ( path == QLatin1String( "root" ) )
  {
    QList<QfLocalFileItem> favoriteItems;
    for ( const QString &item : mFavorites )
    {
      if ( QFileInfo::exists( item ) )
      {
        favoriteItems << QfLocalFileItem( ItemMetaType::Favorite, ItemType::SimpleFolder, getCurrentTitleFromPath( item ), QString(), item );
      }
    }

    std::sort( favoriteItems.begin(), favoriteItems.end(), []( const QfLocalFileItem &a, const QfLocalFileItem &b ) { return a.title() < b.title(); } );
    mItems.append( favoriteItems );

    const QString applicationDirectory = QfPlatformUtilities::instance()->applicationDirectory();
    if ( !applicationDirectory.isEmpty() )
    {
      mItems << QfLocalFileItem( ItemMetaType::Folder, ItemType::ApplicationFolder, tr( "QField files directory" ), QString(), applicationDirectory );
    }

    const QStringList additionalApplicationDirectories = QfPlatformUtilities::instance()->additionalApplicationDirectories();
    for ( const QString &item : additionalApplicationDirectories )
    {
      QFileInfo fi( item );
      if ( fi.exists() )
      {
        mItems << QfLocalFileItem( ItemMetaType::Folder, ItemType::QfExternalStorage, tr( "Additional files directory" ), QString(), fi.absoluteFilePath() );
      }
    }

    const QStringList rootDirectories = QfPlatformUtilities::instance()->rootDirectories();
    for ( const QString &item : rootDirectories )
    {
      QFileInfo fi( item );
      if ( fi.exists() )
      {
        mItems << QfLocalFileItem( ItemMetaType::Folder, ItemType::SimpleFolder, fi.absoluteFilePath(), QString(), fi.absoluteFilePath() );
      }
    }
  }
  else
  {
    QDir dir( path );
    if ( dir.exists() )
    {
      const QStringList items = dir.entryList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::IgnoreCase );
      QList<QfLocalFileItem> folders;
      QList<QfLocalFileItem> files;
      QList<QfLocalFileItem> projects;
      QList<QfLocalFileItem> datasets;
      for ( const QString &item : items )
      {
        QFileInfo fi( path + QDir::separator() + item );
        if ( fi.isDir() )
        {
          folders << QfLocalFileItem( ItemMetaType::Folder, ItemType::SimpleFolder, getCurrentTitleFromPath( fi.absoluteFilePath() ), QString(), fi.absoluteFilePath() );
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
            QRegularExpression re( QStringLiteral( "^(.*)_[A-Za-z]{2}$" ) );
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
            projects << QfLocalFileItem( ItemMetaType::Project, ItemType::ProjectFile, fi.completeBaseName(), suffix, fi.absoluteFilePath(), fi.size() );
          }
          else if ( SUPPORTED_VECTOR_EXTENSIONS.contains( suffix ) && suffix != QStringLiteral( "pdf" ) )
          {
            datasets << QfLocalFileItem( ItemMetaType::Dataset, ItemType::VectorDataset, fi.completeBaseName(), suffix, fi.absoluteFilePath(), fi.size() );
          }
          else if ( SUPPORTED_RASTER_EXTENSIONS.contains( suffix ) )
          {
            datasets << QfLocalFileItem( ItemMetaType::Dataset, ItemType::RasterDataset, fi.completeBaseName(), suffix, fi.absoluteFilePath(), fi.size() );
          }
          else if ( SUPPORTED_FILE_EXTENSIONS.contains( suffix ) )
          {
            files << QfLocalFileItem( ItemMetaType::File, ItemType::OtherFile, fi.completeBaseName(), suffix, fi.absoluteFilePath(), fi.size() );
          }
        }
      }
      mItems << folders << projects << datasets << files;
    }
  }

  endResetModel();

  const QString cloudProjectId = QfCloudUtils::getProjectId( currentPath() );
  if ( !cloudProjectId.isEmpty() && cloudProjectId != mCloudFilesProjectId )
  {
    fetchCloudStatus();
  }
  else if ( mCloudFilesFetched )
  {
    updateCloudStatus();
  }
}

int QfLocalFilesModel::rowCount( const QModelIndex &parent ) const
{
  if ( !parent.isValid() )
    return static_cast<int>( mItems.size() );
  else
    return 0;
}

QVariant QfLocalFilesModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mItems.size() || index.row() < 0 )
    return QVariant();

  const QfLocalFileItem &item = mItems[index.row()];

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
      return QfWebdavConnection::hasWebdavConfiguration( item.path() );

    case ItemCheckedRole:
      return item.checked();

    case ItemCloudStatusRole:
      return item.cloudStatus();
  }

  return QVariant();
}

QfLocalFileItem QfLocalFilesModel::get( int index ) const
{
  if ( index < 0 || index >= mItems.size() )
    return QfLocalFileItem();

  return mItems[index];
}

bool QfLocalFilesModel::inSelectionMode()
{
  if ( currentTitle() == tr( "Home" ) )
    return false;

  return std::any_of( mItems.begin(), mItems.end(), []( const QfLocalFileItem &item ) { return item.checked(); } );
}

int QfLocalFilesModel::selectedCount() const
{
  return static_cast<int>( std::count_if( mItems.begin(), mItems.end(), []( const QfLocalFileItem &item ) { return item.checked(); } ) );
}

qint64 QfLocalFilesModel::selectedSize() const
{
  return std::accumulate( mItems.begin(), mItems.end(), qint64( 0 ), []( qint64 sum, const QfLocalFileItem &item ) {
    return item.checked() ? sum + item.size() : sum;
  } );
}

void QfLocalFilesModel::setChecked( const int &mIdx, const bool &checked )
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
    emit selectionDetailsChanged();
  }
}

void QfLocalFilesModel::clearSelection()
{
  for ( QfLocalFileItem &item : mItems )
  {
    item.setChecked( false );
  }
  emit inSelectionModeChanged();
  emit selectionDetailsChanged();
  emit dataChanged( index( 0, 0, QModelIndex() ), index( static_cast<int>( mItems.size() ) - 1, 0, QModelIndex() ), { ItemCheckedRole } );
}

QfCloudConnection *QfLocalFilesModel::cloudConnection() const
{
  return mCloudConnection;
}

void QfLocalFilesModel::setCloudConnection( QfCloudConnection *cloudConnection )
{
  if ( mCloudConnection == cloudConnection )
  {
    return;
  }

  if ( mCloudConnection )
  {
    disconnect( mCloudConnection, nullptr, this, nullptr );
  }

  mCloudConnection = cloudConnection;

  if ( mCloudConnection )
  {
    connect( mCloudConnection, &QfCloudConnection::pendingAttachmentsUploadFinished, this, &QfLocalFilesModel::fetchCloudStatus );
  }

  emit cloudConnectionChanged();

  fetchCloudStatus();
}

void QfLocalFilesModel::clearCloudFiles()
{
  mCloudFilesProjectId.clear();
  mCloudFiles.clear();
  mCloudFilesFetched = false;
  mCloudFilesRequestId++;
  setIsFetchingCloudStatus( false );
}

void QfLocalFilesModel::setIsFetchingCloudStatus( bool isFetching )
{
  if ( mIsFetchingCloudStatus == isFetching )
  {
    return;
  }

  mIsFetchingCloudStatus = isFetching;
  emit isFetchingCloudStatusChanged();
}

void QfLocalFilesModel::fetchCloudStatus()
{
  clearCloudFiles();

  if ( !mCloudConnection || mCloudConnection->status() != QfCloudConnection::ConnectionStatus::LoggedIn )
  {
    return;
  }

  const QString projectId = QfCloudUtils::getProjectId( currentPath() );
  if ( projectId.isEmpty() )
  {
    return;
  }

  mCloudFilesProjectId = projectId;
  setIsFetchingCloudStatus( true );
  fetchCloudFiles( QStringLiteral( "/api/v1/files/%1/" ).arg( projectId ) );
}

void QfLocalFilesModel::fetchCloudFiles( const QString &endpoint )
{
  const int requestId = mCloudFilesRequestId;
  QfNetworkReply *reply = mCloudConnection->get( endpoint );
  connect( reply, &QfNetworkReply::finished, this, [this, reply, requestId]() {
    QNetworkReply *rawReply = reply->currentRawReply();
    reply->deleteLater();

    if ( requestId != mCloudFilesRequestId )
    {
      return;
    }

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      clearCloudFiles();
      return;
    }

    const QJsonArray files = QJsonDocument::fromJson( rawReply->readAll() ).array();
    for ( const QJsonValueConstRef &fileValue : files )
    {
      const QJsonObject fileObject = fileValue.toObject();
      mCloudFiles.insert( fileObject.value( QStringLiteral( "name" ) ).toString(), qMakePair( fileObject.value( QStringLiteral( "size" ) ).toInteger(), fileObject.value( QStringLiteral( "md5sum" ) ).toString() ) );
    }

    const QString nextPage = QString::fromUtf8( rawReply->rawHeader( QByteArrayLiteral( "X-Next-Page" ) ) );
    if ( !nextPage.isEmpty() && mCloudConnection )
    {
      fetchCloudFiles( nextPage );
      return;
    }

    mCloudFilesFetched = true;
    setIsFetchingCloudStatus( false );
    updateCloudStatus();
  } );
}

void QfLocalFilesModel::updateCloudStatus()
{
  if ( mCloudFilesProjectId.isEmpty() || !mCloudConnection )
  {
    return;
  }

  const QString projectFilePath = QfCloudUtils::localProjectFilePath( mCloudConnection->username(), mCloudFilesProjectId );
  if ( projectFilePath.isEmpty() )
  {
    return;
  }

  const QDir projectDir = QFileInfo( projectFilePath ).absoluteDir();
  if ( !currentPath().startsWith( projectDir.absolutePath() ) )
  {
    return;
  }

  QList<QPair<QString, QPair<qint64, QString>>> candidates;
  for ( const QfLocalFileItem &item : std::as_const( mItems ) )
  {
    if ( item.metaType() == ItemMetaType::Folder || item.metaType() == ItemMetaType::Favorite )
    {
      continue;
    }

    const auto it = mCloudFiles.constFind( projectDir.relativeFilePath( item.path() ) );
    if ( it != mCloudFiles.constEnd() )
    {
      candidates << qMakePair( item.path(), it.value() );
    }
  }

  if ( candidates.isEmpty() )
  {
    return;
  }

  QtConcurrent::run( [candidates]() {
    QHash<QString, CloudStatus> statuses;
    for ( const auto &candidate : candidates )
    {
      const bool sameSize = QFileInfo( candidate.first ).size() == candidate.second.first;
      statuses.insert( candidate.first, sameSize && QfFileUtils::fileEtag( candidate.first ) == candidate.second.second ? CloudStatus::Synchronized : CloudStatus::Divergent );
    }
    return statuses;
  } )
    .then( this, [this]( const QHash<QString, CloudStatus> &statuses ) {
      for ( int i = 0; i < mItems.size(); i++ )
      {
        const auto it = statuses.constFind( mItems[i].path() );
        if ( it != statuses.constEnd() && mItems[i].cloudStatus() != it.value() )
        {
          mItems[i].setCloudStatus( it.value() );
          const QModelIndex changedIndex = index( i, 0, QModelIndex() );
          emit dataChanged( changedIndex, changedIndex, { ItemCloudStatusRole } );
        }
      }
    } );
}
```


