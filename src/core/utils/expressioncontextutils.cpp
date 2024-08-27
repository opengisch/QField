/***************************************************************************
                            expressioncontextutils.cpp
                              -------------------
              begin                : 5.12.2017
              copyright            : (C) 2017 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "expressioncontextutils.h"

#include <qgsapplication.h>
#include <qgsgeometry.h>
#include <qgsmaplayer.h>
#include <qgsproject.h>


void addPositionVariable( QgsExpressionContextScope *scope, const QString &name, const QVariant &value, bool positionLocked, const QVariant &defaultValue = QVariant() )
{
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "gnss_%1" ).arg( name ), value, true, true ) );
  if ( positionLocked )
    scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_%1" ).arg( name ), value, true, true ) );
  else
    scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_%1" ).arg( name ), defaultValue, true, true ) );
}

ExpressionContextUtils::ExpressionContextUtils( QObject *parent )
  : QObject( parent )
{
}

QgsExpressionContextScope *ExpressionContextUtils::positionScope( const GnssPositionInformation &positionInformation, bool positionLocked )
{
  QgsExpressionContextScope *scope = new QgsExpressionContextScope( QObject::tr( "Position" ) );

  const QgsGeometry point = QgsGeometry( new QgsPoint( positionInformation.longitude(), positionInformation.latitude(), positionInformation.elevation() ) );
  const QDateTime timestamp = positionInformation.utcDateTime();
  const qreal direction = positionInformation.direction();
  const qreal groundSpeed = positionInformation.speed();
  const qreal orientation = positionInformation.orientation();
  const qreal magneticVariation = positionInformation.magneticVariation();
  const qreal horizontalAccuracy = positionInformation.hacc();
  const qreal verticalAccuracy = positionInformation.vacc();
  const qreal horizontalVerticalAccuracy = positionInformation.hvacc();
  const qreal verticalSpeed = positionInformation.verticalSpeed();
  const qreal precisionDilution = positionInformation.pdop();
  const qreal horizontalDilution = positionInformation.hdop();
  const qreal verticalDilution = positionInformation.vdop();
  const int numberOfUsedSatelites = positionInformation.satellitesUsed();
  const QList<int> usedSatelites = positionInformation.satPrn();
  const QString qualityDescription = positionInformation.qualityDescription();
  const QString fixStatusDescription = positionInformation.fixStatusDescription();
  const QString fixMode = positionInformation.fixMode();
  const int averagedCount = positionInformation.averagedCount();
  const QString sourceName = positionInformation.sourceName();

  addPositionVariable( scope, QStringLiteral( "coordinate" ), QVariant::fromValue<QgsGeometry>( point ), positionLocked );
  addPositionVariable( scope, QStringLiteral( "timestamp" ), timestamp, positionLocked );
  addPositionVariable( scope, QStringLiteral( "direction" ), direction, positionLocked );
  addPositionVariable( scope, QStringLiteral( "ground_speed" ), groundSpeed, positionLocked );
  addPositionVariable( scope, QStringLiteral( "orientation" ), orientation, positionLocked );
  addPositionVariable( scope, QStringLiteral( "magnetic_variation" ), magneticVariation, positionLocked );
  addPositionVariable( scope, QStringLiteral( "horizontal_accuracy" ), horizontalAccuracy, positionLocked );
  addPositionVariable( scope, QStringLiteral( "vertical_accuracy" ), verticalAccuracy, positionLocked );
  addPositionVariable( scope, QStringLiteral( "3d_accuracy" ), horizontalVerticalAccuracy, positionLocked );
  addPositionVariable( scope, QStringLiteral( "vertical_speed" ), verticalSpeed, positionLocked );
  addPositionVariable( scope, QStringLiteral( "source_name" ), sourceName, positionLocked, QStringLiteral( "manual" ) );
  addPositionVariable( scope, QStringLiteral( "horizontal_accuracy" ), horizontalAccuracy, positionLocked );
  addPositionVariable( scope, QStringLiteral( "vertical_accuracy" ), verticalAccuracy, positionLocked );

  addPositionVariable( scope, QStringLiteral( "pdop" ), precisionDilution, positionLocked );
  addPositionVariable( scope, QStringLiteral( "hdop" ), horizontalDilution, positionLocked );
  addPositionVariable( scope, QStringLiteral( "vdop" ), verticalDilution, positionLocked );
  addPositionVariable( scope, QStringLiteral( "number_of_used_satellites" ), numberOfUsedSatelites, positionLocked );
  addPositionVariable( scope, QStringLiteral( "used_satellites" ), QVariant::fromValue( usedSatelites ), positionLocked );
  addPositionVariable( scope, QStringLiteral( "quality_description" ), qualityDescription, positionLocked );
  addPositionVariable( scope, QStringLiteral( "fix_status_description" ), fixStatusDescription, positionLocked );
  addPositionVariable( scope, QStringLiteral( "fix_mode" ), fixMode, positionLocked );
  addPositionVariable( scope, QStringLiteral( "averaged_count" ), averagedCount, positionLocked );

  return scope;
}

QgsExpressionContextScope *ExpressionContextUtils::mapToolCaptureScope( const SnappingResult &topSnappingResult )
{
  QgsExpressionContextScope *scope = new QgsExpressionContextScope( QObject::tr( "Map Tool Capture" ) );

  QVariantList matchList;

  QVariantMap matchMap;

  matchMap.insert( QStringLiteral( "valid" ), topSnappingResult.isValid() );
  matchMap.insert( QStringLiteral( "layer" ), QVariant::fromValue<QgsWeakMapLayerPointer>( QgsWeakMapLayerPointer( topSnappingResult.layer() ) ) );
  matchMap.insert( QStringLiteral( "feature_id" ), topSnappingResult.featureId() );
  matchMap.insert( QStringLiteral( "vertex_index" ), topSnappingResult.vertexIndex() );
  matchMap.insert( QStringLiteral( "distance" ), topSnappingResult.distance() );

  matchList.append( matchMap );

  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "snapping_results" ), matchList ) );

  return scope;
}

QgsExpressionContextScope *ExpressionContextUtils::cloudUserScope( const CloudUserInformation &cloudUserInformation )
{
  QgsExpressionContextScope *scope = new QgsExpressionContextScope( QObject::tr( "Cloud User Info" ) );

  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "cloud_username" ), cloudUserInformation.username, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "cloud_useremail" ), cloudUserInformation.email, true, true ) );
  return scope;
}

QVariantMap ExpressionContextUtils::layerVariables( QgsMapLayer *layer )
{
  if ( !layer )
  {
    return QVariantMap();
  }

  const QStringList variableNames = layer->customProperty( QStringLiteral( "variableNames" ) ).toStringList();
  const QStringList variableValues = layer->customProperty( QStringLiteral( "variableValues" ) ).toStringList();

  QVariantMap variables;
  for ( int i = 0; i < variableNames.size(); i++ )
  {
    variables[variableNames.at( i )] = variableValues.at( i );
  }
  return variables;
}

void ExpressionContextUtils::setLayerVariable( QgsMapLayer *layer, const QString &name, const QVariant &value )
{
  if ( !layer )
  {
    return;
  }

  QStringList variableNames = layer->customProperty( QStringLiteral( "variableNames" ) ).toStringList();
  QStringList variableValues = layer->customProperty( QStringLiteral( "variableValues" ) ).toStringList();

  variableNames << name;
  variableValues << value.toString();

  layer->setCustomProperty( QStringLiteral( "variableNames" ), variableNames );
  layer->setCustomProperty( QStringLiteral( "variableValues" ), variableValues );
}

void ExpressionContextUtils::setLayerVariables( QgsMapLayer *layer, const QVariantMap &variables )
{
  if ( !layer )
  {
    return;
  }

  QStringList variableNames;
  QStringList variableValues;

  QVariantMap::const_iterator it = variables.constBegin();
  while ( ++it != variables.constEnd() )
  {
    variableNames << it.key();
    variableValues << it.value().toString();
  }

  layer->setCustomProperty( QStringLiteral( "variableNames" ), variableNames );
  layer->setCustomProperty( QStringLiteral( "variableValues" ), variableValues );
}

void ExpressionContextUtils::removeLayerVariable( QgsMapLayer *layer, const QString &name )
{
  if ( !layer )
  {
    return;
  }

  QVariantMap variables = layerVariables( layer );
  if ( variables.remove( name ) )
  {
    setLayerVariables( layer, variables );
  }
}

QVariantMap ExpressionContextUtils::projectVariables( QgsProject *project )
{
  if ( !project )
  {
    return QVariantMap();
  }

  return project->customVariables();
}

void ExpressionContextUtils::setProjectVariable( QgsProject *project, const QString &name, const QVariant &value )
{
  if ( !project )
  {
    return;
  }

  QVariantMap variables = project->customVariables();
  variables.insert( name, value );
  project->setCustomVariables( variables );
}

void ExpressionContextUtils::setProjectVariables( QgsProject *project, const QVariantMap &variables )
{
  if ( !project )
  {
    return;
  }

  project->setCustomVariables( variables );
}

void ExpressionContextUtils::removeProjectVariable( QgsProject *project, const QString &name )
{
  if ( !project )
  {
    return;
  }

  QVariantMap variables = project->customVariables();
  if ( variables.remove( name ) )
  {
    project->setCustomVariables( variables );
  }
}

QVariantMap ExpressionContextUtils::globalVariables()
{
  return QgsApplication::customVariables();
}

void ExpressionContextUtils::setGlobalVariable( const QString &name, const QVariant &value )
{
  QgsApplication::setCustomVariable( name, value );
}

void ExpressionContextUtils::setGlobalVariables( const QVariantMap &variables )
{
  QgsApplication::setCustomVariables( variables );
}

void ExpressionContextUtils::removeGlobalVariable( const QString &name )
{
  QVariantMap variables = QgsApplication::customVariables();
  if ( variables.remove( name ) )
  {
    QgsApplication::setCustomVariables( variables );
  }
}
