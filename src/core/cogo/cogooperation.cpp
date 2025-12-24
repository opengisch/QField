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
#include <qgspoint.h>

QList<CogoParameter> CogoOperationPointAtXYZ::parameters() const
{
  QList<CogoParameter> parameters;
  parameters << CogoParameter( QStringLiteral( "point" ), QStringLiteral( "point" ) );
  return parameters;
}

bool CogoOperationPointAtXYZ::checkReadiness( const QVariantMap &parameters ) const
{
  if ( parameters.contains( QStringLiteral( "point" ) ) )
  {
    if ( parameters[QStringLiteral( "point" )].canConvert<QgsPoint>() )
    {
      const QgsPoint point = parameters[QStringLiteral( "point" )].value<QgsPoint>();
      return !point.isEmpty();
    }
  }
  return false;
}

QList<CogoParameter> CogoOperationPointAtDistanceAngle::parameters() const
{
  QList<CogoParameter> parameters;
  parameters << CogoParameter( QStringLiteral( "point" ), QStringLiteral( "point" ) )
             << CogoParameter( QStringLiteral( "distance" ), QStringLiteral( "distance" ) )
             << CogoParameter( QStringLiteral( "angle" ), QStringLiteral( "angle" ) );
  return parameters;
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
