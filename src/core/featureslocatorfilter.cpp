#include "featureslocatorfilter.h"

#include <QAction>

#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include <qgsmaplayermodel.h>
#include <qgsfeedback.h>

#include "qgsquickmapsettings.h"
#include "locatorhighlight.h"


FeaturesLocatorFilter::FeaturesLocatorFilter( QgsQuickMapSettings *mapSettings, LocatorHighlight *locatorHighlight, QObject *parent )
  : QgsLocatorFilter( parent )
  , mMapSettings( mapSettings )
  , mLocatorHighlight( locatorHighlight )
{
  setUseWithoutPrefix( true );
}

FeaturesLocatorFilter *FeaturesLocatorFilter::clone() const
{
  return new FeaturesLocatorFilter( mMapSettings, mLocatorHighlight );
}

void FeaturesLocatorFilter::prepare( const QString &string, const QgsLocatorContext &context )
{
  if ( string.length() < 3 || context.usingPrefix )
    return;

  const QMap<QString, QgsMapLayer *> layers = QgsProject::instance()->mapLayers();
  for ( auto it = layers.constBegin(); it != layers.constEnd(); ++it )
  {
    QgsVectorLayer *layer = qobject_cast< QgsVectorLayer *>( it.value() );
    if ( !layer || !layer->flags().testFlag( QgsMapLayer::Searchable ) )
      continue;

    QgsExpression expression( layer->displayExpression() );
    QgsExpressionContext context;
    context.appendScopes( QgsExpressionContextUtils::globalProjectLayerScopes( layer ) );
    expression.prepare( &context );

    QgsFeatureRequest req;
    req.setSubsetOfAttributes( expression.referencedAttributeIndexes( layer->fields() ).toList() );
    if ( !expression.needsGeometry() )
      req.setFlags( QgsFeatureRequest::NoGeometry );
    req.setFilterExpression( QStringLiteral( "%1 ILIKE '%%2%'" )
                             .arg( layer->displayExpression() )
                             .arg( string ) );
    req.setLimit( 30 );

    PreparedLayer preparedLayer;
    preparedLayer.expression = expression;
    preparedLayer.context = context;
    preparedLayer.layerId = layer->id();
    preparedLayer.layerName = layer->name();
    preparedLayer.iterator =  layer->getFeatures( req );
    preparedLayer.layerIcon = QgsMapLayerModel::iconForLayer( layer );

    mPreparedLayers.append( preparedLayer );
  }
}

void FeaturesLocatorFilter::fetchResults( const QString &string, const QgsLocatorContext &, QgsFeedback *feedback )
{
  int foundInCurrentLayer;
  int foundInTotal = 0;
  QgsFeature f;

  // we cannot used const loop since iterator::nextFeature is not const
  for ( PreparedLayer preparedLayer : mPreparedLayers )
  {
    foundInCurrentLayer = 0;
    while ( preparedLayer.iterator.nextFeature( f ) )
    {
      if ( feedback->isCanceled() )
        return;

      QgsLocatorResult result;
      result.group = preparedLayer.layerName;

      preparedLayer.context.setFeature( f );

      result.displayString = preparedLayer.expression.evaluate( &( preparedLayer.context ) ).toString();

      result.userData = QVariantList() << f.id() << preparedLayer.layerId;
      result.icon = preparedLayer.layerIcon;
      result.score = static_cast< double >( string.length() ) / result.displayString.size();
      QAction *action = new QAction( tr( "Open form" ) );
      action->setData( QStringLiteral( "ic_create_white_24dp" ) );
      result.contextMenuActions.insert( OpenForm, action );

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
  triggerResultFromContextMenu( result, NoEntry );
}

void FeaturesLocatorFilter::triggerResultFromContextMenu( const QgsLocatorResult &result, const int id )
{
  QVariantList dataList = result.userData.toList();
  QgsFeatureId fid = dataList.at( 0 ).toLongLong();
  QString layerId = dataList.at( 1 ).toString();
  QgsVectorLayer *layer = qobject_cast< QgsVectorLayer *>( QgsProject::instance()->mapLayer( layerId ) );
  if ( !layer )
    return;

  QgsFeature f;
  QgsFeatureIterator it = layer->getFeatures( QgsFeatureRequest().setFilterFid( fid ).setNoAttributes() );
  it.nextFeature( f );
  QgsGeometry geom = f.geometry();
  if ( geom.isNull() || geom.constGet()->isEmpty() )
    return;
  QgsRectangle r = mMapSettings->mapSettings().layerExtentToOutputExtent( layer, geom.boundingBox() );

  if ( r.isEmpty() )
    mMapSettings->setCenter( QgsPoint( r.center() ) );
  else
    mMapSettings->setExtent( r.scaled( 1.2 ) );

  mLocatorHighlight->highlightGeometry( geom, layer->crs() );
}
