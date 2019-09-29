/***************************************************************************
 qgsquickcoordinatetransformer.cpp
  --------------------------------------
  Date                 : 1.6.2017
  Copyright            : (C) 2017 by Matthias Kuhn
  Email                :  matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsquickcoordinatetransformer.h"
#include <qgslogger.h>

QgsQuickCoordinateTransformer::QgsQuickCoordinateTransformer( QObject *parent )
  : QObject( parent )
{
  mCoordinateTransform.setSourceCrs( QgsCoordinateReferenceSystem::fromEpsgId( 4326 ) );
}

QgsPoint QgsQuickCoordinateTransformer::projectedPosition() const
{
  return mProjectedPosition;
}

QgsPoint QgsQuickCoordinateTransformer::sourcePosition() const
{
  return mSourcePosition;
}

void QgsQuickCoordinateTransformer::setSourcePosition( const QgsPoint &sourcePosition )
{
#if 0
  float r = static_cast <float>( rand() ) / static_cast <float>( RAND_MAX ) / 100;
  float rd = static_cast <float>( rand() ) / static_cast <float>( RAND_MAX ) / 100;
  float rz = static_cast <float>( rand() ) / static_cast <float>( RAND_MAX ) * 100;

  sourcePosition.setLatitude( 46.9483 + r );
  sourcePosition.setLongitude( 7.44225 + rd );
  sourcePosition.setAltitude( 400 + rz );
#endif

  if ( mSourcePosition == sourcePosition )
    return;

  mSourcePosition = sourcePosition;

  emit sourcePositionChanged();
  updatePosition();
}

QgsCoordinateReferenceSystem QgsQuickCoordinateTransformer::destinationCrs() const
{
  return mCoordinateTransform.destinationCrs();
}

void QgsQuickCoordinateTransformer::setDestinationCrs( const QgsCoordinateReferenceSystem &destinationCrs )
{
  if ( destinationCrs == mCoordinateTransform.destinationCrs() )
    return;

  mCoordinateTransform.setDestinationCrs( destinationCrs );
  emit destinationCrsChanged();
  updatePosition();
}

QgsCoordinateReferenceSystem QgsQuickCoordinateTransformer::sourceCrs() const
{
  return mCoordinateTransform.sourceCrs();
}

void QgsQuickCoordinateTransformer::setSourceCrs( const QgsCoordinateReferenceSystem &sourceCrs )
{
  if ( sourceCrs == mCoordinateTransform.sourceCrs() )
    return;

  mCoordinateTransform.setSourceCrs( sourceCrs );

  emit sourceCrsChanged();
  updatePosition();
}

void QgsQuickCoordinateTransformer::setTransformContext( const QgsCoordinateTransformContext &context )
{
  mCoordinateTransform.setContext( context );
  emit transformContextChanged();
}

QgsCoordinateTransformContext QgsQuickCoordinateTransformer::transformContext() const
{
  return mCoordinateTransform.context();
}

void QgsQuickCoordinateTransformer::updatePosition()
{
  double x = mSourcePosition.x();
  double y = mSourcePosition.y();
  double z = mSourcePosition.z();

  // If Z is NaN, coordinate transformation (proj4) will
  // also set X and Y to NaN. But we also want to get projected
  // coords if we do not have any Z coordinate.
  if ( std::isnan( z ) )
  {
    z = 0;
  }

  try
  {
    mCoordinateTransform.transformInPlace( x, y, z );
  }
  catch ( const QgsCsException &exp )
  {
    QgsDebugMsg( exp.what() );
  }

  mProjectedPosition = QgsPoint( x, y );
  mProjectedPosition.addZValue( mSourcePosition.z() + mDeltaZ );

  emit projectedPositionChanged();
}

qreal QgsQuickCoordinateTransformer::deltaZ() const
{
  return mDeltaZ;
}

void QgsQuickCoordinateTransformer::setDeltaZ(const qreal& deltaZ)
{
  emit deltaZChanged();
  mDeltaZ = deltaZ;
}
