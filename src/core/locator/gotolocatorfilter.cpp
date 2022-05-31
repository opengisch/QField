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
#include "locatormodelsuperbridge.h"
#include "qgsquickmapsettings.h"

#include <QAction>
#include <QRegularExpression>
#if _QGIS_VERSION_INT >= 32500
#include <qgscoordinatereferencesystemutils.h>
#endif
#include <qgscoordinateutils.h>
#include <qgsexpressioncontextutils.h>
#include <qgsfeedback.h>
#include <qgspoint.h>
#include <qgsproject.h>


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

  bool firstOk = false;
  bool secondOk = false;
  double firstNumber = 0.0;
  double secondNumber = 0.0;
  bool posIsWgs84 = false;
  const QLocale locale;

  // Coordinates such as 106.8468,-6.3804
  QRegularExpression separatorRx( QStringLiteral( "^([0-9\\-\\%1\\%2]*)[\\s%3]*([0-9\\-\\%1\\%2]*)$" ).arg( locale.decimalPoint(), locale.groupSeparator(), locale.decimalPoint() != ',' && locale.groupSeparator() != ',' ? QStringLiteral( "\\," ) : QString() ) );
  QRegularExpressionMatch match = separatorRx.match( string.trimmed() );
  if ( match.hasMatch() )
  {
    firstNumber = locale.toDouble( match.captured( 1 ), &firstOk );
    secondNumber = locale.toDouble( match.captured( 2 ), &secondOk );
  }

  if ( !match.hasMatch() || !firstOk || !secondOk )
  {
    // Digit detection using user locale failed, use default C decimal separators
    separatorRx = QRegularExpression( QStringLiteral( "^([0-9\\-\\.]*)[\\s\\,]*([0-9\\-\\.]*)$" ) );
    match = separatorRx.match( string.trimmed() );
    if ( match.hasMatch() )
    {
      firstNumber = match.captured( 1 ).toDouble( &firstOk );
      secondNumber = match.captured( 2 ).toDouble( &secondOk );
    }
  }

#if _QGIS_VERSION_INT >= 32500
  if ( !match.hasMatch() )
  {
    // Check if the string is a pair of decimal degrees with [N,S,E,W] suffixes
    separatorRx = QRegularExpression( QStringLiteral( "^\\s*([-]?\\d{1,3}(?:[\\.\\%1]\\d+)?\\s*[NSEWnsew])[\\s\\,]*([-]?\\d{1,3}(?:[\\.\\%1]\\d+)?\\s*[NSEWnsew])\\s*$" )
                                        .arg( locale.decimalPoint() ) );
    match = separatorRx.match( string.trimmed() );
    if ( match.hasMatch() )
    {
      posIsWgs84 = true;
      bool isEasting = false;
      firstNumber = QgsCoordinateUtils::degreeToDecimal( match.captured( 1 ), &firstOk, &isEasting );
      secondNumber = QgsCoordinateUtils::degreeToDecimal( match.captured( 2 ), &secondOk );
      // normalize to northing (i.e. Y) first
      if ( isEasting )
        std::swap( firstNumber, secondNumber );
    }
  }
