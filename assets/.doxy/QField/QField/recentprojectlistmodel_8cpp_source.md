

# File recentprojectlistmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**recentprojectlistmodel.cpp**](recentprojectlistmodel_8cpp.md)

[Go to the documentation of this file](recentprojectlistmodel_8cpp.md)


```C++
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
#include "qfield.h"
#include "qfieldcloudutils.h"
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
  roles[ProjectThumbnailRole] = "ProjectThumbnail";

  return roles;
}

void RecentProjectListModel::reloadModel()
{
  beginResetModel();
  mRecentProjects.clear();

  QSettings settings;
  mRecentProjects = recentProjects( true );

  const bool sampleProjectsAdded = settings.value( QStringLiteral( "QField/recentProjectsAdded" ), false ).toBool();
  if ( !sampleProjectsAdded )
  {
    const QString sampleProjectsDirectory = PlatformUtilities::instance()->systemLocalDataLocation( QLatin1String( "sample_projects" ) );
    const QString sampleProjectsJson = QStringLiteral( "%1/sample_projects.json" ).arg( sampleProjectsDirectory );
    if ( QFileInfo::exists( sampleProjectsJson ) )
    {
      bool sampleProjectsJsonIsValid = true;
      QFile sampleProjectsFile( sampleProjectsJson );
      QJsonDocument doc;
      if ( sampleProjectsFile.open( QIODevice::ReadOnly ) )
      {
        QJsonParseError error;
        doc = QJsonDocument::fromJson( sampleProjectsFile.readAll(), &error );
        if ( doc.isNull() || !doc.isArray() )
        {
          sampleProjectsJsonIsValid = false;
        }
      }
      else
      {
        sampleProjectsJsonIsValid = false;
      }

      if ( sampleProjectsJsonIsValid )
      {
        const QJsonArray values = doc.array();
        for ( const QJsonValueConstRef &value : values )
        {
          if ( !value.isObject() )
          {
            continue;
          }

          const QJsonObject valueObject = value.toObject();
          mRecentProjects.append( RecentProject( LinkProject,
                                                 valueObject.value( QStringLiteral( "title" ) ).toString(),
                                                 valueObject.value( QStringLiteral( "link" ) ).toString(),
                                                 QStringLiteral( "%1/%2" ).arg( sampleProjectsDirectory, valueObject.value( QStringLiteral( "thumbnail" ) ).toString() ) ) );

          settings.beginGroup( QStringLiteral( "/qgis/recentProjects/%1" ).arg( mRecentProjects.count() ) );
          settings.setValue( QStringLiteral( "title" ), mRecentProjects.last().title );
          settings.setValue( QStringLiteral( "path" ), mRecentProjects.last().path );
          settings.setValue( QStringLiteral( "thumbnail" ), mRecentProjects.last().thumbnail );
          settings.endGroup();
        }
      }
    }
    settings.setValue( QStringLiteral( "QField/recentProjectsAdded" ), true );
  }

  endResetModel();
}

int RecentProjectListModel::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mRecentProjects.size() ) : 0;
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
    case ProjectThumbnailRole:
      return mRecentProjects.at( index.row() ).thumbnail;
  }

  return QVariant();
}

void RecentProjectListModel::removeRecentProject( const QString &path )
{
  QList<RecentProject> projects = recentProjects();
  bool removed = false;
  for ( int idx = 0; idx < projects.count(); idx++ )
  {
    if ( projects.at( idx ).path == path )
    {
      projects.removeAt( idx );
      removed = true;
      break;
    }
  }
  if ( removed )
  {
    saveRecentProjects( projects );
  }
}

QList<RecentProjectListModel::RecentProject> RecentProjectListModel::recentProjects( bool skipNonExistent )
{
  QSettings settings;
  QList<RecentProject> projects;

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

    const QString path = settings.value( QStringLiteral( "path" ) ).toString();
    const QFileInfo fi( path );

    ProjectType type = LocalDataset;
    if ( path.startsWith( QFieldCloudUtils::localCloudDirectory() ) )
    {
      type = CloudProject;
    }
    else if ( path.startsWith( "http://", Qt::CaseInsensitive ) || path.startsWith( "https://", Qt::CaseInsensitive ) )
    {
      type = LinkProject;
    }
    else if ( SUPPORTED_PROJECT_EXTENSIONS.contains( fi.suffix() ) )
    {
      type = LocalProject;
    }
    else
    {
      type = LocalDataset;
    }

    if ( !skipNonExistent || type == LinkProject || fi.exists() )
    {
      projects.append( RecentProject( type,
                                      settings.value( QStringLiteral( "title" ) ).toString(),
                                      path,
                                      settings.value( QStringLiteral( "thumbnail" ) ).toString() ) );
    }

    settings.endGroup();
  }
  settings.endGroup();

  return projects;
}

void RecentProjectListModel::saveRecentProjects( const QList<RecentProject> &projects )
{
  QSettings settings;
  settings.remove( QStringLiteral( "/qgis/recentProjects" ) );
  for ( int idx = 0; idx < projects.count() && idx < 5; idx++ )
  {
    settings.beginGroup( QStringLiteral( "/qgis/recentProjects/%1" ).arg( idx ) );
    settings.setValue( QStringLiteral( "title" ), projects.at( idx ).title );
    settings.setValue( QStringLiteral( "path" ), projects.at( idx ).path );
    settings.setValue( QStringLiteral( "thumbnail" ), projects.at( idx ).thumbnail );
    settings.endGroup();
  }
}
```


