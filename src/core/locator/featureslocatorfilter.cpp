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

QStringList FeaturesLocatorFilter::prepare( const QString &string, const QgsLocatorContext &locatorContext )
{
  Q_UNUSED( locatorContext );

  if ( string.length() < 3 )
    return QStringList();

  mPreparedLayers.clear();
  const QMap<QString, QgsMapLayer *> layers = QgsProject::instance()->mapLayers();
  for ( auto it = layers.constBegin(); it != layers.constEnd(); ++it )
  {
    QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( it.value() );
    if ( !layer || !layer->dataProvider() || !layer->flags().testFlag( QgsMapLayer::Searchable ) )
      continue;

    QgsExpression expression( layer->displayExpression() );
    QgsExpressionContext expressionContext;
    expressionContext.appendScopes( QgsExpressionContextUtils::globalProjectLayerScopes( layer ) );
    expression.prepare( &expressionContext );

    QgsFeatureRequest req;
    req.setSubsetOfAttributes( expression.referencedAttributeIndexes( layer->fields() ).values() );
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
    preparedLayer->context = expressionContext;
    preparedLayer->layerId = layer->id();
    preparedLayer->layerName = layer->name();
    preparedLayer->featureSource.reset( new QgsVectorLayerFeatureSource( layer ) );
    preparedLayer->request = req;
    preparedLayer->layerIcon = QgsMapLayerModel::iconForLayer( layer );
    preparedLayer->layerGeometryType = layer->geometryType();

    mPreparedLayers.append( preparedLayer );
  }

  return QStringList();
}

void FeaturesLocatorFilter::fetchResults( const QString &string, const QgsLocatorContext &, QgsFeedback *feedback )
{
  int foundInTotal = 0;
  QgsFeature f;

  // we cannot used const loop since iterator::nextFeature is not const
  for ( auto preparedLayer : std::as_const( mPreparedLayers ) )
  {
    int foundInCurrentLayer = 0;
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
      if ( preparedLayer->layerGeometryType != QgsWkbTypes::NullGeometry && preparedLayer->layerGeometryType != QgsWkbTypes::UnknownGeometry )
      {
        result.actions << QgsLocatorResult::ResultAction( Navigation, tr( "Set feature as destination" ), QStringLiteral( "ic_navigation_flag_purple_24dp" ) );
      }

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

  QgsFeature feature;
  QgsFeatureRequest featureRequest = QgsFeatureRequest().setFilterFid( fid );

  if ( actionId == OpenForm )
  {
    QMap<QgsVectorLayer *, QgsFeatureRequest> requests;
    requests.insert( layer, featureRequest );
    mLocatorBridge->featureListController()->model()->setFeatures( requests );
    mLocatorBridge->featureListController()->selection()->setFocusedItem( 0 );
    mLocatorBridge->featureListController()->requestFeatureFormState();
  }
  else if ( actionId == Navigation )
  {
    if ( !mLocatorBridge->navigation() )
      return;

    QgsFeatureIterator it = layer->getFeatures( featureRequest );
    it.nextFeature( feature );
    if ( feature.hasGeometry() )
    {
      mLocatorBridge->navigation()->setDestinationFeature( feature, layer );
    }
    else
    {
      mLocatorBridge->emitMessage( tr( "Feature has no geometry" ) );
    }
  }
  else
  {
    QgsFeatureIterator it = layer->getFeatures( featureRequest.setNoAttributes() );
    it.nextFeature( feature );
    const QgsGeometry geom = feature.geometry();
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
        const QgsPointXY center = mLocatorBridge->mapSettings()->mapSettings().mapToLayerCoordinates( layer, r.center() );
        const QgsRectangle extentRect = mLocatorBridge->mapSettings()->mapSettings().mapToLayerCoordinates( layer, mLocatorBridge->mapSettings()->visibleExtent() ).scaled( 1.0 / scaleFactor, &center );
        const QgsFeatureRequest pointRequest = QgsFeatureRequest().setFilterRect( extentRect ).setLimit( 1000 ).setNoAttributes();
        QgsFeatureIterator fit = layer->getFeatures( pointRequest );
        QgsFeature pointFeature;
        QgsPointXY closestPoint;
        double closestSquaredDistance = pow( extentRect.width() + extentRect.height(), 2.0 );
        bool pointFound = false;
        while ( fit.nextFeature( pointFeature ) )
        {
          QgsPointXY point = pointFeature.geometry().asPoint();
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
          const QgsPointXY rCenter = r.center();
          r.scale( scaleFactor, &rCenter );
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
