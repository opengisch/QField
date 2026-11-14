

# File qfdrawingtemplatemodel.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfdrawingtemplatemodel.cpp**](qfdrawingtemplatemodel_8cpp.md)

[Go to the documentation of this file](qfdrawingtemplatemodel_8cpp.md)


```C++
/***************************************************************************
  qfdrawingtemplatemodel.cpp

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

#include "qfdrawingtemplatemodel.h"
#include "qffileutils.h"
#include "qfplatformutilities.h"

#include <QDir>
#include <QFile>

QfDrawingTemplateModel::QfDrawingTemplateModel( QObject *parent )
  : QAbstractListModel( parent )
{
  reloadModel();
}

QHash<int, QByteArray> QfDrawingTemplateModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[TemplateTypeRole] = "templateType";
  roles[TemplateTitleRole] = "templateTitle";
  roles[TemplatePathRole] = "templatePath";

  return roles;
}

void QfDrawingTemplateModel::reloadModel()
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
  dirs << QfPlatformUtilities::instance()->appDataDirs();
  for ( const QString &dir : dirs )
  {
    QDir templateDir( dir + QStringLiteral( "drawing_templates/" ) );
    if ( templateDir.exists() )
    {
      const QStringList templates = templateDir.entryList( QStringList() << "*.*", QDir::Files );
      for ( const QString &templateFile : templates )
      {
        const QFileInfo templateInfo( dir + QStringLiteral( "drawing_templates/" ) + templateFile );
        if ( QfFileUtils::isImageMimeTypeSupported( QfFileUtils::mimeTypeName( templateInfo.absoluteFilePath() ) ) )
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

int QfDrawingTemplateModel::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mTemplates.size() ) : 0;
}

QVariant QfDrawingTemplateModel::data( const QModelIndex &index, int role ) const
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

QString QfDrawingTemplateModel::projectFilePath() const
{
  return mProjectFilePath;
}

void QfDrawingTemplateModel::setProjectFilePath( const QString &path )
{
  if ( mProjectFilePath == path )
  {
    return;
  }

  mProjectFilePath = path;
  emit projectFilePathChanged();

  reloadModel();
}

bool QfDrawingTemplateModel::hasProjectTemplate() const
{
  return mHasProjectTemplate;
}
```


