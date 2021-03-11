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
#include "platformutilities.h"
#include "qgismobileapp.h"
#include "qfieldcloudutils.h"

#include <QSettings>
#include <QFile>
#include <QDir>

RecentProjectListModel::RecentProjectListModel( QObject *parent )
  : QAbstractListModel( parent )
{
  reloadModel();
}

QHash<int, QByteArray> RecentProjectListModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[ProjectTypeRole]  = "ProjectType";
  roles[ProjectTitleRole]  = "ProjectTitle";
  roles[ProjectPathRole]  = "ProjectPath";
  roles[ProjectDemoRole]  = "ProjectDemo";

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

    QString path = settings.value( QStringLiteral( "path" ) ).toString();
    QFileInfo fi( path );
    if ( fi.exists() )
    {
      ProjectType projectType = path.startsWith( QFieldCloudUtils::localCloudDirectory() )
        ? CloudProject
        : SUPPORTED_PROJECT_EXTENSIONS.contains( fi.suffix() )
          ? LocalProject
          : LocalDataset;
      mRecentProjects.append( RecentProject( projectType,
                                             settings.value( QStringLiteral( "title" ) ).toString(),
                                             path,
                                             settings.value( QStringLiteral( "demo" ), false ).toBool() ) );
    }

    settings.endGroup();
  }
  settings.endGroup();
  
  // update demo projects
  const QList<RecentProject> demoProjects
  {
    RecentProject( LocalProject, QStringLiteral( "Simple Bee Farming Demo" ), QStringLiteral( "/qfield/demo_projects/simple_bee_farming.qgs" ), true ),
    RecentProject( LocalProject, QStringLiteral( "Advanced Bee Farming Demo" ), QStringLiteral( "/qfield/demo_projects/advanced_bee_farming.qgs" ), true ),
    RecentProject( LocalProject, QStringLiteral( "Live QField Users Survey Demo" ), QStringLiteral( "/qfield/demo_projects/live_qfield_users_survey.qgs" ), true )
  };
  for ( const RecentProject &demoProject : demoProjects )
  {
    bool recentProjectsContainsDemoProject = false;
    QMutableListIterator<RecentProject> recentProject( mRecentProjects );
    QString demoProjectPath( PlatformUtilities::instance()->systemGenericDataLocation() + demoProject.path );

    while ( recentProject.hasNext() )
    {
      recentProject.next();

      if ( recentProject.value().path.endsWith( demoProject.path ) )
      {
        // update path: on iOS the path seems to change at each run time
        recentProject.value().path = demoProjectPath;
        recentProject.value().demo = true;
        recentProjectsContainsDemoProject = true;
        break;
      }
    }
    if ( !recentProjectsContainsDemoProject )
    {
      mRecentProjects << demoProject;
      mRecentProjects.last().path = demoProjectPath;
    }
  }

  QMutableListIterator<RecentProject> recentProject( mRecentProjects );
  while ( recentProject.hasNext() )
  {
    recentProject.next();

    if ( ! QFile::exists( recentProject.value().path ) )
      recentProject.remove();
  }

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

  switch ( static_cast<Role>( role ) )
  {
    case ProjectTypeRole:
      return mRecentProjects.at( index.row() ).type;
    case ProjectTitleRole:
      return mRecentProjects.at( index.row() ).title;
    case ProjectPathRole:
      return mRecentProjects.at( index.row() ).path;
    case ProjectDemoRole:
      return mRecentProjects.at( index.row() ).demo;
  }

  return QVariant();
}
