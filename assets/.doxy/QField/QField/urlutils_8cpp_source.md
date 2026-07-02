

# File urlutils.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**urlutils.cpp**](urlutils_8cpp.md)

[Go to the documentation of this file](urlutils_8cpp.md)


```C++
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
#include <QUrlQuery>

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

QString UrlUtils::toLocalFile( const QString &url )
{
  if ( url.startsWith( QStringLiteral( "file://" ) ) )
  {
    const QUrl urlObject( url );
    return urlObject.toLocalFile();
  }

  return url;
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


QVariantMap UrlUtils::getActionDetails( const QString &url )
{
  QVariantMap details;
  if ( url.trimmed().isEmpty() )
    return details;

  QUrl actionUrl( url );
  if ( actionUrl.scheme().toLower() == QStringLiteral( "qfield" ) )
  {
    // deal with qfield://<type>?parameters URLs
    details["type"] = actionUrl.authority();
  }
  else if ( actionUrl.authority() == QStringLiteral( "qfield.org" ) && actionUrl.path().startsWith( "/action/" ) )
  {
    // deal with https://qfield.org/action/<type>?parameters URLs
    // the type matches everything that follows the beginning of the path (i.e./action/)
    details["type"] = actionUrl.path().mid( 8 );
  }
  else
  {
    return details;
  }

  if ( !actionUrl.query().isEmpty() )
  {
    const QList<std::pair<QString, QString>> queryItems = QUrlQuery( actionUrl ).queryItems( QUrl::FullyDecoded );
    for ( const std::pair<QString, QString> &queryItem : queryItems )
    {
      details[queryItem.first] = queryItem.second;
    }
  }

  return details;
}

QString UrlUtils::createActionUrl( const QString &scheme, const QString &type, const QVariantMap &details )
{
  QUrl url;
  if ( scheme == QStringLiteral( "qfield" ) )
  {
    url.setScheme( scheme );
    url.setAuthority( type );
  }
  else if ( scheme == QStringLiteral( "https" ) )
  {
    url.setScheme( scheme );
    url.setAuthority( QStringLiteral( "qfield.org" ) );
    url.setPath( QStringLiteral( "/action/%1" ).arg( type ) );
  }

  QUrlQuery query;
  for ( const QString &key : details.keys() )
  {
    query.addQueryItem( key, details.value( key ).toString() );
  }
  url.setQuery( query );

  return url.toString();
}

QString UrlUtils::createEncodedUrl( const QVariantMap &parameters )
{
  QUrlQuery url;

  const QStringList keys = parameters.keys();
  for ( const QString &key : keys )
  {
    // Exclusion of : and / characters needed to generate URIs compatible with QGIS < 4
    url.addQueryItem( key, QUrl::toPercentEncoding( parameters[key].toString(), ":/?" ) );
  }

  const QString encodedUrl = url.toString( QUrl::FullyEncoded );
  return encodedUrl.isEmpty() ? QByteArray( "" ) : encodedUrl.toLatin1();
}
```


