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

#include <qgsfillsymbollayer.h>
#include <qgslayoutatlas.h>
#include <qgslayoutmanager.h>
#include <qgslinesymbollayer.h>
#include <qgsmarkersymbollayer.h>
#include <qgsprintlayout.h>
#include <qgsproject.h>
#include <qgssinglesymbolrenderer.h>
#include <qgssymbol.h>
#include <qgssymbollayer.h>
#include <qgsmarkersymbol.h>
#include <qgslinesymbol.h>
#include <qgsfillsymbol.h>
#include <qgsvectorlayer.h>
#include <qgswkbtypes.h>
#include <qgsmessagelog.h>

LayerUtils::LayerUtils( QObject *parent )
  : QObject( parent )
{
}

QgsSymbol *LayerUtils::defaultSymbol( QgsVectorLayer *layer )
{
  QgsSymbol *symbol = nullptr;
  QgsSymbolLayerList symbolLayers;
  switch ( layer->geometryType() )
  {
    case QgsWkbTypes::PointGeometry:
    {
      QgsSimpleMarkerSymbolLayer *symbolLayer = new QgsSimpleMarkerSymbolLayer( QgsSimpleMarkerSymbolLayerBase::Circle, 2.6, 0.0, DEFAULT_SCALE_METHOD, QColor( 255, 0, 0, 100 ), QColor( 255, 0, 0 ) );
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

QString LayerUtils::fieldType( const QgsField &field ) const
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
    if ( ! layer->editBuffer() || !layer->startEditing() )
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
