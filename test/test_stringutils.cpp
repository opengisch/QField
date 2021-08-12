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

#include "qfield_testbase.h"
#include "utils/stringutils.h"

#include <gtest/gtest.h>


TEST( StringUtils, InsertLinks )
{
  EXPECT_EQ( StringUtils::insertLinks( QStringLiteral( "http://osm.org/" ) ), QStringLiteral( "<a href=\"http://osm.org/\">http://osm.org/</a>" ) );
  EXPECT_EQ( StringUtils::insertLinks( QStringLiteral( "https://osm.org/" ) ), QStringLiteral( "<a href=\"https://osm.org/\">https://osm.org/</a>" ) );
  EXPECT_EQ( StringUtils::insertLinks( QStringLiteral( "before https://osm.org/ after" ) ), QStringLiteral( "before <a href=\"https://osm.org/\">https://osm.org/</a> after" ) );
  EXPECT_EQ( StringUtils::insertLinks( QStringLiteral( "before https://osm.org/path?resource=;or=this%20one after" ) ), QStringLiteral( "before <a href=\"https://osm.org/path?resource=;or=this%20one\">https://osm.org/path?resource=;or=this%20one</a> after" ) );
}

TEST( StringUtils, FuzzyMatch )
{
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra", "Quercus rubra" ), true );
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra", "quercus r" ), true );
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra", "Pinus nigra" ), false );
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra", "Quercus" ), true );
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra", "qUERCUS" ), true );
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra", "ERcU" ), true );
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra", "rubra" ), true );
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra", "Rubra" ), true );
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra", "bra" ), true );
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra forma variegata", "rubra varieg" ), true );
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra (forma variegata)", "quercus forma" ), true );
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra", "q r" ), true );
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra", "q   r" ), true );
  EXPECT_EQ( StringUtils::fuzzyMatch( "Quercus rubra", "q   ubra" ), false );
}
