

# File main.cpp

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**main.cpp**](main_8cpp.md)

[Go to the documentation of this file](main_8cpp.md)


```C++
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
#include "translatormanager.h"
#if WITH_SENTRY
#include "sentry_wrapper.h"
#endif

#if defined( Q_OS_ANDROID )
#include "qfieldcloudservice.h"
#include "qfieldpositioningservice.h"
#endif

#include <qgsapplication.h>
#include <qgsauthmanager.h>
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
  QCoreApplication::setAttribute( Qt::AA_DisableShaderDiskCache );

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

void initAuthManager( QgsAuthManager *authManager )
{
#ifndef Q_OS_LINUX
  authManager->setPasswordHelperEnabled( false );
  if ( authManager->verifyMasterPassword( QStringLiteral( "qfield" ) ) )
  {
    // migrating authentication database
    authManager->setMasterPassword( QStringLiteral( "qfield" ) );
    authManager->setPasswordHelperEnabled( true );

    QRandomGenerator generator = QRandomGenerator::securelySeeded();
    QString pw;
    pw.resize( 32 );
    static const QString sPwChars = QStringLiteral( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-{}[]" );
    for ( int i = 0; i < pw.size(); ++i )
    {
      pw[i] = sPwChars.at( generator.bounded( 0, sPwChars.length() ) );
    }

    authManager->resetMasterPassword( pw, QStringLiteral( "qfield" ), false );
    authManager->passwordHelperSync();
  }

  authManager->setPasswordHelperEnabled( true );
  if ( !authManager->masterPasswordHashInDatabase() )
  {
    // if no master password set by user yet, just generate a new one and store it in the system keychain
    authManager->createAndStoreRandomMasterPasswordInKeyChain();
  }
#else
  authManager->setPasswordHelperEnabled( false );
  authManager->setMasterPassword( QStringLiteral( "qfield" ) );
#endif
}

int main( int argc, char **argv )
{
  qputenv( "QT_ANDROID_DISABLE_ACCESSIBILITY", "1" );

  QCoreApplication::setOrganizationName( "OPENGIS.ch" );
  QCoreApplication::setOrganizationDomain( "opengis.ch" );
  QCoreApplication::setApplicationName( qfield::appName );

  Q_INIT_RESOURCE( qml );

#if defined( Q_OS_ANDROID )
  if ( argc > 1 )
  {
    if ( strcmp( argv[1], "--cloudservice" ) == 0 )
    {
      // This service only deals with background attachment uploads;
      // it will terminate once all uploads are done
      QFieldCloudService service( argc, argv );

      PlatformUtilities *platformUtils = PlatformUtilities::instance();
      platformUtils->initSystem();

      // Let's make sure we have a writable path for the QGIS profile on every platform
      const QString profilePath = platformUtils->systemLocalDataLocation( QStringLiteral( "profiles/default" ) );
      QDir().mkdir( profilePath );

#ifdef RELATIVE_PREFIX_PATH
      qputenv( "SSL_CERT_FILE", QDir::toNativeSeparators( PlatformUtilities::instance()->systemSharedDataLocation() + "/cacert.pem" ).toLocal8Bit() );
      qputenv( "GDAL_DATA", QDir::toNativeSeparators( PlatformUtilities::instance()->systemSharedDataLocation() + "/gdal" ).toLocal8Bit() );
#endif

      // Workaround QgsApplication::initQgis crashing adding app fonts
      QDir profileDir( profilePath );
      profileDir.rename( QStringLiteral( "fonts" ), QStringLiteral( "fonts-disabled" ) );

      QgsApplication::init( profilePath );
      QgsApplication::initQgis();
#ifdef RELATIVE_PREFIX_PATH
      QgsApplication::setPkgDataPath( PlatformUtilities::instance()->systemSharedDataLocation() + QStringLiteral( "/qgis" ) );
#endif
      QgsApplication::createDatabase();
      initAuthManager( QgsApplication::authManager() );

      profileDir.rmdir( QStringLiteral( "fonts" ) );
      profileDir.rename( QStringLiteral( "fonts-disabled" ), QStringLiteral( "fonts" ) );

      service.initService();
      return service.exec();
    }
    else if ( strcmp( argv[1], "--positioningservice" ) == 0 )
    {
      QFieldPositioningService service( argc, argv );
      return service.exec();
    }
  }
#endif

  initGraphics();

  // Read settings, use a dummy app to get access to QSettings
  QCoreApplication *dummyApp = new QCoreApplication( argc, argv );
  const QSettings settings;
  const QString customLanguage = settings.value( "/customLanguage", QString() ).toString();

  PlatformUtilities *platformUtils = PlatformUtilities::instance();
  platformUtils->initSystem();

  // Let's make sure we have a writable path for the QGIS profile on every platform
  const QString profilePath = platformUtils->systemLocalDataLocation( QStringLiteral( "profiles/default" ) );
  QDir().mkdir( profilePath );

#if WITH_SENTRY
  const bool enableSentry = settings.value( "/enableInfoCollection", true ).toBool();
  if ( enableSentry )
    sentry_wrapper::init();
  // Make sure everything flushes when exiting the app
  auto sentryClose = qScopeGuard( [] { sentry_wrapper::close(); } );
#endif

  QTranslator *qfieldTranslator = TranslatorManager::instance()->qfieldTranslator();
  QTranslator *qtTranslator = TranslatorManager::instance()->qtTranslator();
  bool qfieldTranslatorLoaded = false;
  bool qtTranslatorLoaded = false;
  if ( !customLanguage.isEmpty() )
  {
    qfieldTranslatorLoaded = qfieldTranslator->load( QStringLiteral( "qfield_%1" ).arg( customLanguage ), QStringLiteral( ":/i18n/" ), "_" );
    qtTranslatorLoaded = qtTranslator->load( QStringLiteral( "qt_%1" ).arg( customLanguage ), QStringLiteral( ":/i18n/" ), "_" );
  }
  dummyApp->installTranslator( qtTranslator );
  dummyApp->installTranslator( qfieldTranslator );

  delete dummyApp;

  QtWebView::initialize();

  QgsApplication app( argc, argv, true, profilePath, QStringLiteral( "mobile" ) );

  if ( !qfieldTranslatorLoaded || qfieldTranslator->isEmpty() )
  {
    ( void ) qfieldTranslator->load( QStringLiteral( "qfield_%1" ).arg( QLocale().name() ), QStringLiteral( ":/i18n/" ), "_" );
  }
  if ( !qtTranslatorLoaded || qtTranslator->isEmpty() )
  {
    ( void ) qtTranslator->load( QStringLiteral( "qt_%1" ).arg( QLocale().name() ), QStringLiteral( ":/i18n/" ), "_" );
  }

  if ( !customLanguage.isEmpty() )
  {
    QLocale customLocale( customLanguage );
    QLocale::setDefault( customLocale );
    QgsApplication::setTranslation( customLanguage );
    // Set locale to emit QgsApplication's localeChanged signal
    QgsApplication::setLocale( QLocale() );
  }

  QLocale locale;
  const QString localeName = locale.name();
  const qsizetype localeTagSeparator = localeName.indexOf( QStringLiteral( "_" ) );
  QgsApplication::settingsLocaleUserLocale->setValue( localeName.mid( 0, localeTagSeparator ) );

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

#if defined( Q_OS_ANDROID ) || defined( Q_OS_IOS )
  for ( const QString &dataDir : dataDirs )
  {
    const QFileInfo pgServiceFileInfo( QStringLiteral( "%1/pg_service.conf" ).arg( dataDir ) );
    if ( pgServiceFileInfo.exists() && pgServiceFileInfo.isReadable() )
    {
      const QString systemLocalDataPath = platformUtils->systemLocalDataLocation( QString() );

      QFile pgServiceFile( QStringLiteral( "%1/pg_service.conf" ).arg( dataDir ) );
      pgServiceFile.open( QFile::ReadOnly | QFile::Text );
      QTextStream textStream( &pgServiceFile );
      QString psServiceFileContent = textStream.readAll();
      pgServiceFile.close();

      const QStringList keys = QStringList() << QStringLiteral( "sslrootcert" ) << QStringLiteral( "sslcert" ) << QStringLiteral( "sslkey" );
      for ( const QString &key : keys )
      {
        const QRegularExpression rx( QStringLiteral( "%1=(.*)" ).arg( key ) );
        QRegularExpressionMatchIterator matchIt = rx.globalMatch( psServiceFileContent );
        while ( matchIt.hasNext() )
        {
          const QRegularExpressionMatch match = matchIt.next();
          const QString fileName = match.captured( 1 ).trimmed();

          // Check if the file is relative to the pg_service.conf, in which case copy to user-owned location, use absolute path, and change permissions
          const QString filePath = QStringLiteral( "%1/%2" ).arg( dataDir, fileName );
          const QFileInfo fileInfo( filePath );
          if ( QFileInfo::exists( filePath ) )
          {
            const QString newFilePath = QStringLiteral( "%1/%2" ).arg( systemLocalDataPath, fileName );
            if ( QFileInfo::exists( newFilePath ) )
            {
              QFile newFile( newFilePath );
              newFile.remove();
            }
            QFile::copy( filePath, newFilePath );
            QFile::setPermissions( newFilePath, QFileDevice::ReadOwner | QFileDevice::WriteOwner );
            psServiceFileContent.replace( QStringLiteral( "%1=%2" ).arg( key, match.captured( 1 ) ), QStringLiteral( "%1=%2" ).arg( key, newFilePath ) );
          }
        }
      }

      const QString localPgServiceFileName = QStringLiteral( "%1/pg_service.conf" ).arg( systemLocalDataPath );
      QFile localPgServiceFile( localPgServiceFileName );
      localPgServiceFile.open( QFile::WriteOnly );
      localPgServiceFile.write( psServiceFileContent.toUtf8() );
      localPgServiceFile.close();

      setenv( "PGSYSCONFDIR", systemLocalDataPath.toUtf8(), true );
      break;
    }
  }
#endif

#if WITH_SENTRY
  sentry_wrapper::install_message_handler();
#endif
  app.initQgis();
  app.setThemeName( settings.value( "/Themes", "default" ).toString() );
#ifdef RELATIVE_PREFIX_PATH
  app.setPkgDataPath( PlatformUtilities::instance()->systemSharedDataLocation() + QStringLiteral( "/qgis" ) );
#endif
  app.createDatabase();

  initAuthManager( QgsApplication::authManager() );

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

  app.installTranslator( qtTranslator );
  app.installTranslator( qfieldTranslator );

  qputenv( "QT_QUICK_CONTROLS_STYLE", QByteArray( "Material" ) );
  qputenv( "QT_QUICK_CONTROLS_MATERIAL_VARIANT", QByteArray( "Dense" ) );

  QgisMobileapp mApp( &app );

#ifdef WITH_SPIX
  spix::AnyRpcServer server;
  auto bot = new spix::QtQmlBot();
  bot->runTestServer( server );
#endif

  return app.exec();
}
```


