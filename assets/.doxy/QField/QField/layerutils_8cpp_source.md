

# File layerutils.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**layerutils.cpp**](layerutils_8cpp.md)

[Go to the documentation of this file](layerutils_8cpp.md)


```C++
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

#include <QQmlEngine>
#include <QScopeGuard>
#include <qgsfillsymbol.h>
#include <qgsfillsymbollayer.h>
#include <qgshuesaturationfilter.h>
#include <qgsjsonutils.h>
#include <qgslabelobstaclesettings.h>
#include <qgslayoutatlas.h>
#include <qgslayoutmanager.h>
#include <qgslinesymbol.h>
#include <qgslinesymbollayer.h>
#include <qgsmaplayerelevationproperties.h>
#include <qgsmarkersymbol.h>
#include <qgsmarkersymbollayer.h>
#include <qgsmemoryproviderutils.h>
#include <qgsmessagelog.h>
#include <qgspallabeling.h>
#include <qgsprintlayout.h>
#include <qgsproject.h>
#include <qgsprojectstylesettings.h>
#include <qgsrasterlayer.h>
#include <qgsrasterlayerelevationproperties.h>
#include <qgssinglesymbolrenderer.h>
#include <qgssymbol.h>
#include <qgssymbollayer.h>
#include <qgstextbuffersettings.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayerlabeling.h>
#include <qgsvectorlayerutils.h>
#include <qgswkbtypes.h>

LayerUtils::LayerUtils( QObject *parent )
  : QObject( parent )
{
}

void LayerUtils::setDefaultRenderer( QgsVectorLayer *layer, QgsProject *project, const QString &attachmentField, const QString &colorField )
{
  if ( !layer )
    return;

  bool hasSymbol = true;
  Qgis::SymbolType symbolType = Qgis::SymbolType::Marker;
  switch ( layer->geometryType() )
  {
    case Qgis::GeometryType::Point:
      symbolType = Qgis::SymbolType::Marker;
      break;
    case Qgis::GeometryType::Line:
      symbolType = Qgis::SymbolType::Line;
      break;
    case Qgis::GeometryType::Polygon:
      symbolType = Qgis::SymbolType::Fill;
      break;
    case Qgis::GeometryType::Unknown:
    case Qgis::GeometryType::Null:
    default:
      hasSymbol = false;
      break;
  }

  if ( !hasSymbol )
  {
    return;
  }

  QgsSymbol *symbol = project ? project->styleSettings()->defaultSymbol( symbolType ) : nullptr;
  if ( !symbol )
  {
    symbol = LayerUtils::defaultSymbol( layer, attachmentField, colorField );
  }

  QgsSingleSymbolRenderer *renderer = new QgsSingleSymbolRenderer( symbol );
  layer->setRenderer( renderer );
}

QgsSymbol *LayerUtils::defaultSymbol( QgsVectorLayer *layer, const QString &attachmentField, const QString &colorField )
{
  QgsSymbol *symbol = nullptr;

  if ( !layer )
  {
    return symbol;
  }

  QgsSymbolLayerList symbolLayers;
  switch ( layer->geometryType() )
  {
    case Qgis::GeometryType::Point:
    {
      if ( !attachmentField.isEmpty() )
      {
        QgsSymbolLayerList subSymbolLayers;
        QgsRasterMarkerSymbolLayer *rasterMarkerSymbolLayer = new QgsRasterMarkerSymbolLayer( QString(), 2.6, 0.0 );
        rasterMarkerSymbolLayer->setSize( 6.0 );
        rasterMarkerSymbolLayer->setDataDefinedProperty( QgsSymbolLayer::Property::Size, QgsProperty::fromExpression( QStringLiteral( "scale_linear( @map_scale, 1000, 5000, @value * 5.5, @value )" ), true ) );
        rasterMarkerSymbolLayer->setDataDefinedProperty( QgsSymbolLayer::Property::Name, QgsProperty::fromExpression( QStringLiteral( "if(@map_scale < 5000, @project_folder || '/' || \"%1\", '')" ).arg( attachmentField ), true ) );
        subSymbolLayers << rasterMarkerSymbolLayer;

        QgsCentroidFillSymbolLayer *centroidFillSymbolLayer = new QgsCentroidFillSymbolLayer();
        centroidFillSymbolLayer->setClipPoints( true );
        centroidFillSymbolLayer->setSubSymbol( new QgsMarkerSymbol( subSymbolLayers ) );
        subSymbolLayers.clear();
        subSymbolLayers << centroidFillSymbolLayer;

        QgsFilledMarkerSymbolLayer *fillSymbolLayer = new QgsFilledMarkerSymbolLayer( Qgis::MarkerShape::Circle, 2.6, 0.0 );
        fillSymbolLayer->setSize( 2.4 );
        if ( !colorField.isEmpty() )
        {
          fillSymbolLayer->setDataDefinedProperty( QgsSymbolLayer::Property::StrokeColor, QgsProperty::fromExpression( QStringLiteral( "if(\"%1\" is not null and \"%1\" != '', \"%1\", @value)" ).arg( colorField ), true ) );
        }
        fillSymbolLayer->setDataDefinedProperty( QgsSymbolLayer::Property::Size, QgsProperty::fromExpression( QStringLiteral( "if(@map_scale < 5000 and \"%1\" is not null and \"%1\" != '', scale_linear( @map_scale, 1000, 5000, @value * 5.5, @value ), @value)" ).arg( attachmentField ), true ) );
        fillSymbolLayer->setSubSymbol( new QgsFillSymbol( subSymbolLayers ) );
        symbolLayers << fillSymbolLayer;

        QgsSimpleMarkerSymbolLayer *symbolLayer = new QgsSimpleMarkerSymbolLayer( Qgis::MarkerShape::Circle, 2.6, 0.0, DEFAULT_SCALE_METHOD, QColor( 55, 126, 184, 100 ), QColor( 55, 126, 184 ) );
        symbolLayer->setSize( 2.4 );
        symbolLayer->setStrokeWidth( 0.6 );
        if ( !colorField.isEmpty() )
        {
          symbolLayer->setDataDefinedProperty( QgsSymbolLayer::Property::FillColor, QgsProperty::fromExpression( QStringLiteral( "if(@map_scale < 5000 and \"%1\" is not null and \"%1\" != '', '255,0,0,0', if(\"%2\" is not null and \"%2\" != '', set_color_part(\"%2\", 'alpha', 100), @value))" ).arg( attachmentField, colorField ), true ) );
          symbolLayer->setDataDefinedProperty( QgsSymbolLayer::Property::StrokeColor, QgsProperty::fromExpression( QStringLiteral( "if(\"%1\" is not null and \"%1\" != '', \"%1\", @value)" ).arg( colorField ), true ) );
        }
        else
        {
          symbolLayer->setDataDefinedProperty( QgsSymbolLayer::Property::FillColor, QgsProperty::fromExpression( QStringLiteral( "if(@map_scale < 5000 and \"%1\" is not null and \"%1\" != '', '255,0,0,0', @value))" ).arg( attachmentField ), true ) );
        }
        symbolLayer->setDataDefinedProperty( QgsSymbolLayer::Property::Size, QgsProperty::fromExpression( QStringLiteral( "if(@map_scale < 5000 and \"%1\" is not null and \"%1\" != '', scale_linear( @map_scale, 1000, 5000, @value * 5.5, @value ), @value)" ).arg( attachmentField ), true ) );
        symbolLayers << symbolLayer;
      }
      else
      {
        QgsSimpleMarkerSymbolLayer *symbolLayer = new QgsSimpleMarkerSymbolLayer( Qgis::MarkerShape::Circle, 2.6, 0.0, DEFAULT_SCALE_METHOD, QColor( 55, 126, 184, 100 ), QColor( 55, 126, 184 ) );
        symbolLayer->setStrokeWidth( 0.6 );
        if ( !colorField.isEmpty() )
        {
          symbolLayer->setDataDefinedProperty( QgsSymbolLayer::Property::FillColor, QgsProperty::fromExpression( QStringLiteral( "if(\"%1\" is not null and \"%1\" != '', set_color_part(\"%1\", 'alpha', 100), @value)" ).arg( colorField ), true ) );
        }
        symbolLayer->setDataDefinedProperty( QgsSymbolLayer::Property::StrokeColor, QgsProperty::fromExpression( QStringLiteral( "if(\"%1\" is not null and \"%1\" != '', \"%1\", @value)" ).arg( colorField ), true ) );
        symbolLayers << symbolLayer;
      }
      symbol = new QgsMarkerSymbol( symbolLayers );
      break;
    }

    case Qgis::GeometryType::Line:
    {
      QgsSimpleLineSymbolLayer *symbolLayer = new QgsSimpleLineSymbolLayer( QColor( 55, 126, 184 ), 0.6 ); // cppcheck-suppress constVariablePointer
      if ( !colorField.isEmpty() )
      {
        symbolLayer->setDataDefinedProperty( QgsSymbolLayer::Property::StrokeColor, QgsProperty::fromExpression( QStringLiteral( "if(\"%1\" is not null and \"%1\" != '', \"%1\", @value)" ).arg( colorField ), true ) );
      }
      symbolLayers << symbolLayer;
      symbol = new QgsLineSymbol( symbolLayers );
      break;
    }

    case Qgis::GeometryType::Polygon:
    {
      QgsSimpleFillSymbolLayer *symbolLayer = new QgsSimpleFillSymbolLayer( QColor( 55, 126, 184, 100 ), DEFAULT_SIMPLEFILL_STYLE, QColor( 55, 126, 184 ), DEFAULT_SIMPLEFILL_BORDERSTYLE, 0.6 ); // cppcheck-suppress constVariablePointer
      symbolLayers << symbolLayer;
      symbol = new QgsFillSymbol( symbolLayers );
      break;
    }

    case Qgis::GeometryType::Unknown:
    case Qgis::GeometryType::Null:
      break;
  }
  return symbol;
}

void LayerUtils::setDefaultLabeling( QgsVectorLayer *layer, QgsProject *project )
{
  QgsTextFormat textFormat = project ? project->styleSettings()->defaultTextFormat() : QgsTextFormat();
  textFormat.setSize( 8 );
  textFormat.setSizeUnit( Qgis::RenderUnit::Points );
  textFormat.buffer().setEnabled( true );
  textFormat.buffer().setSize( 0.5 );
  textFormat.buffer().setSizeUnit( Qgis::RenderUnit::Millimeters );
  textFormat.buffer().setColor( QColor( 255, 255, 255, 150 ) );
  QgsAbstractVectorLayerLabeling *labeling = LayerUtils::defaultLabeling( layer, textFormat );
  if ( labeling )
  {
    layer->setLabeling( labeling );
    layer->setLabelsEnabled( layer->geometryType() == Qgis::GeometryType::Point );
  }
}

QgsAbstractVectorLayerLabeling *LayerUtils::defaultLabeling( QgsVectorLayer *layer, QgsTextFormat textFormat )
{
  QgsAbstractVectorLayerLabeling *labeling = nullptr;

  if ( !layer )
  {
    return labeling;
  }

  bool foundFriendlyIdentifier = true;
  QString fieldName = QgsVectorLayerUtils::guessFriendlyIdentifierField( layer->fields(), &foundFriendlyIdentifier );
  if ( !foundFriendlyIdentifier )
  {
    return labeling;
  }

  QgsPalLayerSettings settings;
  settings.fieldName = fieldName;
  settings.obstacleSettings().setIsObstacle( true );
  switch ( layer->geometryType() )
  {
    case Qgis::GeometryType::Point:
    {
      settings.placement = Qgis::LabelPlacement::OrderedPositionsAroundPoint;
      settings.offsetType = Qgis::LabelOffsetType::FromSymbolBounds;
      break;
    }

    case Qgis::GeometryType::Line:
    {
      settings.placement = Qgis::LabelPlacement::Curved;
      break;
    }

    case Qgis::GeometryType::Polygon:
    {
      settings.placement = Qgis::LabelPlacement::AroundPoint;
      settings.obstacleSettings().setType( QgsLabelObstacleSettings::ObstacleType::PolygonBoundary );
      break;
    }

    case Qgis::GeometryType::Unknown:
    case Qgis::GeometryType::Null:
      break;
  }

  if ( !textFormat.isValid() )
  {
    textFormat.setSize( 9 );
    textFormat.setSizeUnit( Qgis::RenderUnit::Points );
    textFormat.setColor( QColor( 0, 0, 0 ) );

    QgsTextBufferSettings bufferSettings;
    bufferSettings.setEnabled( true );
    bufferSettings.setColor( QColor( 255, 255, 255 ) );
    bufferSettings.setSize( 1 );
    bufferSettings.setSizeUnit( Qgis::RenderUnit::Millimeters );
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
  QgsRasterLayerElevationProperties *elevationProperties = static_cast<QgsRasterLayerElevationProperties *>( layer->elevationProperties() );
  elevationProperties->setEnabled( true );
  elevationProperties->setProfileSymbology( Qgis::ProfileSurfaceSymbology::FillBelow );
  elevationProperties->profileFillSymbol()->setColor( QColor( 130, 130, 130 ) );
  return layer;
}

QgsMapLayer *LayerUtils::createBasemap( const QString &style )
{
  QgsRasterLayer *layer = nullptr;
  if ( style.compare( QStringLiteral( "lightgray" ) ) == 0 )
  {
    layer = new QgsRasterLayer( OPENSTREETMAP_URL, QStringLiteral( "OpenStreetMap" ), QLatin1String( "wms" ) );
    layer->hueSaturationFilter()->setGrayscaleMode( QgsHueSaturationFilter::GrayscaleLightness );
  }
  else if ( style.compare( QStringLiteral( "darkgray" ) ) == 0 )
  {
    layer = new QgsRasterLayer( OPENSTREETMAP_URL, QStringLiteral( "OpenStreetMap" ), QLatin1String( "wms" ) );
    layer->hueSaturationFilter()->setGrayscaleMode( QgsHueSaturationFilter::GrayscaleLightness );
    layer->hueSaturationFilter()->setInvertColors( true );
  }
  else
  {
    layer = new QgsRasterLayer( OPENSTREETMAP_URL, QStringLiteral( "OpenStreetMap" ), QLatin1String( "wms" ) );
  }
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

bool LayerUtils::isFeatureAdditionLocked( QgsMapLayer *layer )
{
  return layer ? ( ( layer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), false ).toBool() && !layer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), false ).toBool() ) || ( layer->customProperty( QStringLiteral( "QFieldSync/is_feature_addition_locked" ), false ).toBool() && !layer->customProperty( QStringLiteral( "QFieldSync/is_feature_addition_locked" ), false ).toBool() ) ) : false;
}

void LayerUtils::selectFeaturesInLayer( QgsVectorLayer *layer, const QList<int> &fids, Qgis::SelectBehavior behavior )
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
  return QVariant( QMetaType( field.type() ) ).typeName();
}

bool LayerUtils::addFeature( QgsVectorLayer *layer, QgsFeature feature )
{
  if ( layer )
  {
    return layer->addFeature( feature );
  }
  return false;
}

bool LayerUtils::deleteFeature( QgsProject *project, QgsVectorLayer *layer, const QgsFeatureId fid, bool flushBuffer )
{
  if ( !project )
  {
    return false;
  }

  if ( !layer )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing, no layer" ), "QField", Qgis::Warning );
    return false;
  }

  const bool wasEditing = layer->editBuffer();
  if ( !wasEditing && !layer->startEditing() )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing" ), "QField", Qgis::Warning );
    return false;
  }
  flushBuffer = flushBuffer || !wasEditing;

  bool isSuccess = true;

  // delete parent and related features
  QgsVectorLayer::DeleteContext deleteContext( true, project );
  if ( layer->deleteFeature( fid, &deleteContext ) )
  {
    if ( flushBuffer )
    {
      // commit changes
      if ( !layer->commitChanges( !wasEditing ) )
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
        {
          continue;
        }

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

  if ( !flushBuffer && !isSuccess )
  {
    const QList<QgsVectorLayer *> constHandledLayers = deleteContext.handledLayers();
    for ( QgsVectorLayer *vl : constHandledLayers )
      if ( vl != layer )
      {
        if ( !vl->rollBack() )
        {
          QgsMessageLog::logMessage( tr( "Cannot rollback layer changes in layer %1" ).arg( vl->name() ), "QField", Qgis::Critical );
        }
      }

    if ( !layer->rollBack() )
    {
      QgsMessageLog::logMessage( tr( "Cannot rollback layer changes in layer %1" ).arg( layer->name() ), "QField", Qgis::Critical );
    }
  }

  return isSuccess;
}

QgsFeature LayerUtils::duplicateFeature( QgsVectorLayer *layer, QgsFeature feature )
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

  const bool wasEditing = layer->editBuffer();
  if ( !wasEditing && !layer->startEditing() )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing" ), "QField", Qgis::Warning );
    return QgsFeature();
  }

  // When duplicating a feature, insure the source primary ID is correctly set to null (i.e. new feature within the source dataset)
  QString sourcePrimaryKeys = layer->customProperty( QStringLiteral( "QFieldSync/sourceDataPrimaryKeys" ) ).toString();
  if ( layer->fields().lookupField( sourcePrimaryKeys ) >= 0 )
  {
    const int sourcePrimaryKeysIndex = layer->fields().lookupField( sourcePrimaryKeys );
    if ( !layer->fields().at( sourcePrimaryKeysIndex ).defaultValueDefinition().isValid() )
    {
      feature.setAttribute( sourcePrimaryKeysIndex, QVariant() );
    }
  }

  QgsFeature duplicatedFeature;
  QMetaObject::Connection connection = connect( layer, &QgsVectorLayer::featureAdded, [layer, &duplicatedFeature]( QgsFeatureId fid ) {
    duplicatedFeature = layer->getFeature( fid );
  } );
  auto sweaper = qScopeGuard( [layer, connection] { layer->disconnect( connection ); } );

  QgsVectorLayerUtils::QgsDuplicateFeatureContext duplicateFeatureContext;
  duplicatedFeature = QgsVectorLayerUtils::duplicateFeature( layer, feature, QgsProject::instance(), duplicateFeatureContext );
  const auto duplicateFeatureContextLayers = duplicateFeatureContext.layers();

  // commit changes
  if ( !layer->commitChanges( !wasEditing ) )
  {
    const QString msgs = layer->commitErrors().join( QStringLiteral( "\n" ) );
    QgsMessageLog::logMessage( tr( "Cannot add new feature in layer \"%1\". Reason:\n%2" ).arg( layer->name(), msgs ), "QField", Qgis::Warning );

    for ( QgsVectorLayer *chl : duplicateFeatureContextLayers )
    {
      chl->rollBack();
    }

    return QgsFeature();
  }

  // we have to re-apply referenced feature attribute value to take into account value changes form the
  // data provider (e.g. autogenerated fid)
  const QList<QgsRelation> relations = QgsProject::instance()->relationManager()->referencedRelations( layer );
  for ( QgsVectorLayer *chl : duplicateFeatureContextLayers )
  {
    const QgsFeatureIds fids = duplicateFeatureContext.duplicatedFeatures( chl );
    for ( const auto &relation : relations )
    {
      if ( relation.referencingLayer() == chl )
      {
        const QList<QgsRelation::FieldPair> fieldPairs = relation.fieldPairs();
        for ( const auto &fieldPair : fieldPairs )
        {
          for ( auto fid : fids )
          {
            chl->changeAttributeValue( fid, chl->fields().indexOf( fieldPair.referencingField() ), duplicatedFeature.attribute( fieldPair.referencedField() ) );
          }
        }
      }
    }

    // Insure the source primary ID of a duplicated child feature is correctly set to null (i.e. new feature within the source dataset)
    sourcePrimaryKeys = chl->customProperty( QStringLiteral( "QFieldSync/sourceDataPrimaryKeys" ) ).toString();
    if ( !sourcePrimaryKeys.isEmpty() && chl->fields().lookupField( sourcePrimaryKeys ) >= 0 )
    {
      const int sourcePrimaryKeysIndex = chl->fields().lookupField( sourcePrimaryKeys );
      if ( !chl->fields().at( sourcePrimaryKeysIndex ).defaultValueDefinition().isValid() )
      {
        for ( auto fid : fids )
        {
          chl->changeAttributeValue( fid, sourcePrimaryKeysIndex, QVariant() );
        }
      }
    }

    chl->commitChanges();
  }

  return duplicatedFeature;
}

bool LayerUtils::hasMValue( QgsVectorLayer *layer )
{
  if ( !layer )
    return false;

  return QgsWkbTypes::hasM( layer->wkbType() );
}

QgsVectorLayer *LayerUtils::loadVectorLayer( const QString &uri, const QString &name, const QString &provider )
{
  QgsVectorLayer *layer = new QgsVectorLayer( uri, name, provider );
  QQmlEngine::setObjectOwnership( layer, QQmlEngine::CppOwnership );
  return layer;
}

QgsRasterLayer *LayerUtils::loadRasterLayer( const QString &uri, const QString &name, const QString &provider )
{
  QgsRasterLayer *layer = new QgsRasterLayer( uri, name, provider );
  QQmlEngine::setObjectOwnership( layer, QQmlEngine::CppOwnership );
  return layer;
}

QgsVectorLayer *LayerUtils::memoryLayerFromJsonString( const QString &name, const QString &string, const QgsCoordinateReferenceSystem &crs )
{
  const QgsFields fields = QgsJsonUtils::stringToFields( string );
  QgsFeatureList features = QgsJsonUtils::stringToFeatureList( string, fields );
  if ( features.isEmpty() )
  {
    return nullptr;
  }

  QgsVectorLayer *layer = LayerUtils::createMemoryLayer( name, fields, features[0].geometry().wkbType(), crs );
  if ( QgsVectorDataProvider *dataProvider = layer->dataProvider() )
  {
    dataProvider->addFeatures( features );
  }
  return layer;
}

QgsVectorLayer *LayerUtils::createMemoryLayer( const QString &name, const QgsFields &fields, Qgis::WkbType geometryType, const QgsCoordinateReferenceSystem &crs )
{
  QgsVectorLayer *layer = QgsMemoryProviderUtils::createMemoryLayer( name, fields, geometryType, crs );
  QQmlEngine::setObjectOwnership( layer, QQmlEngine::CppOwnership );
  LayerUtils::setDefaultRenderer( layer );
  return layer;
}

FeatureIterator LayerUtils::createFeatureIteratorFromExpression( QgsVectorLayer *layer, const QString &expression )
{
  const QgsFeatureRequest request = QgsFeatureRequest( QgsExpression( expression ) );
  return FeatureIterator( layer, request );
}

FeatureIterator LayerUtils::createFeatureIteratorFromRectangle( QgsVectorLayer *layer, const QgsRectangle &rectangle )
{
  const QgsFeatureRequest request = QgsFeatureRequest( rectangle );
  return FeatureIterator( layer, request );
}
```


