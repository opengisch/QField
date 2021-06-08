/***************************************************************************
 digitizinglogger.cpp - DigitizingLogger
  ---------------------
 begin                : 7.6.2021
 copyright            : (C) 2021 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "digitizinglogger.h"
#include "expressioncontextutils.h"

#include <qgslayertree.h>
#include <qgsmessagelog.h>
#include <qgsvectorlayerutils.h>
#include <qgswkbtypes.h>

DigitizingLogger::DigitizingLogger()
{
}

void DigitizingLogger::setCategory( const QString &category )
{
  if ( mCategory == category )
    return;

  mCategory = category;

  emit categoryChanged();
}

void DigitizingLogger::setPositionInformation( const GnssPositionInformation &positionInformation )
{
  mPositionInformation = positionInformation;

  emit positionInformationChanged();
}

void DigitizingLogger::setPositionLocked( bool positionLocked )
{
  if ( mPositionLocked == positionLocked )
    return;

  mPositionLocked = positionLocked;

  emit positionLockedChanged();
}

void DigitizingLogger::setTopSnappingResult( const SnappingResult &topSnappingResult )
{
  mTopSnappingResult = topSnappingResult;

  emit topSnappingResultChanged();
}

void DigitizingLogger::setProject( QgsProject *project )
{
  if ( project == mProject )
    return;

  if ( mProject )
  {
    disconnect( mProject, &QgsProject::readProject, this, &DigitizingLogger::findLogsLayer );
  }

  mProject = project;
  connect( mProject, &QgsProject::readProject, this, &DigitizingLogger::findLogsLayer );

  clearCoordinates();
  findLogsLayer();

  emit projectChanged();
}

void DigitizingLogger::findLogsLayer()
{
  mLayer = nullptr;
  if ( mProject && mProject->layerTreeRoot() )
  {
    const QList< QgsLayerTreeLayer * > items = mProject->layerTreeRoot()->findLayers();
    for ( const auto *item : items )
    {
      QgsVectorLayer *layer = qobject_cast< QgsVectorLayer * >( item->layer() );
      if ( layer && layer->geometryType() == QgsWkbTypes::PointGeometry && item->layer()->customProperty( QStringLiteral( "digitizingLogsLayer" ), false ).toBool() )
      {
        if ( layer->dataProvider() && layer->dataProvider()->capabilities() & QgsVectorDataProvider::AddFeatures )
        {
          mLayer = layer;
        }
        break;
      }
    }
  }
}

void DigitizingLogger::addCoordinate( const QgsPoint &point )
{
  if ( !mLayer )
    return;

  QgsFeature feature = QgsFeature( mLayer->fields() );
  QgsGeometry geom( point.clone() );
  if ( mProject->crs() != mLayer->crs() )
  {
    QgsCoordinateTransform ct( mProject->crs(), mLayer->crs(), mProject->transformContext() );
    try
    {
      geom.transform( ct );
    }
    catch ( QgsCsException & )
    {
      QgsDebugMsg( "Could not transform current coordinate" );
      return;
    }
  }
  feature.setGeometry( geom );

  QgsExpressionContext expressionContext = mLayer->createExpressionContext();

  if ( mPositionInformation.isValid() )
    expressionContext << ExpressionContextUtils::positionScope( mPositionInformation, mPositionLocked );

  if ( mTopSnappingResult.isValid() )
    expressionContext << ExpressionContextUtils::mapToolCaptureScope( mTopSnappingResult );

  expressionContext << ExpressionContextUtils::cloudUserScope( mCloudUserInformation );

  QgsExpressionContextScope *scope = new QgsExpressionContextScope( QObject::tr( "Digitizing Logger" ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "digitizing_logger_category" ), mCategory, true, true ) );
  expressionContext << scope;

  expressionContext.setFeature( feature );

  const QgsFields fields = mLayer->fields();
  for ( int i = 0; i < fields.count(); ++i )
  {
    if ( fields.at( i ).defaultValueDefinition().isValid() )
    {
      QgsExpression exp( fields.at( i ).defaultValueDefinition().expression() );
      exp.prepare( &expressionContext );
      if ( exp.hasParserError() )
        QgsMessageLog::logMessage( tr( "Default value expression for the digitizing logger's %2 field has a parser error: %3" ).arg( mLayer->name(), fields.at( i ).name(), exp.parserErrorString() ), QStringLiteral( "QField" ) );

      QVariant value = exp.evaluate( &expressionContext );
      if ( exp.hasEvalError() )
        QgsMessageLog::logMessage( tr( "Default value expression for the digitizing logger's %2 field has an evaluation error: %3" ).arg( mLayer->name(), fields.at( i ).name(), exp.evalErrorString() ), QStringLiteral( "QField" ) );

      feature.setAttribute( i, value );
    }
    else
    {
      feature.setAttribute( i, QVariant() );
    }
  }

  qDebug() << "ADD";
  mPointFeatures << feature;
  qDebug() << mPointFeatures.size();
}

void DigitizingLogger::writeCoordinates()
{
  if ( !mLayer )
    return;

  qDebug() << "WRITE";
  if ( mLayer->startEditing() )
  {
    for ( const auto &pointFeature : std::as_const( mPointFeatures ) )
    {
      QgsFeature createdFeature = QgsVectorLayerUtils::createFeature( mLayer, pointFeature.geometry(), pointFeature.attributes().toMap() );
      qDebug() << "XXX";
      if ( !mLayer->addFeature( createdFeature ) )
      {
        qDebug() << "add error";
      }
    }

    if ( !mLayer->commitChanges( true ) )
    {
      qDebug() << "commit error";
    }
  }
  else
  {
    qDebug() << "start editing error";
  }
}

void DigitizingLogger::clearCoordinates()
{
  mPointFeatures.clear();
}
