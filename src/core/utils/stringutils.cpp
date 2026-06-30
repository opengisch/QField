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

#include <QFileInfo>
#include <QRegularExpression>
#include <QUuid>
#include <qgscoordinatereferencesystemutils.h>
#include <qgsstringutils.h>

StringUtils::StringUtils( QObject *parent )
  : QObject( parent )
{
}

QString StringUtils::insertLinks( const QString &string )
{
  return QgsStringUtils::insertLinks( string );
}

bool StringUtils::hasLinks( const QString &string )
{
  // These expressions are taken from QgsStringUtils::insertLinks
  const thread_local QRegularExpression urlRegEx( QStringLiteral( "(\\b(([\\w-]+://?|www[.])[^\\s()<>]+(?:\\([\\w\\d]+\\)|([^!\"#$%&'()*+,\\-./:;<=>?@[\\\\\\]^_`{|}~\\s]|/))))" ) );
  const thread_local QRegularExpression emailRegEx( QStringLiteral( "([\\w._%+-]+@[\\w.-]+\\.[A-Za-z]+)" ) );

  return string.contains( urlRegEx ) || string.contains( emailRegEx );
}

QString StringUtils::createUuid()
{
  return QUuid::createUuid().toString();
}

double StringUtils::calcFuzzyScore( const QString &string, const QString &searchTerm )
{
  // Match accent-insensitively (e.g. "bez" matches "Béziers"), like the callers' search filter.
  const QString unaccentedString = QgsStringUtils::unaccent( string );
  const QString unaccentedSearchTerm = QgsStringUtils::unaccent( searchTerm );

  double fuzzyScore = 0.0;
  if ( unaccentedString.startsWith( unaccentedSearchTerm, Qt::CaseInsensitive ) )
  {
    fuzzyScore += 0.5;
  }
  else
  {
    static QRegularExpression whitespaceRegex( QStringLiteral( "\\W+" ) );
    const QStringList parts = unaccentedString.split( whitespaceRegex );
    const QStringList termParts = unaccentedSearchTerm.split( whitespaceRegex );
    const qsizetype termPartsCount = termParts.size();

    qsizetype lastMatchedTermPartIdx = -1;
    qsizetype matchedTermItems = 0;
    for ( const QString &part : parts )
    {
      for ( qsizetype i = lastMatchedTermPartIdx + 1; i < termPartsCount; i++ )
      {
        if ( part.startsWith( termParts[i], Qt::CaseInsensitive ) )
        {
          lastMatchedTermPartIdx = i;
          matchedTermItems++;
          break;
        }
      }
    }

    if ( lastMatchedTermPartIdx >= 0 && matchedTermItems == termPartsCount )
    {
      fuzzyScore += 0.5;
    }
  }

  fuzzyScore += QgsStringUtils::fuzzyScore( unaccentedString, unaccentedSearchTerm ) * 0.5;
  return fuzzyScore;
};

QString StringUtils::pointInformation( const QgsPoint &point, const QgsCoordinateReferenceSystem &crs )
{
  if ( crs.authid() == QStringLiteral( "EPSG:4326" ) )
  {
    // For WGS84 coordinates, use the most universally accepted form (i.e. lat, lon)
    return QStringLiteral( "%1, %2" ).arg( QString::number( point.y(), 'f', 5 ), QString::number( point.x(), 'f', 5 ) );
  }

  QString firstSuffix;
  QString secondSuffix;
  const bool currentCrsIsXY = QgsCoordinateReferenceSystemUtils::defaultCoordinateOrderForCrs( crs ) == Qgis::CoordinateOrder::XY;
  const QList<Qgis::CrsAxisDirection> axisList = crs.axisOrdering();
  if ( axisList.size() >= 2 && crs.isGeographic() )
  {
    firstSuffix = QgsCoordinateReferenceSystemUtils::axisDirectionToAbbreviatedString( axisList.at( 0 ) );
    secondSuffix = QgsCoordinateReferenceSystemUtils::axisDirectionToAbbreviatedString( axisList.at( 1 ) );
  }
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
  for ( qsizetype i = 0; i < string.size(); i++ )
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
        qsizetype index = i + 1 < string.size() ? i + 1 : string.size() - 1;
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
  return values;
}

