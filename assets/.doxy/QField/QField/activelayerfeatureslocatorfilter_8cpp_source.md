

# File activelayerfeatureslocatorfilter.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**activelayerfeatureslocatorfilter.cpp**](activelayerfeatureslocatorfilter_8cpp.md)

[Go to the documentation of this file](activelayerfeatureslocatorfilter_8cpp.md)


```C++
/***************************************************************************
  activelayerfeatureslocatorfilter.cpp

 ---------------------
 begin                : 30.08.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "activelayerfeatureslocatorfilter.h"
#include "featurelistextentcontroller.h"
#include "locatormodelsuperbridge.h"
#include "qgsquickmapsettings.h"

#include <QAction>
#include <qgsexpressioncontextutils.h>
#include <qgsfeedback.h>
#include <qgsmaplayermodel.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

#include <math.h>


ActiveLayerFeaturesLocatorFilter::ActiveLayerFeaturesLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent )
  : QgsLocatorFilter( parent )
  , mLocatorBridge( locatorBridge )
{
  setUseWithoutPrefix( false );
}

ActiveLayerFeaturesLocatorFilter *ActiveLayerFeaturesLocatorFilter::clone() const
{
  return new ActiveLayerFeaturesLocatorFilter( mLocatorBridge );
}

QString ActiveLayerFeaturesLocatorFilter::fieldRestriction( QString &searchString, bool *isRestricting )
{
  QString _fieldRestriction;
  searchString = searchString.trimmed();
  if ( isRestricting )
  {
    *isRestricting = searchString.startsWith( '@' );
  }
  if ( searchString.startsWith( '@' ) )
  {
    _fieldRestriction = searchString.left( std::min( searchString.indexOf( ' ' ), searchString.length() ) ).remove( 0, 1 );
    searchString = searchString.mid( _fieldRestriction.length() + 2 );
  }
  return _fieldRestriction;
}

QStringList ActiveLayerFeaturesLocatorFilter::prepare( const QString &string, const QgsLocatorContext &locatorContext )
{
  // Normally skip very short search strings, unless when specifically searching using this filter or try to match fields
  if ( string.length() < 3 && !locatorContext.usingPrefix && !string.startsWith( '@' ) )
    return QStringList();

  QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( mLocatorBridge->activeLayer() );
  if ( !layer )
    return QStringList();

  mLayerIsSpatial = layer->isSpatial();
  mDispExpression = QgsExpression( layer->displayExpression() );
  mContext.appendScopes( QgsExpressionContextUtils::globalProjectLayerScopes( layer ) );
  mDispExpression.prepare( &mContext );

  // determine if search is restricted to a specific field
  QString searchString = string;
  bool isRestricting = false;
  QString _fieldRestriction = fieldRestriction( searchString, &isRestricting );
  bool allowNumeric = false;
  double numericalValue = searchString.toDouble( &allowNumeric );

  // search in display expression if no field restriction
  if ( !isRestricting )
  {
    QgsFeatureRequest req;
    req.setSubsetOfAttributes( qgis::setToList( mDispExpression.referencedAttributeIndexes( layer->fields() ) ) );
    if ( !mDispExpression.needsGeometry() )
    {
      req.setFlags( Qgis::FeatureRequestFlag::NoGeometry );
    }
    QString enhancedSearch = searchString;
    enhancedSearch.replace( ' ', '%' );
    req.setFilterExpression( QStringLiteral( "%1 ILIKE '%%2%'" )
                               .arg( layer->displayExpression(), enhancedSearch ) );
    req.setLimit( mMaxTotalResults );
    mDisplayTitleIterator = layer->getFeatures( req );
  }
  else
  {
    mDisplayTitleIterator = QgsFeatureIterator();
  }

  // build up request expression
  QStringList expressionParts;
  QStringList completionList;
  const QgsFields fields = layer->fields();
  QgsAttributeList subsetOfAttributes = qgis::setToList( mDispExpression.referencedAttributeIndexes( layer->fields() ) );
  for ( const QgsField &field : fields )
  {
    if ( field.configurationFlags().testFlag( Qgis::FieldConfigurationFlag::NotSearchable ) )
    {
      continue;
    }

    if ( isRestricting )
    {
      if ( !field.name().startsWith( _fieldRestriction ) )
      {
        continue;
      }

      int index = layer->fields().indexFromName( field.name() );
      if ( !subsetOfAttributes.contains( index ) )
      {
        subsetOfAttributes << index;
      }

      // if we are trying to find a field (and not searching anything yet)
      // keep the list of matching fields to display them as results
      if ( searchString.isEmpty() && _fieldRestriction != field.name() )
      {
        mFieldsCompletion << field.name();
      }
    }
    else if ( searchString.isEmpty() )
    {
      mFieldsCompletion << field.name();
    }

    // the completion list (returned by the current method) is used by the locator line edit directly
    completionList.append( QStringLiteral( "@%1 " ).arg( field.name() ) );

    if ( field.type() == QMetaType::QString )
    {
      expressionParts << QStringLiteral( "%1 ILIKE '%%2%'" ).arg( QgsExpression::quotedColumnRef( field.name() ), searchString );
    }
    else if ( allowNumeric && field.isNumeric() )
    {
      expressionParts << QStringLiteral( "%1 = %2" ).arg( QgsExpression::quotedColumnRef( field.name() ), QString::number( numericalValue, 'g', 17 ) );
    }
  }

  QString expression = QStringLiteral( "(%1)" ).arg( expressionParts.join( QLatin1String( " ) OR ( " ) ) );

  QgsFeatureRequest req;
  if ( !mDispExpression.needsGeometry() )
  {
    req.setFlags( Qgis::FeatureRequestFlag::NoGeometry );
  }
  req.setFilterExpression( expression );
  if ( isRestricting )
  {
    req.setSubsetOfAttributes( subsetOfAttributes );
  }

  req.setLimit( mMaxTotalResults );
  mFieldIterator = layer->getFeatures( req );

  mLayerId = layer->id();
  mLayerName = layer->name();
  mAttributeAliases.clear();
  for ( int idx = 0; idx < layer->fields().size(); ++idx )
  {
    mAttributeAliases.append( layer->attributeDisplayName( idx ) );
  }

  return completionList;
}

void ActiveLayerFeaturesLocatorFilter::fetchResults( const QString &string, const QgsLocatorContext &, QgsFeedback *feedback )
{
  QgsFeatureIds featuresFound;
  QgsFeature f;
  QString searchString = string;
  fieldRestriction( searchString );

  if ( searchString.trimmed().isEmpty() )
  {
    // propose available fields for restriction
    for ( const QString &field : std::as_const( mFieldsCompletion ) )
    {
      QgsLocatorResult result;
      result.displayString = QStringLiteral( "@%1" ).arg( field );
      result.group = mLayerName;
      result.description = tr( "Limit the search to the field '%1'" ).arg( field );
      result.setUserData( QVariantMap( { { QStringLiteral( "type" ), QVariant::fromValue( ResultType::FieldRestriction ) },
                                         { QStringLiteral( "search_text" ), QStringLiteral( "%1 @%2 " ).arg( prefix(), field ) } } ) );
      result.score = 1;
      emit resultFetched( result );
    }
    return;
  }

  // search in display title
  if ( mDisplayTitleIterator.isValid() )
  {
    while ( mDisplayTitleIterator.nextFeature( f ) )
    {
      if ( feedback->isCanceled() )
        return;

      mContext.setFeature( f );

      QgsLocatorResult result;
      result.displayString = mDispExpression.evaluate( &mContext ).toString();
      result.group = mLayerName;

      result.setUserData( QVariantList() << f.id() << mLayerId );
      result.score = static_cast<double>( searchString.length() ) / result.displayString.size();
      result.actions << QgsLocatorResult::ResultAction( OpenForm, tr( "Open form" ), QStringLiteral( "qrc:/themes/qfield/nodpi/ic_baseline-list_white_24dp.svg?color=mainColor" ) );
      if ( mLayerIsSpatial )
      {
        result.actions << QgsLocatorResult::ResultAction( Navigation, tr( "Set feature as destination" ), QStringLiteral( "qrc:/themes/qfield/nodpi/ic_navigation_flag_purple_24dp.svg" ) );
      }

      emit resultFetched( result );

      featuresFound << f.id();
      if ( featuresFound.count() >= mMaxTotalResults )
        break;
    }
  }

  // search in fields
  while ( mFieldIterator.nextFeature( f ) )
  {
    if ( feedback->isCanceled() )
      return;

    // do not display twice the same feature
    if ( featuresFound.contains( f.id() ) )
      continue;

    QgsLocatorResult result;

    mContext.setFeature( f );

    // find matching field content
    int idx = 0;
    const QgsAttributes attributes = f.attributes();
    for ( const QVariant &var : attributes )
    {
      QString attrString = var.toString();
      if ( attrString.contains( searchString, Qt::CaseInsensitive ) )
      {
        if ( idx < mAttributeAliases.count() )
        {
          result.description = QStringLiteral( "%1 (%2)" ).arg( attrString, mAttributeAliases[idx] );
        }
        else
        {
          result.description = attrString;
        }
        break;
      }
      idx++;
    }
    if ( result.description.isEmpty() )
      continue; //not sure how this result slipped through...

    result.displayString = mDispExpression.evaluate( &mContext ).toString();
    result.group = mLayerName;
    result.setUserData( QVariantList() << f.id() << mLayerId );
    result.score = static_cast<double>( searchString.length() ) / result.displayString.size();
    result.actions << QgsLocatorResult::ResultAction( OpenForm, tr( "Open form" ), QStringLiteral( "qrc:/themes/qfield/nodpi/ic_baseline-list_white_24dp.svg?color=mainColor" ) );
    if ( mLayerIsSpatial )
    {
      result.actions << QgsLocatorResult::ResultAction( Navigation, tr( "Set feature as destination" ), QStringLiteral( "qrc:/themes/qfield/nodpi/ic_navigation_flag_purple_24dp.svg" ) );
    }

    emit resultFetched( result );

    featuresFound << f.id();
    if ( featuresFound.count() >= mMaxTotalResults )
      break;
  }
}

void ActiveLayerFeaturesLocatorFilter::triggerResult( const QgsLocatorResult &result )
{
  triggerResultFromAction( result, Normal );
}

void ActiveLayerFeaturesLocatorFilter::triggerResultFromAction( const QgsLocatorResult &result, const int actionId )
{
  const QVariantMap data = result.userData().toMap();
  switch ( data.value( QStringLiteral( "type" ) ).value<ResultType>() )
  {
    case ResultType::FieldRestriction:
    {
      QTimer::singleShot( 100, [this, data] { emit mLocatorBridge->requestSearchTextChange( data.value( "search_text" ).toString() ); } );
      break;
    }

    case ResultType::Feature:
    {
      const QVariantList dataList = result.userData().toList();
      const QgsFeatureId fid = dataList.at( 0 ).toLongLong();
      const QString layerId = dataList.at( 1 ).toString();
      QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( QgsProject::instance()->mapLayer( layerId ) );
      if ( !layer )
        return;

      const bool geometryless = layer->geometryType() == Qgis::GeometryType::Null || layer->geometryType() == Qgis::GeometryType::Unknown;
      QgsFeature feature;
      QgsFeatureRequest featureRequest = QgsFeatureRequest().setFilterFid( fid );

      if ( actionId == OpenForm || geometryless )
      {
        QMap<QgsVectorLayer *, QgsFeatureRequest> requests;
        requests.insert( layer, featureRequest );
        mLocatorBridge->featureListController()->selection()->setFocusedItem( -1 );
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
      else if ( actionId == Normal )
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
          if ( layer->geometryType() == Qgis::GeometryType::Point && r.isEmpty() )
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
            r.scale( 1.25 );
          }
        }

        if ( r.isEmpty() || mLocatorBridge->keepScale() )
        {
          emit mLocatorBridge->requestJumpToPoint( QgsPoint( r.center() ), -1, true );
        }
        else
        {
          const double scale = mLocatorBridge->mapSettings()->computeScaleForExtent( r, true );
          emit mLocatorBridge->requestJumpToPoint( QgsPoint( r.center() ), scale, true );
        }

        mLocatorBridge->geometryHighlighter()->setProperty( "qgsGeometry", geom );
        mLocatorBridge->geometryHighlighter()->setProperty( "crs", layer->crs() );
      }
      break;
    }
  }
}
```


