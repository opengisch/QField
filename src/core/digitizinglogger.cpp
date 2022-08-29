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

#include <QDateTime>
#include <qgsexpressioncontextutils.h>
#include <qgslayertree.h>
#include <qgsmessagelog.h>
#include <qgsvectorlayerutils.h>
#include <qgswkbtypes.h>

DigitizingLogger::DigitizingLogger()
{
}

void DigitizingLogger::setType( const QString &type )
{
  if ( mType == type )
    return;

  mType = type;

  emit typeChanged();
}

void DigitizingLogger::setPositionInformation( const GnssPositionInformation &positionInformation )
{
  if ( mPositionInformation == positionInformation )
    return;

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
  if ( mTopSnappingResult == topSnappingResult )
    return;

  mTopSnappingResult = topSnappingResult;

  emit topSnappingResultChanged();
}

void DigitizingLogger::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  if ( mProject )
  {
    disconnect( mProject, &QgsProject::readProject, this, &DigitizingLogger::findLogsLayer );
  }

  mProject = project;

  if ( mProject )
  {
    connect( mProject, &QgsProject::readProject, this, &DigitizingLogger::findLogsLayer );
  }

  clearCoordinates();
  findLogsLayer();

  emit projectChanged();
}

void DigitizingLogger::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  mMapSettings = mapSettings;

  emit mapSettingsChanged();
}

void DigitizingLogger::setCloudUserInformation( const CloudUserInformation &cloudUserInformation )
{
  mCloudUserInformation = cloudUserInformation;

  emit cloudUserInformationChanged();
}

void DigitizingLogger::setDigitizingLayer( QgsVectorLayer *layer )
{
  if ( mDigitizingLayer == layer )
    return;

  mDigitizingLayer = layer;

  emit digitizingLayerChanged();
}

void DigitizingLogger::findLogsLayer()
{
  mLogsLayer = nullptr;
  if ( mProject )
  {
    const QString logsLayerId = mProject->readEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "digitizingLogsLayer" ) );
    if ( !logsLayerId.isEmpty() )
    {
      QgsLayerTreeLayer *item = mProject->layerTreeRoot()->findLayer( logsLayerId );
      if ( item )
      {
        QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( item->layer() );
        if ( layer && layer->geometryType() == QgsWkbTypes::PointGeometry && layer->dataProvider() && layer->dataProvider()->capabilities() & QgsVectorDataProvider::AddFeatures )
        {
          mLogsLayer = layer;
        }
      }
    }
  }
}

void DigitizingLogger::addCoordinate( const QgsPoint &point )
{
  if ( !mLogsLayer || mType.isEmpty() )
    return;

  QgsFeature feature = QgsFeature( mLogsLayer->fields() );
  QgsGeometry geom( point.clone() );
  if ( mProject->crs() != mLogsLayer->crs() )
  {
    QgsCoordinateTransform ct( mProject->crs(), mLogsLayer->crs(), mProject->transformContext() );
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
  feature.setGeometry( geom.coerceToType( mLogsLayer->wkbType() ).at( 0 ) );

  QgsExpressionContext expressionContext = mLogsLayer->createExpressionContext();

  if ( mMapSettings )
    expressionContext << QgsExpressionContextUtils::mapSettingsScope( mMapSettings->mapSettings() );

  if ( mPositionInformation.isValid() )
    expressionContext << ExpressionContextUtils::positionScope( mPositionInformation, mPositionLocked );

  if ( mTopSnappingResult.isValid() )
    expressionContext << ExpressionContextUtils::mapToolCaptureScope( mTopSnappingResult );

  expressionContext << ExpressionContextUtils::cloudUserScope( mCloudUserInformation );

  QgsExpressionContextScope *scope = new QgsExpressionContextScope( QObject::tr( "Digitizing Logger" ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "digitizing_type" ), mType, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "digitizing_datetime" ), QDateTime::currentDateTime(), true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "digitizing_layer_name" ), mDigitizingLayer ? mDigitizingLayer->name() : QString(), true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "digitizing_layer_id" ), mDigitizingLayer ? mDigitizingLayer->id() : QString(), true, true ) );
  expressionContext << scope;

  expressionContext.setFeature( feature );

  const QgsFields fields = mLogsLayer->fields();
  for ( int i = 0; i < fields.count(); ++i )
  {
    if ( fields.at( i ).defaultValueDefinition().isValid() )
    {
      QgsExpression exp( fields.at( i ).defaultValueDefinition().expression() );
      exp.prepare( &expressionContext );
      if ( exp.hasParserError() )
        QgsMessageLog::logMessage( tr( "Default value expression for the digitizing logger's %2 field has a parser error: %3" ).arg( mLogsLayer->name(), fields.at( i ).name(), exp.parserErrorString() ), QStringLiteral( "QField" ) );

      QVariant value = exp.evaluate( &expressionContext );
      if ( exp.hasEvalError() )
        QgsMessageLog::logMessage( tr( "Default value expression for the digitizing logger's %2 field has an evaluation error: %3" ).arg( mLogsLayer->name(), fields.at( i ).name(), exp.evalErrorString() ), QStringLiteral( "QField" ) );

      feature.setAttribute( i, value );
    }
    else
    {
      feature.setAttribute( i, QVariant() );
    }
  }

  mPointFeatures << feature;
}

void DigitizingLogger::removeLastCoordinate()
{
  if ( !mPointFeatures.isEmpty() )
    mPointFeatures.removeLast();
}

void DigitizingLogger::writeCoordinates()
{
  if ( !mLogsLayer )
    return;

  if ( mLogsLayer->startEditing() )
  {
    for ( const auto &pointFeature : std::as_const( mPointFeatures ) )
    {
      QgsFeature createdFeature = QgsVectorLayerUtils::createFeature( mLogsLayer, pointFeature.geometry(), pointFeature.attributes().toMap() );
      if ( !mLogsLayer->addFeature( createdFeature ) )
      {
        QgsMessageLog::logMessage( tr( "Digitizing logs layer feature addition failed" ), QStringLiteral( "QField" ) );
      }
    }

    if ( !mLogsLayer->commitChanges( true ) )
    {
      QgsMessageLog::logMessage( tr( "Digitizing logs layer change commits failed" ), QStringLiteral( "QField" ) );
    }
    else
    {
      clearCoordinates();
    }
  }
  else
  {
    QgsMessageLog::logMessage( tr( "Digitizing logs layer editing failed" ), QStringLiteral( "QField" ) );
  }
}

void DigitizingLogger::clearCoordinates()
{
  mPointFeatures.clear();
}
