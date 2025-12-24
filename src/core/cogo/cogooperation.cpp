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
  const double angleDegree = parameters[QStringLiteral( "angle" )].toDouble() - 90.0;
  const double angleRadian = angleDegree * M_PI / 180;

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

  const QString candidate = parameters[QStringLiteral( "candidate" )].toString();
  if ( candidate.isEmpty() )
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
