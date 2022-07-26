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

#include "appinterface.h"
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

#include <proj.h>

#if WITH_SENTRY
#include <sentry.h>
#endif

#ifdef ANDROID
#include <android/log.h>
#endif

static QtMessageHandler originalMessageHandler = nullptr;

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

#if ANDROID
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

  if ( originalMessageHandler )
    originalMessageHandler( type, context, msg );
}

void initGraphics()
{
  // Enables antialiasing in QML scenes
  QSurfaceFormat format;
  format.setSamples( 4 );
  QSurfaceFormat::setDefaultFormat( format );
  QGuiApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
}

int main( int argc, char **argv )
{
  initGraphics();

  // Read settings, use a dummy app to get access to QSettings
  QCoreApplication *dummyApp = new QCoreApplication( argc, argv );
  QCoreApplication::setOrganizationName( "OPENGIS.ch" );
  QCoreApplication::setOrganizationDomain( "opengis.ch" );
  QCoreApplication::setApplicationName( qfield::appName );
  QString customLanguage;
  bool enableSentry = false;
  {
    QSettings settings;
    customLanguage = settings.value( "/customLanguage", QString() ).toString();
    enableSentry = settings.value( "/enableInfoCollection", true ).toBool();
  }
  delete dummyApp;

  // Init resources
  Q_INIT_RESOURCE( qml );

#if WITH_SENTRY
  if ( enableSentry )
  {
    PlatformUtilities::instance()->initiateSentry();
    // Make sure everything flushes when exiting the app
  }
  auto sentryClose = qScopeGuard( [] { sentry_close(); } );
#else
  Q_UNUSED( enableSentry );
#endif

  QtWebView::initialize();

  if ( !customLanguage.isEmpty() )
    QgsApplication::setTranslation( customLanguage );

  PlatformUtilities *platformUtils = PlatformUtilities::instance();
  platformUtils->initSystem();

  QSettings settings;
  // Let's make sure we have a writable path for the qgis_profile on every platform
  const QString profilePath = platformUtils->systemLocalDataLocation( QStringLiteral( "/qgis_profile" ) );
  QDir().mkdir( profilePath );

  QgsApplication app( argc, argv, true, profilePath, QStringLiteral( "mobile" ) );
  app.setThemeName( settings.value( "/Themes", "default" ).toString() );
  app.setPkgDataPath( PlatformUtilities::instance()->systemGenericDataLocation() + QStringLiteral( "/qgis" ) );
  app.createDatabase();

#ifdef RELATIVE_PREFIX_PATH
  app.setPrefixPath( app.applicationDirPath() + "/..", true );
  qputenv( "GDAL_DATA", QDir::toNativeSeparators( app.applicationDirPath() + "/../share/gdal" ).toLocal8Bit() );
  const QString projPath( QDir::toNativeSeparators( app.applicationDirPath() + "/../share/proj" ) );
#else
  const QString projPath = PlatformUtilities::instance()->systemGenericDataLocation() + QStringLiteral( "/proj/data" );
#endif

  // cppcheck-suppress knownConditionTrueFalse
  // cppcheck-suppress reademptycontainer
  if ( !projPath.isNull() )
  {
    qInfo() << "Proj path: " << projPath;
    const char *projPaths[] { projPath.toUtf8().constData() };
    proj_context_set_search_paths( nullptr, 1, projPaths );
  }
  else
  {
    qInfo() << "Proj path: {System}";
  }

  originalMessageHandler = qInstallMessageHandler( qfMessageHandler );
  app.initQgis();

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

#if WITH_SENTRY
  QObject::connect( AppInterface::instance(), &AppInterface::submitLog, []( const QString &message ) {
    sentry_capture_event( sentry_value_new_message_event(
      SENTRY_LEVEL_INFO,
      "custom",
      message.toUtf8().constData() ) );
  } );
#endif

#ifdef WITH_SPIX
  spix::AnyRpcServer server;
  auto bot = new spix::QtQmlBot();
  bot->runTestServer( server );
#endif

  return app.exec();
}