#endif

  if ( !match.hasMatch() )
  {
    // Check if the string is a pair of degree minute second
    separatorRx = QRegularExpression( QStringLiteral( "^((?:([-+nsew])\\s*)?\\d{1,3}(?:[^0-9.]+[0-5]?\\d)?[^0-9.]+[0-5]?\\d(?:[\\.\\%1]\\d+)?[^0-9.,]*[-+nsew]?)[,\\s]+((?:([-+nsew])\\s*)?\\d{1,3}(?:[^0-9.]+[0-5]?\\d)?[^0-9.]+[0-5]?\\d(?:[\\.\\%1]\\d+)?[^0-9.,]*[-+nsew]?)$" )
                                        .arg( locale.decimalPoint() ) );
    match = separatorRx.match( string.trimmed() );
    if ( match.hasMatch() )
    {
      posIsWgs84 = true;
      bool isEasting = false;
      firstNumber = QgsCoordinateUtils::dmsToDecimal( match.captured( 1 ), &firstOk, &isEasting );
      secondNumber = QgsCoordinateUtils::dmsToDecimal( match.captured( 3 ), &secondOk );
      // normalize to northing (i.e. Y) first
      if ( isEasting )
        std::swap( firstNumber, secondNumber );
    }
  }

  const QgsCoordinateReferenceSystem currentCrs = mLocatorBridge->mapSettings()->mapSettings().destinationCrs();
  const QgsCoordinateReferenceSystem wgs84Crs( QStringLiteral( "EPSG:4326" ) );

  if ( firstOk && secondOk )
  {
    QVariantMap data;
#if _QGIS_VERSION_INT >= 32500
    const bool currentCrsIsXY = QgsCoordinateReferenceSystemUtils::defaultCoordinateOrderForCrs( currentCrs ) == Qgis::CoordinateOrder::XY;
    const bool withinWgs84 = wgs84Crs.bounds().contains( secondNumber, firstNumber );
#else
    const bool currentCrsIsXY = true;
    const bool withinWgs84 = wgs84Crs.bounds().contains( firstNumber, secondNumber );
#endif
    if ( !posIsWgs84 && currentCrs != wgs84Crs )
    {
      const QgsPointXY point( currentCrsIsXY ? firstNumber : secondNumber,
                              currentCrsIsXY ? secondNumber : firstNumber );
      data.insert( QStringLiteral( "point" ), point );

      QString firstSuffix;
      QString secondSuffix;
#if _QGIS_VERSION_INT >= 32500
      const QList<Qgis::CrsAxisDirection> axisList = currentCrs.axisOrdering();
      if ( axisList.size() >= 2 )
      {
        firstSuffix = QgsCoordinateReferenceSystemUtils::axisDirectionToAbbreviatedString( axisList.at( 0 ) );
        secondSuffix = QgsCoordinateReferenceSystemUtils::axisDirectionToAbbreviatedString( axisList.at( 1 ) );
      }
#endif
      QgsLocatorResult result;
      result.filter = this;
      result.displayString = tr( "Go to %1%2 %3%4 (Map CRS, %5)" ).arg( locale.toString( firstNumber, 'g', 10 ), firstSuffix, locale.toString( secondNumber, 'g', 10 ), secondSuffix, currentCrs.userFriendlyIdentifier() );
      result.userData = data;
      result.score = 0.9;
      emit resultFetched( result );
    }

    if ( withinWgs84 )
    {
#if _QGIS_VERSION_INT >= 32500
      const QgsPointXY point( secondNumber, firstNumber );
#else
      const QgsPointXY point( firstNumber, secondNumber );
#endif
      if ( currentCrs != wgs84Crs )
      {
        const QgsCoordinateTransform transform( wgs84Crs, currentCrs, QgsProject::instance()->transformContext() );
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
        data[QStringLiteral( "point" )] = transformedPoint;
      }
      else
      {
        data[QStringLiteral( "point" )] = point;
      }

      QgsLocatorResult result;
      result.filter = this;
      result.displayString = tr( "Go to %1°N %2°E (%3)" ).arg( locale.toString( point.y(), 'g', 10 ), locale.toString( point.x(), 'g', 10 ), wgs84Crs.userFriendlyIdentifier() );
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

void GotoLocatorFilter::triggerResultFromAction( const QgsLocatorResult &result, const int actionId )
{
  QVariantMap data = result.userData.toMap();
  QgsGeometry geom( QgsGeometry::fromPointXY( data[QStringLiteral( "point" )].value<QgsPointXY>() ) );

  if ( actionId == Navigation )
  {
    if ( mLocatorBridge->navigation() )
    {
      mLocatorBridge->navigation()->setDestination( geom.vertexAt( 0 ) );
    }
  }
  else
  {
    mLocatorBridge->mapSettings()->setCenter( geom.vertexAt( 0 ) );

    mLocatorBridge->locatorHighlightGeometry()->setProperty( "qgsGeometry", geom );
    mLocatorBridge->locatorHighlightGeometry()->setProperty( "crs", mLocatorBridge->mapSettings()->mapSettings().destinationCrs() );
  }
}
