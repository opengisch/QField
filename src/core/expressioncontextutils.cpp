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
#include "qgsgeometry.h"
#include <QtPositioning/QGeoPositionInfoSource>

QgsExpressionContextScope *ExpressionContextUtils::positionScope( const GnssPositionInformation &positionInformation )
{
  QgsExpressionContextScope *scope = new QgsExpressionContextScope( QObject::tr( "Position" ) );
  ;
  const QgsGeometry point = QgsGeometry( new QgsPoint( positionInformation.longitude(), positionInformation.latitude(), positionInformation.elevation() ) );
  const QDateTime timestamp = positionInformation.utcDateTime();
  const qreal direction = positionInformation.direction();
  const qreal groundSpeed = positionInformation.speed();
  //nmea-todo const qreal magneticVariation = positionInformation.magneticVariation();
  const qreal horizontalAccuracy = positionInformation.hacc();
  const qreal verticalAccuracy = positionInformation.vacc();
//nmea-todo const qreal verticalSpeed = positionInfo.attribute( QGeoPositionInfo::Attribute::VerticalSpeed );

  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_coordinate" ), QVariant::fromValue<QgsGeometry>( point ), true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_timestamp" ), timestamp, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_direction" ), direction, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_ground_speed" ), groundSpeed, true, true ) );
  //nmea-todo scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_magnetic_variation" ), magneticVariation, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_horizontal_accuracy" ), horizontalAccuracy, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_vertical_accuracy" ), verticalAccuracy, true, true ) );
  //nmea-todoscope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_vertical_speed" ), verticalSpeed, true, true ) );
  //nmea-todoscope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_source_name" ), source->sourceName(), true, true ) );

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
