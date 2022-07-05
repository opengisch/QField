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


TEST_CASE( "UrlUtils" )
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
