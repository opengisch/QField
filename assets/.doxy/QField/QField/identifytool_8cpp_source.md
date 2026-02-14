

# File identifytool.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**identifytool.cpp**](identifytool_8cpp.md)

[Go to the documentation of this file](identifytool_8cpp.md)


```C++
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
#include <qgsfeaturestore.h>
#include <qgsproject.h>
#include <qgsrasteridentifyresult.h>
#include <qgsrasterlayer.h>
#include <qgsrenderer.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayertemporalproperties.h>
#include <qgsvectortilelayer.h>
#include <qgsvectortileloader.h>
#include <qgsvectortilemvtdecoder.h>
#include <qgsvectortileutils.h>

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

    if ( QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( layer ) )
    {
      QList<IdentifyResult> results = identifyVectorLayer( vl, mapPoint );
      mModel->appendFeatures( results );
    }
    else if ( QgsRasterLayer *rl = qobject_cast<QgsRasterLayer *>( layer ) )
    {
      QList<IdentifyResult> results = identifyRasterLayer( rl, mapPoint );
      mModel->appendFeatures( results );
    }
    else if ( QgsVectorTileLayer *vtl = qobject_cast<QgsVectorTileLayer *>( layer ) )
    {
      QList<IdentifyResult> results = identifyVectorTileLayer( vtl, mapPoint );
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
    {
      req.setFilterExpression( temporalFilter );
    }
    req.setLimit( QSettings().value( "/QField/identify/limit", 200 ).toInt() );
    req.setFlags( Qgis::FeatureRequestFlag::ExactIntersect );

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
  catch ( const QgsCsException &cse )
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

QList<IdentifyTool::IdentifyResult> IdentifyTool::identifyRasterLayer( QgsRasterLayer *layer, const QgsPointXY &point ) const
{
  QList<IdentifyTool::IdentifyResult> results;
  if ( !layer->dataProvider() || !layer->isValid() )
    return results;

  std::unique_ptr<QgsRasterDataProvider> dataProvider( layer->dataProvider()->clone() );
  const Qgis::RasterInterfaceCapabilities capabilities = dataProvider->capabilities();

  if ( !( capabilities & Qgis::RasterInterfaceCapability::Identify ) )
    return results;

  if ( !( capabilities & Qgis::RasterInterfaceCapability::IdentifyFeature ) )
    return results;

  const QgsPointXY pointInLayerCoordinates = toLayerCoordinates( layer, point );
  const double mapUnitsPerPixel = mMapSettings->mapSettings().mapUnitsPerPixel();
  QgsRasterIdentifyResult identifyResult;
  // We can only use current map canvas context (extent, width, height) if layer is not reprojected,
  if ( dataProvider->crs() != mMapSettings->mapSettings().destinationCrs() )
  {
    // To get some reasonable response for point/line WMS vector layers we must
    // use a context with approximately a resolution in layer CRS units
    // corresponding to current map canvas resolution (for examplei UMN Mapserver
    // in msWMSFeatureInfo() -> msQueryByRect() is using requested pixel
    // + TOLERANCE (layer param) for feature selection)
    QgsRectangle r;
    r.setXMinimum( point.x() - mapUnitsPerPixel / 2. );
    r.setXMaximum( point.x() + mapUnitsPerPixel / 2. );
    r.setYMaximum( point.y() + mapUnitsPerPixel / 2. );
    r.setYMinimum( point.y() - mapUnitsPerPixel / 2. );
    r = toLayerCoordinates( layer, r ); // will be a bit larger
    // Mapserver (6.0.3, for example) does not work with 1x1 pixel box
    // but that is fixed (the rect is enlarged) in the WMS provider

    identifyResult = dataProvider->identify( pointInLayerCoordinates, Qgis::RasterIdentifyFormat::Feature, r, 1, 1 );
  }
  else
  {
    // It would be nice to use the same extent and size which was used for drawing,
    // so that WCS can use cache from last draw, unfortunately QgsRasterLayer::draw()
    // is doing some tricks with extent and size to align raster to output which
    // would be difficult to replicate here.
    // Note: cutting the extent may result in slightly different x and y resolutions
    // and thus shifted point calculated back in QGIS WMS (using average resolution)
    //viewExtent = dprovider->extent().intersect( &viewExtent );

    // Width and height are calculated from not projected extent and we hope that
    // are similar to source width and height used to reproject layer for drawing.
    // TODO: may be very dangerous, because it may result in different resolutions
    // in source CRS, and WMS server (QGIS server) calcs wrong coor using average resolution.
    const QgsRectangle extent = mMapSettings->mapSettings().extent();
    const int width = static_cast<int>( std::round( extent.width() / mapUnitsPerPixel ) );
    const int height = static_cast<int>( std::round( extent.height() / mapUnitsPerPixel ) );

    identifyResult = dataProvider->identify( point, Qgis::RasterIdentifyFormat::Feature, extent, width, height );
  }

  QMap<int, QVariant> identifyResults = identifyResult.results();
  for ( auto it = identifyResults.constBegin(); it != identifyResults.constEnd(); ++it )
  {
    const QVariant &result = it.value();
    if ( result.userType() == QMetaType::Type::Bool && !result.toBool() )
    {
      // sublayer not visible or not queryable
      continue;
    }

    if ( result.userType() == QMetaType::Type::QString )
    {
      // error
      // TODO: better error reporting
      continue;
    }

    // list of feature stores for a single sublayer
    const QgsFeatureStoreList featureStoreList = result.value<QgsFeatureStoreList>();
    for ( const QgsFeatureStore &featureStore : featureStoreList )
    {
      const QgsFeatureList storeFeatures = featureStore.features();
      for ( const QgsFeature &feature : storeFeatures )
      {
        // WMS sublayer and feature type, a sublayer may contain multiple feature types.
        // Sublayer name may be the same as layer name and feature type name
        // may be the same as sublayer. We try to avoid duplicities in label.
        QString sublayer = featureStore.params().value( QStringLiteral( "sublayer" ) ).toString();
        QString featureType = featureStore.params().value( QStringLiteral( "featureType" ) ).toString();
        // Strip UMN MapServer '_feature'
        featureType.remove( QStringLiteral( "_feature" ) );

        QStringList labels;
        if ( sublayer.compare( layer->name(), Qt::CaseInsensitive ) != 0 && sublayer.compare( QStringLiteral( "Null" ), Qt::CaseInsensitive ) != 0 )
        {
          labels << sublayer;
        }
        if ( ( featureType.compare( sublayer, Qt::CaseInsensitive ) != 0 || labels.isEmpty() ) && featureType.compare( QStringLiteral( "Null" ), Qt::CaseInsensitive ) != 0 )
        {
          labels << featureType;
        }


        results.append( IdentifyResult( layer, feature, !labels.isEmpty() ? QStringLiteral( "%1 - %2" ).arg( labels.join( QStringLiteral( " - " ) ), layer->name() ) : layer->name() ) );
      }
    }
  }

  return results;
}

QList<IdentifyTool::IdentifyResult> IdentifyTool::identifyVectorTileLayer( QgsVectorTileLayer *layer, const QgsPointXY &point ) const
{
  QList<IdentifyTool::IdentifyResult> results;
  if ( !layer || !layer->isSpatial() )
    return results;

  if ( !layer->isInScaleRange( mMapSettings->mapSettings().scale() ) )
  {
    return results;
  }

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

    const double tileScale = layer->tileMatrixSet().calculateTileScaleForMap(
      mMapSettings->mapSettings().scale(),
      mMapSettings->mapSettings().destinationCrs(),
      mMapSettings->mapSettings().extent(),
      mMapSettings->outputSize(),
      mMapSettings->outputDpi() );

    const int tileZoom = layer->tileMatrixSet().scaleToZoomLevel( tileScale );
    const QgsTileMatrix tileMatrix = layer->tileMatrixSet().tileMatrix( tileZoom );
    const QgsTileRange tileRange = tileMatrix.tileRangeFromExtent( r );

    const QVector<QgsTileXYZ> tiles = layer->tileMatrixSet().tilesInRange( tileRange, tileZoom );
    for ( const QgsTileXYZ &tileID : tiles )
    {
      const QgsVectorTileRawData data = layer->getRawTile( tileID );
      if ( data.data.isEmpty() )
        continue; // failed to get data

      QgsVectorTileMVTDecoder decoder( layer->tileMatrixSet() );
      if ( !decoder.decode( data ) )
        continue; // failed to decode

      QMap<QString, QgsFields> perLayerFields;
      const QStringList layerNames = decoder.layers();
      for ( const QString &layerName : layerNames )
      {
        QSet<QString> fieldNames = qgis::listToSet( decoder.layerFieldNames( layerName ) );
        perLayerFields[layerName] = QgsVectorTileUtils::makeQgisFields( fieldNames );
      }

      const QgsVectorTileFeatures features = decoder.layerFeatures( perLayerFields, QgsCoordinateTransform() );
      const QStringList featuresLayerNames = features.keys();
      for ( const QString &layerName : featuresLayerNames )
      {
        const QgsFields fFields = perLayerFields[layerName];
        const QVector<QgsFeature> &layerFeatures = features[layerName];
        for ( const QgsFeature &f : layerFeatures )
        {
          if ( f.fields().isEmpty() )
            continue;

          if ( f.geometry().intersects( r ) )
          {
            results.append( IdentifyResult( layer, f, QStringLiteral( "%1 - %2" ).arg( layer->name(), layerName ) ) );
          }
        }
      }
    }
  }
  catch ( QgsCsException &cse )
  {
    Q_UNUSED( cse )
    // catch exception for 'invalid' point and proceed with no features found
    QgsDebugError( QStringLiteral( "Caught CRS exception %1" ).arg( cse.what() ) );
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
  if ( mDeactivated == deactivated )
    return;

  if ( deactivated && mModel )
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

QgsPointXY IdentifyTool::toLayerCoordinates( QgsMapLayer *layer, const QgsPointXY &point ) const
{
  return mMapSettings->mapSettings().mapToLayerCoordinates( layer, point );
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
```


