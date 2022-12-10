/***************************************************************************
  layerutils.cpp - LayerUtils

 ---------------------
 begin                : 01.03.2021
 copyright            : (C) 2020 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "layerutils.h"

#include <QScopeGuard>
#include <qgsfillsymbol.h>
#include <qgsfillsymbollayer.h>
#include <qgslabelobstaclesettings.h>
#include <qgslayoutatlas.h>
#include <qgslayoutmanager.h>
#include <qgslinesymbol.h>
#include <qgslinesymbollayer.h>
#include <qgsmarkersymbol.h>
#include <qgsmarkersymbollayer.h>
#include <qgsmessagelog.h>
#include <qgspallabeling.h>
#include <qgsprintlayout.h>
#include <qgsproject.h>
#include <qgsrasterlayer.h>
#include <qgssinglesymbolrenderer.h>
#include <qgssymbol.h>
#include <qgssymbollayer.h>
#include <qgstextbuffersettings.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayerlabeling.h>
#include <qgsvectorlayerutils.h>
#include <qgswkbtypes.h>
#if _QGIS_VERSION_INT >= 32700
#include <qgsmaplayerelevationproperties.h>
#include <qgsrasterlayerelevationproperties.h>
#endif

LayerUtils::LayerUtils( QObject *parent )
  : QObject( parent )
{
}

QgsSymbol *LayerUtils::defaultSymbol( QgsVectorLayer *layer )
{
  QgsSymbol *symbol = nullptr;

  if ( !layer )
    return symbol;

  QgsSymbolLayerList symbolLayers;
  switch ( layer->geometryType() )
  {
    case QgsWkbTypes::PointGeometry:
    {
#if _QGIS_VERSION_INT >= 32300
      QgsSimpleMarkerSymbolLayer *symbolLayer = new QgsSimpleMarkerSymbolLayer( Qgis::MarkerShape::Circle, 2.6, 0.0, DEFAULT_SCALE_METHOD, QColor( 255, 0, 0, 100 ), QColor( 255, 0, 0 ) );
#else
      QgsSimpleMarkerSymbolLayer *symbolLayer = new QgsSimpleMarkerSymbolLayer( QgsSimpleMarkerSymbolLayerBase::Circle, 2.6, 0.0, DEFAULT_SCALE_METHOD, QColor( 255, 0, 0, 100 ), QColor( 255, 0, 0 ) );
#endif
      symbolLayer->setStrokeWidth( 0.6 );
      symbolLayers << symbolLayer;
      symbol = new QgsMarkerSymbol( symbolLayers );
      break;
    }

    case QgsWkbTypes::LineGeometry:
    {
      QgsSimpleLineSymbolLayer *symbolLayer = new QgsSimpleLineSymbolLayer( QColor( 255, 0, 0 ), 0.6 );
      symbolLayers << symbolLayer;
      symbol = new QgsLineSymbol( symbolLayers );
      break;
    }

    case QgsWkbTypes::PolygonGeometry:
    {
      QgsSimpleFillSymbolLayer *symbolLayer = new QgsSimpleFillSymbolLayer( QColor( 255, 0, 0, 100 ), DEFAULT_SIMPLEFILL_STYLE, QColor( 255, 0, 0 ), DEFAULT_SIMPLEFILL_BORDERSTYLE, 0.6 );
      symbolLayers << symbolLayer;
      symbol = new QgsFillSymbol( symbolLayers );
      break;
    }

    case QgsWkbTypes::UnknownGeometry:
    case QgsWkbTypes::NullGeometry:
      break;
  }
  return symbol;
}

QgsAbstractVectorLayerLabeling *LayerUtils::defaultLabeling( QgsVectorLayer *layer, QgsTextFormat textFormat )
{
  QgsAbstractVectorLayerLabeling *labeling = nullptr;

  if ( !layer )
    return labeling;

#if _QGIS_VERSION_INT >= 32100
  bool foundFriendlyIdentifier = true;
  QString fieldName = QgsVectorLayerUtils::guessFriendlyIdentifierField( layer->fields(), &foundFriendlyIdentifier );
  if ( !foundFriendlyIdentifier )
    return labeling;
#else
  QString fieldName = QgsVectorLayerUtils::guessFriendlyIdentifierField( layer->fields() );
#endif

  QgsPalLayerSettings settings;
  settings.fieldName = fieldName;
  settings.obstacleSettings().setIsObstacle( true );
  switch ( layer->geometryType() )
  {
    case QgsWkbTypes::PointGeometry:
    {
#if _QGIS_VERSION_INT >= 32500
      settings.placement = Qgis::LabelPlacement::OrderedPositionsAroundPoint;
      settings.offsetType = Qgis::LabelOffsetType::FromSymbolBounds;
#else
      settings.placement = QgsPalLayerSettings::OrderedPositionsAroundPoint;
      settings.offsetType = QgsPalLayerSettings::FromSymbolBounds;
#endif
      break;
    }

    case QgsWkbTypes::LineGeometry:
    {
#if _QGIS_VERSION_INT >= 32500
      settings.placement = Qgis::LabelPlacement::Curved;
#else
      settings.placement = QgsPalLayerSettings::Curved;
#endif
      break;
    }

    case QgsWkbTypes::PolygonGeometry:
    {
#if _QGIS_VERSION_INT >= 32500
      settings.placement = Qgis::LabelPlacement::AroundPoint;
#else
      settings.placement = QgsPalLayerSettings::AroundPoint;
#endif
      settings.obstacleSettings().setType( QgsLabelObstacleSettings::PolygonBoundary );
      break;
    }

    case QgsWkbTypes::UnknownGeometry:
    case QgsWkbTypes::NullGeometry:
      break;
  }

  if ( !textFormat.isValid() )
  {
    textFormat.setSize( 9 );
    textFormat.setSizeUnit( QgsUnitTypes::RenderPoints );
    textFormat.setColor( QColor( 0, 0, 0 ) );

    QgsTextBufferSettings bufferSettings;
    bufferSettings.setEnabled( true );
    bufferSettings.setColor( QColor( 255, 255, 255 ) );
    bufferSettings.setSize( 1 );
    bufferSettings.setSizeUnit( QgsUnitTypes::RenderMillimeters );
    textFormat.setBuffer( bufferSettings );
  }
  settings.setFormat( textFormat );

  labeling = new QgsVectorLayerSimpleLabeling( settings );

  return labeling;
}

QgsRasterLayer *LayerUtils::createOnlineElevationLayer()
{
  QgsRasterLayer *layer = new QgsRasterLayer( QStringLiteral( "interpretation=terrariumterrain&type=xyz&url=https://s3.amazonaws.com/elevation-tiles-prod/terrarium/%7Bz%7D/%7Bx%7D/%7By%7D.png&zmax=15&zmin=0" ),
                                              QStringLiteral( "elevation" ), QStringLiteral( "wms" ) );
#if _QGIS_VERSION_INT >= 32700
  QgsRasterLayerElevationProperties *elevationProperties = static_cast<QgsRasterLayerElevationProperties *>( layer->elevationProperties() );
  elevationProperties->setEnabled( true );
  elevationProperties->setProfileSymbology( Qgis::ProfileSurfaceSymbology::FillBelow );
  elevationProperties->profileFillSymbol()->setColor( QColor( 130, 130, 130 ) );
#endif
  return layer;
}

bool LayerUtils::isAtlasCoverageLayer( QgsVectorLayer *layer )
{
  if ( !layer || !QgsProject::instance()->layoutManager() )
    return false;

  const QList<QgsPrintLayout *> printLayouts = QgsProject::instance()->layoutManager()->printLayouts();
  for ( QgsPrintLayout *printLayout : printLayouts )
  {
    if ( printLayout->atlas() )
    {
      if ( printLayout->atlas()->coverageLayer() == layer )
        return true;
    }
  }

  return false;
}

#if _QGIS_VERSION_INT >= 32100
void LayerUtils::selectFeaturesInLayer( QgsVectorLayer *layer, const QList<int> &fids, Qgis::SelectBehavior behavior )
#else
void LayerUtils::selectFeaturesInLayer( QgsVectorLayer *layer, const QList<int> &fids, QgsVectorLayer::SelectBehavior behavior )
#endif
{
  if ( !layer )
    return;
  QgsFeatureIds qgsFids;
  for ( const int &fid : fids )
    qgsFids << fid;
  layer->selectByIds( qgsFids, behavior );
}

QString LayerUtils::fieldType( const QgsField &field )
{
  return QVariant( field.type() ).typeName();
}

bool LayerUtils::deleteFeature( QgsProject *project, QgsVectorLayer *layer, const QgsFeatureId fid, bool shouldWriteChanges )
{
  if ( !project )
    return false;

  if ( !layer )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing, no layer" ), "QField", Qgis::Warning );
    return false;
  }

  if ( !shouldWriteChanges )
  {
    if ( !layer->startEditing() || !layer->editBuffer() )
    {
      QgsMessageLog::logMessage( tr( "Cannot start editing" ), "QField", Qgis::Warning );
      return false;
    }
  }
  else
  {
    if ( !layer->editBuffer() )
    {
      return false;
    }
  }

  bool isSuccess = true;

  // delete parent and related features
  QgsVectorLayer::DeleteContext deleteContext( true, project );
  if ( layer->deleteFeature( fid, &deleteContext ) )
  {
    if ( !shouldWriteChanges )
    {
      // commit changes
      if ( !layer->commitChanges() )
      {
        const QString msgs = layer->commitErrors().join( QStringLiteral( "\n" ) );
        QgsMessageLog::logMessage( tr( "Cannot commit deletion of feature %2 in layer \"%1\". Reason:\n%3" ).arg( layer->name() ).arg( fid ).arg( msgs ), QStringLiteral( "QField" ), Qgis::Warning );
        isSuccess = false;
      }
    }

    if ( isSuccess )
    {
      // loop and commit referenced layers in reverse
      QList<QgsVectorLayer *> constHandledLayers = deleteContext.handledLayers();

      for ( QList<QgsVectorLayer *>::reverse_iterator it = constHandledLayers.rbegin(); it != constHandledLayers.rend(); ++it )
      {
        QgsVectorLayer *vl = *it;

        if ( vl == layer )
          continue;

        if ( !vl->commitChanges() )
        {
          const QString msgs = vl->commitErrors().join( QStringLiteral( "\n" ) );
          QgsMessageLog::logMessage( tr( "Cannot commit deletion in layer \"%1\". Reason:\n%3" ).arg( vl->name() ).arg( msgs ), QStringLiteral( "QField" ), Qgis::Warning );
          isSuccess = false;
          break;
        }
      }
    }
  }
  else
  {
    QgsMessageLog::logMessage( tr( "Cannot delete feature %1" ).arg( fid ), "QField", Qgis::Warning );

    isSuccess = false;
  }

  if ( !shouldWriteChanges )
  {
    if ( !isSuccess )
    {
      const QList<QgsVectorLayer *> constHandledLayers = deleteContext.handledLayers();
      for ( QgsVectorLayer *vl : constHandledLayers )
        if ( vl != layer )
          if ( !vl->rollBack() )
            QgsMessageLog::logMessage( tr( "Cannot rollback layer changes in layer %1" ).arg( vl->name() ), "QField", Qgis::Critical );

      if ( !layer->rollBack() )
        QgsMessageLog::logMessage( tr( "Cannot rollback layer changes in layer %1" ).arg( layer->name() ), "QField", Qgis::Critical );
    }
  }

  return isSuccess;
}

QgsFeature LayerUtils::duplicateFeature( QgsVectorLayer *layer, const QgsFeature &feature )
{
  if ( !layer )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing, no layer" ), "QField", Qgis::Warning );
    return QgsFeature();
  }

  if ( !feature.isValid() )
  {
    QgsMessageLog::logMessage( tr( "Cannot copy invalid feature" ), "QField", Qgis::Warning );
    return QgsFeature();
  }

  if ( !layer->startEditing() || !layer->editBuffer() )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing" ), "QField", Qgis::Warning );
    return QgsFeature();
  }

  QgsFeature duplicatedFeature;
  QMetaObject::Connection connection = connect( layer, &QgsVectorLayer::featureAdded, [layer, &duplicatedFeature]( QgsFeatureId fid ) {
    duplicatedFeature = layer->getFeature( fid );
  } );
  auto sweaper = qScopeGuard( [layer, connection] { layer->disconnect( connection ); } );

  duplicatedFeature = QgsVectorLayerUtils::createFeature( layer, feature.geometry(), feature.attributes().toMap() );
  if ( layer->addFeature( duplicatedFeature ) )
  {
    // commit changes
    if ( !layer->commitChanges() )
    {
      const QString msgs = layer->commitErrors().join( QStringLiteral( "\n" ) );
      QgsMessageLog::logMessage( tr( "Cannot add new feature in layer \"%1\". Reason:\n%2" ).arg( layer->name(), msgs ), "QField", Qgis::Warning );
      return QgsFeature();
    }
  }
  else
  {
    QgsMessageLog::logMessage( tr( "Cannot add new feature in layer \"%1\"." ).arg( layer->name() ), "QField", Qgis::Warning );
    return QgsFeature();
  }

  return duplicatedFeature;
}

bool LayerUtils::hasMValue( QgsVectorLayer *layer )
{
  if ( !layer )
    return false;

  return QgsWkbTypes::hasM( layer->wkbType() );
}
