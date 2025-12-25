/***************************************************************************
                              cogoregistry.cpp
                              -------------------
              begin                : 2025.12.21
              copyright            : (C) 2025 by Mathieu Pellerin
              email                : mathieu (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "cogooperation.h"

#include <QMap>
#include <QVariant>
#include <qgsgeometryutils.h>
#include <qgspoint.h>

QList<CogoParameter> CogoOperationPointAtXYZ::parameters() const
{
  QList<CogoParameter> parameters;
  parameters << CogoParameter( QStringLiteral( "point" ), QStringLiteral( "point" ) );
  return parameters;
}

QList<CogoVisualGuide> CogoOperationPointAtXYZ::visualGuides( const QVariantMap &parameters, QgsQuickMapSettings *mapSettings ) const
{
  QList<CogoVisualGuide> guides;

  if ( !mapSettings )
  {
    return guides;
  }

  if ( parameters.contains( QStringLiteral( "point" ) ) && parameters[QStringLiteral( "point" )].canConvert<QgsPoint>() )
  {
    QgsPoint point = parameters[QStringLiteral( "point" )].value<QgsPoint>();
    QPointF screenPoint = mapSettings->coordinateToScreen( point );
    guides << CogoVisualGuide( CogoVisualGuide::Point, { { QStringLiteral( "point" ), screenPoint }, { QStringLiteral( "size" ), POINT_DEFAULT_SIZE * RESULT_SIZE_FACTOR } } );
  }

  return guides;
}

bool CogoOperationPointAtXYZ::checkReadiness( const QVariantMap &parameters ) const
{
  if ( !parameters.contains( QStringLiteral( "point" ) ) )
  {
    return false;
  }

  if ( !parameters[QStringLiteral( "point" )].canConvert<QgsPoint>() )
  {
    return false;
  }

  const QgsPoint point = parameters[QStringLiteral( "point" )].value<QgsPoint>();
  if ( point.isEmpty() )
  {
    return false;
  }

  return true;
}

bool CogoOperationPointAtXYZ::execute( const QVariantMap &parameters, RubberbandModel *rubberbandModel ) const
{
  if ( !rubberbandModel || !checkReadiness( parameters ) )
  {
    return false;
  }

  const QgsPoint point = parameters[QStringLiteral( "point" )].value<QgsPoint>();
  rubberbandModel->addVertexFromPoint( point );

  return true;
}

QList<CogoParameter> CogoOperationPointAtDistanceAngle::parameters() const
{
  QList<CogoParameter> parameters;
  parameters << CogoParameter( QStringLiteral( "point" ), QStringLiteral( "point" ) )
             << CogoParameter( QStringLiteral( "distance" ), QStringLiteral( "distance" ) )
             << CogoParameter( QStringLiteral( "angle" ), QStringLiteral( "angle" ) );
  return parameters;
}

QList<CogoVisualGuide> CogoOperationPointAtDistanceAngle::visualGuides( const QVariantMap &parameters, QgsQuickMapSettings *mapSettings ) const
{
  QList<CogoVisualGuide> guides;

  if ( !mapSettings )
  {
    return guides;
  }

  if ( parameters.contains( QStringLiteral( "point" ) ) && parameters[QStringLiteral( "point" )].canConvert<QgsPoint>() )
  {
    const QgsPoint point = parameters[QStringLiteral( "point" )].value<QgsPoint>();
    const QPointF screenPoint = mapSettings->coordinateToScreen( point );

    bool distanceOk;
    const double distance = parameters[QStringLiteral( "distance" )].toDouble( &distanceOk );
    bool angleOk;
    const double angleDegree = parameters[QStringLiteral( "angle" )].toDouble( &angleOk );
    if ( distanceOk && angleOk )
    {
      const double angleRadian = ( angleDegree - 90 ) * M_PI / 180;
      const double x = point.x() + ( distance * std::cos( -angleRadian ) );
      const double y = point.y() + ( distance * std::sin( -angleRadian ) );
      const QPointF screenNewPoint = mapSettings->coordinateToScreen( QgsPoint( x, y ) );
      const QPolygonF screenPolygon( QList<QPointF>() << screenPoint << screenNewPoint );

      guides << CogoVisualGuide( CogoVisualGuide::Line, { { QStringLiteral( "polyline" ), screenPolygon }, { QStringLiteral( "size" ), LINE_DEFAULT_SIZE } } );
      guides << CogoVisualGuide( CogoVisualGuide::Point, { { QStringLiteral( "point" ), screenPoint }, { QStringLiteral( "size" ), POINT_DEFAULT_SIZE } } );
      guides << CogoVisualGuide( CogoVisualGuide::Point, { { QStringLiteral( "point" ), screenNewPoint }, { QStringLiteral( "size" ), POINT_DEFAULT_SIZE * RESULT_SIZE_FACTOR } } );
    }
    else
    {
      guides << CogoVisualGuide( CogoVisualGuide::Point, { { QStringLiteral( "point" ), screenPoint }, { QStringLiteral( "size" ), POINT_DEFAULT_SIZE } } );
    }
  }

  return guides;
}

bool CogoOperationPointAtDistanceAngle::checkReadiness( const QVariantMap &parameters ) const
{
  if ( !parameters.contains( QStringLiteral( "point" ) ) || !parameters.contains( QStringLiteral( "distance" ) ) || !parameters.contains( QStringLiteral( "angle" ) ) )
  {
    return false;
  }

  if ( !parameters[QStringLiteral( "point" )].canConvert<QgsPoint>() )
  {
    return false;
  }

  const QgsPoint point = parameters[QStringLiteral( "point" )].value<QgsPoint>();
  if ( point.isEmpty() )
  {
    return false;
  }

  bool ok;
  double value = parameters[QStringLiteral( "distance" )].toDouble( &ok );
  if ( !ok )
  {
    return false;
  }

  value = parameters[QStringLiteral( "angle" )].toDouble( &ok );
  if ( !ok )
  {
    return false;
  }

  return true;
}

bool CogoOperationPointAtDistanceAngle::execute( const QVariantMap &parameters, RubberbandModel *rubberbandModel ) const
{
  if ( !rubberbandModel || !checkReadiness( parameters ) )
  {
    return false;
  }

  const QgsPoint point = parameters[QStringLiteral( "point" )].value<QgsPoint>();
  const double distance = parameters[QStringLiteral( "distance" )].toDouble();
  const double angleDegree = parameters[QStringLiteral( "angle" )].toDouble();
  const double angleRadian = ( angleDegree - 90 ) * M_PI / 180;

  const double x = point.x() + ( distance * std::cos( -angleRadian ) );
  const double y = point.y() + ( distance * std::sin( -angleRadian ) );

  rubberbandModel->addVertexFromPoint( QgsPoint( x, y ) );

  return true;
}

QList<CogoParameter> CogoOperationPointAtIntersectionCircles::parameters() const
{
  QList<CogoParameter> parameters;
  parameters << CogoParameter( QStringLiteral( "point" ), QStringLiteral( "point1" ), QStringLiteral( "Circle #1" ) )
             << CogoParameter( QStringLiteral( "distance" ), QStringLiteral( "distance1" ), QStringLiteral( "Circle #1: Radius" ) )
             << CogoParameter( QStringLiteral( "point" ), QStringLiteral( "point2" ), QStringLiteral( "Circle #2" ) )
             << CogoParameter( QStringLiteral( "distance" ), QStringLiteral( "distance2" ), QStringLiteral( "Circle #2: Radius" ) )
             << CogoParameter( QStringLiteral( "enum" ), QStringLiteral( "candidate" ), QStringLiteral( "Candidate" ), { { QStringLiteral( "options" ), QStringList() << QStringLiteral( "A" ) << QStringLiteral( "B" ) } } );
  return parameters;
}

QList<CogoVisualGuide> CogoOperationPointAtIntersectionCircles::visualGuides( const QVariantMap &parameters, QgsQuickMapSettings *mapSettings ) const
{
  QList<CogoVisualGuide> guides;

  if ( !mapSettings )
  {
    return guides;
  }

  QgsPoint point1;
  bool distance1Ok = false;
  double distance1 = 0.0;
  QgsPoint point2;
  bool distance2Ok = false;
  double distance2 = 0.0;

  if ( parameters.contains( QStringLiteral( "point1" ) ) && parameters[QStringLiteral( "point1" )].canConvert<QgsPoint>() )
  {
    point1 = parameters[QStringLiteral( "point1" )].value<QgsPoint>();
    QPointF screenPoint = mapSettings->coordinateToScreen( point1 );
    guides << CogoVisualGuide( CogoVisualGuide::Point, { { QStringLiteral( "point" ), screenPoint }, { QStringLiteral( "size" ), POINT_DEFAULT_SIZE } } );

    if ( parameters.contains( QStringLiteral( "distance1" ) ) )
    {
      distance1 = parameters[QStringLiteral( "distance1" )].toDouble( &distance1Ok );
      if ( distance1Ok )
      {
        const double radius = distance1 / mapSettings->mapUnitsPerPoint();
        guides << CogoVisualGuide( CogoVisualGuide::Circle, { { QStringLiteral( "center" ), screenPoint }, { QStringLiteral( "radius" ), radius }, { QStringLiteral( "size" ), LINE_DEFAULT_SIZE } } );
      }
    }
  }

  if ( parameters.contains( QStringLiteral( "point2" ) ) && parameters[QStringLiteral( "point2" )].canConvert<QgsPoint>() )
  {
    point2 = parameters[QStringLiteral( "point2" )].value<QgsPoint>();
    QPointF screenPoint = mapSettings->coordinateToScreen( point2 );
    guides << CogoVisualGuide( CogoVisualGuide::Point, { { QStringLiteral( "point" ), screenPoint }, { QStringLiteral( "size" ), POINT_DEFAULT_SIZE } } );

    if ( parameters.contains( QStringLiteral( "distance2" ) ) )
    {
      distance2 = parameters[QStringLiteral( "distance2" )].toDouble( &distance2Ok );
      if ( distance2Ok )
      {
        const double radius = distance2 / mapSettings->mapUnitsPerPoint();
        guides << CogoVisualGuide( CogoVisualGuide::Circle, { { QStringLiteral( "center" ), screenPoint }, { QStringLiteral( "radius" ), radius }, { QStringLiteral( "size" ), LINE_DEFAULT_SIZE } } );
      }
    }
  }

  if ( distance1Ok && distance2Ok && !point1.isEmpty() && !point2.isEmpty() )
  {
    bool isCandidateA = !parameters.contains( QStringLiteral( "candidate" ) ) || parameters[QStringLiteral( "candidate" )].toString() != QStringLiteral( "B" );
    QgsPointXY candidateA;
    QgsPointXY candidateB;
    QgsGeometryUtils::circleCircleIntersections( point1, distance1, point2, distance2, candidateA, candidateB );

    if ( !candidateA.isEmpty() )
    {
      QPointF screenPoint = mapSettings->coordinateToScreen( QgsPoint( candidateA ) );
      const double size = POINT_DEFAULT_SIZE * ( isCandidateA ? RESULT_SIZE_FACTOR : 1.0 );
      guides << CogoVisualGuide( CogoVisualGuide::Point, { { QStringLiteral( "point" ), screenPoint }, { QStringLiteral( "size" ), size } } );
      const double labelSize = LABEL_DEFAULT_SIZE * ( isCandidateA ? RESULT_SIZE_FACTOR : 1.0 );
      screenPoint.setX( screenPoint.x() + size / 2 + 4 );
      screenPoint.setY( screenPoint.y() + size / 2 + 4 );
      guides << CogoVisualGuide( CogoVisualGuide::Label, { { QStringLiteral( "text" ), QStringLiteral( "A" ) }, { QStringLiteral( "point" ), screenPoint }, { QStringLiteral( "size" ), labelSize } } );
    }
    if ( !candidateB.isEmpty() )
    {
      QPointF screenPoint = mapSettings->coordinateToScreen( QgsPoint( candidateB ) );
      const double size = POINT_DEFAULT_SIZE * ( isCandidateA ? 1.0 : RESULT_SIZE_FACTOR );
      guides << CogoVisualGuide( CogoVisualGuide::Point, { { QStringLiteral( "point" ), screenPoint }, { QStringLiteral( "size" ), size } } );
      const double labelSize = LABEL_DEFAULT_SIZE * ( isCandidateA ? 1.0 : RESULT_SIZE_FACTOR );
      screenPoint.setX( screenPoint.x() + size / 2 + 4 );
      screenPoint.setY( screenPoint.y() + size / 2 + 4 );
      guides << CogoVisualGuide( CogoVisualGuide::Label, { { QStringLiteral( "text" ), QStringLiteral( "B" ) }, { QStringLiteral( "point" ), screenPoint }, { QStringLiteral( "size" ), labelSize } } );
    }
  }

  return guides;
}

bool CogoOperationPointAtIntersectionCircles::checkReadiness( const QVariantMap &parameters ) const
{
  if ( !parameters.contains( QStringLiteral( "point1" ) ) || !parameters.contains( QStringLiteral( "distance1" ) ) || !parameters.contains( QStringLiteral( "point2" ) ) || !parameters.contains( QStringLiteral( "distance2" ) ) || !parameters.contains( QStringLiteral( "candidate" ) ) )
  {
    return false;
  }

  if ( !parameters[QStringLiteral( "point1" )].canConvert<QgsPoint>() || !parameters[QStringLiteral( "point2" )].canConvert<QgsPoint>() )
  {
    return false;
  }

  QgsPoint point = parameters[QStringLiteral( "point1" )].value<QgsPoint>();
  if ( point.isEmpty() )
  {
    return false;
  }

  point = parameters[QStringLiteral( "point2" )].value<QgsPoint>();
  if ( point.isEmpty() )
  {
    return false;
  }

  bool ok;
  double value = parameters[QStringLiteral( "distance1" )].toDouble( &ok );
  if ( !ok )
  {
    return false;
  }

  value = parameters[QStringLiteral( "distance2" )].toDouble( &ok );
  if ( !ok )
  {
    return false;
  }

  return true;
}

bool CogoOperationPointAtIntersectionCircles::execute( const QVariantMap &parameters, RubberbandModel *rubberbandModel ) const
{
  if ( !rubberbandModel || !checkReadiness( parameters ) )
  {
    return false;
  }

  const QgsPoint point1 = parameters[QStringLiteral( "point1" )].value<QgsPoint>();
  const QgsPoint point2 = parameters[QStringLiteral( "point2" )].value<QgsPoint>();
  const double distance1 = parameters[QStringLiteral( "distance1" )].toDouble();
  const double distance2 = parameters[QStringLiteral( "distance2" )].toDouble();
  const QString candidate = parameters[QStringLiteral( "candidate" )].toString();

  QgsPointXY candidateA;
  QgsPointXY candidateB;
  QgsGeometryUtils::circleCircleIntersections( point1, distance1, point2, distance2, candidateA, candidateB );

  rubberbandModel->addVertexFromPoint( QgsPoint( candidate == QStringLiteral( "B" ) ? candidateB : candidateA ) );

  return true;
}
