/***************************************************************************
  finlandlocatorfilter.cpp

 ---------------------
  Date                 : March 2021
  Copyright            : (C) 2021 by Mathieu Pellerin
  Email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "finlandlocatorfilter.h"
#include "locatormodelsuperbridge.h"
#include "qgsquickmapsettings.h"

#include <qgsgeocoderresult.h>
#include <qgsproject.h>

#include <QAction>


FinlandLocatorFilter::FinlandLocatorFilter( QgsGeocoderInterface *geocoder, LocatorModelSuperBridge *locatorBridge )
  : QgsAbstractGeocoderLocatorFilter( QStringLiteral( "pelias-finland" ), QStringLiteral( "Finnish address search" ), QStringLiteral( "fia" ), geocoder )
  , mLocatorBridge( locatorBridge )
{
  setBoundingBox( QgsRectangle( 19.0832098, 59.4541578, 31.5867071, 70.0922939) );
  setFetchResultsDelay( 1000 );
  setUseWithoutPrefix( false );
}

FinlandLocatorFilter *FinlandLocatorFilter::clone() const
{
  return new FinlandLocatorFilter( geocoder(), mLocatorBridge );
}

void FinlandLocatorFilter::handleGeocodeResult( const QgsGeocoderResult &result )
{
  const QgsCoordinateReferenceSystem currentCrs = mLocatorBridge->mapSettings()->mapSettings().destinationCrs();
  const QgsCoordinateReferenceSystem wgs84Crs( QStringLiteral( "EPSG:4326" ) );

  QgsCoordinateTransform ct( wgs84Crs, currentCrs, QgsProject::instance()->transformContext() );
  QgsGeometry transformedGeometry = result.geometry();
  try
  {
    transformedGeometry.transform( ct );
  }
  catch ( const QgsException &e )
  {
    Q_UNUSED( e )
    return;
  }
  catch(...)
  {
    // catch any other errors
    return;
  }

  mLocatorBridge->mapSettings()->setCenter( transformedGeometry.centroid().vertexAt( 0 ) );

  mLocatorBridge->locatorHighlightGeometry()->setProperty( "qgsGeometry", result.geometry() );
  mLocatorBridge->locatorHighlightGeometry()->setProperty( "crs", result.crs() );
}
