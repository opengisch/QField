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
  emit sourceCoordinateChanged();
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
  catch(...)
  {
    // catch any other errors
    QgsDebugMsg( "Transform exception caught - possibly because of missing gsb file." );
  }

  if ( mSkipAltitudeTransformation )
    z = mSourcePosition.z();

  mProjectedPosition = QgsPoint( x, y );
  mProjectedPosition.addZValue( z + mDeltaZ );

  emit projectedPositionChanged();
}

bool QgsQuickCoordinateTransformer::skipAltitudeTransformation() const
{
  return mSkipAltitudeTransformation;
}

void QgsQuickCoordinateTransformer::setSkipAltitudeTransformation( bool skipAltitudeTransformation )
{
  if ( mSkipAltitudeTransformation == skipAltitudeTransformation )
    return;

  mSkipAltitudeTransformation = skipAltitudeTransformation;
  emit skipAltitudeTransformationChanged();
}

QGeoCoordinate QgsQuickCoordinateTransformer::sourceCoordinate() const
{
  return QGeoCoordinate( mSourcePosition.y(), mSourcePosition.x(), mSourcePosition.z() );
}

void QgsQuickCoordinateTransformer::setSourceCoordinate( const QGeoCoordinate &sourceCoordinate )
{
  if ( qgsDoubleNear( sourceCoordinate.latitude(), mSourcePosition.y() )
       && qgsDoubleNear( sourceCoordinate.longitude(), mSourcePosition.x() )
       && qgsDoubleNear( sourceCoordinate.altitude(), mSourcePosition.z() )
     )
    return;

  mSourcePosition = QgsPoint( sourceCoordinate.longitude(), sourceCoordinate.latitude(), sourceCoordinate.altitude() );
  emit sourcePositionChanged();
  emit sourceCoordinateChanged();
  updatePosition();
}

qreal QgsQuickCoordinateTransformer::deltaZ() const
{
  return mDeltaZ;
}

void QgsQuickCoordinateTransformer::setDeltaZ( const qreal &deltaZ )
{
  if ( qgsDoubleNear( mDeltaZ, deltaZ ) )
    return;

  if ( std::isnan( deltaZ ) )
    mDeltaZ = 0;
  else
    mDeltaZ = deltaZ;

  emit deltaZChanged();
}
