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
#include "qgsquickmapsettings.h"

#include <qgsexpressioncontextutils.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>


FeatureUtils::FeatureUtils( QObject *parent )
  : QObject( parent )
{
}

QgsFeature FeatureUtils::createFeature( QgsVectorLayer *layer, QgsGeometry geometry )
{
  QgsFeature f( layer ? layer->fields() : QgsFields() );
  f.setGeometry( geometry );
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

QgsRectangle FeatureUtils::extent( QgsQuickMapSettings *mapSettings, QgsVectorLayer *layer, const QgsFeature &feature, const double &rightEdge, const double &bottomEdge )
{
  if ( mapSettings && layer && layer->geometryType() != Qgis::GeometryType::Unknown && layer->geometryType() != Qgis::GeometryType::Null )
  {
    QgsCoordinateTransform transf( layer->crs(), mapSettings->destinationCrs(), mapSettings->mapSettings().transformContext() );
    QgsGeometry geom( feature.geometry() );
    if ( !geom.isNull() )
    {
      geom.transform( transf );

      QgsRectangle extent;
      QSizeF outputSize = mapSettings->outputSize() / mapSettings->devicePixelRatio();
      const double rightPercentage = rightEdge / outputSize.width();
      const double bottomPercentage = bottomEdge / outputSize.height();
      if ( geom.type() == Qgis::GeometryType::Point )
      {
        extent = mapSettings->extent();
        QgsVector delta = QgsPointXY( geom.asPoint() ) - extent.center();
        const double deltaX = delta.x() + ( rightEdge > 0.0 ? mapSettings->mapUnitsPerPoint() * outputSize.width() * ( 0.5 - rightPercentage / 2.0 ) : 0.0 );
        const double deltaY = delta.y() - ( bottomEdge > 0.0 ? mapSettings->mapUnitsPerPoint() * outputSize.height() * ( 0.5 - ( 1.0 - bottomPercentage ) / 2.0 ) : 0.0 );
        extent.setXMinimum( extent.xMinimum() + deltaX );
        extent.setXMaximum( extent.xMaximum() + deltaX );
        extent.setYMinimum( extent.yMinimum() + deltaY );
        extent.setYMaximum( extent.yMaximum() + deltaY );
      }
      else
      {
        extent = geom.boundingBox();
        extent = extent.buffered( std::max( extent.width(), extent.height() ) / 6.0 );

        if ( rightEdge > 0.0 )
        {
          extent.setXMaximum( extent.xMaximum() + ( extent.xMaximum() - extent.xMinimum() ) * rightPercentage );
        }
        if ( bottomEdge > 0.0 )
        {
          extent.setYMinimum( extent.yMinimum() - ( extent.yMaximum() - extent.yMinimum() ) * bottomPercentage * 2 );
        }
      }

      return extent;
    }
  }

  return QgsRectangle();
}
