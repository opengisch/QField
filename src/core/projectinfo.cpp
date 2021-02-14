/***************************************************************************
  projectinfo.cpp - ProjectInfo

 ---------------------
 begin                : 14.2.2021
 copyright            : (C) 2021 by Mathieu Pellerin
 email                : nirvn dot asia at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "projectinfo.h"

#include <QFileInfo>
#include <QSettings>
#include <QString>

ProjectInfo::ProjectInfo( QObject *parent ) : QObject( parent )
{
  mSaveExtentTimer.setSingleShot( true );
  connect( &mSaveExtentTimer, &QTimer::timeout, this, &ProjectInfo::extentChanged );
}

void ProjectInfo::setFilePath( const QString &filePath )
{
  if ( mFilePath == filePath )
    return;
  
  mFilePath = filePath;
  
  emit filePathChanged();
}

QString ProjectInfo::filePath() const
{
  return mFilePath;
}

void ProjectInfo::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( !mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, &ProjectInfo::extentChanged );
  }
  
  mMapSettings = mapSettings;
  connect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, [=] { mSaveExtentTimer.start( 750 ); } );
  
  emit mapSettingsChanged();
}

QgsQuickMapSettings *ProjectInfo::mapSettings() const
{
  return mMapSettings;
}

void ProjectInfo::setLayerTree( FlatLayerTreeModel *layerTree )
{
  if ( !layerTree )
    return;

  if ( mLayerTree )
  {
    disconnect( mLayerTree, &FlatLayerTreeModel::mapThemeChanged, this, &ProjectInfo::mapThemeChanged );
  }

  mLayerTree = layerTree;
  connect( mLayerTree, &FlatLayerTreeModel::mapThemeChanged, this, &ProjectInfo::mapThemeChanged );

  emit layerTreeChanged();
}

FlatLayerTreeModel *ProjectInfo::layerTree() const
{
  return mLayerTree;
}

void ProjectInfo::extentChanged()
{
  if ( mFilePath.isEmpty() )
    return;

  QFileInfo fi( mFilePath );
  if ( fi.exists() )
  {
    QSettings settings;
    const QgsRectangle extent = mMapSettings->extent();
    settings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1" ).arg( mFilePath ) );
    settings.setValue( QStringLiteral( "filesize" ), fi.size() );
    settings.setValue( QStringLiteral( "extent" ), QStringLiteral( "%1|%2|%3|%4" ).arg( qgsDoubleToString( extent.xMinimum() ),
                                                                                        qgsDoubleToString( extent.xMaximum() ),
                                                                                        qgsDoubleToString( extent.yMinimum() ),
                                                                                        qgsDoubleToString( extent.yMaximum() ) ) );
    settings.endGroup();
  }
}

void ProjectInfo::mapThemeChanged()
{
  if ( mFilePath.isEmpty() )
    return;

  QFileInfo fi( mFilePath );
  if ( fi.exists() )
  {
    QSettings settings;
    settings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1" ).arg( mFilePath ) );
    settings.setValue( QStringLiteral( "maptheme" ), mLayerTree->mapTheme() );
    settings.endGroup();
  }
}
