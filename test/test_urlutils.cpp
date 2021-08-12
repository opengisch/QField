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

#include "qfield_testbase.h"
#include "utils/urlutils.h"

#include <QDebug>
#include <QFileInfo>
#include <gtest/gtest.h>


TEST( UrlUtils, HasError )
{
  // should be considered relative
  EXPECT_TRUE( UrlUtils::isRelativeOrFileUrl( QStringLiteral( "path/to/file" ) ) );
  EXPECT_TRUE( UrlUtils::isRelativeOrFileUrl( QStringLiteral( "/path/to/file" ) ) );
  EXPECT_TRUE( UrlUtils::isRelativeOrFileUrl( QStringLiteral( "file:///path/to/file" ) ) );

  // should NOT be considered relative
  EXPECT_TRUE( !UrlUtils::isRelativeOrFileUrl( QStringLiteral( "http://osm.org" ) ) );
  EXPECT_TRUE( !UrlUtils::isRelativeOrFileUrl( QStringLiteral( "http://osm.org/test?query=1" ) ) );
  EXPECT_TRUE( !UrlUtils::isRelativeOrFileUrl( QStringLiteral( "https://osm.org/test?query=1" ) ) );
}
