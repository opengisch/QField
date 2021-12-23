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

#include "fileutils.h"
#include "qgismobileapp.h"

#include <qgsapplication.h>
#include <qgslogger.h>

#ifdef WITH_SPIX
#include <Spix/AnyRpcServer.h>
#include <Spix/QtQmlBot.h>
#endif
#include "qfield.h"

#include <QApplication>
#include <QDialog>
#include <QDir>
#include <QLabel>
#include <QLocale>
#include <QMainWindow>
#include <QSettings>
#include <QStandardPaths>
#include <QTranslator>
#include <QtWebView/QtWebView>

#if WITH_SENTRY
#include <sentry.h>
#endif

#ifdef ANDROID
#include <android/log.h>
#endif

static const char *
  logLevelForMessageType( QtMsgType msgType )
{
  switch ( msgType )
  {
    case QtDebugMsg:
      return "debug";
    case QtWarningMsg:
      return "warning";
    case QtCriticalMsg:
      return "error";
    case QtFatalMsg:
      return "fatal";
    case QtInfoMsg:
      Q_FALLTHROUGH();
    default:
      return "info";
  }
}

const char *const applicationName = "QField";
void qfMessageHandler( QtMsgType type, const QMessageLogContext &context, const QString &msg )
{
#if WITH_SENTRY
  sentry_value_t crumb
    = sentry_value_new_breadcrumb( "default", qUtf8Printable( msg ) );

  sentry_value_set_by_key(
    crumb, "category", sentry_value_new_string( context.category ) );
  sentry_value_set_by_key(
    crumb, "level", sentry_value_new_string( logLevelForMessageType( type ) ) );

  sentry_value_t location = sentry_value_new_object();
  sentry_value_set_by_key(
    location, "file", sentry_value_new_string( context.file ) );
  sentry_value_set_by_key(
    location, "line", sentry_value_new_int32( context.line ) );
  sentry_value_set_by_key( crumb, "data", location );

  sentry_add_breadcrumb( crumb );
#endif

  QString report = msg;
  if ( context.file && !QString( context.file ).isEmpty() )
  {
    report += " in file ";
    report += QString( context.file );
    report += " line ";
    report += QString::number( context.line );
  }

  if ( context.function && !QString( context.function ).isEmpty() )
  {
    report += +" function ";
    report += QString( context.function );
  }

#if ANDROID
  const char *const local = report.toLocal8Bit().constData();
  switch ( type )
  {
    case QtDebugMsg:
      __android_log_write( ANDROID_LOG_DEBUG, applicationName, local );
      break;
    case QtInfoMsg:
      __android_log_write( ANDROID_LOG_INFO, applicationName, local );
      break;
    case QtWarningMsg:
      __android_log_write( ANDROID_LOG_WARN, applicationName, local );
      break;
    case QtCriticalMsg:
      __android_log_write( ANDROID_LOG_ERROR, applicationName, local );
      break;
    case QtFatalMsg:
    default:
      __android_log_write( ANDROID_LOG_FATAL, applicationName, local );
      abort();
  }
#endif
}

