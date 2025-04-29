/***************************************************************************
                        urlutils.cpp
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

#include "urlutils.h"

#include <QFileInfo>
#include <QUrl>

UrlUtils::UrlUtils( QObject *parent )
  : QObject( parent )
{
}


bool UrlUtils::isRelativeOrFileUrl( const QString &url )
{
  if ( url.startsWith( QStringLiteral( "file://" ) ) )
    return true;

  return QUrl( url ).isRelative();
}

QUrl UrlUtils::fromString( const QString &url )
{
  if ( QFileInfo::exists( url ) )
  {
    return QUrl::fromLocalFile( url );
  }

  return QUrl( url );
}

QString UrlUtils::urlDetail( const QString &url, const QString &detail )
{
  QUrl urlInterface( url );
  if ( detail.compare( QStringLiteral( "scheme" ), Qt::CaseInsensitive ) == 0 )
  {
    return urlInterface.scheme();
  }
  else if ( detail.compare( QStringLiteral( "authority" ), Qt::CaseInsensitive ) == 0 )
  {
    return urlInterface.authority();
  }
  else if ( detail.compare( QStringLiteral( "path" ), Qt::CaseInsensitive ) == 0 )
  {
    return urlInterface.path();
  }
  else if ( detail.compare( QStringLiteral( "filename" ), Qt::CaseInsensitive ) == 0 )
  {
    return urlInterface.fileName();
  }
  else if ( detail.compare( QStringLiteral( "query" ), Qt::CaseInsensitive ) == 0 )
  {
    return urlInterface.query();
  }
  return QString();
}
