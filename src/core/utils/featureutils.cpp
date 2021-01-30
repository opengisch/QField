/***************************************************************************
  featureutils.cpp - FeatureUtils

 ---------------------
 begin                : 05.03.2020
 copyright            : (C) 2020 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "featureutils.h"

#include <qgsexpressioncontextutils.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include <qgssymbol.h>
#include <qgssymbollayer.h>
#include <qgsmarkersymbollayer.h>
#include <qgslinesymbollayer.h>
#include <qgsfillsymbollayer.h>
#include <qgswkbtypes.h>
#include "qgssinglesymbolrenderer.h"


FeatureUtils::FeatureUtils(QObject *parent) : QObject(parent)
{

}

QgsFeature FeatureUtils::initFeature(QgsVectorLayer *layer, QgsGeometry geometry)
{
  QgsFeature f(layer->fields());
  f.setGeometry(geometry);
  return f;
}

QString FeatureUtils::displayName( QgsVectorLayer *layer, const QgsFeature &feature )
{
  if ( !layer )
    return QString();

  QgsExpressionContext context = QgsExpressionContext()
                                 << QgsExpressionContextUtils::globalScope()
                                 << QgsExpressionContextUtils::projectScope( QgsProject::instance() )
                                 << QgsExpressionContextUtils::layerScope( layer );
  context.setFeature( feature );

  QString name = QgsExpression( layer->displayExpression() ).evaluate( &context ).toString();
  if ( name.isEmpty() )
    name = QString::number( feature.id() );

  return name;
}

QgsSymbol *FeatureUtils::defaultSymbol( QgsVectorLayer *layer )
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
      QgsSimpleFillSymbolLayer *symbolLayer = new QgsSimpleFillSymbolLayer( QColor( 255, 0, 0, 100), DEFAULT_SIMPLEFILL_STYLE, QColor( 255, 0, 0), DEFAULT_SIMPLEFILL_BORDERSTYLE, 0.6 );
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
