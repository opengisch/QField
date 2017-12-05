#include "expressioncontextutils.h"

#include <QtPositioning/QGeoPositionInfoSource>

QgsExpressionContextScope* ExpressionContextUtils::positionScope( QGeoPositionInfoSource *source )
{
  QgsExpressionContextScope *scope = new QgsExpressionContextScope( QObject::tr( "Position" ) );
  QGeoPositionInfo positionInfo = source->lastKnownPosition();

  const QGeoCoordinate geoCoord = positionInfo.coordinate();
  const QgsPoint point( geoCoord.longitude(), geoCoord.latitude(), geoCoord.altitude() );
  const QDateTime timestamp = positionInfo.timestamp();
  const qreal direction = positionInfo.attribute( QGeoPositionInfo::Attribute::Direction );
  const qreal groundSpeed = positionInfo.attribute( QGeoPositionInfo::Attribute::GroundSpeed );
  const qreal magneticVariation = positionInfo.attribute( QGeoPositionInfo::Attribute::MagneticVariation );
  const qreal horizontalAccuracy = positionInfo.attribute( QGeoPositionInfo::Attribute::HorizontalAccuracy );
  const qreal verticalAccuracy = positionInfo.attribute( QGeoPositionInfo::Attribute::VerticalAccuracy );
  const qreal verticalSpeed = positionInfo.attribute( QGeoPositionInfo::Attribute::VerticalSpeed );

  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_coordinate" ), QVariant::fromValue<QgsPoint>( point ), true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_timestamp" ), timestamp, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_direction" ), direction, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_ground_speed" ), groundSpeed, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_magnetic_variation" ), magneticVariation, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_horizontal_accuracy" ), horizontalAccuracy, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_vertical_accuracy" ), verticalAccuracy, true, true ) );
  scope->addVariable( QgsExpressionContextScope::StaticVariable( QStringLiteral( "position_vertical_speed" ), verticalSpeed, true, true ) );

  return scope;
}
