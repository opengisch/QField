/***************************************************************************
                        test_stringutils.h
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
#include "utils/stringutils.h"


TEST_CASE( "StringUtils" )
{
  SECTION( "InsertLinks" )
  {
    REQUIRE( StringUtils::insertLinks( QStringLiteral( "http://osm.org/" ) ) == QStringLiteral( "<a href=\"http://osm.org/\">http://osm.org/</a>" ) );
    REQUIRE( StringUtils::insertLinks( QStringLiteral( "https://osm.org/" ) ) == QStringLiteral( "<a href=\"https://osm.org/\">https://osm.org/</a>" ) );
    REQUIRE( StringUtils::insertLinks( QStringLiteral( "before https://osm.org/ after" ) ) == QStringLiteral( "before <a href=\"https://osm.org/\">https://osm.org/</a> after" ) );
    REQUIRE( StringUtils::insertLinks( QStringLiteral( "before https://osm.org/path?resource=;or=this%20one after" ) ) == QStringLiteral( "before <a href=\"https://osm.org/path?resource=;or=this%20one\">https://osm.org/path?resource=;or=this%20one</a> after" ) );
  }

  SECTION( "HighlightText" )
  {
    REQUIRE( StringUtils::highlightText( "QField roxx", "", QColor( Qt::black ) ) == "QField roxx" );
    REQUIRE( StringUtils::highlightText( "QField roxx", "rox", QColor( Qt::black ) ) == "QField <span style=\"text-decoration:underline;color:#000000\">rox</span>x" );
    REQUIRE( StringUtils::highlightText( "QField roxx", "doxx", QColor( Qt::black ) ) == "QField roxx" );
    REQUIRE( StringUtils::highlightText( "QField <roxx>", "rox", QColor( Qt::black ) ) == "QField &lt;<span style=\"text-decoration:underline;color:#000000\">rox</span>x&gt;" );
    REQUIRE( StringUtils::highlightText( "QField rox - rox", "rox", QColor( Qt::black ) ) == "QField <span style=\"text-decoration:underline;color:#000000\">rox</span> - <span style=\"text-decoration:underline;color:#000000\">rox</span>" );
  }
}
