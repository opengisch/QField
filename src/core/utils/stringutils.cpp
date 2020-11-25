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

#include "qgsstringutils.h"
#include <QRegularExpression>
#include <QDebug>


StringUtils::StringUtils( QObject *parent )
  : QObject( parent )
{

}


QString StringUtils::insertLinks( const QString &string )
{
  return QgsStringUtils::insertLinks( string );
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