int main( int argc, char **argv )
{
#if WITH_SENTRY
#ifndef ANDROID
  sentry_options_t *options = sentry_options_new();
  sentry_options_set_dsn( options, qfield::sentryDsn.toUtf8().constData() );
  // sentry_options_set_debug( options, 1 );
  sentry_init( options );
#endif

  // Make sure everything flushes
  auto sentryClose = qScopeGuard( [] { sentry_close(); } );

  sentry_capture_event( sentry_value_new_message_event(
    /*   level */ SENTRY_LEVEL_INFO,
    /*  logger */ "custom",
    /* message */ "It works!" ) );
#endif

  // A dummy app for reading settings that need to be used before constructing the real app
  QCoreApplication *dummyApp = new QCoreApplication( argc, argv );
  QCoreApplication::setOrganizationName( "OPENGIS.ch" );
  QCoreApplication::setOrganizationDomain( "opengis.ch" );
  QCoreApplication::setApplicationName( qfield::appName );
  QString customLanguage;
  {
    QSettings settings;
    customLanguage = settings.value( "/customLanguage", QString() ).toString();
  }
  delete dummyApp;

  if ( !customLanguage.isEmpty() )
    QgsApplication::setTranslation( customLanguage );

  PlatformUtilities::instance()->initSystem();

  QGuiApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
  QtWebView::initialize();

#if defined( Q_OS_ANDROID )
  QString projPath = PlatformUtilities::instance()->systemGenericDataLocation() + QStringLiteral( "/proj" );
  qputenv( "PROJ_LIB", projPath.toUtf8() );
  QgsApplication app( argc, argv, true, PlatformUtilities::instance()->systemGenericDataLocation() + QStringLiteral( "/qgis/resources" ), QStringLiteral( "mobile" ) );

  QSettings settings;

  app.setThemeName( settings.value( "/Themes", "default" ).toString() );
  app.setPrefixPath( "" QGIS_INSTALL_DIR, true );
  app.setPluginPath( PlatformUtilities::instance()->systemGenericDataLocation() + QStringLiteral( "/plugins" ) );
  app.setPkgDataPath( PlatformUtilities::instance()->systemGenericDataLocation() + QStringLiteral( "/qgis" ) );
#elif defined( Q_OS_IOS )
  QString projPath = PlatformUtilities::instance()->systemGenericDataLocation() + QStringLiteral( "/proj" );
  qputenv( "PROJ_LIB", projPath.toUtf8() );
  QgsApplication app( argc, argv, true, PlatformUtilities::instance()->systemGenericDataLocation() + QStringLiteral( "/qgis/resources" ), QStringLiteral( "mobile" ) );
  app.setPkgDataPath( PlatformUtilities::instance()->systemGenericDataLocation() + QStringLiteral( "/qgis" ) );
#else
  QgsApplication app( argc, argv, true );
  QSettings settings;
  app.setThemeName( settings.value( "/Themes", "default" ).toString() );

#if defined( Q_OS_WIN )
  qputenv( "GDAL_DATA", QDir::toNativeSeparators( app.applicationDirPath() + "/../share/gdal" ).toLocal8Bit() );
  qputenv( "PROJ_LIB", QDir::toNativeSeparators( app.applicationDirPath() + "/../share/proj" ).toLocal8Bit() );
#endif
#ifdef RELATIVE_PREFIX_PATH
  app.setPrefixPath( app.applicationDirPath() + "/..", true );
#else
  app.setPrefixPath( CMAKE_INSTALL_PREFIX, true );
#endif
#endif

  qInstallMessageHandler( qfMessageHandler );
  app.initQgis();

  //set NativeFormat for settings
  QSettings::setDefaultFormat( QSettings::NativeFormat );

  // Set up the QSettings environment must be done after qapp is created
  QCoreApplication::setOrganizationName( "OPENGIS.ch" );
  QCoreApplication::setOrganizationDomain( "opengis.ch" );
  QCoreApplication::setApplicationName( qfield::appName );

  QTranslator qfieldTranslator;
  QTranslator qtTranslator;
  if ( !customLanguage.isEmpty() )
  {
    qfieldTranslator.load( QStringLiteral( "qfield_%1" ).arg( customLanguage ), QStringLiteral( ":/i18n/" ), "_" );
    qtTranslator.load( QStringLiteral( "qt_%1" ).arg( customLanguage ), QStringLiteral( ":/i18n/" ), "_" );
  }
  if ( qfieldTranslator.isEmpty() )
    qfieldTranslator.load( QLocale(), "qfield", "_", ":/i18n/" );
  if ( qtTranslator.isEmpty() )
    qtTranslator.load( QLocale(), "qt", "_", ":/i18n/" );

  app.installTranslator( &qtTranslator );
  app.installTranslator( &qfieldTranslator );

  QgisMobileapp mApp( &app );

#ifdef WITH_SPIX
  spix::AnyRpcServer server;
  auto bot = new spix::QtQmlBot();
  bot->runTestServer( server );
#endif

  return app.exec();
}
