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
#include "utils/qfurlutils.h"

#include <QDebug>
#include <QFileInfo>
#include <QTemporaryFile>
#include <QUrl>


TEST_CASE( "UrlUtils" )
{
  SECTION( "isRelativeOrFileUrl" )
  {
    // should be considered relative
    REQUIRE( QfUrlUtils::isRelativeOrFileUrl( QStringLiteral( "path/to/file" ) ) );
    REQUIRE( QfUrlUtils::isRelativeOrFileUrl( QStringLiteral( "/path/to/file" ) ) );
    REQUIRE( QfUrlUtils::isRelativeOrFileUrl( QStringLiteral( "file:///path/to/file" ) ) );

    // should NOT be considered relative
    REQUIRE( !QfUrlUtils::isRelativeOrFileUrl( QStringLiteral( "http://osm.org" ) ) );
    REQUIRE( !QfUrlUtils::isRelativeOrFileUrl( QStringLiteral( "http://osm.org/test?query=1" ) ) );
    REQUIRE( !QfUrlUtils::isRelativeOrFileUrl( QStringLiteral( "https://osm.org/test?query=1" ) ) );
  }

  SECTION( "fromString" )
  {
    // a file that exists will be transformed into a file:// URL
    QTemporaryFile tmpFile( QStringLiteral( "test.jpg" ) );
    REQUIRE( QfUrlUtils::fromString( tmpFile.fileName() ).toString() == QUrl::fromLocalFile( tmpFile.fileName() ).toString() );

    // a string that doesn't link to an existing file will not transform into a file:// URL
    REQUIRE( QfUrlUtils::fromString( QStringLiteral( "/my/missing/file.txt" ) ).toString() == QStringLiteral( "/my/missing/file.txt" ) );

    // a URL string (e.g. http(s)) will be handled as such
    REQUIRE( QfUrlUtils::fromString( QStringLiteral( "https://www.opengis.ch/" ) ).toString() == QStringLiteral( "https://www.opengis.ch/" ) );
  }

  SECTION( "urlDetail" )
  {
    const QString url( "https://sub.qfield.org/latest/project.zip?date=now&check=1" );

    REQUIRE( QfUrlUtils::urlDetail( url, "scheme" ) == QStringLiteral( "https" ) );
    REQUIRE( QfUrlUtils::urlDetail( url, "authority" ) == QStringLiteral( "sub.qfield.org" ) );
    REQUIRE( QfUrlUtils::urlDetail( url, "path" ) == QStringLiteral( "/latest/project.zip" ) );
    REQUIRE( QfUrlUtils::urlDetail( url, "filename" ) == QStringLiteral( "project.zip" ) );
    REQUIRE( QfUrlUtils::urlDetail( url, "query" ) == QStringLiteral( "date=now&check=1" ) );
  }

  SECTION( "getActionDetails" )
  {
    QVariantMap details = QfUrlUtils::getActionDetails( "https://qfield.org/action/local?import=https://my.website.com/project.zip" );
    REQUIRE( details["type"] == QStringLiteral( "local" ) );
    REQUIRE( details["import"] == QStringLiteral( "https://my.website.com/project.zip" ) );

    details = QfUrlUtils::getActionDetails( "https://qfield.org/action/local?import=https%3A%2F%2Fmy.website.com%2Fproject.zip" );
    REQUIRE( details["type"] == QStringLiteral( "local" ) );
    REQUIRE( details["import"] == QStringLiteral( "https://my.website.com/project.zip" ) );

    details = QfUrlUtils::getActionDetails( "qfield://local?import=https://my.website.com/project.zip" );
    REQUIRE( details["type"] == QStringLiteral( "local" ) );
    REQUIRE( details["import"] == QStringLiteral( "https://my.website.com/project.zip" ) );

    details = QfUrlUtils::getActionDetails( "qfield://local?import=https%3A%2F%2Fmy.website.com%2Fproject.zip" );
    REQUIRE( details["type"] == QStringLiteral( "local" ) );
    REQUIRE( details["import"] == QStringLiteral( "https://my.website.com/project.zip" ) );
  }

  SECTION( "createActionUrl" )
  {
    QVariantMap details;
    details["project"] = QStringLiteral( "123-456-789" );

    QString url = QfUrlUtils::createActionUrl( "qfield", "cloud", details );
    REQUIRE( url == QStringLiteral( "qfield://cloud?project=123-456-789" ) );
    url = QfUrlUtils::createActionUrl( "https", "cloud", details );
    REQUIRE( url == QStringLiteral( "https://qfield.org/action/cloud?project=123-456-789" ) );

    details.clear();
    details["import"] = QStringLiteral( "https://my.website.com/folder/project.zip?param=1" );
    url = QfUrlUtils::createActionUrl( "qfield", "local", details );
    REQUIRE( url == QStringLiteral( "qfield://local?import=https://my.website.com/folder/project.zip?param%3D1" ) );
    url = QfUrlUtils::createActionUrl( "https", "local", details );
    REQUIRE( url == QStringLiteral( "https://qfield.org/action/local?import=https://my.website.com/folder/project.zip?param%3D1" ) );
  }
}
