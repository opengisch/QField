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

#include "platformutilities.h"
#include "qfieldcloudutils.h"
#include "qgismobileapp.h"
#include "recentprojectlistmodel.h"

#include <QDir>
#include <QFile>
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
  roles[ProjectSampleRole] = "ProjectSample";

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
    if ( fi.exists() && fi.isReadable() )
    {
      ProjectType projectType = path.startsWith( QFieldCloudUtils::localCloudDirectory() )
                                  ? CloudProject
                                : SUPPORTED_PROJECT_EXTENSIONS.contains( fi.suffix() )
                                  ? LocalProject
                                  : LocalDataset;
      mRecentProjects.append( RecentProject( projectType,
                                             settings.value( QStringLiteral( "title" ) ).toString(),
                                             path,
                                             settings.value( QStringLiteral( "sample" ), false ).toBool() ) );
    }

    settings.endGroup();
  }
  settings.endGroup();

  // update sample projects
  const QList<RecentProject> sampleProjects {
    RecentProject( LocalProject, QStringLiteral( "Bee Farming Sample Project" ), QStringLiteral( "/bees.qgz" ), true ),
    RecentProject( LocalProject, QStringLiteral( "Wasterwater Management Sample Project" ), QStringLiteral( "/wastewater.qgz" ), true ),
    RecentProject( LocalProject, QStringLiteral( "Live QField Users Survey" ), QStringLiteral( "/live_qfield_users_survey.qgs" ), true ) };
  for ( const RecentProject &sampleProject : sampleProjects )
  {
    bool recentProjectsContainsSampleProject = false;
    QMutableListIterator<RecentProject> recentProject( mRecentProjects );
    QString sampleProjectPath( PlatformUtilities::instance()->systemLocalDataLocation( QLatin1String( "sample_projects" ) ) + sampleProject.path );

    while ( recentProject.hasNext() )
    {
      recentProject.next();

      if ( recentProject.value().path.endsWith( sampleProject.path ) )
      {
#ifdef Q_OS_IOS
        // update path: on iOS the path seems to change at each run time
        recentProject.value().path = sampleProjectPath;
#endif
        recentProject.value().sample = true;
        recentProjectsContainsSampleProject = true;
        break;
      }
    }
    if ( !recentProjectsContainsSampleProject )
    {
      mRecentProjects << sampleProject;
      mRecentProjects.last().path = sampleProjectPath;
    }
  }

  QMutableListIterator<RecentProject> recentProject( mRecentProjects );
  while ( recentProject.hasNext() )
  {
    recentProject.next();

    if ( !QFile::exists( recentProject.value().path ) )
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
    case ProjectSampleRole:
      return mRecentProjects.at( index.row() ).sample;
  }

  return QVariant();
}
