/***************************************************************************
                        test_appinterface.h
                        --------------------
  begin                : April 2025
  copyright            : (C) 2025 by Mathieu Pellerin
  email                : mathieu@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "appinterface.h"
#include "catch2.h"


TEST_CASE( "AppInterface" )
{
  AppInterface iface( nullptr );

  SECTION( "getActionDetails" )
  {
    QVariantMap details = iface.getActionDetails( "https://qfield.org/action/local?import=https://my.website.com/project.zip" );
    REQUIRE( details["type"] == QStringLiteral( "local" ) );
    REQUIRE( details["import"] == QStringLiteral( "https://my.website.com/project.zip" ) );

    details = iface.getActionDetails( "https://qfield.org/action/local?import=https%3A%2F%2Fmy.website.com%2Fproject.zip" );
    REQUIRE( details["type"] == QStringLiteral( "local" ) );
    REQUIRE( details["import"] == QStringLiteral( "https://my.website.com/project.zip" ) );

    details = iface.getActionDetails( "qfield://local?import=https://my.website.com/project.zip" );
    REQUIRE( details["type"] == QStringLiteral( "local" ) );
    REQUIRE( details["import"] == QStringLiteral( "https://my.website.com/project.zip" ) );

    details = iface.getActionDetails( "qfield://local?import=https%3A%2F%2Fmy.website.com%2Fproject.zip" );
    REQUIRE( details["type"] == QStringLiteral( "local" ) );
    REQUIRE( details["import"] == QStringLiteral( "https://my.website.com/project.zip" ) );
  }
}
