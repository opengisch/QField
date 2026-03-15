

# File projectinfo.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**projectinfo.cpp**](projectinfo_8cpp.md)

[Go to the documentation of this file](projectinfo_8cpp.md)


```C++
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


#include "expressioncontextutils.h"
#include "projectinfo.h"

#include <QDateTime>
#include <QFileInfo>
#include <QString>
#include <QTextDocument>
#include <qgscolorutils.h>
#include <qgslayertree.h>
#include <qgslayertreemodel.h>
#include <qgslinesymbol.h>
#include <qgsmaplayerstyle.h>
#include <qgsmarkersymbol.h>
#include <qgssymbollayerutils.h>

ProjectInfo::ProjectInfo( QObject *parent )
  : QObject( parent )
{
  mSaveExtentTimer.setSingleShot( true );
  mSaveRotationTimer.setSingleShot( true );
  mSaveTemporalStateTimer.setSingleShot( true );
  connect( &mSaveExtentTimer, &QTimer::timeout, this, &ProjectInfo::saveExtent );
  connect( &mSaveRotationTimer, &QTimer::timeout, this, &ProjectInfo::saveRotation );
  connect( &mSaveTemporalStateTimer, &QTimer::timeout, this, &ProjectInfo::saveTemporalState );
}

void ProjectInfo::setFilePath( const QString &filePath )
{
  if ( mFilePath == filePath )
    return;

  mFilePath = filePath;

  emit filePathChanged();
  emit stateModeChanged();
  emit activeLayerChanged();
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
    disconnect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &ProjectInfo::rotationChanged );
    disconnect( mMapSettings, &QgsQuickMapSettings::temporalStateChanged, this, &ProjectInfo::temporalStateChanged );
  }

  mMapSettings = mapSettings;
  connect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, &ProjectInfo::extentChanged );
  connect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &ProjectInfo::rotationChanged );
  connect( mMapSettings, &QgsQuickMapSettings::temporalStateChanged, this, &ProjectInfo::temporalStateChanged );

  emit mapSettingsChanged();
}

QgsQuickMapSettings *ProjectInfo::mapSettings() const
{
  return mMapSettings;
}

void ProjectInfo::setLayerTree( FlatLayerTreeModel *layerTree )
{
  if ( mLayerTree == layerTree )
    return;

  if ( mLayerTree )
  {
    disconnect( mLayerTree, &FlatLayerTreeModel::mapThemeChanged, this, &ProjectInfo::mapThemeChanged );
  }

  mLayerTree = layerTree;

  if ( mLayerTree )
  {
    connect( mLayerTree, &FlatLayerTreeModel::mapThemeChanged, this, &ProjectInfo::mapThemeChanged );
  }

  emit layerTreeChanged();
}

FlatLayerTreeModel *ProjectInfo::layerTree() const
{
  return mLayerTree;
}

void ProjectInfo::setTrackingModel( TrackingModel *trackingModel )
{
  if ( mTrackingModel == trackingModel )
    return;

  mTrackingModel = trackingModel;

  emit trackingModelChanged();
}

TrackingModel *ProjectInfo::trackingModel() const
{
  return mTrackingModel;
}


void ProjectInfo::saveTracker( QgsVectorLayer *layer )
{
  if ( !layer || !mTrackingModel || !mTrackingModel->layerInTracking( layer ) )
    return;

  const Tracker *tracker = mTrackingModel->trackerForLayer( layer );

  mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/trackers/%1" ).arg( layer->id() ) );
  mSettings.setValue( "minimumDistance", tracker->minimumDistance() );
  mSettings.setValue( "timeInterval", tracker->timeInterval() );
  mSettings.setValue( "sensorCapture", tracker->sensorCapture() );
  mSettings.setValue( "conjunction", tracker->conjunction() );
  mSettings.setValue( "maximumDistance", tracker->maximumDistance() );
  mSettings.setValue( "measureType", static_cast<int>( tracker->measureType() ) );
  mSettings.setValue( "visible", tracker->visible() );
  mSettings.setValue( "featureId", tracker->feature().id() );
  mSettings.endGroup();
}

QModelIndex ProjectInfo::restoreTracker( QgsVectorLayer *layer )
{
  if ( !layer || !mTrackingModel || mTrackingModel->layerInTracking( layer ) )
    return QModelIndex();

  if ( !mSettings.contains( QStringLiteral( "/qgis/projectInfo/trackers/%1/minimumDistance" ).arg( layer->id() ) ) )
    return QModelIndex();

  QModelIndex index = mTrackingModel->createTracker( layer );
  Tracker *tracker = mTrackingModel->data( index, TrackingModel::TrackerPointer ).value<Tracker *>();

  mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/trackers/%1" ).arg( layer->id() ) );
  tracker->setTimeInterval( mSettings.value( "timeInterval", 0 ).toInt() );
  tracker->setMinimumDistance( mSettings.value( "minimumDistance", 0 ).toDouble() );
  tracker->setMaximumDistance( mSettings.value( "maximumDistance", 0 ).toDouble() );
  tracker->setSensorCapture( mSettings.value( "sensorCapture", false ).toBool() );
  tracker->setConjunction( mSettings.value( "conjunction", false ).toBool() );
  tracker->setMeasureType( static_cast<Tracker::MeasureType>( mSettings.value( "measureType", 0 ).toInt() ) );
  tracker->setVisible( mSettings.value( "visible", true ).toBool() );
  const QgsFeatureId fid = mSettings.value( "featureId", FID_NULL ).toLongLong();
  if ( fid >= 0 )
  {
    QgsFeature feature = layer->getFeature( fid );
    tracker->setFeature( feature );
  }
  else
  {
    tracker->setFeature( QgsFeature( layer->fields() ) );
  }
  mSettings.endGroup();

  return index;
}

void ProjectInfo::extentChanged()
{
  mSaveExtentTimer.start( 1000 );
}

void ProjectInfo::saveExtent()
{
  if ( mFilePath.isEmpty() )
    return;

  const QgsRectangle extent = mMapSettings->extent();
  mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1" ).arg( mFilePath ) );
  mSettings.setValue( QStringLiteral( "extent" ), QStringLiteral( "%1|%2|%3|%4" ).arg( qgsDoubleToString( extent.xMinimum() ), qgsDoubleToString( extent.xMaximum() ), qgsDoubleToString( extent.yMinimum() ), qgsDoubleToString( extent.yMaximum() ) ) );
  mSettings.endGroup();
}

void ProjectInfo::rotationChanged()
{
  mSaveRotationTimer.start( 1000 );
}

void ProjectInfo::saveRotation()
{
  if ( mFilePath.isEmpty() )
    return;

  mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1" ).arg( mFilePath ) );
  mSettings.setValue( QStringLiteral( "rotation" ), mMapSettings->rotation() );
  mSettings.endGroup();
}

void ProjectInfo::temporalStateChanged()
{
  mSaveTemporalStateTimer.start( 1000 );
}

void ProjectInfo::saveTemporalState()
{
  if ( mFilePath.isEmpty() )
    return;

  mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1" ).arg( mFilePath ) );
  mSettings.setValue( QStringLiteral( "isTemporal" ), mMapSettings->isTemporal() );
  mSettings.setValue( QStringLiteral( "StartDateTime" ), mMapSettings->temporalBegin().toTimeZone( QTimeZone( QTimeZone::Initialization::LocalTime ) ).toString( Qt::ISODateWithMs ) );
  mSettings.setValue( QStringLiteral( "EndDateTime" ), mMapSettings->temporalEnd().toTimeZone( QTimeZone( QTimeZone::Initialization::LocalTime ) ).toString( Qt::ISODateWithMs ) );
  mSettings.endGroup();
}

void ProjectInfo::saveLayerStyle( QgsMapLayer *layer )
{
  if ( mFilePath.isEmpty() || !layer )
    return;

  const bool isDataset = QgsProject::instance()->readBoolEntry( QStringLiteral( "QField" ), QStringLiteral( "isDataset" ), false );

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

  mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1/layerStyles/%2" ).arg( mFilePath, id ) );
  mSettings.setValue( QStringLiteral( "opacity" ), layer->opacity() );
  if ( QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer ) )
  {
    mSettings.setValue( QStringLiteral( "labelsEnabled" ), vlayer->labelsEnabled() );
  }
  mSettings.endGroup();
}

void ProjectInfo::saveLayerTreeState()
{
  if ( mFilePath.isEmpty() || !mLayerTree )
    return;

  const bool isDataset = QgsProject::instance()->readBoolEntry( QStringLiteral( "QField" ), QStringLiteral( "isDataset" ), false );
  if ( !isDataset )
  {
    QgsMapThemeCollection mapCollection( QgsProject::instance() );
    const QgsMapThemeCollection::MapThemeRecord rec = QgsMapThemeCollection::createThemeFromCurrentState( mLayerTree->layerTreeModel()->rootGroup(), mLayerTree->layerTreeModel() );
    mapCollection.insert( QStringLiteral( "::QFieldLayerTreeState" ), rec );

    const QDomDocumentType documentType = QDomImplementation().createDocumentType( QStringLiteral( "qgis" ), QStringLiteral( "http://mrcc.com/qgis.dtd" ), QStringLiteral( "SYSTEM" ) );
    QDomDocument document( documentType );

    document.appendChild( document.createElement( QStringLiteral( "qgis" ) ) );
    mapCollection.writeXml( document );

    mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1" ).arg( mFilePath ) );
    mSettings.setValue( QStringLiteral( "layertreestate" ), document.toString() );
    mSettings.remove( QStringLiteral( "maptheme" ) );
    mSettings.endGroup();
  }
}

bool ProjectInfo::snappingEnabled() const
{
  if ( mFilePath.isEmpty() )
    return false;

  return mSettings.value( QStringLiteral( "/qgis/projectInfo/%1/layerSnapping/enabled" ).arg( mFilePath ), false ).toBool();
}

void ProjectInfo::setSnappingEnabled( bool enabled )
{
  if ( mFilePath.isEmpty() )
    return;

  mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1/layerSnapping" ).arg( mFilePath ) );
  mSettings.setValue( QStringLiteral( "enabled" ), enabled );
  mSettings.endGroup();

  emit snappingEnabledChanged();
}

CloudUserInformation ProjectInfo::cloudUserInformation() const
{
  if ( mFilePath.isEmpty() )
    return CloudUserInformation( QString(), QString() );

  CloudUserInformation userinfo(
    mSettings.value( QStringLiteral( "/qgis/projectInfo/%1/cloudUserInfo/json" ).arg( mFilePath ), QStringLiteral( "{}" ) )
      .toJsonValue()
      .toObject() );

  return userinfo;
}

void ProjectInfo::setCloudUserInformation( const CloudUserInformation cloudUserInformation )
{
  if ( mFilePath.isEmpty() )
    return;

  if ( cloudUserInformation.isEmpty() )
    return;

  mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1/cloudUserInfo" ).arg( mFilePath ) );
  mSettings.setValue( QStringLiteral( "json" ), cloudUserInformation.toJson() );
  mSettings.endGroup();

  emit cloudUserInformationChanged();
}

void ProjectInfo::restoreCloudUserInformation()
{
  emit cloudUserInformationChanged();
}

void ProjectInfo::saveLayerSnappingConfiguration( QgsMapLayer *layer )
{
  if ( mFilePath.isEmpty() )
    return;

  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer )
    return;

  QgsSnappingConfig config = QgsProject::instance()->snappingConfig();
  QgsSnappingConfig::IndividualLayerSettings layerConfig = config.individualLayerSettings( vlayer );

  const bool isDataset = QgsProject::instance()->readBoolEntry( QStringLiteral( "QField" ), QStringLiteral( "isDataset" ), false );

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

  mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1/layerSnapping/%2" ).arg( mFilePath, id ) );
  mSettings.setValue( QStringLiteral( "enabled" ), layerConfig.enabled() );
  mSettings.endGroup();
}

void ProjectInfo::saveLayerRememberedFields( QgsMapLayer *layer )
{
  if ( mFilePath.isEmpty() )
    return;

  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer )
    return;

  QVariantMap rememberedFields;
  QgsEditFormConfig config = vlayer->editFormConfig();
  const QgsFields fields = vlayer->fields();
  for ( int i = 0; i < fields.size(); i++ )
  {
#if _QGIS_VERSION_INT >= 39900
    rememberedFields.insert( fields.at( i ).name(), config.reuseLastValuePolicy( i ) == Qgis::AttributeFormReuseLastValuePolicy::AllowedDefaultOn );
#else
    rememberedFields.insert( fields.at( i ).name(), config.reuseLastValue( i ) );
#endif
  }

  const bool isDataset = QgsProject::instance()->readBoolEntry( QStringLiteral( "QField" ), QStringLiteral( "isDataset" ), false );

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

  mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1/layerFields/%2" ).arg( mFilePath, id ) );
  mSettings.setValue( QStringLiteral( "remembered" ), rememberedFields );
  mSettings.endGroup();
}

void ProjectInfo::setStateMode( const QString &mode )
{
  if ( mFilePath.isEmpty() )
    return;

  mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1" ).arg( mFilePath ) );
  mSettings.setValue( QStringLiteral( "stateMode" ), mode );
  mSettings.endGroup();
}

QString ProjectInfo::stateMode() const
{
  if ( mSettings.contains( QStringLiteral( "/qgis/projectInfo/%1/stateMode" ).arg( mFilePath ) ) )
  {
    return mSettings.value( QStringLiteral( "/qgis/projectInfo/%1/stateMode" ).arg( mFilePath ), QStringLiteral( "browse" ) ).toString();
  }

  return QgsProject::instance()->readEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "initialMapMode" ), QStringLiteral( "browse" ) );
}

void ProjectInfo::setActiveLayer( QgsMapLayer *layer )
{
  if ( mFilePath.isEmpty() || !layer )
    return;

  mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1" ).arg( mFilePath ) );
  mSettings.setValue( QStringLiteral( "activeLayer" ), layer->id() );
  mSettings.endGroup();

  emit activeLayerChanged();
}

QgsMapLayer *ProjectInfo::activeLayer() const
{
  QString layerId;
  if ( mSettings.contains( QStringLiteral( "/qgis/projectInfo/%1/activeLayer" ).arg( mFilePath ) ) )
  {
    layerId = mSettings.value( QStringLiteral( "/qgis/projectInfo/%1/activeLayer" ).arg( mFilePath ) ).toString();
  }
  else
  {
    layerId = QgsProject::instance()->readEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "initialActiveLayer" ) );
  }
  return !layerId.isEmpty() ? QgsProject::instance()->mapLayer( layerId ) : nullptr;
}

void ProjectInfo::mapThemeChanged()
{
  if ( mFilePath.isEmpty() )
    return;

  mSettings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1" ).arg( mFilePath ) );
  if ( !mLayerTree->mapTheme().isEmpty() )
  {
    mSettings.setValue( QStringLiteral( "maptheme" ), mLayerTree->mapTheme() );
    mSettings.remove( QStringLiteral( "layertreestate" ) );
  }
  else
  {
    mSettings.remove( QStringLiteral( "maptheme" ) );
  }
  mSettings.endGroup();
}

void ProjectInfo::saveVariable( const QString &name, const QString &value )
{
  if ( mFilePath.isEmpty() )
    return;

  mSettings.setValue( QStringLiteral( "/qgis/projectInfo/%1/variables/%2" ).arg( mFilePath, name ), value );
}

void ProjectInfo::restoreSettings( QString &projectFilePath, QgsProject *project, QgsQuickMapCanvasMap *mapCanvas, FlatLayerTreeModel *layerTree )
{
  QSettings settings;

  const double rotation = settings.value( QStringLiteral( "qgis/projectInfo/%1/rotation" ).arg( projectFilePath ), mapCanvas->mapSettings()->rotation() ).toDouble();
  if ( !std::isnan( rotation ) )
  {
    mapCanvas->mapSettings()->setRotation( rotation );
  }

  const bool isTemporal = settings.value( QStringLiteral( "/qgis/projectInfo/%1/isTemporal" ).arg( projectFilePath ), false ).toBool();
  const QString begin = settings.value( QStringLiteral( "/qgis/projectInfo/%1/StartDateTime" ).arg( projectFilePath ), QString() ).toString();
  const QString end = settings.value( QStringLiteral( "/qgis/projectInfo/%1/EndDateTime" ).arg( projectFilePath ), QString() ).toString();
  if ( !begin.isEmpty() && !end.isEmpty() )
  {
    mapCanvas->mapSettings()->setTemporalBegin( QDateTime::fromString( begin, Qt::ISODateWithMs ) );
    mapCanvas->mapSettings()->setTemporalEnd( QDateTime::fromString( end, Qt::ISODateWithMs ) );
    mapCanvas->mapSettings()->setIsTemporal( isTemporal );
  }

  settings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1/layerStyles" ).arg( projectFilePath ) );
  QStringList ids = settings.childGroups();
  if ( !ids.isEmpty() )
  {
    const bool isDataset = project->readBoolEntry( QStringLiteral( "QField" ), QStringLiteral( "isDataset" ), false );
    const QList<QgsMapLayer *> mapLayers = isDataset ? project->layerStore()->mapLayers().values() : QList<QgsMapLayer *>();

    for ( QString id : std::as_const( ids ) )
    {
      const double opacity = settings.value( QStringLiteral( "%1/opacity" ).arg( id ), 1.0 ).toDouble();
      const bool labelsEnabled = settings.value( QStringLiteral( "%1/labelsEnabled" ).arg( id ), false ).toBool();

      // Remove the :: prefix to get actual layer id or source
      id = id.mid( 2 );

      QgsMapLayer *layer = nullptr;
      if ( isDataset )
      {
        auto match = std::find_if( mapLayers.begin(), mapLayers.end(), [&id]( QgsMapLayer *ml ) { return ml && ml->source() == id; } );
        if ( match != mapLayers.end() )
        {
          layer = *match;
        }
      }
      else
      {
        layer = project->layerStore()->mapLayer( id );
      }

      if ( layer )
      {
        layer->setOpacity( opacity );
        if ( QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer ) )
        {
          if ( vlayer->labeling() )
          {
            vlayer->setLabelsEnabled( labelsEnabled );
          }
        }
      }
    }
  }
  settings.endGroup();


  settings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1/layerFields" ).arg( projectFilePath ) );
  ids = settings.childGroups();
  if ( !ids.isEmpty() )
  {
    const bool isDataset = project->readBoolEntry( QStringLiteral( "QField" ), QStringLiteral( "isDataset" ), false );
    const QList<QgsMapLayer *> mapLayers = isDataset ? project->layerStore()->mapLayers().values() : QList<QgsMapLayer *>();

    for ( QString id : std::as_const( ids ) )
    {
      const QVariantMap rememberedFields = settings.value( QStringLiteral( "%1/remembered" ).arg( id ), QStringList() ).toMap();

      // Remove the :: prefix to get actual layer id or source
      id = id.mid( 2 );

      QgsMapLayer *layer = nullptr;
      if ( isDataset )
      {
        auto match = std::find_if( mapLayers.begin(), mapLayers.end(), [&id]( QgsMapLayer *ml ) { return ml && ml->source() == id; } );
        if ( match != mapLayers.end() )
        {
          layer = *match;
        }
      }
      else
      {
        layer = project->layerStore()->mapLayer( id );
      }

      if ( QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer ) )
      {
        QgsEditFormConfig config = vlayer->editFormConfig();
        const QStringList fieldNames = rememberedFields.keys();
        for ( const QString &fieldName : fieldNames )
        {
#if _QGIS_VERSION_INT >= 39900
          config.setReuseLastValuePolicy( vlayer->fields().indexFromName( fieldName ), Qgis::AttributeFormReuseLastValuePolicy::AllowedDefaultOn );
#else
          config.setReuseLastValue( vlayer->fields().indexFromName( fieldName ), rememberedFields[fieldName].toBool() );
#endif
        }
        vlayer->setEditFormConfig( config );
      }
    }
  }
  settings.endGroup();

  const QString mapTheme = settings.value( QStringLiteral( "/qgis/projectInfo/%1/maptheme" ).arg( projectFilePath ), QString() ).toString();
  const QString layerTreeState = settings.value( QStringLiteral( "/qgis/projectInfo/%1/layertreestate" ).arg( projectFilePath ), QString() ).toString();
  if ( !mapTheme.isEmpty() )
  {
    layerTree->setMapTheme( mapTheme );
  }
  else if ( !layerTreeState.isEmpty() )
  {
    QDomDocument document;
    document.setContent( layerTreeState );

    QgsMapThemeCollection mapCollection( project );
    mapCollection.readXml( document );
    mapCollection.applyTheme( QStringLiteral( "::QFieldLayerTreeState" ), layerTree->layerTreeModel()->rootGroup(), layerTree->layerTreeModel() );
  }

  settings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1/layerSnapping" ).arg( projectFilePath ) );
  const QStringList values = settings.allKeys();
  if ( !values.isEmpty() )
  {
    QgsSnappingConfig config = project->snappingConfig();
    if ( values.contains( QStringLiteral( "enabled" ) ) )
    {
      config.setEnabled( settings.value( QStringLiteral( "enabled" ), true ).toBool() );
    }

    ids = settings.childGroups();
    if ( !ids.isEmpty() )
    {
      const bool isDataset = project->readBoolEntry( QStringLiteral( "QField" ), QStringLiteral( "isDataset" ), false );
      const QList<QgsMapLayer *> mapLayers = isDataset ? project->layerStore()->mapLayers().values() : QList<QgsMapLayer *>();

      for ( QString id : std::as_const( ids ) )
      {
        const double enabled = settings.value( QStringLiteral( "%1/enabled" ).arg( id ), false ).toBool();

        // Remove the :: prefix to get actual layer id or source
        id = id.mid( 2 );

        QgsMapLayer *layer = nullptr;
        if ( isDataset )
        {
          auto match = std::find_if( mapLayers.begin(), mapLayers.end(), [&id]( QgsMapLayer *ml ) { return ml && ml->source() == id; } );
          if ( match != mapLayers.end() )
          {
            layer = *match;
          }
        }
        else
        {
          layer = project->layerStore()->mapLayer( id );
        }

        if ( layer )
        {
          if ( QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer ) )
          {
            QgsSnappingConfig::IndividualLayerSettings layerConfig = config.individualLayerSettings( vlayer );
            layerConfig.setEnabled( enabled );
            config.setIndividualLayerSettings( vlayer, layerConfig );
          }
        }
      }
    }
    project->setSnappingConfig( config );
  }
  settings.endGroup();

  settings.beginGroup( QStringLiteral( "/qgis/projectInfo/%1/variables" ).arg( projectFilePath ) );
  const QStringList variableNames = settings.allKeys();
  for ( const QString &name : variableNames )
  {
    ExpressionContextUtils::setProjectVariable( project, name, settings.value( name ).toString() );
  }
}

QVariantMap ProjectInfo::getTitleDecorationConfiguration()
{
  QVariantMap configuration;
  const QString configurationName = QStringLiteral( "TitleLabel" );

  if ( QgsProject::instance()->readBoolEntry( configurationName, QStringLiteral( "/Enabled" ), false ) )
  {
    QString text = QgsProject::instance()->readEntry( configurationName, QStringLiteral( "/Label" ), QString() );
    if ( !text.isEmpty() )
    {
      text.replace( QStringLiteral( "\n" ), QStringLiteral( "<br>" ) );
      QTextDocument doc;
      doc.setHtml( text );
      text = doc.toPlainText();
    }

    QColor backgroundColor = QgsColorUtils::colorFromString( QgsProject::instance()->readEntry( configurationName, QStringLiteral( "/BackgroundColor" ), QStringLiteral( "0,0,0,99" ) ) );
    QColor color = QColor( Qt::black );
    QColor outlineColor = QColor( Qt::white );
    bool hasOutline = false;

    QDomDocument doc;
    QDomElement elem;
    const QString textXml = QgsProject::instance()->readEntry( configurationName, QStringLiteral( "/Font" ) );
    if ( !textXml.isEmpty() )
    {
      doc.setContent( textXml );
      elem = doc.documentElement();
      QgsReadWriteContext rwContext;
      rwContext.setPathResolver( QgsProject::instance()->pathResolver() );
      QgsTextFormat textFormat;
      textFormat.readXml( elem, rwContext );

      color = textFormat.color();
      color.setAlphaF( textFormat.opacity() );
      if ( textFormat.buffer().enabled() )
      {
        hasOutline = true;
        outlineColor = textFormat.buffer().color();
        outlineColor.setAlphaF( textFormat.buffer().opacity() );
      }
    }

    configuration["text"] = text;
    configuration["backgroundColor"] = backgroundColor;
    configuration["color"] = color;
    configuration["hasOutline"] = hasOutline;
    configuration["outlineColor"] = outlineColor;
  }
  else
  {
    configuration["text"] = QString();
    configuration["backgroundColor"] = QColor( Qt::transparent );
    configuration["color"] = QColor( Qt::black );
    configuration["hasOutline"] = false;
    configuration["outlineColor"] = QColor( Qt::white );
  }

  return configuration;
}

QVariantMap ProjectInfo::getCopyrightDecorationConfiguration()
{
  QVariantMap configuration;
  const QString configurationName = QStringLiteral( "CopyrightLabel" );

  if ( QgsProject::instance()->readBoolEntry( configurationName, QStringLiteral( "/Enabled" ), false ) )
  {
    QString text = QgsProject::instance()->readEntry( configurationName, QStringLiteral( "/Label" ), QString() );
    if ( !text.isEmpty() )
    {
      text.replace( QStringLiteral( "\n" ), QStringLiteral( "<br>" ) );
      QTextDocument doc;
      doc.setHtml( text );
      text = doc.toPlainText();
    }

    QColor backgroundColor = QColor( Qt::transparent );
    QColor color = QColor( Qt::black );
    QColor outlineColor = QColor( Qt::white );
    bool hasOutline = false;

    QDomDocument doc;
    QDomElement elem;
    const QString textXml = QgsProject::instance()->readEntry( configurationName, QStringLiteral( "/Font" ) );
    if ( !textXml.isEmpty() )
    {
      doc.setContent( textXml );
      elem = doc.documentElement();
      QgsReadWriteContext rwContext;
      rwContext.setPathResolver( QgsProject::instance()->pathResolver() );
      QgsTextFormat textFormat;
      textFormat.readXml( elem, rwContext );

      color = textFormat.color();
      color.setAlphaF( textFormat.opacity() );
      if ( textFormat.buffer().enabled() )
      {
        hasOutline = true;
        outlineColor = textFormat.buffer().color();
        outlineColor.setAlphaF( textFormat.buffer().opacity() );
      }
    }

    configuration["text"] = text;
    configuration["backgroundColor"] = backgroundColor;
    configuration["color"] = color;
    configuration["hasOutline"] = hasOutline;
    configuration["outlineColor"] = outlineColor;
  }
  else
  {
    configuration["text"] = QString();
    configuration["backgroundColor"] = QColor( Qt::transparent );
    configuration["color"] = QColor( Qt::black );
    configuration["hasOutline"] = false;
    configuration["outlineColor"] = QColor( Qt::white );
  }

  return configuration;
}

QVariantMap ProjectInfo::getImageDecorationConfiguration()
{
  QVariantMap configuration;
  const QString configurationName = QStringLiteral( "Image" );

  if ( QgsProject::instance()->readBoolEntry( configurationName, QStringLiteral( "/Enabled" ), false ) )
  {
    QString imagePath = QgsProject::instance()->readEntry( configurationName, QStringLiteral( "/ImagePath" ), QString() );
    const QString resolvedPath = QgsSymbolLayerUtils::svgSymbolNameToPath( imagePath, QgsProject::instance()->pathResolver() );
    const QFileInfo fileInfo( resolvedPath );
    if ( fileInfo.exists() )
    {
      imagePath = resolvedPath;
    }
    else
    {
      imagePath = QStringLiteral( ":/images/app_logo.svg" );
    }

    QColor fillColor = QgsColorUtils::colorFromString( QgsProject::instance()->readEntry( configurationName, QStringLiteral( "/Color" ), QStringLiteral( "#000000" ) ) );
    QColor strokeColor = QgsColorUtils::colorFromString( QgsProject::instance()->readEntry( configurationName, QStringLiteral( "/OutlineColor" ), QStringLiteral( "#FFFFFF" ) ) );

    configuration["source"] = imagePath;
    configuration["fillColor"] = fillColor;
    configuration["strokeColor"] = strokeColor;
  }
  else
  {
    configuration["source"] = QString();
    configuration["fillColor"] = QColor( Qt::black );
    configuration["strokeColor"] = QColor( Qt::white );
  }

  return configuration;
}

QVariantMap ProjectInfo::getGridDecorationConfiguration()
{
  QVariantMap configuration;
  const QString configurationName = QStringLiteral( "Grid" );

  if ( QgsProject::instance()->readBoolEntry( configurationName, QStringLiteral( "/Enabled" ), false ) )
  {
    bool hasLines = false;
    QColor lineColor = QColor( Qt::black );
    bool hasMarkers = false;
    QColor markerColor = QColor( Qt::black );
    bool hasAnnotations = false;
    QColor annotationColor = QColor( Qt::black );
    QColor annotationOutlineColor = QColor( Qt::white );
    bool hasAnnotationOutline = false;

    double xInterval = QgsProject::instance()->readDoubleEntry( configurationName, QStringLiteral( "/IntervalX" ), 50.0 );
    double yInterval = QgsProject::instance()->readDoubleEntry( configurationName, QStringLiteral( "/IntervalY" ), 50.0 );
    double xOffset = QgsProject::instance()->readDoubleEntry( configurationName, QStringLiteral( "/OffsetX" ), 0.0 );
    double yOffset = QgsProject::instance()->readDoubleEntry( configurationName, QStringLiteral( "/OffsetY" ), 0.0 );

    QgsReadWriteContext rwContext;
    rwContext.setPathResolver( QgsProject::instance()->pathResolver() );
    QDomDocument doc;
    QDomElement elem;
    QString xml;

    hasAnnotations = QgsProject::instance()->readBoolEntry( configurationName, QStringLiteral( "/ShowAnnotation" ), false );
    const int annotationPrecision = QgsProject::instance()->readNumEntry( configurationName, QStringLiteral( "/AnnotationPrecision" ), 0 );
    if ( hasAnnotations )
    {
      xml = QgsProject::instance()->readEntry( configurationName, QStringLiteral( "/Font" ) );
      if ( !xml.isEmpty() )
      {
        doc.setContent( xml );
        elem = doc.documentElement();
        QgsTextFormat textFormat;
        textFormat.readXml( elem, rwContext );

        annotationColor = textFormat.color();
        annotationColor.setAlphaF( textFormat.opacity() );
        if ( textFormat.buffer().enabled() )
        {
          hasAnnotationOutline = true;
          annotationOutlineColor = textFormat.buffer().color();
          annotationOutlineColor.setAlphaF( textFormat.buffer().opacity() );
        }
      }
      else
      {
        hasAnnotations = false;
      }
    }

    // QgsDecorationGrid::GridStyle values: 0 = line, 1 = marker
    const int gridStyle = QgsProject::instance()->readNumEntry( configurationName, QStringLiteral( "/Style" ), 0 );

    if ( gridStyle == 0 )
    {
      xml = QgsProject::instance()->readEntry( configurationName, QStringLiteral( "/LineSymbol" ) );
      if ( !xml.isEmpty() )
      {
        doc.setContent( xml );
        elem = doc.documentElement();
        std::unique_ptr<QgsLineSymbol> lineSymbol( QgsSymbolLayerUtils::loadSymbol<QgsLineSymbol>( elem, rwContext ) );
        if ( lineSymbol )
        {
          hasLines = true;
          lineColor = lineSymbol->color();
        }
      }
    }
    else
    {
      xml = QgsProject::instance()->readEntry( configurationName, QStringLiteral( "/MarkerSymbol" ) );
      if ( !xml.isEmpty() )
      {
        doc.setContent( xml );
        elem = doc.documentElement();
        std::unique_ptr<QgsMarkerSymbol> markerSymbol( QgsSymbolLayerUtils::loadSymbol<QgsMarkerSymbol>( elem, rwContext ) );
        if ( markerSymbol )
        {
          hasMarkers = true;
          markerColor = markerSymbol->color();
        }
      }
    }

    configuration["xInterval"] = xInterval;
    configuration["yInterval"] = yInterval;
    configuration["xOffset"] = xOffset;
    configuration["yOffset"] = yOffset;
    configuration["hasLines"] = hasLines;
    configuration["lineColor"] = lineColor;
    configuration["hasMarkers"] = hasMarkers;
    configuration["markerColor"] = markerColor;
    configuration["hasAnnotations"] = hasAnnotations;
    configuration["annotationPrecision"] = annotationPrecision;
    configuration["annotationColor"] = annotationColor;
    configuration["annotationHasOutline"] = hasAnnotationOutline;
    configuration["annotationOutlineColor"] = annotationOutlineColor;
  }
  else
  {
    configuration["xInterval"] = 0.0;
    configuration["yInterval"] = 0.0;
    configuration["xOffset"] = 0.0;
    configuration["yOffset"] = 0.0;
    configuration["hasLines"] = false;
    configuration["lineColor"] = QColor();
    configuration["hasMarkers"] = false;
    configuration["markerColor"] = QColor();
    configuration["hasAnnotations"] = false;
    configuration["annotationPrecision"] = 0;
    configuration["annotationColor"] = QColor();
    configuration["annotationHasOutline"] = false;
    configuration["annotationOutlineColor"] = QColor();
  }

  return configuration;
}

QgsMapLayer *ProjectInfo::getDefaultActiveLayerForMapTheme( const QString &mapTheme )
{
  if ( mapTheme.isEmpty() )
  {
    return nullptr;
  }

  const QString json = QgsProject::instance()->readEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "/mapThemesActiveLayers" ) );
  const QJsonDocument document = QJsonDocument::fromJson( json.toUtf8() );

  QJsonObject entries = document.object();
  if ( entries.contains( mapTheme ) )
  {
    const QString mapLayerId = entries.value( mapTheme ).toString();
    return QgsProject::instance()->mapLayer( mapLayerId );
  }

  return nullptr;
}
```


