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
#include "qgismobileapp.h"

#include <QDir>
#include <QFile>

LocalFilesModel::LocalFilesModel( QObject *parent )
  : QAbstractListModel( parent )
{
  resetToRoot();
}

QHash<int, QByteArray> LocalFilesModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[ItemTypeRole] = "ItemType";
  roles[ItemTitleRole] = "ItemTitle";
  roles[ItemPathRole] = "ItemPath";
  return roles;
}

void LocalFilesModel::resetToRoot()
{
  mHistory.clear();
  mHistory << QStringLiteral( "root" );

  emit currentPathChanged();

  reloadModel();
}

void LocalFilesModel::resetToPath( const QString &path )
{
  mHistory.clear();
  mHistory << path;

  emit currentPathChanged();

  reloadModel();
}

void LocalFilesModel::setCurrentPath( const QString &path )
{
  mHistory << path;

  emit currentPathChanged();

  reloadModel();
}

QString LocalFilesModel::currentPath() const
{
  return ( !mHistory.isEmpty() ? mHistory.last() : QString() );
}

void LocalFilesModel::moveUp()
{
  mHistory.removeLast();
  reloadModel();
}

void LocalFilesModel::reloadModel()
{
  beginResetModel();
  mItems.clear();

  const QString path = currentPath();
  if ( path.compare( QStringLiteral( "root" ) ) )
  {
    const QStringList rootDirectories = PlatformUtilities::instance()->rootDirectories();
    for ( const QString &item : rootDirectories )
    {
      QFileInfo fi( item );
      if ( fi.exists() )
      {
        mItems << Item( ItemType::Folder, fi.baseName(), fi.absolutePath() );
      }
    }
  }
  else
  {
    QDir dir( path );
    if ( dir.exists() )
    {
      const QStringList items = dir.entryList( QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::IgnoreCase );
      for ( const QString &item : items )
      {
        QFileInfo fi( path + QDir::separator() + item );
        if ( fi.isDir() )
        {
          mItems << Item( ItemType::Folder, fi.baseName(), fi.absolutePath() );
        }
        else
        {
          const QString suffix = fi.completeSuffix().toLower();
          if ( SUPPORTED_PROJECT_EXTENSIONS.contains( suffix ) )
          {
            mItems << Item( ItemType::Project, fi.baseName(), fi.absolutePath() );
          }
          else if ( SUPPORTED_VECTOR_EXTENSIONS.contains( suffix ) )
          {
            mItems << Item( ItemType::VectorDataset, fi.baseName(), fi.absolutePath() );
          }
          else if ( SUPPORTED_RASTER_EXTENSIONS.contains( suffix ) )
          {
            mItems << Item( ItemType::RasterDataset, fi.baseName(), fi.absolutePath() );
          }
        }
      }
    }
  }

  endResetModel();
}

int LocalFilesModel::rowCount( const QModelIndex &parent ) const
{
  if ( !parent.isValid() )
    return mItems.size();
  else
    return 0;
}

QVariant LocalFilesModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mItems.size() || index.row() < 0 )
    return QVariant();

  switch ( static_cast<Role>( role ) )
  {
    case ItemTypeRole:
      return mItems[index.row()].type;

    case ItemTitleRole:
      return mItems[index.row()].title;

    case ItemPathRole:
      return mItems[index.row()].path;
  }

  return QVariant();
}
