/***************************************************************************
  identifytool.cpp - IdentifyTool

 ---------------------
 begin                : 30.8.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "identifytool.h"
#include "multifeaturelistmodel.h"
#include "qgsquickmapsettings.h"

#include <qgsexpressioncontextutils.h>
#include <qgsproject.h>
#include <qgsrenderer.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayertemporalproperties.h>

IdentifyTool::IdentifyTool( QObject *parent )
  : QObject( parent )
  , mMapSettings( nullptr )
  , mSearchRadiusMm( 5 )
{
}

QgsQuickMapSettings *IdentifyTool::mapSettings() const
{
  return mMapSettings;
}

void IdentifyTool::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mapSettings == mMapSettings )
    return;

  mMapSettings = mapSettings;
  emit mapSettingsChanged();
}

void IdentifyTool::identify( const QPointF &point ) const
{
  if ( mDeactivated )
    return;

  if ( !mModel || !mMapSettings )
  {
    qWarning() << "Unable to use IdentifyTool without mapSettings or model property set.";
    return;
  }

  mModel->clear( true );

  QgsPointXY mapPoint = mMapSettings->screenToCoordinate( point );

  const QList<QgsMapLayer *> layers = mModel->selectedLayer() ? QList<QgsMapLayer *>() << mModel->selectedLayer() : mMapSettings->mapSettings().layers();
  for ( QgsMapLayer *layer : layers )
  {
    if ( !layer->flags().testFlag( QgsMapLayer::Identifiable ) )
      continue;

    QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( layer );
    if ( vl )
    {
      QList<IdentifyResult> results = identifyVectorLayer( vl, mapPoint );
      mModel->appendFeatures( results );
    }
  }

  emit identifyFinished();
}

QList<IdentifyTool::IdentifyResult> IdentifyTool::identifyVectorLayer( QgsVectorLayer *layer, const QgsPointXY &point ) const
{
  QList<IdentifyResult> results;

  if ( !layer || !layer->isSpatial() )
    return results;

  if ( !layer->isInScaleRange( mMapSettings->mapSettings().scale() ) )
    return results;

  QString temporalFilter;
  if ( mMapSettings->isTemporal() )
  {
    if ( !layer->temporalProperties()->isVisibleInTemporalRange( mMapSettings->mapSettings().temporalRange() ) )
      return results;

    QgsVectorLayerTemporalContext temporalContext;
    temporalContext.setLayer( layer );
    temporalFilter = qobject_cast<const QgsVectorLayerTemporalProperties *>( layer->temporalProperties() )->createFilterString( temporalContext, mMapSettings->mapSettings().temporalRange() );
  }

  QgsFeatureList featureList;

  // toLayerCoordinates will throw an exception for an 'invalid' point.
  // For example, if you project a world map onto a globe using EPSG 2163
  // and then click somewhere off the globe, an exception will be thrown.
  try
  {
    // create the search rectangle
    double searchRadius = searchRadiusMU();

    QgsRectangle r;
    r.setXMinimum( point.x() - searchRadius );
    r.setXMaximum( point.x() + searchRadius );
    r.setYMinimum( point.y() - searchRadius );
    r.setYMaximum( point.y() + searchRadius );

    r = toLayerCoordinates( layer, r );

    QgsFeatureRequest req;
    req.setFilterRect( r );
    if ( !temporalFilter.isEmpty() )
      req.setFilterExpression( temporalFilter );
    req.setLimit( QSettings().value( "/QField/identify/limit", 200 ).toInt() );
#if _QGIS_VERSION_INT >= 33500
    req.setFlags( Qgis::FeatureRequestFlag::ExactIntersect );
#else
    req.setFlags( QgsFeatureRequest::ExactIntersect );
#endif

    QgsAttributeTableConfig config = layer->attributeTableConfig();
    if ( !config.sortExpression().isEmpty() )
    {
      req.addOrderBy( config.sortExpression(), config.sortOrder() == Qt::AscendingOrder );
    }
    else if ( !layer->displayExpression().isEmpty() )
    {
      req.addOrderBy( layer->displayExpression() );
    }

    QgsFeatureIterator fit = layer->getFeatures( req );
    QgsFeature f;
    while ( fit.nextFeature( f ) )
      featureList << QgsFeature( f );
  }
  catch ( QgsCsException &cse )
  {
    Q_UNUSED( cse );
    // catch exception for 'invalid' point and proceed with no features found
  }

  QgsRenderContext context( QgsRenderContext::fromMapSettings( mMapSettings->mapSettings() ) );
  context.setExpressionContext( QgsExpressionContext( QgsExpressionContextUtils::globalProjectLayerScopes( layer ) ) );
  context.expressionContext() << QgsExpressionContextUtils::mapSettingsScope( mMapSettings->mapSettings() );
  QgsFeatureRenderer *renderer = layer->renderer();

  if ( renderer )
  {
    renderer->startRender( context, layer->fields() );
  }

  for ( const QgsFeature &feature : std::as_const( featureList ) )
  {
    context.expressionContext().setFeature( feature );
    if ( renderer && !renderer->willRenderFeature( const_cast<QgsFeature &>( feature ), context ) )
      continue;

    results.append( IdentifyResult( layer, feature ) );
  }

  if ( renderer )
  {
    renderer->stopRender( context );
  }

  return results;
}

MultiFeatureListModel *IdentifyTool::model() const
{
  return mModel;
}

void IdentifyTool::setModel( MultiFeatureListModel *model )
{
  if ( model == mModel )
    return;

  mModel = model;
  emit modelChanged();
}

void IdentifyTool::setDeactivated( bool deactivated )
{
  if ( deactivated )
    mModel->clear();
  mDeactivated = deactivated;
}

double IdentifyTool::searchRadiusMU( const QgsRenderContext &context ) const
{
  return mSearchRadiusMm * context.scaleFactor() * context.mapToPixel().mapUnitsPerPixel();
}

double IdentifyTool::searchRadiusMU() const
{
  QgsRenderContext context = QgsRenderContext::fromMapSettings( mMapSettings->mapSettings() );
  return searchRadiusMU( context );
}

QgsRectangle IdentifyTool::toLayerCoordinates( QgsMapLayer *layer, const QgsRectangle &rect ) const
{
  return mMapSettings->mapSettings().mapToLayerCoordinates( layer, rect );
}

double IdentifyTool::searchRadiusMm() const
{
  return mSearchRadiusMm;
}

void IdentifyTool::setSearchRadiusMm( double searchRadiusMm )
{
  if ( mSearchRadiusMm == searchRadiusMm )
    return;

  mSearchRadiusMm = searchRadiusMm;
  emit searchRadiusMmChanged();
}
