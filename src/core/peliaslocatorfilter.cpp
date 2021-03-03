/***************************************************************************
  peliaslocatorfilter.cpp

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

#include "peliaslocatorfilter.h"
#include "locatormodelsuperbridge.h"
#include "qgsquickmapsettings.h"

#include <qgsgeocoderresult.h>
#include <qgsproject.h>

#include <QAction>


PeliasLocatorFilter::PeliasLocatorFilter( QgsGeocoderInterface *geocoder, LocatorModelSuperBridge *locatorBridge )
  : QgsAbstractGeocoderLocatorFilter( QStringLiteral( "peliasgeocoder" ), tr( "Pelias Geocoder" ), QStringLiteral( "search" ), geocoder )
  , mLocatorBridge( locatorBridge )
{
  setFetchResultsDelay( 1000 );
  setUseWithoutPrefix( true );
}

PeliasLocatorFilter *PeliasLocatorFilter::clone() const
{
  return new PeliasLocatorFilter( geocoder(), mLocatorBridge );
}

void PeliasLocatorFilter::handleGeocodeResult( const QgsGeocoderResult &result )
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
