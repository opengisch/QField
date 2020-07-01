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

#include <QtTest>
#include <QFileInfo>
#include <QDebug>

#include "qfield_testbase.h"

#include "utils/urlutils.h"


class TestUrlUtils: public QObject
{
    Q_OBJECT
  private slots:

    void testHasError()
    {
      // should be considered relative
      QVERIFY( UrlUtils::isRelativeOrFileUrl( QStringLiteral( "path/to/file" ) ) );
      QVERIFY( UrlUtils::isRelativeOrFileUrl( QStringLiteral( "/path/to/file" ) ) );
      QVERIFY( UrlUtils::isRelativeOrFileUrl( QStringLiteral( "file:///path/to/file" ) ) );

      // should NOT be considered relative
      QVERIFY( ! UrlUtils::isRelativeOrFileUrl( QStringLiteral( "http://osm.org" ) ) );
      QVERIFY( ! UrlUtils::isRelativeOrFileUrl( QStringLiteral( "http://osm.org/test?query=1" ) ) );
      QVERIFY( ! UrlUtils::isRelativeOrFileUrl( QStringLiteral( "https://osm.org/test?query=1" ) ) );
    }


};

QFIELDTEST_MAIN( TestUrlUtils )
#include "test_urlutils.moc"
