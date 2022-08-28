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

#include <QDateTime>
#include <QFileInfo>
#include <QSettings>
#include <QString>
#include <qgslayertree.h>
#include <qgslayertreemodel.h>

ProjectInfo::ProjectInfo( QObject *parent )
  : QObject( parent )
{
  mSaveExtentTimer.setSingleShot( true );
  connect( &mSaveExtentTimer, &QTimer::timeout, this, &ProjectInfo::saveExtent );
  connect( &mSaveTemporalStateTimer, &QTimer::timeout, this, &ProjectInfo::saveTemporalState );
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
    disconnect( mMapSettings, &QgsQuickMapSettings::temporalStateChanged, this, &ProjectInfo::temporalStateChanged );
  }

  mMapSettings = mapSettings;
  connect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, &ProjectInfo::extentChanged );
  connect( mMapSettings, &QgsQuickMapSettings::temporalStateChanged, this, &ProjectInfo::temporalStateChanged );

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
  mSaveExtentTimer.start( 1000 );
}

void ProjectInfo::saveExtent()
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
    settings.setValue( QStringLiteral( "extent" ), QStringLiteral( "%1|%2|%3|%4" ).arg( qgsDoubleToString( extent.xMinimum() ), qgsDoubleToString( extent.xMaximum() ), qgsDoubleToString( extent.yMinimum() ), qgsDoubleToString( extent.yMaximum() ) ) );
    settings.endGroup();
  }
}

void ProjectInfo::temporalStateChanged()
{
  mSaveTemporalStateTimer.start( 1000 );
}

void ProjectInfo::saveTemporalState()
{
  if ( mFilePath.isEmpty() )
    return;

  QFileInfo fi( mFilePath );
  if ( fi.exists() )
  {
    QSettings settings;
    settings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1" ).arg( mFilePath ) );
    settings.setValue( QStringLiteral( "filesize" ), fi.size() );
    settings.setValue( QStringLiteral( "isTemporal" ), mMapSettings->isTemporal() );
    settings.setValue( QStringLiteral( "StartDateTime" ), mMapSettings->temporalBegin().toTimeSpec( Qt::LocalTime ).toString( Qt::ISODateWithMs ) );
    settings.setValue( QStringLiteral( "EndDateTime" ), mMapSettings->temporalEnd().toTimeSpec( Qt::LocalTime ).toString( Qt::ISODateWithMs ) );
    settings.endGroup();
  }
}

void ProjectInfo::saveLayerStyle( QgsMapLayer *layer ) const
{
  if ( mFilePath.isEmpty() || !layer )
    return;

  QFileInfo fi( mFilePath );
  if ( fi.exists() )
  {
    const bool isDataset = QgsProject::instance()->readBoolEntry( QStringLiteral( "QField" ), QStringLiteral( "isDataset" ), false );
    QgsMapLayerStyle style;
    style.readFromLayer( layer );

    // Prefix id with :: to avoid loss of slash on linux paths
    QString id( QStringLiteral( "::" ) );
    if ( isDataset )
    {
      // For non-project datasets, the layer id is random, use the source URI
      id += layer->source();
    }
    else
    {
      id += layer->id();
    }

    QSettings settings;
    settings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1/layerstyles" ).arg( mFilePath ) );
    settings.setValue( id, style.xmlData() );
    settings.endGroup();
  }
}

void ProjectInfo::saveLayerTreeState() const
{
  if ( mFilePath.isEmpty() || !mLayerTree )
    return;

  const bool isDataset = QgsProject::instance()->readBoolEntry( QStringLiteral( "QField" ), QStringLiteral( "isDataset" ), false );
  QFileInfo fi( mFilePath );
  if ( fi.exists() && !isDataset )
  {
    QgsMapThemeCollection mapCollection( QgsProject::instance() );
    const QgsMapThemeCollection::MapThemeRecord rec = QgsMapThemeCollection::createThemeFromCurrentState( mLayerTree->layerTreeModel()->rootGroup(), mLayerTree->layerTreeModel() );
    mapCollection.insert( QStringLiteral( "::QFieldLayerTreeState" ), rec );

    const QDomDocumentType documentType = QDomImplementation().createDocumentType( QStringLiteral( "qgis" ), QStringLiteral( "http://mrcc.com/qgis.dtd" ), QStringLiteral( "SYSTEM" ) );
    QDomDocument document( documentType );

    document.appendChild( document.createElement( QStringLiteral( "qgis" ) ) );
    mapCollection.writeXml( document );

    QSettings settings;
    settings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1" ).arg( mFilePath ) );
    settings.setValue( QStringLiteral( "filesize" ), fi.size() );
    settings.setValue( QStringLiteral( "layertreestate" ), document.toString() );
    settings.remove( QStringLiteral( "maptheme" ) );
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
    settings.setValue( QStringLiteral( "filesize" ), fi.size() );
    if ( !mLayerTree->mapTheme().isEmpty() )
    {
      settings.setValue( QStringLiteral( "maptheme" ), mLayerTree->mapTheme() );
      settings.remove( QStringLiteral( "layertreestate" ) );
    }
    else
    {
      settings.remove( QStringLiteral( "maptheme" ) );
    }
    settings.endGroup();
  }
}
