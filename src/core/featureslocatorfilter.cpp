/***************************************************************************
  featureslocatorfilter.cpp

 ---------------------
 begin                : 01.12.2018
 copyright            : (C) 2018 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "featurelistextentcontroller.h"
#include "featureslocatorfilter.h"
#include "locatormodelsuperbridge.h"
#include "qgsgeometrywrapper.h"
#include "qgsquickmapsettings.h"

#include <QAction>
#include <qgsexpressioncontextutils.h>
#include <qgsfeedback.h>
#include <qgsmaplayermodel.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

#include <math.h>


FeaturesLocatorFilter::FeaturesLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent )
  : QgsLocatorFilter( parent )
  , mLocatorBridge( locatorBridge )
{
  setUseWithoutPrefix( true );
}

FeaturesLocatorFilter *FeaturesLocatorFilter::clone() const
{
  return new FeaturesLocatorFilter( mLocatorBridge );
}

void FeaturesLocatorFilter::prepare( const QString &string, const QgsLocatorContext &context )
{
  Q_UNUSED( context );

  if ( string.length() < 3 )
    return;

  mPreparedLayers.clear();
  const QMap<QString, QgsMapLayer *> layers = QgsProject::instance()->mapLayers();
  for ( auto it = layers.constBegin(); it != layers.constEnd(); ++it )
  {
    QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( it.value() );
    if ( !layer || !layer->dataProvider() || !layer->flags().testFlag( QgsMapLayer::Searchable ) )
      continue;

    QgsExpression expression( layer->displayExpression() );
    QgsExpressionContext context;
    context.appendScopes( QgsExpressionContextUtils::globalProjectLayerScopes( layer ) );
    expression.prepare( &context );

    QgsFeatureRequest req;
    req.setSubsetOfAttributes( expression.referencedAttributeIndexes( layer->fields() ).toList() );
    if ( !expression.needsGeometry() )
      req.setFlags( QgsFeatureRequest::NoGeometry );
    QString enhancedSearch = string;
    enhancedSearch.replace( " ", "%" );
    req.setFilterExpression( QStringLiteral( "%1 ILIKE '%%2%'" )
                             .arg( layer->displayExpression() )
                             .arg( enhancedSearch ) );
    req.setLimit( 30 );

    std::shared_ptr<PreparedLayer> preparedLayer( new PreparedLayer() );
    preparedLayer->expression = expression;
    preparedLayer->context = context;
    preparedLayer->layerId = layer->id();
    preparedLayer->layerName = layer->name();
    preparedLayer->featureSource.reset( new QgsVectorLayerFeatureSource( layer ) );
    preparedLayer->request = req;
    preparedLayer->layerIcon = QgsMapLayerModel::iconForLayer( layer );

    mPreparedLayers.append( preparedLayer );
  }
}

void FeaturesLocatorFilter::fetchResults( const QString &string, const QgsLocatorContext &, QgsFeedback *feedback )
{
  int foundInCurrentLayer;
  int foundInTotal = 0;
  QgsFeature f;

  // we cannot used const loop since iterator::nextFeature is not const
  for ( auto preparedLayer : qgis::as_const( mPreparedLayers ) )
  {
    foundInCurrentLayer = 0;
    QgsFeatureIterator it = preparedLayer->featureSource->getFeatures( preparedLayer->request );
    while ( it.nextFeature( f ) )
    {
      if ( feedback->isCanceled() )
        return;

      QgsLocatorResult result;
      result.group = preparedLayer->layerName;

      preparedLayer->context.setFeature( f );

      result.displayString = preparedLayer->expression.evaluate( &( preparedLayer->context ) ).toString();

      result.userData = QVariantList() << f.id() << preparedLayer->layerId;
      result.icon = preparedLayer->layerIcon;
      result.score = static_cast<double>( string.length() ) / result.displayString.size();
      result.actions << QgsLocatorResult::ResultAction( OpenForm, tr( "Open form" ), QStringLiteral( "ic_baseline-list_alt-24px" ) );

      emit resultFetched( result );

      foundInCurrentLayer++;
      foundInTotal++;
      if ( foundInCurrentLayer >= mMaxResultsPerLayer )
        break;
    }
    if ( foundInTotal >= mMaxTotalResults )
      break;
  }
}

void FeaturesLocatorFilter::triggerResult( const QgsLocatorResult &result )
{
  triggerResultFromAction( result, Normal );
}

void FeaturesLocatorFilter::triggerResultFromAction( const QgsLocatorResult &result, const int actionId )
{
  QVariantList dataList = result.userData.toList();
  QgsFeatureId fid = dataList.at( 0 ).toLongLong();
  QString layerId = dataList.at( 1 ).toString();
  QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( QgsProject::instance()->mapLayer( layerId ) );
  if ( !layer )
    return;

  QgsFeature f;
  QgsFeatureRequest req = QgsFeatureRequest().setFilterFid( fid );

  if ( actionId == OpenForm )
  {
    QMap<QgsVectorLayer *, QgsFeatureRequest> requests;
    requests.insert( layer, req );
    mLocatorBridge->featureListController()->model()->setFeatures( requests );
    mLocatorBridge->featureListController()->selection()->setSelection( 0 );
    mLocatorBridge->featureListController()->requestFeatureFormState();
  }
  else
  {
    QgsFeatureIterator it = layer->getFeatures( req.setNoAttributes() );
    it.nextFeature( f );
    QgsGeometry geom = f.geometry();
    if ( geom.isNull() || geom.constGet()->isEmpty() )
    {
      mLocatorBridge->emitMessage( tr( "Feature has no geometry" ) );
      return;
    }
    QgsRectangle r = mLocatorBridge->mapSettings()->mapSettings().layerExtentToOutputExtent( layer, geom.boundingBox() );

    // zoom in if point cannot be distinguished from others
    // code taken from QgsMapCanvas::zoomToSelected
    if ( !mLocatorBridge->keepScale() )
    {
      if ( layer->geometryType() == QgsWkbTypes::PointGeometry && r.isEmpty() )
      {
        int scaleFactor = 5;
        QgsPointXY center = mLocatorBridge->mapSettings()->mapSettings().mapToLayerCoordinates( layer, r.center() );
        QgsRectangle extentRect = mLocatorBridge->mapSettings()->mapSettings().mapToLayerCoordinates( layer, mLocatorBridge->mapSettings()->visibleExtent() ).scaled( 1.0 / scaleFactor, &center );
        QgsFeatureRequest req = QgsFeatureRequest().setFilterRect( extentRect ).setLimit( 1000 ).setNoAttributes();
        QgsFeatureIterator fit = layer->getFeatures( req );
        QgsFeature f;
        QgsPointXY closestPoint;
        double closestSquaredDistance = pow( extentRect.width() + extentRect.height(), 2.0 );
        bool pointFound = false;
        while ( fit.nextFeature( f ) )
        {
          QgsPointXY point = f.geometry().asPoint();
          double sqrDist = point.sqrDist( center );
          if ( sqrDist > closestSquaredDistance || sqrDist < 4 * std::numeric_limits<double>::epsilon() )
            continue;
          pointFound = true;
          closestPoint = point;
          closestSquaredDistance = sqrDist;
        }
        if ( pointFound )
        {
          // combine selected point with closest point and scale this rect
          r.combineExtentWith( mLocatorBridge->mapSettings()->mapSettings().layerToMapCoordinates( layer, closestPoint ) );
          r.scale( scaleFactor, &center );
        }
      }
      else if ( !r.isEmpty() )
      {
        r.scale( 5 );
      }
    }

    if ( r.isEmpty() || mLocatorBridge->keepScale() )
      mLocatorBridge->mapSettings()->setCenter( QgsPoint( r.center() ) );
    else
      mLocatorBridge->mapSettings()->setExtent( r );


    mLocatorBridge->locatorHighlightGeometry()->setProperty( "qgsGeometry", geom );
    mLocatorBridge->locatorHighlightGeometry()->setProperty( "crs", layer->crs() );
  }
}
