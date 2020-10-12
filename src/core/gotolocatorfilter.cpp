/***************************************************************************
  gotolocatorfilter.cpp

 ---------------------
 begin                : 01.08.2020
 copyright            : (C) 2020 by Mathieu Pellerin
 email                : nirvn dot asia at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "gotolocatorfilter.h"

#include <QAction>

#include <qgscoordinateutils.h>
#include <qgsexpressioncontextutils.h>
#include <qgsfeedback.h>
#include <qgspoint.h>
#include <qgsproject.h>

#include "locatormodelsuperbridge.h"
#include "qgsquickmapsettings.h"


GotoLocatorFilter::GotoLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent )
  : QgsLocatorFilter( parent )
  , mLocatorBridge( locatorBridge )
{
  setUseWithoutPrefix( true );
}

GotoLocatorFilter *GotoLocatorFilter::clone() const
{
  return new GotoLocatorFilter( mLocatorBridge );
}

void GotoLocatorFilter::fetchResults( const QString &string, const QgsLocatorContext &, QgsFeedback *feedback )
{
  Q_UNUSED( feedback )
  const QgsCoordinateReferenceSystem currentCrs = mLocatorBridge->mapSettings()->mapSettings().destinationCrs();
  const QgsCoordinateReferenceSystem wgs84Crs( QStringLiteral( "EPSG:4326" ) );

  bool okX = false;
  bool okY = false;
  double posX = 0.0;
  double posY = 0.0;
  bool posIsDms = false;
  QLocale locale;

  // Coordinates such as 106.8468,-6.3804
  QRegularExpression separatorRx( QStringLiteral( "^([0-9\\-\\%1\\%2]*)[\\s%3]*([0-9\\-\\%1\\%2]*)$" ).arg( locale.decimalPoint(),
                                  locale.groupSeparator(),
                                  locale.decimalPoint() != ',' && locale.groupSeparator() != ',' ? QStringLiteral( "\\," ) : QString() ) );
  QRegularExpressionMatch match = separatorRx.match( string.trimmed() );
  if ( match.hasMatch() )
  {
    posX = locale.toDouble( match.captured( 1 ), &okX );
    posY = locale.toDouble( match.captured( 2 ), &okY );
  }

  if ( !match.hasMatch() || !okX || !okY )
  {
    // Digit detection using user locale failed, use default C decimal separators
    separatorRx = QRegularExpression( QStringLiteral( "^([0-9\\-\\.]*)[\\s\\,]*([0-9\\-\\.]*)$" ) );
    match = separatorRx.match( string.trimmed() );
    if ( match.hasMatch() )
    {
      posX = match.captured( 1 ).toDouble( &okX );
      posY = match.captured( 2 ).toDouble( &okY );
    }
  }

  if ( !match.hasMatch() )
  {
    // Check if the string is a pair of degree minute second
    separatorRx = QRegularExpression( QStringLiteral( "^((?:([-+nsew])\\s*)?\\d{1,3}(?:[^0-9.]+[0-5]?\\d)?[^0-9.]+[0-5]?\\d(?:\\.\\d+)?[^0-9.]*[-+nsew]?)\\s+((?:([-+nsew])\\s*)?\\d{1,3}(?:[^0-9.]+[0-5]?\\d)?[^0-9.]+[0-5]?\\d(?:\\.\\d+)?[^0-9.]*[-+nsew]?)$" ) );
    match = separatorRx.match( string.trimmed() );
    if ( match.hasMatch() )
    {
      posIsDms = true;
      posX = QgsCoordinateUtils::dmsToDecimal( match.captured( 1 ), &okX );
      posY = QgsCoordinateUtils::dmsToDecimal( match.captured( 3 ), &okY );
    }
  }

  if ( okX && okY )
  {
    QVariantMap data;
    QgsPointXY point( posX, posY );
    data.insert( QStringLiteral( "point" ), point );

    bool withinWgs84 = wgs84Crs.bounds().contains( point );
    if ( !posIsDms && currentCrs != wgs84Crs )
    {
      QgsLocatorResult result;
      result.filter = this;
      result.displayString = tr( "Go to %1 %2 (Map CRS)" ).arg( locale.toString( point.x(), 'g', 10 ), locale.toString( point.y(), 'g', 10 ) );
      result.userData = data;
      result.score = 0.9;
      emit resultFetched( result );
    }

    if ( withinWgs84 )
    {
      if ( currentCrs != wgs84Crs )
      {
        QgsCoordinateTransform transform( wgs84Crs, currentCrs, QgsProject::instance()->transformContext() );
        QgsPointXY transformedPoint;
        try
        {
          transformedPoint = transform.transform( point );
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
        data[QStringLiteral( "point" )] = transformedPoint;
      }

      QgsLocatorResult result;
      result.filter = this;
      result.displayString = tr( "Go to %1° %2° (WGS84)" ).arg( locale.toString( point.x(), 'g', 10 ), locale.toString( point.y(), 'g', 10 ) );
      result.userData = data;
      result.score = 1.0;
      emit resultFetched( result );
    }
  }
  return;
}

void GotoLocatorFilter::triggerResult( const QgsLocatorResult &result )
{
  triggerResultFromAction( result, Normal );
}

void GotoLocatorFilter::triggerResultFromAction( const QgsLocatorResult &result, const int )
{
  QVariantMap data = result.userData.toMap();

  QgsGeometry geom( QgsGeometry::fromPointXY( data[QStringLiteral( "point" )].value<QgsPointXY>() ) );

  mLocatorBridge->mapSettings()->setCenter( geom.vertexAt( 0 ) );

  mLocatorBridge->locatorHighlightGeometry()->setProperty( "qgsGeometry", geom );
  mLocatorBridge->locatorHighlightGeometry()->setProperty( "crs", mLocatorBridge->mapSettings()->mapSettings().destinationCrs() );
}
