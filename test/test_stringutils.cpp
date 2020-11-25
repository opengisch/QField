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

#include <QtTest>

#include "qfield_testbase.h"

#include "utils/stringutils.h"


class TestStringUtils: public QObject
{
    Q_OBJECT
  private slots:

    void testInsertLinks()
    {
      QCOMPARE( StringUtils::insertLinks( QStringLiteral( "http://osm.org/" ) ), QStringLiteral( "<a href=\"http://osm.org/\">http://osm.org/</a>" ) );
      QCOMPARE( StringUtils::insertLinks( QStringLiteral( "https://osm.org/" ) ), QStringLiteral( "<a href=\"https://osm.org/\">https://osm.org/</a>" ) );
      QCOMPARE( StringUtils::insertLinks( QStringLiteral( "before https://osm.org/ after" ) ), QStringLiteral( "before <a href=\"https://osm.org/\">https://osm.org/</a> after" ) );
      QCOMPARE( StringUtils::insertLinks( QStringLiteral( "before https://osm.org/path?resource=;or=this%20one after" ) ), QStringLiteral( "before <a href=\"https://osm.org/path?resource=;or=this%20one\">https://osm.org/path?resource=;or=this%20one</a> after" ) );
    }

    void testFuzzyMatch()
    {
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra", "Quercus rubra" ), true );
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra", "quercus r" ), true );
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra", "Pinus nigra" ), false );
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra", "Quercus" ), true );
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra", "qUERCUS" ), true );
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra", "ERcU" ), true );
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra", "rubra" ), true );
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra", "Rubra" ), true );
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra", "bra" ), true );
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra forma variegata", "rubra varieg" ), true );
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra (forma variegata)", "quercus forma" ), true );
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra", "q r" ), true );
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra", "q   r" ), true );
      QCOMPARE( StringUtils::fuzzyMatch( "Quercus rubra", "q   ubra" ), false );
    }

};

QFIELDTEST_MAIN( TestStringUtils )
#include "test_stringutils.moc"