QString StringUtils::replaceFilenameTags( const QString &string, const QString &filename )
{
  // IMPORTANT: an equivalent Java function QFieldUtils on Android needs to be kept synchronized
  QString replacedString = string;

  QFileInfo fi( filename );
  QString completeSuffix = fi.completeSuffix();
  if ( completeSuffix == QStringLiteral( "mp4a" ) )
  {
    completeSuffix = QStringLiteral( "m4a" );
  }
  replacedString.replace( QStringLiteral( "{filename}" ), fi.fileName() );
  replacedString.replace( QStringLiteral( "{extension}" ), completeSuffix );

  return replacedString;
}

QString StringUtils::highlightText( const QString &string, const QString &highlightText, const QColor &highlightColor )
{
  if ( highlightText.isEmpty() )
  {
    return string.toHtmlEscaped();
  }

  const QString unaccentedString = QgsStringUtils::unaccent( string );
  const QString unaccentedHighlight = QgsStringUtils::unaccent( highlightText );
  if ( unaccentedString.length() != string.length() )
  {
    return string.toHtmlEscaped();
  }

  QStringList patterns { QRegularExpression::escape( unaccentedHighlight ) };
  if ( !unaccentedString.contains( QRegularExpression( patterns.constFirst(), QRegularExpression::CaseInsensitiveOption ) ) )
  {
    patterns = unaccentedHighlight.split( ' ', Qt::SkipEmptyParts );
    patterns.removeDuplicates();
    for ( QString &part : patterns )
    {
      part = QRegularExpression::escape( part );
    }
  }
  if ( patterns.isEmpty() )
  {
    return string.toHtmlEscaped();
  }

  const QRegularExpression expression( patterns.join( '|' ), QRegularExpression::CaseInsensitiveOption );
  const QString spanStyle = highlightColor.isValid() ? QStringLiteral( "color:%1" ).arg( highlightColor.name() ) : QString();
  QString formattedString;
  qsizetype cursor = 0;
  QRegularExpressionMatchIterator matchIterator = expression.globalMatch( unaccentedString );
  while ( matchIterator.hasNext() )
  {
    const QRegularExpressionMatch match = matchIterator.next();
    formattedString += string.mid( cursor, match.capturedStart() - cursor ).toHtmlEscaped();
    formattedString += QStringLiteral( "<span style=\"text-decoration:underline;%1\">%2</span>" ).arg( spanStyle, string.mid( match.capturedStart(), match.capturedLength() ).toHtmlEscaped() );
    cursor = match.capturedEnd();
  }
  formattedString += string.mid( cursor ).toHtmlEscaped();

  return formattedString;
}

QString StringUtils::snippet( const QString &string, int desiredCharactersLength )
{
  if ( string.isEmpty() )
  {
    return QString();
  }

  QString snippet = string.trimmed().replace( QRegularExpression( QString( "[\r\n]+" ) ), " " );
  if ( snippet.size() <= desiredCharactersLength )
  {
    return snippet;
  }
  snippet.clear();

  const QString paragraph = string.split( QRegularExpression( QString( "[\r\n]+" ) ), Qt::SkipEmptyParts ).at( 0 );
  const QStringList sentences = paragraph.split( QRegularExpression( QString( "\\." ) ), Qt::SkipEmptyParts );
  for ( const QString &sentence : sentences )
  {
    if ( snippet.size() + sentence.size() < desiredCharactersLength )
    {
      snippet += QStringLiteral( "%1." ).arg( sentence );
    }
    else
    {
      if ( snippet.isEmpty() )
      {
        const QStringList parts = string.split( QRegularExpression( QString( "\\s" ) ), Qt::SkipEmptyParts );
        if ( !parts.isEmpty() )
        {
          for ( const QString &part : parts )
          {
            if ( snippet.size() + part.size() < desiredCharactersLength )
            {
              snippet += QStringLiteral( "%1 " ).arg( part );
            }
          }
        }

        if ( snippet.isEmpty() )
        {
          snippet = QStringLiteral( "%1" ).arg( sentence.mid( 0, desiredCharactersLength ) );
        }

        snippet += QStringLiteral( "…" );
      }
      break;
    }
  }

  return snippet;
}
