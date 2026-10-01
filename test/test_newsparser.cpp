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

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QSignalSpy>
#include <QTemporaryDir>


class ScopedIniSettings
{
  public:
    explicit ScopedIniSettings( const QString &root )
      : mPreviousDefaultFormat( QSettings::defaultFormat() )
      , mPreviousOrganizationName( QCoreApplication::organizationName() )
      , mPreviousApplicationName( QCoreApplication::applicationName() )
    {
      QSettings probe( QSettings::IniFormat, QSettings::UserScope, QStringLiteral( "ProbeOrg" ), QStringLiteral( "ProbeApp" ) );
      mPreviousIniUserPath = QFileInfo( probe.fileName() ).absolutePath();

      QSettings::setDefaultFormat( QSettings::IniFormat );
      QSettings::setPath( QSettings::IniFormat, QSettings::UserScope, root );
      QCoreApplication::setOrganizationName( QStringLiteral( "QFieldUnitTests" ) );
      QCoreApplication::setApplicationName( QStringLiteral( "NewsParserTests" ) );
    }

    ~ScopedIniSettings()
    {
      QCoreApplication::setOrganizationName( mPreviousOrganizationName );
      QCoreApplication::setApplicationName( mPreviousApplicationName );
      QSettings::setPath( QSettings::IniFormat, QSettings::UserScope, mPreviousIniUserPath );
      QSettings::setDefaultFormat( mPreviousDefaultFormat );
    }

  private:
    QSettings::Format mPreviousDefaultFormat;
    QString mPreviousIniUserPath;
    QString mPreviousOrganizationName;
    QString mPreviousApplicationName;
};


static const QLatin1String enabledSinceKey( "/QField/News/EnabledSince" );
static const QLatin1String dismissedUrlsKey( "/QField/News/DismissedUrls" );

static const QUrl danubeUrl( QStringLiteral( "https://qfield.org/blog/2026/09/08/qfield-4.3-danube-summer-of-stability/" ) );
static const QUrl projectCreationUrl( QStringLiteral( "https://qfield.org/blog/2026/08/10/faster-project-creation/" ) );
static const QUrl pluginsUrl( QStringLiteral( "https://qfield.org/blog/2026/02/12/qfield-search-and-routing-plugins-updates/" ) );


static QUrl feedFixtureUrl()
{
  return QUrl::fromLocalFile( QStringLiteral( "%1/news_feed.xml" ).arg( TEST_DATA_DIR ) );
}

static QByteArray feedFixtureContent()
{
  QFile file( QStringLiteral( "%1/news_feed.xml" ).arg( TEST_DATA_DIR ) );
  REQUIRE( file.open( QIODevice::ReadOnly ) );
  return file.readAll();
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
  ScopedIniSettings scopedSettings( settingsDirectory.path() );

  SECTION( "parseFeed" )
  {
    const QList<QfNewsItem> items = QfNewsParser::parseFeed( feedFixtureContent() );
    REQUIRE( items.size() == 3 );

    REQUIRE( items[0].title == QStringLiteral( "QField 4.3 “Danube”: Summer of stability" ) );
    REQUIRE( items[0].description == QStringLiteral( "The next version of QField is here." ) );
    REQUIRE( items[0].publicationDate == QDateTime::fromString( QStringLiteral( "2026-09-08T00:02:00Z" ), Qt::ISODate ) );
    REQUIRE( items[0].imageUrl == QUrl( QStringLiteral( "https://qfield.org/blog/2026/09/08/qfield-4.3-danube-summer-of-stability/splash43.webp" ) ) );
    REQUIRE( items[0].url == danubeUrl );

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
    parser.setFeedUrl( feedFixtureUrl() );
    REQUIRE( !parser.enabled() );

    parser.fetch();
    REQUIRE( !parser.isFetching() );
    REQUIRE( parser.items().isEmpty() );
  }

  SECTION( "onlyNewsPublishedSinceEnabled" )
  {
    QfNewsParser parser;
    parser.setFeedUrl( feedFixtureUrl() );

    QSignalSpy enabledSpy( &parser, &QfNewsParser::enabledChanged );
    parser.setEnabled( true );
    REQUIRE( parser.enabled() );
    REQUIRE( enabledSpy.count() == 1 );
    REQUIRE( QSettings().value( enabledSinceKey ).toDateTime().isValid() );

    fetchAndWait( parser );
    REQUIRE( parser.items().isEmpty() );

    QSettings().setValue( enabledSinceKey, QDateTime::fromString( QStringLiteral( "2026-03-01T00:00:00Z" ), Qt::ISODate ) );
    fetchAndWait( parser );
    REQUIRE( parser.items().size() == 2 );
    REQUIRE( parser.items()[0].url == danubeUrl );
    REQUIRE( parser.items()[1].url == projectCreationUrl );
  }

  SECTION( "dismiss" )
  {
    QfNewsParser parser;
    parser.setFeedUrl( feedFixtureUrl() );
    parser.setEnabled( true );
    QSettings().setValue( enabledSinceKey, QDateTime::fromString( QStringLiteral( "2026-01-01T00:00:00Z" ), Qt::ISODate ) );
    fetchAndWait( parser );
    REQUIRE( parser.items().size() == 3 );

    QSignalSpy itemsSpy( &parser, &QfNewsParser::itemsChanged );
    parser.dismiss( projectCreationUrl );
    REQUIRE( itemsSpy.count() == 1 );
    REQUIRE( parser.items().size() == 2 );
    REQUIRE( parser.items()[0].url == danubeUrl );
    REQUIRE( parser.items()[1].url == pluginsUrl );

    QfNewsParser otherParser;
    otherParser.setFeedUrl( feedFixtureUrl() );
    fetchAndWait( otherParser );
    REQUIRE( otherParser.items().size() == 2 );
    REQUIRE( otherParser.items()[0].url == danubeUrl );
    REQUIRE( otherParser.items()[1].url == pluginsUrl );
    REQUIRE( QSettings().value( dismissedUrlsKey ).toStringList() == QStringList { projectCreationUrl.toString() } );
  }

  SECTION( "disablingClearsItems" )
  {
    QfNewsParser parser;
    parser.setFeedUrl( feedFixtureUrl() );
    parser.setEnabled( true );
    QSettings().setValue( enabledSinceKey, QDateTime::fromString( QStringLiteral( "2026-01-01T00:00:00Z" ), Qt::ISODate ) );
    fetchAndWait( parser );
    parser.dismiss( pluginsUrl );
    REQUIRE( parser.items().size() == 2 );

    parser.setEnabled( false );
    REQUIRE( !parser.enabled() );
    REQUIRE( parser.items().isEmpty() );
    REQUIRE( !QSettings().contains( enabledSinceKey ) );
    REQUIRE( !QSettings().contains( dismissedUrlsKey ) );
  }
}
