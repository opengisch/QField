/***************************************************************************
                            main.cpp  -  main for qgis mobileapp
                            based on src/browser/main.cpp
                              -------------------
              begin                : Wed Apr 04 10:48:28 CET 2012
              copyright            : (C) 2012 by Marco Bernasocchi
              email                : marco@bernawebdesign.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "platformutilities.h"
#include "qfield.h"
#include "qgismobileapp.h"
#if WITH_SENTRY
#include "sentry_wrapper.h"
#endif

#if defined( Q_OS_ANDROID )
#include "qfieldservice.h"
#endif

#include <qgsapplication.h>
#include <qgslogger.h>
#include <qgsprojutils.h>
#include <qgsstyle.h>

#ifdef WITH_SPIX
#include <Spix/AnyRpcServer.h>
#include <Spix/QtQmlBot.h>
#endif

#include <QApplication>
#include <QDialog>
#include <QDir>
#include <QEventLoop>
#include <QLabel>
#include <QLocale>
#include <QMainWindow>
#include <QSettings>
#include <QStandardPaths>
#include <QTranslator>
#include <QtWebView/QtWebView>

#include <proj.h>

#if HAVE_STATIC_QCA_PLUGINS
#include <QtPlugin>
Q_IMPORT_PLUGIN( opensslPlugin )
#endif

void initGraphics()
{
#ifdef WITH_SPIX
  // Set antialiasing method to vertex to get same antialiasing across environments
  qputenv( "QSG_ANTIALIASING_METHOD", "vertex" );
#endif

#if not defined( Q_OS_ANDROID )
  // Enables antialiasing in QML scenes
  // Avoid enabling on Android OS as it leads to serious regressions on old devices
  QSurfaceFormat format;
  format.setSamples( 4 );
  QSurfaceFormat::setDefaultFormat( format );
#endif
}

int main( int argc, char **argv )
{
  if ( argc > 1 && strcmp( argv[1], "--service" ) == 0 )
  {
    QCoreApplication::setOrganizationName( "OPENGIS.ch" );
    QCoreApplication::setOrganizationDomain( "opengis.ch" );
    QCoreApplication::setApplicationName( qfield::appName );

#if defined( Q_OS_ANDROID )
    // For now the service only deals with background attachment uploads and will terminate once all uploads are done
    QFieldService app( argc, argv );
#endif
    return 0;
  }

  initGraphics();

  // Read settings, use a dummy app to get access to QSettings
  QCoreApplication *dummyApp = new QCoreApplication( argc, argv );
  QCoreApplication::setOrganizationName( "OPENGIS.ch" );
  QCoreApplication::setOrganizationDomain( "opengis.ch" );
  QCoreApplication::setApplicationName( qfield::appName );
  const QSettings settings;
  const QString customLanguage = settings.value( "/customLanguage", QString() ).toString();

#if WITH_SENTRY
  const bool enableSentry = settings.value( "/enableInfoCollection", true ).toBool();
  if ( enableSentry )
    sentry_wrapper::init();
  // Make sure everything flushes when exiting the app
  auto sentryClose = qScopeGuard( [] { sentry_wrapper::close(); } );
#endif

  Q_INIT_RESOURCE( qml );

  QTranslator qfieldTranslator;
  QTranslator qtTranslator;
  bool qfieldTranslatorLoaded = false;
  bool qtTranslatorLoaded = false;
  if ( !customLanguage.isEmpty() )
  {
    qfieldTranslatorLoaded = qfieldTranslator.load( QStringLiteral( "qfield_%1" ).arg( customLanguage ), QStringLiteral( ":/i18n/" ), "_" );
    qtTranslatorLoaded = qtTranslator.load( QStringLiteral( "qt_%1" ).arg( customLanguage ), QStringLiteral( ":/i18n/" ), "_" );
  }
  if ( !qfieldTranslatorLoaded || qfieldTranslator.isEmpty() )
    ( void ) qfieldTranslator.load( QLocale(), "qfield", "_", ":/i18n/" );
  if ( !qtTranslatorLoaded || qtTranslator.isEmpty() )
    ( void ) qtTranslator.load( QLocale(), "qt", "_", ":/i18n/" );

  dummyApp->installTranslator( &qtTranslator );
  dummyApp->installTranslator( &qfieldTranslator );

  QtWebView::initialize();

  PlatformUtilities *platformUtils = PlatformUtilities::instance();
  platformUtils->initSystem();

  // Let's make sure we have a writable path for the qgis_profile on every platform
  const QString profilePath = platformUtils->systemLocalDataLocation( QStringLiteral( "/qgis_profile" ) );
  QDir().mkdir( profilePath );
  delete dummyApp;

  QgsApplication app( argc, argv, true, profilePath, QStringLiteral( "mobile" ) );

  if ( !customLanguage.isEmpty() )
  {
    QLocale customLocale( customLanguage );
    QLocale::setDefault( customLocale );
    QgsApplication::setTranslation( customLanguage );
    // Set locale to emit QgsApplication's localeChanged signal
    QgsApplication::setLocale( QLocale() );
  }

  const QString qfieldFontName( qgetenv( "QFIELD_FONT_NAME" ) );
  if ( !qfieldFontName.isEmpty() )
  {
    const QString qfieldFont( qgetenv( "QFIELD_FONT_TTF" ) );
    const int qfieldFontSize = QString( qgetenv( "QFIELD_FONT_SIZE" ) ).toInt();
    QFontDatabase::addApplicationFont( qfieldFont );
    app.setFont( QFont( qfieldFontName, qfieldFontSize ) );
  }

  QStringList projSearchPaths = QgsProjUtils::searchPaths();
#ifdef RELATIVE_PREFIX_PATH
  qputenv( "SSL_CERT_FILE", QDir::toNativeSeparators( PlatformUtilities::instance()->systemSharedDataLocation() + "/cacert.pem" ).toLocal8Bit() );
  qputenv( "GDAL_DATA", QDir::toNativeSeparators( PlatformUtilities::instance()->systemSharedDataLocation() + "/gdal" ).toLocal8Bit() );
  projSearchPaths << QDir::toNativeSeparators( PlatformUtilities::instance()->systemSharedDataLocation() + "/proj" );
  qInfo() << "Proj path: " << projSearchPaths.constLast();
#else
  app.setPrefixPath( QGIS_PREFIX_PATH, true );
  qInfo() << "Proj path: {System}";
#endif

  // add extra proj search path to allow copying of transformation grid files
  const QStringList dataDirs = PlatformUtilities::instance()->appDataDirs();
  for ( const QString &dataDir : dataDirs )
  {
    projSearchPaths << QStringLiteral( "%1/proj/" ).arg( dataDir );
  }

  if ( !projSearchPaths.isEmpty() )
  {
    char **newPaths = new char *[projSearchPaths.count()];
    for ( int i = 0; i < projSearchPaths.count(); ++i )
    {
      newPaths[i] = strdup( projSearchPaths.at( i ).toUtf8().constData() );
    }
    proj_context_set_search_paths( nullptr, static_cast<int>( projSearchPaths.count() ), newPaths );
    for ( int i = 0; i < projSearchPaths.count(); ++i )
    {
      free( newPaths[i] );
    }
    delete[] newPaths;
  }

#if WITH_SENTRY
  sentry_wrapper::install_message_handler();
#endif
  app.initQgis();
  app.setThemeName( settings.value( "/Themes", "default" ).toString() );
#ifdef RELATIVE_PREFIX_PATH
  app.setPkgDataPath( PlatformUtilities::instance()->systemSharedDataLocation() + QStringLiteral( "/qgis" ) );
#endif
  app.createDatabase();

  if ( !qfieldFontName.isEmpty() )
  {
    QgsStyle *defaultStyle = QgsStyle::defaultStyle();
    QgsTextFormat textFormat = defaultStyle->defaultTextFormat();
    QFont font = textFormat.font();
    font.setFamily( qfieldFontName );
    textFormat.setFont( font );
    defaultStyle->addTextFormat( QStringLiteral( "Default" ), textFormat, true );
  }

  QSettings::setDefaultFormat( QSettings::NativeFormat );

  // Set up the QSettings environment must be done after qapp is created
  QCoreApplication::setOrganizationName( "OPENGIS.ch" );
  QCoreApplication::setOrganizationDomain( "opengis.ch" );
  QCoreApplication::setApplicationName( qfield::appName );

  app.installTranslator( &qtTranslator );
  app.installTranslator( &qfieldTranslator );

  qputenv( "QT_QUICK_CONTROLS_STYLE", QByteArray( "Material" ) );

  QgisMobileapp mApp( &app );

#ifdef WITH_SPIX
  spix::AnyRpcServer server;
  auto bot = new spix::QtQmlBot();
  bot->runTestServer( server );
#endif

  return app.exec();
}
