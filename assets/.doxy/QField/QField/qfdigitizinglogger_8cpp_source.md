

# File qfdigitizinglogger.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfdigitizinglogger.cpp**](qfdigitizinglogger_8cpp.md)

[Go to the documentation of this file](qfdigitizinglogger_8cpp.md)


```C++
/***************************************************************************
 qfdigitizinglogger.cpp - QfDigitizingLogger
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

#include "qfdigitizinglogger.h"
#include "qfexpressioncontextutils.h"

#include <QDateTime>
#include <qgsexpressioncontextutils.h>
#include <qgslayertree.h>
#include <qgsmessagelog.h>
#include <qgsvectorlayerutils.h>
#include <qgswkbtypes.h>

QfDigitizingLogger::QfDigitizingLogger()
{
}

void QfDigitizingLogger::setType( const QString &type )
{
  if ( mType == type )
    return;

  mType = type;

  emit typeChanged();
}

void QfDigitizingLogger::setPositionInformation( const QfGnssPositionInformation &positionInformation )
{
  if ( mPositionInformation == positionInformation )
    return;

  mPositionInformation = positionInformation;

  emit positionInformationChanged();
}

void QfDigitizingLogger::setPositionLocked( bool positionLocked )
{
  if ( mPositionLocked == positionLocked )
    return;

  mPositionLocked = positionLocked;

  emit positionLockedChanged();
}

void QfDigitizingLogger::setTopSnappingResult( const QfSnappingResult &topSnappingResult )
{
  if ( mTopSnappingResult == topSnappingResult )
    return;

  mTopSnappingResult = topSnappingResult;

  emit topSnappingResultChanged();
}

void QfDigitizingLogger::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  if ( mProject )
  {
    disconnect( mProject, &QgsProject::readProject, this, &QfDigitizingLogger::findLogsLayer );
  }

  mProject = project;

  if ( mProject )
  {
    connect( mProject, &QgsProject::readProject, this, &QfDigitizingLogger::findLogsLayer );
  }

  clearCoordinates();
  findLogsLayer();

  emit projectChanged();
}

void QfDigitizingLogger::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  mMapSettings = mapSettings;

  emit mapSettingsChanged();
}

void QfDigitizingLogger::setCloudUserInformation( const QfCloudUserInformation &cloudUserInformation )
{
  mCloudUserInformation = cloudUserInformation;

  emit cloudUserInformationChanged();
}

void QfDigitizingLogger::setDigitizingLayer( QgsVectorLayer *layer )
{
  if ( mDigitizingLayer == layer )
    return;

  mDigitizingLayer = layer;

  emit digitizingLayerChanged();
}

void QfDigitizingLogger::findLogsLayer()
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
        if ( layer && layer->geometryType() == Qgis::GeometryType::Point && layer->dataProvider() && layer->dataProvider()->capabilities() & Qgis::VectorProviderCapability::AddFeatures )
        {
          mLogsLayer = layer;
        }
      }
    }
  }
}

void QfDigitizingLogger::addCoordinate( const QgsPoint &point )
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
      QgsDebugMsgLevel( "Could not transform current coordinate", 2 );
      return;
    }
  }
  feature.setGeometry( geom.coerceToType( mLogsLayer->wkbType() ).at( 0 ) );

  QgsExpressionContext expressionContext = mLogsLayer->createExpressionContext();

  if ( mMapSettings )
    expressionContext << QgsExpressionContextUtils::mapSettingsScope( mMapSettings->mapSettings() );

  if ( mPositionInformation.isValid() )
    expressionContext << QfExpressionContextUtils::positionScope( mPositionInformation, mPositionLocked );

  if ( mTopSnappingResult.isValid() )
    expressionContext << QfExpressionContextUtils::mapToolCaptureScope( mTopSnappingResult );

  expressionContext << QfExpressionContextUtils::cloudUserScope( mCloudUserInformation );

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

void QfDigitizingLogger::removeLastCoordinate()
{
  if ( !mPointFeatures.isEmpty() )
    mPointFeatures.removeLast();
}

void QfDigitizingLogger::writeCoordinates()
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

void QfDigitizingLogger::clearCoordinates()
{
  mPointFeatures.clear();
}
```


