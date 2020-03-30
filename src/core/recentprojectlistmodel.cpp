/***************************************************************************
  recentprojectlistmodel.h

 ---------------------
 begin                : 02.1.2020
 copyright            : (C) 2020 by Mathieu Pellerin
 email                : nirvn dot asia at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "recentprojectlistmodel.h"

#include <QSettings>

RecentProjectListModel::RecentProjectListModel( QObject *parent )
  : QAbstractListModel( parent )
{
  reloadModel();
}

QHash<int, QByteArray> RecentProjectListModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[ProjectTypeRole] = "ProjectType";
  roles[ProjectTitleRole] = "ProjectTitle";
  roles[ProjectPathRole] = "ProjectPath";

  return roles;
}

void RecentProjectListModel::reloadModel()
{
  beginResetModel();
  mRecentProjects.clear();
  QSettings settings;
  settings.beginGroup( "/qgis/recentProjects" );
  const QStringList projectKeysList = settings.childGroups();
  QList<int> projectKeys;
  // This is overdoing it since we're clipping the recent projects list to five items at the moment, but might as well be futureproof
  for ( const QString &key : projectKeysList )
  {
    projectKeys.append( key.toInt() );
  }
  for ( int i = 0; i < projectKeys.count(); i++ )
  {
    settings.beginGroup( QString::number( projectKeys.at( i ) ) );
    mRecentProjects.append( RecentProject( LocalProject,
                                           settings.value( QStringLiteral( "title" ) ).toString(),
                                           settings.value( QStringLiteral( "path" ) ).toString() ) );
    settings.endGroup();
  }
  settings.endGroup();
  endResetModel();
}

int RecentProjectListModel::rowCount( const QModelIndex &parent ) const
{
  if ( !parent.isValid() )
    return mRecentProjects.size();
  else
    return 0;
}

QVariant RecentProjectListModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mRecentProjects.size() || index.row() < 0 )
    return QVariant();

  if ( role == ProjectTypeRole )
    return mRecentProjects.at( index.row() ).type;
  else if ( role == ProjectTitleRole )
    return mRecentProjects.at( index.row() ).title;
  else if ( role == ProjectPathRole )
    return mRecentProjects.at( index.row() ).path;

  return QVariant();
}
