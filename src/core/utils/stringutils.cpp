/***************************************************************************
                        stringutils.cpp
                        ---------------
  begin                : Jun 2020
  copyright            : (C) 2020 by Ivan Ivanov
  email                : ivan@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "stringutils.h"

#include <QRegularExpression>
#include <QUuid>
#if _QGIS_VERSION_INT >= 32500
#include <qgscoordinatereferencesystemutils.h>
#endif
#include <qgsstringutils.h>

StringUtils::StringUtils( QObject *parent )
  : QObject( parent )
{
}


QString StringUtils::insertLinks( const QString &string )
{
  return QgsStringUtils::insertLinks( string );
}

QString StringUtils::createUuid()
{
  return QUuid::createUuid().toString();
}

bool StringUtils::fuzzyMatch( const QString &source, const QString &term )
{
  if ( source.contains( term, Qt::CaseInsensitive ) )
    return true;

  const QRegularExpression whitespaceRegex( QStringLiteral( "\\W+" ) );
  const QStringList parts = source.split( whitespaceRegex );
  const QStringList termParts = term.split( whitespaceRegex );
  const int termPartsCount = termParts.length();
  int lastMatchedTermPartIdx = -1;
  int matchedTermItems = 0;

  for ( const QString &part : parts )
  {
    for ( int i = lastMatchedTermPartIdx + 1; i < termPartsCount; i++ )
    {
      if ( part.startsWith( termParts[i], Qt::CaseInsensitive ) )
      {
        lastMatchedTermPartIdx = i;
        matchedTermItems++;
        break;
      }
    }
  }

  return lastMatchedTermPartIdx >= 0 && matchedTermItems == termPartsCount
           ? true
           : false;
}

QString StringUtils::pointInformation( const QgsPoint &point, const QgsCoordinateReferenceSystem &crs )
{
  QString firstSuffix;
  QString secondSuffix;
#if _QGIS_VERSION_INT >= 32500
  const bool currentCrsIsXY = QgsCoordinateReferenceSystemUtils::defaultCoordinateOrderForCrs( crs ) == Qgis::CoordinateOrder::XY;
  const QList<Qgis::CrsAxisDirection> axisList = crs.axisOrdering();
  if ( axisList.size() >= 2 && crs.isGeographic() )
  {
    firstSuffix = QgsCoordinateReferenceSystemUtils::axisDirectionToAbbreviatedString( axisList.at( 0 ) );
    secondSuffix = QgsCoordinateReferenceSystemUtils::axisDirectionToAbbreviatedString( axisList.at( 1 ) );
  }
#else
  const bool currentCrsIsXY = true;
  firstSuffix = crs.isGeographic() ? QStringLiteral( "E" ) : QString();
  secondSuffix = crs.isGeographic() ? QStringLiteral( "N" ) : QString();
#endif
  const QString firstNumber = QString::number( currentCrsIsXY ? point.x() : point.y(),
                                               'f', crs.isGeographic() ? 5 : 2 );
  const QString secondNumber = QString::number( currentCrsIsXY ? point.y() : point.x(),
                                                'f', crs.isGeographic() ? 5 : 2 );
  return QStringLiteral( "%1%2, %3%4 — %5: %6" ).arg( firstNumber, firstSuffix, secondNumber, secondSuffix, crs.authid(), crs.description() );
}

const QString StringUtils::stringListToCsv( QStringList list )
{
  for ( QString &value : list )
  {
    bool quoteColumn = false;
    if ( value.contains( "\"" ) )
    {
      value = value.replace( "\"", "\"\"" );
      quoteColumn = true;
    }
    if ( quoteColumn || value.contains( QRegularExpression( "," ) ) )
    {
      value = ( "\"" + value + "\"" );
    }
  }

  return list.join( ',' );
}

const QStringList StringUtils::csvToStringList( const QString &string )
{
  QStringList values;
  QString value;
  bool inQuote = false;
  for ( int i = 0; i < string.size(); i++ )
  {
    QChar current = string.at( i );
    if ( !inQuote )
    {
      if ( current == ',' )
      {
        values << value;
        value.clear();
      }
      else if ( current == '"' )
      {
        inQuote = true;
      }
      else
      {
        value += current;
      }
    }
    else
    {
      if ( current == '"' )
      {
        int index = i + 1 < string.size() ? i + 1 : string.size() - 1;
        if ( index != i && string.at( index ) == '"' )
        {
          value += '"';
          i++;
        }
        else
        {
          inQuote = false;
        }
      }
      else
      {
        value += current;
      }
    }
  }
  values << value;
  return std::move( values );
}
