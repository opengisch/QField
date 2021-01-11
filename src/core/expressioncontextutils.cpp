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
  const QString sourceName = positionInformation.sourceName();

  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_coordinate" ), QVariant::fromValue<QgsGeometry>( point ), true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_timestamp" ), timestamp, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_direction" ), direction, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_ground_speed" ), groundSpeed, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_magnetic_variation" ), magneticVariation, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_horizontal_accuracy" ), horizontalAccuracy, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_vertical_accuracy" ), verticalAccuracy, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_3d_accuracy" ), horizontalVerticalAccuracy, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_vertical_speed" ), verticalSpeed, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_source_name" ), sourceName, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_horizontal_accuracy" ), horizontalAccuracy, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_vertical_accuracy" ), verticalAccuracy, true, true ) );

  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_pdop" ), precisionDilution, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_hdop" ), horizontalDilution, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_vdop" ), verticalDilution, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_number_of_used_satellites" ), numberOfUsedSatelites, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_used_satellites" ),  QVariant::fromValue( usedSatelites ), true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_quality_description" ), qualityDescription, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_fix_status_description" ), fixStatusDescription, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_fix_mode" ), fixMode, true, true ) );

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
