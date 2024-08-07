/***************************************************************************
                        test_urlutils.h
                        --------------------
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

#include "catch2.h"
#include "utils/urlutils.h"

#include <QDebug>
#include <QFileInfo>
#include <QTemporaryFile>
#include <QUrl>


TEST_CASE( "UrlUtils" )
{
  SECTION( "isRelativeOrFileUrl" )
  {
    // should be considered relative
    REQUIRE( UrlUtils::isRelativeOrFileUrl( QStringLiteral( "path/to/file" ) ) );
    REQUIRE( UrlUtils::isRelativeOrFileUrl( QStringLiteral( "/path/to/file" ) ) );
    REQUIRE( UrlUtils::isRelativeOrFileUrl( QStringLiteral( "file:///path/to/file" ) ) );

    // should NOT be considered relative
    REQUIRE( !UrlUtils::isRelativeOrFileUrl( QStringLiteral( "http://osm.org" ) ) );
    REQUIRE( !UrlUtils::isRelativeOrFileUrl( QStringLiteral( "http://osm.org/test?query=1" ) ) );
    REQUIRE( !UrlUtils::isRelativeOrFileUrl( QStringLiteral( "https://osm.org/test?query=1" ) ) );
  }

  SECTION( "fromString" )
  {
    // a file that exists will be transformed into a file:// URL
    QTemporaryFile tmpFile( QStringLiteral( "test.jpg" ) );
    REQUIRE( UrlUtils::fromString( tmpFile.fileName() ).toString() == QUrl::fromLocalFile( tmpFile.fileName() ).toString() );

    // a string that doesn't link to an existing file will not transform into a file:// URL
    REQUIRE( UrlUtils::fromString( QStringLiteral( "/my/missing/file.txt" ) ).toString() == QStringLiteral( "/my/missing/file.txt" ) );

    // a URL string (e.g. http(s)) will be handled as such
    REQUIRE( UrlUtils::fromString( QStringLiteral( "https://www.opengis.ch/" ) ).toString() == QStringLiteral( "https://www.opengis.ch/" ) );
  }
}
