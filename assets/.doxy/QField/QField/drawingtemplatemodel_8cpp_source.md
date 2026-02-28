

# File drawingtemplatemodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**drawingtemplatemodel.cpp**](drawingtemplatemodel_8cpp.md)

[Go to the documentation of this file](drawingtemplatemodel_8cpp.md)


```C++
/***************************************************************************
  drawingtemplatemodel.cpp

 ---------------------
 begin                : 28.03.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "drawingtemplatemodel.h"
#include "fileutils.h"
#include "platformutilities.h"

#include <QDir>
#include <QFile>

DrawingTemplateModel::DrawingTemplateModel( QObject *parent )
  : QAbstractListModel( parent )
{
  reloadModel();
}

QHash<int, QByteArray> DrawingTemplateModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[TemplateTypeRole] = "templateType";
  roles[TemplateTitleRole] = "templateTitle";
  roles[TemplatePathRole] = "templatePath";

  return roles;
}

void DrawingTemplateModel::reloadModel()
{
  beginResetModel();
  mTemplates.clear();

  QStringList dirs;
  bool hasProjectTemplate = false;
  QString projectPath;

  // Project templates
  if ( !mProjectFilePath.isEmpty() )
  {
    QFileInfo projectInfo( mProjectFilePath );
    projectPath = projectInfo.absolutePath() + QStringLiteral( "/" );
    dirs << projectPath;
  }

  // App-wide templates
  dirs << PlatformUtilities::instance()->appDataDirs();
  for ( const QString &dir : dirs )
  {
    QDir templateDir( dir + QStringLiteral( "drawing_templates/" ) );
    if ( templateDir.exists() )
    {
      const QStringList templates = templateDir.entryList( QStringList() << "*.*", QDir::Files );
      for ( const QString &templateFile : templates )
      {
        const QFileInfo templateInfo( dir + QStringLiteral( "drawing_templates/" ) + templateFile );
        if ( FileUtils::isImageMimeTypeSupported( FileUtils::mimeTypeName( templateInfo.absoluteFilePath() ) ) )
        {
          const TemplateType type = !projectPath.isEmpty() && templateDir == projectPath ? ProjectTemplate : AppTemplate;
          mTemplates << Template( type, templateInfo.baseName(), templateInfo.absoluteFilePath() );

          if ( type == ProjectTemplate )
          {
            hasProjectTemplate = true;
          }
        }
      }
    }
  }

  std::sort( mTemplates.begin(), mTemplates.end(), []( const Template &t1, const Template &t2 ) {
    if ( t1.type != t2.type )
    {
      return t2.type == ProjectTemplate;
    }

    return t1.title <= t2.title;
  } );

  // Add blank template
  mTemplates.prepend( Template( AppTemplate, tr( "Blank" ), QString() ) );

  endResetModel();

  if ( mHasProjectTemplate != hasProjectTemplate )
  {
    mHasProjectTemplate = hasProjectTemplate;
    emit hasProjectTemplateChanged();
  }
}

int DrawingTemplateModel::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mTemplates.size() ) : 0;
}

QVariant DrawingTemplateModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mTemplates.size() || index.row() < 0 )
  {
    return QVariant();
  }

  switch ( static_cast<Role>( role ) )
  {
    case TemplateTypeRole:
      return mTemplates.at( index.row() ).type;
    case TemplateTitleRole:
      return mTemplates.at( index.row() ).title;
    case TemplatePathRole:
      return mTemplates.at( index.row() ).path;
  }

  return QVariant();
}

QString DrawingTemplateModel::projectFilePath() const
{
  return mProjectFilePath;
}

void DrawingTemplateModel::setProjectFilePath( const QString &path )
{
  if ( mProjectFilePath == path )
  {
    return;
  }

  mProjectFilePath = path;
  emit projectFilePathChanged();

  reloadModel();
}

bool DrawingTemplateModel::hasProjectTemplate() const
{
  return mHasProjectTemplate;
}
```


