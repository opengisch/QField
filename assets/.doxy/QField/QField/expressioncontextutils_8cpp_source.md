

# File expressioncontextutils.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**expressioncontextutils.cpp**](expressioncontextutils_8cpp.md)

[Go to the documentation of this file](expressioncontextutils_8cpp.md)


```C++
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

  addPositionVariable( scope, QStringLiteral( "coordinate" ), QVariant::fromValue<QgsGeometry>( point ), positionLocked );
  addPositionVariable( scope, QStringLiteral( "timestamp" ), positionInformation.utcDateTime(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "direction" ), positionInformation.direction(), positionLocked ); // Speed direction
  addPositionVariable( scope, QStringLiteral( "ground_speed" ), positionInformation.speed(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "orientation" ), positionInformation.orientation(), positionLocked ); // Compass/magnetometer orientation
  addPositionVariable( scope, QStringLiteral( "magnetic_variation" ), positionInformation.magneticVariation(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "horizontal_accuracy" ), positionInformation.hacc(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "vertical_accuracy" ), positionInformation.vacc(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "3d_accuracy" ), positionInformation.hvacc(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "vertical_speed" ), positionInformation.verticalSpeed(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "source_name" ), positionInformation.sourceName(), positionLocked, QStringLiteral( "manual" ) );

  addPositionVariable( scope, QStringLiteral( "pdop" ), positionInformation.pdop(), positionLocked ); // precision dilution
  addPositionVariable( scope, QStringLiteral( "hdop" ), positionInformation.hdop(), positionLocked ); // horizontal dilution
  addPositionVariable( scope, QStringLiteral( "vdop" ), positionInformation.vdop(), positionLocked ); // vertical dilution
  addPositionVariable( scope, QStringLiteral( "number_of_used_satellites" ), positionInformation.satellitesUsed(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "used_satellites" ), QVariant::fromValue( positionInformation.satPrn() ), positionLocked );
  addPositionVariable( scope, QStringLiteral( "quality_description" ), positionInformation.qualityDescription(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "fix_status_description" ), positionInformation.fixStatusDescription(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "fix_mode" ), positionInformation.fixMode(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "averaged_count" ), positionInformation.averagedCount(), positionLocked );

  addPositionVariable( scope, QStringLiteral( "imu_correction" ), positionInformation.imuCorrection(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "imu_roll" ), positionInformation.imuRoll(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "imu_pitch" ), positionInformation.imuPitch(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "imu_heading" ), positionInformation.imuHeading(), positionLocked );
  addPositionVariable( scope, QStringLiteral( "imu_steering" ), positionInformation.imuSteering(), positionLocked );

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

  if ( !variables.isEmpty() )
  {
    QVariantMap::const_iterator it = variables.constBegin();
    while ( ++it != variables.constEnd() )
    {
      variableNames << it.key();
      variableValues << it.value().toString();
    }
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
```


