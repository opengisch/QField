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
#include <QDebug>

CoordinateTransform::CoordinateTransform(QObject *parent) :
  QObject(parent)
, mSourceCRS( 0 )
, mDestinationCRS( 0 )
{
  connect( this, SIGNAL(destinationCRSChanged()), this, SLOT(updateTransform()));
  connect( this, SIGNAL(sourceCRSChanged()), this, SLOT(updateTransform()));
}

const QPointF CoordinateTransform::transform( const QPointF& pt ) const
{
  QgsPoint p( pt.x(), pt.y() );

  p = mTransform.transform( p );

  return QPointF( p.x(), p.y() );
}

CRS* CoordinateTransform::sourceCRS() const
{
  return mSourceCRS;
}

CRS* CoordinateTransform::destinationCRS() const
{
  return mDestinationCRS;
}

void CoordinateTransform::setSourceCRS( CRS* sourceCRS )
{
  if ( mSourceCRS != sourceCRS )
  {
    mSourceCRS = sourceCRS;
    connect( mSourceCRS, SIGNAL(sridChanged()), this, SLOT(updateTransform()) );
    updateTransform();
    emit sourceCRSChanged();
  }
}

void CoordinateTransform::setDestinationCRS( CRS* destCRS )
{
  if ( mDestinationCRS != destCRS )
  {
    mDestinationCRS = destCRS;
    connect( mDestinationCRS, SIGNAL(sridChanged()), this, SLOT(updateTransform()) );
    updateTransform();
    emit destinationCRSChanged();
  }
}

void CoordinateTransform::updateTransform()
{
  if ( mSourceCRS && mDestinationCRS )
  {
    mTransform.setSourceCrs( mSourceCRS->crs() );
    mTransform.setDestCRS( mDestinationCRS->crs() );
    mTransform.initialise();
  }
}
