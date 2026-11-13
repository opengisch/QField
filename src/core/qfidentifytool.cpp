/***************************************************************************
  qfidentifytool.cpp - QfIdentifyTool

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
#include "qfidentifytool.h"
#include "qfmultifeaturelistmodel.h"
#include "qgsquickmapsettings.h"

#include <QSettings>
#include <QtConcurrent>
#include <qgsexpressioncontextutils.h>
#include <qgsfeaturestore.h>
#include <qgsrasteridentifyresult.h>
#include <qgsrasterlayer.h>
#include <qgsrenderer.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayertemporalproperties.h>
#include <qgsvectortiledataprovider.h>
#include <qgsvectortilelayer.h>
#include <qgsvectortileloader.h>
#include <qgsvectortilemvtdecoder.h>
#include <qgsvectortileutils.h>


QfIdentifyTool::QfIdentifyTool( QObject *parent )
  : QObject( parent )
  , mMapSettings( nullptr )
  , mSearchRadiusMm( 5 )
{
  connect( &mIdentifyWatcher, &QFutureWatcher<QList<IdentifyResult>>::finished, this, &QfIdentifyTool::onIdentifyFinished );
  connect( &mIdentifyWatcher, &QFutureWatcher<QList<IdentifyResult>>::canceled, this, &QfIdentifyTool::onIdentifyFinished );
}

QgsQuickMapSettings *QfIdentifyTool::mapSettings() const
{
  return mMapSettings;
}

void QfIdentifyTool::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
  {
    return;
  }

  mMapSettings = mapSettings;
  emit mapSettingsChanged();
}

QfMarkupManager *QfIdentifyTool::markupManager() const
{
  return mMarkupManager;
}

void QfIdentifyTool::setMarkupManager( QfMarkupManager *markupManager )
{
  if ( mMarkupManager == markupManager )
  {
    return;
  }

  mMarkupManager = markupManager;
  emit markupManagerChanged();
}

void QfIdentifyTool::identify( const QPointF &point )
{
  if ( !mEnabled )
  {
    return;
  }

  if ( mIdentifyWatcher.isRunning() )
  {
    mIdentifyWatcher.cancel();
    mIdentifyWatcher.waitForFinished();
  }

  if ( !mModel || !mMapSettings )
  {
    qWarning() << "Unable to use IdentifyTool without mapSettings or model property set.";
    return;
  }

  mModel->clear( true );

  mPreparedLayers.clear();
  mIdentifiedLayers.clear();

  QgsPointXY mapPoint = mMapSettings->screenToCoordinate( point );

  const QList<QgsMapLayer *> layers = mModel->selectedLayer() ? QList<QgsMapLayer *>() << mModel->selectedLayer() : mMapSettings->mapSettings().layers();
  for ( QgsMapLayer *layer : layers )
  {
    if ( !layer->flags().testFlag( QgsMapLayer::Identifiable ) )
    {
      continue;
    }

    if ( QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( layer ) )
    {
      std::shared_ptr<PreparedLayer> preparedLayer;
      preparedLayer.reset( prepareVectorLayer( vl, mapPoint ) );
      mPreparedLayers.append( preparedLayer );
      mIdentifiedLayers.insert( vl->id(), vl );
    }
    else if ( QgsRasterLayer *rl = qobject_cast<QgsRasterLayer *>( layer ) )
    {
      std::shared_ptr<PreparedLayer> preparedLayer;
      preparedLayer.reset( prepareRasterLayer( rl, mapPoint ) );
      mPreparedLayers.append( preparedLayer );
      mIdentifiedLayers.insert( rl->id(), rl );
    }
    else if ( QgsVectorTileLayer *vtl = qobject_cast<QgsVectorTileLayer *>( layer ) )
    {
      std::shared_ptr<PreparedLayer> preparedLayer;
      preparedLayer.reset( prepareVectorTileLayer( vtl, mapPoint ) );
      mPreparedLayers.append( preparedLayer );
      mIdentifiedLayers.insert( vtl->id(), vtl );
    }
  }

  if ( mMarkupManager )
  {
    const QList<QfMarkupCollection *> collections = mMarkupManager->visibleCollections();
    for ( QfMarkupCollection *collection : collections )
    {
      std::shared_ptr<PreparedLayer> preparedLayer;
      QgsVectorLayer *vl = collection->asVectorLayer();
      preparedLayer.reset( prepareVectorLayer( vl, mapPoint ) );
      mPreparedLayers.append( preparedLayer );
      mIdentifiedLayers.insert( vl->id(), vl );
    }
  }

  if ( mPreparedLayers.isEmpty() )
  {
    return;
  }

  QFuture<QList<QfIdentifyTool::IdentifyResult>> future = QtConcurrent::run( [this]( QPromise<QList<QfIdentifyTool::IdentifyResult>> &promise ) {
    QList<QfIdentifyTool::IdentifyResult> results;
    for ( auto &preparedLayer : std::as_const( mPreparedLayers ) )
    {
      if ( !preparedLayer->isValid )
      {
        continue;
      }

      switch ( preparedLayer->layerType )
      {
        case Qgis::LayerType::Vector:
        {
          results << processVectorLayer( preparedLayer.get() );
          break;
        }

        case Qgis::LayerType::Raster:
        {
          results << processRasterLayer( preparedLayer.get() );
          break;
        }

        case Qgis::LayerType::VectorTile:
        {
          results << processVectorTileLayer( preparedLayer.get() );
          break;
        }

        case Qgis::LayerType::Plugin:
        case Qgis::LayerType::Mesh:
        case Qgis::LayerType::Annotation:
        case Qgis::LayerType::PointCloud:
        case Qgis::LayerType::Group:
        case Qgis::LayerType::TiledScene:
          break;
      }

      if ( promise.isCanceled() )
      {
        return;
      }
    }

    if ( !promise.isCanceled() )
    {
      promise.addResult( results );
    }
  } );

  mIdentifyWatcher.setFuture( future );

  emit isIdentifyingChanged();
}

bool QfIdentifyTool::isIdentifying() const
{
  return mIdentifyWatcher.isRunning();
}

void QfIdentifyTool::onIdentifyFinished()
{
  if ( mIdentifyWatcher.isFinished() )
  {
    QList<IdentifyResult> results = mIdentifyWatcher.result();

    for ( IdentifyResult &result : results )
    {
      if ( mIdentifiedLayers.contains( result.layerId ) )
      {
        result.layer = mIdentifiedLayers.value( result.layerId );
      }
    }

    mModel->appendFeatures( results );

    emit identifyFinished();
  }

  mPreparedLayers.clear();
  mIdentifiedLayers.clear();

  emit isIdentifyingChanged();
}

QfIdentifyTool::PreparedLayer *QfIdentifyTool::prepareVectorLayer( QgsVectorLayer *layer, const QgsPointXY &point ) const
{
  PreparedLayer *preparedLayer = new PreparedLayer();

  if ( !layer || ( !layer->isSpatial() && layer->wkbType() != QgsWkbTypes::flatType( Qgis::WkbType::GeometryCollection ) ) )
  {
    return preparedLayer;
  }

  if ( !layer->isInScaleRange( mMapSettings->mapSettings().scale() ) )
  {
    return preparedLayer;
  }

  QString temporalFilter;
  if ( mMapSettings->isTemporal() )
  {
    if ( !layer->temporalProperties()->isVisibleInTemporalRange( mMapSettings->mapSettings().temporalRange() ) )
    {
      return preparedLayer;
    }

    QgsVectorLayerTemporalContext temporalContext;
    temporalContext.setLayer( layer );
    temporalFilter = qobject_cast<const QgsVectorLayerTemporalProperties *>( layer->temporalProperties() )->createFilterString( temporalContext, mMapSettings->mapSettings().temporalRange() );
  }

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

    preparedLayer->request.setFilterRect( r );
    if ( !temporalFilter.isEmpty() )
    {
      preparedLayer->request.setFilterExpression( temporalFilter );
    }
    preparedLayer->request.setLimit( QSettings().value( "/QField/identify/limit", 200 ).toInt() );
    preparedLayer->request.setFlags( Qgis::FeatureRequestFlag::ExactIntersect );

    QgsAttributeTableConfig config = layer->attributeTableConfig();
    if ( !config.sortExpression().isEmpty() )
    {
      preparedLayer->request.addOrderBy( config.sortExpression(), config.sortOrder() == Qt::AscendingOrder );
    }
    else if ( !layer->displayExpression().isEmpty() )
    {
      preparedLayer->request.addOrderBy( layer->displayExpression() );
    }
  }
  catch ( const QgsCsException & )
  {
    return preparedLayer;
  }

  preparedLayer->fields = layer->fields();

  if ( layer->renderer() )
  {
    preparedLayer->renderContext = QgsRenderContext::fromMapSettings( mMapSettings->mapSettings() );
    preparedLayer->renderContext.setExpressionContext( QgsExpressionContext( QgsExpressionContextUtils::globalProjectLayerScopes( layer ) ) );
    preparedLayer->renderContext.expressionContext() << QgsExpressionContextUtils::mapSettingsScope( mMapSettings->mapSettings() );
    preparedLayer->vectorRenderer.reset( layer->renderer()->clone() );
  }

  preparedLayer->vectorFeatureSource.reset( new QgsVectorLayerFeatureSource( layer ) );

  preparedLayer->layerType = Qgis::LayerType::Vector;
  preparedLayer->layerId = layer->id();
  preparedLayer->layerName = layer->name();

  preparedLayer->isValid = true;
  return preparedLayer;
}

QList<QfIdentifyTool::IdentifyResult> QfIdentifyTool::processVectorLayer( PreparedLayer *preparedLayer ) const
{
  QList<QfIdentifyTool::IdentifyResult> results;
  if ( !preparedLayer || !preparedLayer->isValid || preparedLayer->layerType != Qgis::LayerType::Vector )
  {
    return results;
  }

  QgsFeatureList featureList;
  QgsFeatureIterator fit = preparedLayer->vectorFeatureSource->getFeatures( preparedLayer->request );
  QgsFeature f;
  while ( fit.nextFeature( f ) )
  {
    featureList << QgsFeature( f );
  }

  if ( preparedLayer->vectorRenderer )
  {
    preparedLayer->vectorRenderer->startRender( preparedLayer->renderContext, preparedLayer->fields );
  }

  for ( const QgsFeature &feature : std::as_const( featureList ) )
  {
    if ( preparedLayer->vectorRenderer )
    {
      preparedLayer->renderContext.expressionContext().setFeature( feature );
      if ( !preparedLayer->vectorRenderer->willRenderFeature( const_cast<QgsFeature &>( feature ), preparedLayer->renderContext ) )
        continue;
    }

    results.append( IdentifyResult( preparedLayer->layerId, feature ) );
  }

  if ( preparedLayer->vectorRenderer )
  {
    preparedLayer->vectorRenderer->stopRender( preparedLayer->renderContext );
  }

  return results;
}

QList<QfIdentifyTool::IdentifyResult> QfIdentifyTool::identifyVectorLayer( QgsVectorLayer *layer, const QgsPointXY &point ) const
{
  std::unique_ptr<PreparedLayer> preparedLayer;
  preparedLayer.reset( prepareVectorLayer( layer, point ) );
  QList<QfIdentifyTool::IdentifyResult> results = processVectorLayer( preparedLayer.get() );
  for ( IdentifyResult &result : results )
  {
    result.layer = layer;
  }
  return results;
}

QfIdentifyTool::PreparedLayer *QfIdentifyTool::prepareRasterLayer( QgsRasterLayer *layer, const QgsPointXY &point ) const
{
  PreparedLayer *preparedLayer = new PreparedLayer();

  if ( !layer || !layer->dataProvider() || !layer->isValid() )
  {
    return preparedLayer;
  }

  const Qgis::RasterInterfaceCapabilities capabilities = layer->dataProvider()->capabilities();

  if ( !( capabilities & Qgis::RasterInterfaceCapability::Identify ) )
  {
    return preparedLayer;
  }

  if ( !( capabilities & Qgis::RasterInterfaceCapability::IdentifyFeature ) )
  {
    return preparedLayer;
  }

  preparedLayer->rasterDataProvider.reset( layer->dataProvider()->clone() );
  preparedLayer->rasterPoint = toLayerCoordinates( layer, point );

  const double mapUnitsPerPixel = mMapSettings->mapSettings().mapUnitsPerPixel();
  // We can only use current map canvas context (extent, width, height) if layer is not reprojected,
  if ( preparedLayer->rasterDataProvider->crs() != mMapSettings->mapSettings().destinationCrs() )
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

    preparedLayer->extent = r;
    preparedLayer->rasterWidth = 1;
    preparedLayer->rasterHeight = 1;
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
    preparedLayer->extent = mMapSettings->mapSettings().extent();
    preparedLayer->rasterWidth = static_cast<int>( std::round( preparedLayer->extent.width() / mapUnitsPerPixel ) );
    preparedLayer->rasterHeight = static_cast<int>( std::round( preparedLayer->extent.height() / mapUnitsPerPixel ) );
    ;
  }

  preparedLayer->layerType = Qgis::LayerType::Raster;
  preparedLayer->layerId = layer->id();
  preparedLayer->layerName = layer->name();

  preparedLayer->isValid = true;
  return preparedLayer;
}

QList<QfIdentifyTool::IdentifyResult> QfIdentifyTool::processRasterLayer( PreparedLayer *preparedLayer ) const
{
  QList<QfIdentifyTool::IdentifyResult> results;

  if ( !preparedLayer || !preparedLayer->isValid || preparedLayer->layerType != Qgis::LayerType::Raster )
  {
    return results;
  }

  const QgsRasterIdentifyResult identifyResult = preparedLayer->rasterDataProvider->identify( preparedLayer->rasterPoint, Qgis::RasterIdentifyFormat::Feature, preparedLayer->extent, preparedLayer->rasterWidth, preparedLayer->rasterHeight );
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
        if ( sublayer.compare( preparedLayer->layerName, Qt::CaseInsensitive ) != 0 && sublayer.compare( QStringLiteral( "Null" ), Qt::CaseInsensitive ) != 0 )
        {
          labels << sublayer;
        }
        if ( ( featureType.compare( sublayer, Qt::CaseInsensitive ) != 0 || labels.isEmpty() ) && featureType.compare( QStringLiteral( "Null" ), Qt::CaseInsensitive ) != 0 )
        {
          labels << featureType;
        }


        results.append( IdentifyResult( preparedLayer->layerId, feature, !labels.isEmpty() ? QStringLiteral( "%1 - %2" ).arg( labels.join( QStringLiteral( " - " ) ), preparedLayer->layerName ) : preparedLayer->layerName ) );
      }
    }
  }

  return results;
}

QList<QfIdentifyTool::IdentifyResult> QfIdentifyTool::identifyRasterLayer( QgsRasterLayer *layer, const QgsPointXY &point ) const
{
  std::unique_ptr<PreparedLayer> preparedLayer;
  preparedLayer.reset( prepareRasterLayer( layer, point ) );
  QList<QfIdentifyTool::IdentifyResult> results = processRasterLayer( preparedLayer.get() );
  for ( IdentifyResult &result : results )
  {
    result.layer = layer;
  }
  return results;
}

QfIdentifyTool::PreparedLayer *QfIdentifyTool::prepareVectorTileLayer( QgsVectorTileLayer *layer, const QgsPointXY &point ) const
{
  PreparedLayer *preparedLayer = new PreparedLayer();

  if ( !layer || !layer->dataProvider() || !layer->isSpatial() )
  {
    return preparedLayer;
  }

  if ( !layer->isInScaleRange( mMapSettings->mapSettings().scale() ) )
  {
    return preparedLayer;
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
    preparedLayer->extent = r;
  }
  catch ( const QgsCsException & )
  {
    return preparedLayer;
  }

  preparedLayer->vectorTileScale = layer->tileMatrixSet().calculateTileScaleForMap(
    mMapSettings->mapSettings().scale(),
    mMapSettings->mapSettings().destinationCrs(),
    mMapSettings->mapSettings().extent(),
    mMapSettings->outputSize(),
    mMapSettings->outputDpi() );
  preparedLayer->vectorTileDataProvider.reset( dynamic_cast<QgsVectorTileDataProvider *>( layer->dataProvider() )->clone() );

  preparedLayer->layerType = Qgis::LayerType::VectorTile;
  preparedLayer->layerId = layer->id();
  preparedLayer->layerName = layer->name();

  preparedLayer->isValid = true;
  return preparedLayer;
}

QList<QfIdentifyTool::IdentifyResult> QfIdentifyTool::processVectorTileLayer( PreparedLayer *preparedLayer ) const
{
  QList<QfIdentifyTool::IdentifyResult> results;

  if ( !preparedLayer || !preparedLayer->isValid || preparedLayer->layerType != Qgis::LayerType::VectorTile )
  {
    return results;
  }

  const int tileZoom = preparedLayer->vectorTileDataProvider->tileMatrixSet().scaleToZoomLevel( preparedLayer->vectorTileScale );
  const QgsTileMatrix tileMatrix = preparedLayer->vectorTileDataProvider->tileMatrixSet().tileMatrix( tileZoom );
  const QgsTileRange tileRange = tileMatrix.tileRangeFromExtent( preparedLayer->extent );

  const QVector<QgsTileXYZ> tiles = preparedLayer->vectorTileDataProvider->tileMatrixSet().tilesInRange( tileRange, tileZoom );
  for ( const QgsTileXYZ &tileID : tiles )
  {
    const QgsVectorTileRawData data = preparedLayer->vectorTileDataProvider->readTile( preparedLayer->vectorTileDataProvider->tileMatrixSet(), tileID );
    if ( data.data.isEmpty() )
    {
      continue;
    }

    QgsVectorTileMVTDecoder decoder( preparedLayer->vectorTileDataProvider->tileMatrixSet() );
    if ( !decoder.decode( data ) )
    {
      continue;
    }

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

        if ( f.geometry().intersects( preparedLayer->extent ) )
        {
          results.append( IdentifyResult( preparedLayer->layerId, f, QStringLiteral( "%1 - %2" ).arg( preparedLayer->layerName, layerName ) ) );
        }
      }
    }
  }

  return results;
}

QList<QfIdentifyTool::IdentifyResult> QfIdentifyTool::identifyVectorTileLayer( QgsVectorTileLayer *layer, const QgsPointXY &point ) const
{
  std::unique_ptr<PreparedLayer> preparedLayer;
  preparedLayer.reset( prepareVectorTileLayer( layer, point ) );
  QList<QfIdentifyTool::IdentifyResult> results = processVectorTileLayer( preparedLayer.get() );
  for ( IdentifyResult &result : results )
  {
    result.layer = layer;
  }
  return results;
}

QfMultiFeatureListModel *QfIdentifyTool::model() const
{
  return mModel;
}

void QfIdentifyTool::setModel( QfMultiFeatureListModel *model )
{
  if ( model == mModel )
  {
    return;
  }

  mModel = model;
  emit modelChanged();
}

void QfIdentifyTool::setEnabled( bool enabled )
{
  if ( mEnabled == enabled )
  {
    return;
  }

  mEnabled = enabled;
  emit enabledChanged();

  if ( !mEnabled && mModel )
  {
    mModel->clear();
  }
}

double QfIdentifyTool::searchRadiusMU( const QgsRenderContext &context ) const
{
  return mSearchRadiusMm * context.scaleFactor() * context.mapToPixel().mapUnitsPerPixel();
}

double QfIdentifyTool::searchRadiusMU() const
{
  QgsRenderContext context = QgsRenderContext::fromMapSettings( mMapSettings->mapSettings() );
  return searchRadiusMU( context );
}

QgsRectangle QfIdentifyTool::toLayerCoordinates( QgsMapLayer *layer, QgsRectangle rect ) const
{
  if ( !layer || !mMapSettings )
  {
    return rect;
  }

  QgsCoordinateReferenceSystem crs = layer->crs();
  if ( QgsVectorLayer *vlayer = dynamic_cast<QgsVectorLayer *>( layer ) )
  {
    if ( vlayer->dataProvider() && QgsWkbTypes::flatType( vlayer->wkbType() ) == Qgis::WkbType::GeometryCollection )
    {
      crs = vlayer->dataProvider()->crs();
    }
  }

  try
  {
    const QgsCoordinateTransform ct( mMapSettings->destinationCrs(), crs, mMapSettings->transformContext() );
    if ( ct.isValid() )
    {
      rect = ct.transform( rect );
    }
  }
  catch ( QgsCsException &cse )
  {
    qInfo() << QStringLiteral( "Identify tool Transform error caught: %1" ).arg( cse.what() );
  }

  return rect;
}

QgsPointXY QfIdentifyTool::toLayerCoordinates( QgsMapLayer *layer, QgsPointXY point ) const
{
  if ( !layer || !mMapSettings )
  {
    return point;
  }

  QgsCoordinateReferenceSystem crs = layer->crs();
  if ( QgsVectorLayer *vlayer = dynamic_cast<QgsVectorLayer *>( layer ) )
  {
    if ( vlayer->dataProvider() && QgsWkbTypes::flatType( vlayer->wkbType() ) == Qgis::WkbType::GeometryCollection )
    {
      crs = vlayer->dataProvider()->crs();
    }
  }

  try
  {
    const QgsCoordinateTransform ct( mMapSettings->destinationCrs(), crs, mMapSettings->transformContext() );
    if ( ct.isValid() )
    {
      point = ct.transform( point );
    }
  }
  catch ( QgsCsException &cse )
  {
    qInfo() << QStringLiteral( "Identify tool Transform error caught: %1" ).arg( cse.what() );
  }

  return point;
}

double QfIdentifyTool::searchRadiusMm() const
{
  return mSearchRadiusMm;
}

void QfIdentifyTool::setSearchRadiusMm( double searchRadiusMm )
{
  if ( mSearchRadiusMm == searchRadiusMm )
  {
    return;
  }

  mSearchRadiusMm = searchRadiusMm;
  emit searchRadiusMmChanged();
}
