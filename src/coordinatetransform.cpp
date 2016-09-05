/***************************************************************************
              coordinatetransform.cpp
               ----------------------------------------------------
              date                 : 26.2.2015
              copyright            : (C) 2015 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "coordinatetransform.h"
#include "qgspoint.h"

#include <QDebug>

CoordinateTransform::CoordinateTransform( QObject *parent ) :
  QObject( parent )
{
}

const QPointF CoordinateTransform::transform( const QPointF& pt ) const
{
  QgsPoint p( pt.x(), pt.y() );

  p = mTransform.transform( p );

  return QPointF( p.x(), p.y() );
}

QgsCoordinateReferenceSystem CoordinateTransform::sourceCRS() const
{
  return mTransform.sourceCrs();
}

QgsCoordinateReferenceSystem CoordinateTransform::destinationCRS() const
{
  return mTransform.destinationCrs();
}

void CoordinateTransform::setSourceCRS( QgsCoordinateReferenceSystem sourceCRS )
{
  if ( mTransform.sourceCrs() == sourceCRS )
    return;

  mTransform.setSourceCrs( sourceCRS );
  mTransform.initialise();
  emit sourceCRSChanged();
}

void CoordinateTransform::setDestinationCRS( QgsCoordinateReferenceSystem destCRS )
{
  if ( mTransform.destinationCrs() == destCRS )
    return;

  mTransform.setDestinationCrs( destCRS );
  mTransform.initialise();
  emit destinationCRSChanged();
}
