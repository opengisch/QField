

# File featureutils.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**featureutils.cpp**](featureutils_8cpp.md)

[Go to the documentation of this file](featureutils_8cpp.md)


```C++
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

#include "expressioncontextutils.h"
#include "featureutils.h"
#include "qgsquickmapsettings.h"

#include <qgsexpressioncontextutils.h>
#include <qgsjsonutils.h>
#include <qgsproject.h>
#include <qgsvariantutils.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayerutils.h>

FeatureUtils::FeatureUtils( QObject *parent )
  : QObject( parent )
{
}

QgsFeature FeatureUtils::createBlankFeature( const QgsFields &fields, const QgsGeometry &geometry )
{
  QgsFeature feature( fields );
  feature.setGeometry( geometry );
  return feature;
}

QgsFeature FeatureUtils::createFeature( QgsVectorLayer *layer, const QgsGeometry &geometry, const GnssPositionInformation &positionInformation, const CloudUserInformation &cloudUserInformation )
{
  if ( !layer )
    return QgsFeature();

  QgsFeature feature;
  QgsAttributeMap attributes;
  QgsExpressionContext context = layer->createExpressionContext();
  if ( positionInformation.isValid() )
  {
    context << ExpressionContextUtils::positionScope( positionInformation, false );
  }
  if ( !cloudUserInformation.isEmpty() )
  {
    context << ExpressionContextUtils::cloudUserScope( cloudUserInformation );
  }
  feature = QgsVectorLayerUtils::createFeature( layer, geometry, attributes, &context );
  return feature;
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

QgsRectangle FeatureUtils::extent( QgsQuickMapSettings *mapSettings, QgsVectorLayer *layer, const QgsFeature &feature )
{
  if ( mapSettings && layer && layer->geometryType() != Qgis::GeometryType::Unknown && layer->geometryType() != Qgis::GeometryType::Null )
  {
    QgsCoordinateTransform ct( layer->crs(), mapSettings->destinationCrs(), mapSettings->mapSettings().transformContext() );
    QgsGeometry geom( feature.geometry() );
    if ( !geom.isNull() )
    {
      geom.transform( ct );
      QgsRectangle extent;
      if ( geom.type() == Qgis::GeometryType::Point && geom.constGet()->partCount() == 1 )
      {
        extent = mapSettings->extent();
        QgsVector delta = QgsPointXY( geom.asPoint() ) - extent.center();
        const double deltaX = delta.x();
        const double deltaY = delta.y();
        extent.setXMinimum( extent.xMinimum() + deltaX );
        extent.setXMaximum( extent.xMaximum() + deltaX );
        extent.setYMinimum( extent.yMinimum() + deltaY );
        extent.setYMaximum( extent.yMaximum() + deltaY );
      }
      else
      {
        extent = geom.boundingBox();
        extent = extent.buffered( std::max( extent.width(), extent.height() ) / 6.0 );
      }

      return extent;
    }
  }

  return QgsRectangle();
}

QList<QgsFeature> FeatureUtils::featuresFromJsonString( const QString &string )
{
  const QgsFields fields = QgsJsonUtils::stringToFields( string );
  return QgsJsonUtils::stringToFeatureList( string, fields );
}

QgsField FeatureUtils::createField( const QString &name, FieldType type, const QString &typeName, int length, int precision, const QString &comment )
{
  return QgsField( name, static_cast<QMetaType::Type>( type ), typeName, length, precision, comment );
}

QgsFields FeatureUtils::createFields( const QList<QgsField> &fields )
{
  return QgsFields( fields );
}

bool FeatureUtils::attributeIsNull( const QVariant &value )
{
  return QgsVariantUtils::isUnsetAttributeValue( value ) || QgsVariantUtils::isNull( value );
}
```


