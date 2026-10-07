/***************************************************************************
                        test_newsparser.cpp
                        -------------------
  begin                : October 2026
  copyright            : (C) 2026 by Kaustuv Pokharel
  email                : kaustuv@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#define QFIELDTEST_MAIN

#include "catch2.h"
#include "qfnewsparser.h"
#include "testutils.h"

#include <QDir>
#include <QFile>
#include <QSettings>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTemporaryDir>


static const QString danubeUuid( QStringLiteral( "https://qfield.org/blog/2026/09/08/qfield-4.3-danube-summer-of-stability/" ) );
static const QString projectCreationUuid( QStringLiteral( "https://qfield.org/blog/2026/08/10/faster-project-creation/" ) );
static const QString pluginsUuid( QStringLiteral( "plugins-updates-2026" ) );


static QByteArray feedFixtureContent()
{
  QFile file( QStringLiteral( "%1/news_feed.xml" ).arg( TEST_DATA_DIR ) );
  REQUIRE( file.open( QIODevice::ReadOnly ) );
  return file.readAll();
}

static QString daysAgo( int days )
{
  return QDateTime::currentDateTimeUtc().addDays( -days ).toString( Qt::RFC2822Date );
}

static QUrl writeRecentFeed( const QTemporaryDir &directory )
{
  QByteArray content = feedFixtureContent();
  content.replace( "Tue, 08 Sep 2026 00:02:00 +0000", daysAgo( 2 ).toUtf8() );
  content.replace( "Thu, 12 Feb 2026 00:00:00 +0000", daysAgo( 10 ).toUtf8() );
  content.replace( "Mon, 10 Aug 2026 00:00:00 +0000", daysAgo( 40 ).toUtf8() );

  const QString path = directory.filePath( QStringLiteral( "recent_feed.xml" ) );
  QFile file( path );
  REQUIRE( file.open( QIODevice::WriteOnly ) );
  file.write( content );
  return QUrl::fromLocalFile( path );
}

static void setEnabledSinceDaysAgo( int days )
{
  QSettings().setValue( QfNewsParser::ENABLED_SINCE_SETTINGS_KEY, QDateTime::currentDateTimeUtc().addDays( -days ) );
}

static void fetchAndWait( QfNewsParser &parser )
{
  parser.fetch();
  REQUIRE( parser.isFetching() );
  QSignalSpy spy( &parser, &QfNewsParser::isFetchingChanged );
  REQUIRE( spy.wait() );
  REQUIRE( !parser.isFetching() );
}


TEST_CASE( "NewsParser" )
{
  QTemporaryDir settingsDirectory;
  REQUIRE( settingsDirectory.isValid() );
  const ScopedIniSettings scopedSettings( settingsDirectory.path(), QStringLiteral( "NewsParserTests" ) );

  QStandardPaths::setTestModeEnabled( true );
  QDir( QStringLiteral( "%1/news" ).arg( QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation ) ) ).removeRecursively();

  SECTION( "parseFeed" )
  {
    const QList<QfNewsItem> items = QfNewsParser::parseFeed( feedFixtureContent() );
    REQUIRE( items.size() == 3 );

    REQUIRE( items[0].uuid == danubeUuid );
    REQUIRE( items[0].title == QStringLiteral( "QField 4.3 “Danube”: Summer of stability" ) );
    REQUIRE( items[0].description == QStringLiteral( "The next version of QField is here." ) );
    REQUIRE( items[0].publicationDate == QDateTime::fromString( QStringLiteral( "2026-09-08T00:02:00Z" ), Qt::ISODate ) );
    REQUIRE( items[0].imageUrl == QUrl( QStringLiteral( "https://qfield.org/blog/2026/09/08/qfield-4.3-danube-summer-of-stability/splash43.webp" ) ) );
    REQUIRE( items[0].url == QUrl( danubeUuid ) );

    REQUIRE( items[1].uuid == projectCreationUuid );

    REQUIRE( items[2].uuid == pluginsUuid );
    REQUIRE( items[2].url == QUrl( QStringLiteral( "https://qfield.org/blog/2026/02/12/qfield-search-and-routing-plugins-updates/" ) ) );
    REQUIRE( items[2].description == QStringLiteral( "Our ninjas improved the OSRM Routing plugin. Open QField’s settings panel." ) );
  }

  SECTION( "parseFeedMalformed" )
  {
    REQUIRE( QfNewsParser::parseFeed( QByteArray( "<rss><channel><item><title>Broken" ) ).isEmpty() );
    REQUIRE( QfNewsParser::parseFeed( QByteArray() ).isEmpty() );
  }

  SECTION( "disabledByDefault" )
  {
    QfNewsParser parser;
    parser.setFeedUrl( writeRecentFeed( settingsDirectory ) );
    REQUIRE( !parser.enabled() );

    parser.fetch();
    REQUIRE( !parser.isFetching() );
    REQUIRE( parser.items().isEmpty() );
  }

  SECTION( "noFeedUrl" )
  {
    QfNewsParser parser;
    parser.setEnabled( true );
    REQUIRE( parser.feedUrl().isEmpty() );

    parser.fetch();
    REQUIRE( !parser.isFetching() );
  }

  SECTION( "onlyNewsPublishedSinceEnabled" )
  {
    QfNewsParser parser;
    parser.setFeedUrl( writeRecentFeed( settingsDirectory ) );

    QSignalSpy enabledSpy( &parser, &QfNewsParser::enabledChanged );
    parser.setEnabled( true );
    REQUIRE( parser.enabled() );
    REQUIRE( enabledSpy.count() == 1 );
    REQUIRE( QSettings().value( QfNewsParser::ENABLED_SINCE_SETTINGS_KEY ).toDateTime().isValid() );

    fetchAndWait( parser );
    REQUIRE( parser.items().isEmpty() );

    setEnabledSinceDaysAgo( 5 );
    fetchAndWait( parser );
    REQUIRE( parser.items().size() == 1 );
    REQUIRE( parser.items()[0].uuid == danubeUuid );
  }

  SECTION( "onlyNewsWithinMaximumAge" )
  {
    QfNewsParser parser;
    parser.setFeedUrl( writeRecentFeed( settingsDirectory ) );
    parser.setEnabled( true );
    setEnabledSinceDaysAgo( 60 );

    fetchAndWait( parser );
    REQUIRE( parser.items().size() == 2 );
    REQUIRE( parser.items()[0].uuid == danubeUuid );
    REQUIRE( parser.items()[1].uuid == pluginsUuid );
  }

  SECTION( "dismiss" )
  {
    const QUrl feedUrl = writeRecentFeed( settingsDirectory );
    QfNewsParser parser;
    parser.setFeedUrl( feedUrl );
    parser.setEnabled( true );
    setEnabledSinceDaysAgo( 60 );
    fetchAndWait( parser );
    REQUIRE( parser.items().size() == 2 );

    QSignalSpy itemsSpy( &parser, &QfNewsParser::itemsChanged );
    parser.dismiss( pluginsUuid );
    REQUIRE( itemsSpy.count() == 1 );
    REQUIRE( parser.items().size() == 1 );
    REQUIRE( parser.items()[0].uuid == danubeUuid );

    QfNewsParser otherParser;
    otherParser.setFeedUrl( feedUrl );
    fetchAndWait( otherParser );
    REQUIRE( otherParser.items().size() == 1 );
    REQUIRE( otherParser.items()[0].uuid == danubeUuid );
    REQUIRE( QSettings().value( QfNewsParser::DISMISSED_UUIDS_SETTINGS_KEY ).toStringList() == QStringList { pluginsUuid } );
  }

  SECTION( "restoreLastFetchedFeed" )
  {
    const QUrl feedUrl = writeRecentFeed( settingsDirectory );
    QfNewsParser parser;
    parser.setFeedUrl( feedUrl );
    parser.setEnabled( true );
    setEnabledSinceDaysAgo( 60 );
    fetchAndWait( parser );
    REQUIRE( parser.items().size() == 2 );

    QfNewsParser otherParser;
    otherParser.setFeedUrl( feedUrl );
    REQUIRE( !otherParser.isFetching() );
    REQUIRE( otherParser.items() == parser.items() );

    QSignalSpy itemsSpy( &otherParser, &QfNewsParser::itemsChanged );
    otherParser.setFeedUrl( QUrl::fromLocalFile( settingsDirectory.filePath( QStringLiteral( "missing_feed.xml" ) ) ) );
    REQUIRE( itemsSpy.count() == 1 );
    REQUIRE( otherParser.items().isEmpty() );
  }

  SECTION( "disablingClearsItems" )
  {
    QfNewsParser parser;
    parser.setFeedUrl( writeRecentFeed( settingsDirectory ) );
    parser.setEnabled( true );
    setEnabledSinceDaysAgo( 60 );
    fetchAndWait( parser );
    parser.dismiss( pluginsUuid );
    REQUIRE( parser.items().size() == 1 );

    parser.setEnabled( false );
    REQUIRE( !parser.enabled() );
    REQUIRE( parser.items().isEmpty() );
    REQUIRE( !QSettings().contains( QfNewsParser::ENABLED_SINCE_SETTINGS_KEY ) );
    REQUIRE( !QSettings().contains( QfNewsParser::DISMISSED_UUIDS_SETTINGS_KEY ) );
  }
}
