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

  const double x = point.x() + ( distance * std::cos( angleRadian ) );
  const double y = point.y() + ( distance * std::sin( angleRadian ) );

  rubberbandModel->addVertexFromPoint( QgsPoint( x, y ) );

  return true;
}

QList<CogoParameter> CogoOperationPointAtIntersectionCircles::parameters() const
{
  QList<CogoParameter> parameters;
  parameters << CogoParameter( QStringLiteral( "point" ), QStringLiteral( "point" ), QStringLiteral( "Circle #1" ) )
             << CogoParameter( QStringLiteral( "distance" ), QStringLiteral( "distance" ), QStringLiteral( "Circle #1: Radius" ) )
             << CogoParameter( QStringLiteral( "point" ), QStringLiteral( "point" ), QStringLiteral( "Circle #2" ) )
             << CogoParameter( QStringLiteral( "distance" ), QStringLiteral( "distance" ), QStringLiteral( "Circle #2: Radius" ) );
  return parameters;
}
