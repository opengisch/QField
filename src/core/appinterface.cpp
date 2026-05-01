/***************************************************************************
                            appinterface.cpp
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
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
#include "platformutilities.h"
#include "qfield.h"
#include "qfieldxmlhttprequest.h"
#include "qgismobileapp.h"
#include "translatormanager.h"
#if WITH_SENTRY
#include "sentry_wrapper.h"
#endif

#include <QCoreApplication>
#include <QDirIterator>
#include <QFileInfo>
#include <QImageReader>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QSettings>
#include <QTemporaryFile>
#include <QTranslator>
#include <qgsapplication.h>
#include <qgsauthmanager.h>
#include <qgsmessagelog.h>
#include <qgsnetworkaccessmanager.h>
#include <qgsproject.h>
#include <qgsruntimeprofiler.h>
#include <qgsziputils.h>

AppInterface *AppInterface::sAppInterface = nullptr;

AppInterface::AppInterface( QQmlEngine *engine )
  : mEngine( engine )
{
}

QgisMobileapp *AppInterface::app() const
{
  return qobject_cast<QgisMobileapp *>( mEngine );
}

QObject *AppInterface::rootObject() const
{
  QQmlApplicationEngine *appEngine = qobject_cast<QQmlApplicationEngine *>( mEngine );
  if ( appEngine )
  {
    return appEngine->rootObjects().isEmpty() ? nullptr : appEngine->rootObjects().at( 0 );
  }
  // engine-only path, findChild() walks whatever QQuickTest has parented to the engine
  return mEngine;
}

QObject *AppInterface::createHttpRequest() const
{
  QFieldXmlHttpRequest *request = new QFieldXmlHttpRequest();

  QObject *root = rootObject();
  if ( root && qmlEngine( root ) )
  {
    QQmlEngine::setObjectOwnership( request, QQmlEngine::CppOwnership );
  }

  return request;
}

QObject *AppInterface::findItemByObjectName( const QString &name ) const
{
  QObject *root = rootObject();
  return root ? root->findChild<QObject *>( name ) : nullptr;
}

void AppInterface::addItemToPluginsToolbar( QQuickItem *item ) const
{
  QObject *root = rootObject();
  if ( !root )
  {
    return;
  }

  QQuickItem *toolbar = root->findChild<QQuickItem *>( QStringLiteral( "pluginsToolbar" ) );
  if ( toolbar )
  {
    item->setParentItem( toolbar );
  }
}

void AppInterface::addItemToCanvasActionsToolbar( QQuickItem *item ) const
{
  QObject *root = rootObject();
  if ( !root )
  {
    return;
  }

  QQuickItem *toolbar = root->findChild<QQuickItem *>( QStringLiteral( "canvasMenuActionsToolbar" ) );
  if ( toolbar )
  {
    item->setParentItem( toolbar );
  }
}

void AppInterface::addItemToDashboardActionsToolbar( QQuickItem *item ) const
{
  QObject *root = rootObject();
  if ( !root )
  {
    return;
  }

  QQuickItem *toolbar = root->findChild<QQuickItem *>( QStringLiteral( "dashboardActionsToolbar" ) );
  if ( toolbar )
  {
    item->setParentItem( toolbar );
  }
}

void AppInterface::addItemToMainMenuActionsToolbar( QQuickItem *item ) const
{
  addItemToDashboardActionsToolbar( item );
}

QObject *AppInterface::mainWindow() const
{
  return rootObject();
}

QObject *AppInterface::mapCanvas() const
{
  QObject *root = rootObject();
  if ( !root )
  {
    return nullptr;
  }

  return root->findChild<QObject *>( QStringLiteral( "mapCanvas" ) );
}

QObject *AppInterface::positioning() const
{
  QObject *root = rootObject();
  if ( !root )
  {
    return nullptr;
  }

  return root->findChild<QObject *>( QStringLiteral( "positionSource" ) );
}

bool AppInterface::hasProjectOnLaunch() const
{
  if ( PlatformUtilities::instance()->hasQgsProject() )
  {
    return true;
  }
  else
  {
    if ( QSettings().value( "/QField/loadProjectOnLaunch", true ).toBool() )
    {
      const QString lastProjectFilePath = QSettings().value( QStringLiteral( "QField/lastProjectFilePath" ), QString() ).toString();
      if ( !lastProjectFilePath.isEmpty() && QFileInfo::exists( lastProjectFilePath ) )
      {
        return true;
      }
    }
  }
  return false;
}

bool AppInterface::loadFile( const QString &path, const QString &name )
{
  qInfo() << QStringLiteral( "AppInterface loading file: %1" ).arg( path );
  QgisMobileapp *mobileApp = app();
  if ( !mobileApp )
  {
    return false;
  }
  if ( QFileInfo::exists( path ) )
  {
    return mobileApp->loadProjectFile( path, name );
  }

  const QUrl url( path );
  return mobileApp->loadProjectFile( url.isLocalFile() ? url.toLocalFile() : url.path(), name );
}

void AppInterface::reloadProject()
{
  QgisMobileapp *mobileApp = app();
  if ( mobileApp )
  {
    mobileApp->reloadProjectFile();
  }
}

void AppInterface::readProject()
{
  QgisMobileapp *mobileApp = app();
  if ( mobileApp )
  {
    mobileApp->readProjectFile();
  }
}

QString AppInterface::readProjectEntry( const QString &scope, const QString &key, const QString &def ) const
{
  const QgisMobileapp *mobileApp = app();
  return mobileApp ? mobileApp->readProjectEntry( scope, key, def ) : def;
}

int AppInterface::readProjectNumEntry( const QString &scope, const QString &key, int def ) const
{
  const QgisMobileapp *mobileApp = app();
  return mobileApp ? mobileApp->readProjectNumEntry( scope, key, def ) : def;
}

double AppInterface::readProjectDoubleEntry( const QString &scope, const QString &key, double def ) const
{
  const QgisMobileapp *mobileApp = app();
  return mobileApp ? mobileApp->readProjectDoubleEntry( scope, key, def ) : def;
}

bool AppInterface::readProjectBoolEntry( const QString &scope, const QString &key, bool def ) const
{
  const QgisMobileapp *mobileApp = app();
  return mobileApp ? mobileApp->readProjectBoolEntry( scope, key, def ) : def;
}

bool AppInterface::print( const QString &layoutName )
{
  QgisMobileapp *mobileApp = app();
  return mobileApp ? mobileApp->print( layoutName ) : false;
}

bool AppInterface::printAtlasFeatures( const QString &layoutName, const QList<long long> &featureIds )
{
  QgisMobileapp *mobileApp = app();
  return mobileApp ? mobileApp->printAtlasFeatures( layoutName, featureIds ) : false;
}

void AppInterface::setScreenDimmerTimeout( int timeoutSeconds )
{
  QgisMobileapp *mobileApp = app();
  if ( mobileApp )
  {
    mobileApp->setScreenDimmerTimeout( timeoutSeconds );
  }
}

void AppInterface::setupNetworkProxy() const
{
  QgsNetworkAccessManager::instance()->setupDefaultProxyAndCache();
}

QVariantMap AppInterface::availableLanguages() const
{
  QVariantMap languages;
  QDirIterator it( QStringLiteral( ":/i18n/" ), { QStringLiteral( "*.qm" ) }, QDir::Files );
  while ( it.hasNext() )
  {
    it.next();
    if ( it.fileName().startsWith( "qfield_" ) )
    {
      const qsizetype delimiter = it.fileName().indexOf( '.' );
      const QString languageCode = it.fileName().mid( 7, delimiter - 7 );
      const bool hasCoutryCode = languageCode.indexOf( '_' ) > -1;

      const QLocale locale( languageCode );
      QString displayName;
      if ( languageCode == QStringLiteral( "en" ) )
      {
        displayName = QStringLiteral( "english" );
      }
      else if ( locale.nativeLanguageName().isEmpty() )
      {
        displayName = QStringLiteral( "code (%1)" ).arg( languageCode );
      }
      else
      {
        displayName = locale.nativeLanguageName().toLower() + ( hasCoutryCode ? QStringLiteral( " / %1" ).arg( locale.nativeTerritoryName() ) : QString() );
      }

      languages.insert( languageCode, displayName );
    }
  }
  return languages;
}

void AppInterface::changeLanguage( const QString &languageCode )
{
  if ( !languageCode.isEmpty() && !availableLanguages().contains( languageCode ) )
  {
    qWarning() << "Language code" << languageCode << "is not available, ignoring language change request";
    return;
  }

  QTranslator *qfieldTranslator = TranslatorManager::instance()->qfieldTranslator();
  QTranslator *qtTranslator = TranslatorManager::instance()->qtTranslator();

  QCoreApplication::removeTranslator( qtTranslator );
  QCoreApplication::removeTranslator( qfieldTranslator );

  if ( !qfieldTranslator->load( QStringLiteral( "qfield_%1" ).arg( languageCode ), QStringLiteral( ":/i18n/" ), "_" ) )
  {
    qWarning() << "Failed to load QField translation for" << languageCode;
  }
  if ( !qtTranslator->load( QStringLiteral( "qt_%1" ).arg( languageCode ), QStringLiteral( ":/i18n/" ), "_" ) )
  {
    qWarning() << "Failed to load Qt translation for" << languageCode;
  }

  QCoreApplication::installTranslator( qtTranslator );
  QCoreApplication::installTranslator( qfieldTranslator );

  QSettings settings;
  settings.setValue( QStringLiteral( "/customLanguage" ), languageCode );

  if ( !languageCode.isEmpty() )
  {
    QLocale customLocale( languageCode );
    QLocale::setDefault( customLocale );
    QgsApplication::setTranslation( languageCode );
    QgsApplication::setLocale( QLocale() );
  }
  else
  {
    QLocale systemLocale = QLocale::system();
    QLocale::setDefault( systemLocale );
    QgsApplication::setTranslation( systemLocale.name() );
    QgsApplication::setLocale( systemLocale );
  }

  QgisMobileapp *mobileApp = app();
  if ( mobileApp )
  {
    mobileApp->retranslate();
  }
}

bool AppInterface::isFileExtensionSupported( const QString &filename ) const
{
  const QFileInfo fi( filename );
  const QString suffix = fi.suffix().toLower();
  return SUPPORTED_PROJECT_EXTENSIONS.contains( suffix ) || SUPPORTED_VECTOR_EXTENSIONS.contains( suffix ) || SUPPORTED_RASTER_EXTENSIONS.contains( suffix );
}

bool AppInterface::isAuthenticationConfigurationAvailable( const QString &id ) const
{
  QgsAuthManager *authManager = QgsApplication::authManager();
  QgsAuthMethodConfigsMap configs = authManager->availableAuthMethodConfigs();
  return configs.contains( id );
}

void AppInterface::logMessage( const QString &message )
{
  QgsMessageLog::logMessage( message, QStringLiteral( "QField" ) );
}

void AppInterface::logRuntimeProfiler()
{
  QgsMessageLog::logMessage( QgsApplication::profiler()->asText(), QStringLiteral( "QField" ) );
}

void AppInterface::sendLog( const QString &message, const QString &cloudUser )
{
#if WITH_SENTRY
  sentry_wrapper::capture_event( message.toUtf8().constData(), cloudUser.toUtf8().constData() );
#endif
}

void AppInterface::initiateSentry() const
{
#if WITH_SENTRY
  sentry_wrapper::init();
#endif
}

void AppInterface::closeSentry() const
{
#if WITH_SENTRY
  sentry_wrapper::close();
#endif
}

void AppInterface::clearProject() const
{
  QgisMobileapp *mobileApp = app();
  if ( mobileApp )
  {
    mobileApp->clearProject();
  }
}

void AppInterface::importUrl( const QString &url, const QString &title, bool loadOnImport )
{
  QString sanitizedUrl = url.trimmed();
  if ( sanitizedUrl.isEmpty() )
  {
    return;
  }

  if ( !sanitizedUrl.contains( QRegularExpression( "^([a-z][a-z0-9+\\-.]*):" ) ) )
  {
    // Prepend HTTPS when the URL scheme is missing instead of assured failure
    sanitizedUrl = QStringLiteral( "https://%1" ).arg( sanitizedUrl );
  }

  const QString applicationDirectory = PlatformUtilities::instance()->applicationDirectory();
  if ( applicationDirectory.isEmpty() )
  {
    return;
  }

  QgsNetworkAccessManager *manager = QgsNetworkAccessManager::instance();
  QNetworkRequest request( ( QUrl( sanitizedUrl ) ) );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy );

  emit importTriggered( !title.isEmpty() ? title : request.url().fileName() );

  QNetworkReply *reply = manager->get( request );

  QTemporaryFile *temporaryFile = new QTemporaryFile( reply );
  temporaryFile->setFileTemplate( QStringLiteral( "%1/XXXXXXXXXXXX" ).arg( applicationDirectory ) );
  if ( !temporaryFile->open() )
  {
    reply->abort();
    return;
  }

  connect( reply, &QNetworkReply::downloadProgress, this, [this, reply, temporaryFile]( qint64 bytesReceived, qint64 bytesTotal ) {
    temporaryFile->write( reply->readAll() );
    if ( bytesTotal != 0 )
    {
      emit importProgress( static_cast<double>( bytesReceived ) / bytesTotal );
    }
  } );

  connect( reply, &QNetworkReply::finished, this, [this, url, reply, temporaryFile, applicationDirectory, loadOnImport]() {
    if ( reply->error() == QNetworkReply::NoError )
    {
      QString fileName = reply->url().fileName();
      QString contentDisposition = reply->header( QNetworkRequest::ContentDispositionHeader ).toString();
      if ( !contentDisposition.isEmpty() )
      {
        QRegularExpression rx( QStringLiteral( "filename=\"?([^\";]*)\"?" ) );
        QRegularExpressionMatch match = rx.match( contentDisposition );
        if ( match.hasMatch() )
        {
          fileName = match.captured( 1 );
        }
      }

      QFileInfo fileInfo = QFileInfo( fileName );
      const QString fileSuffix = fileInfo.suffix().toLower();
      const bool isProjectFile = fileSuffix == QLatin1String( "qgs" ) || fileSuffix == QLatin1String( "qgz" );

      QString filePath = QStringLiteral( "%1/%2/%3" ).arg( applicationDirectory, isProjectFile ? QLatin1String( "Imported Projects" ) : QLatin1String( "Imported Datasets" ), fileName );
      {
        int i = 0;
        while ( QFileInfo::exists( filePath ) )
        {
          filePath = QStringLiteral( "%1/%2/%3_%4.%5" ).arg( applicationDirectory, isProjectFile ? QLatin1String( "Imported Projects" ) : QLatin1String( "Imported Datasets" ), fileInfo.completeBaseName(), QString::number( ++i ), fileSuffix );
        }
      }
      QDir( QFileInfo( filePath ).absolutePath() ).mkpath( "." );

      temporaryFile->write( reply->readAll() );
      temporaryFile->setAutoRemove( false );
      temporaryFile->close();
      if ( temporaryFile->rename( filePath ) )
      {
        if ( fileSuffix == QLatin1String( "zip" ) )
        {
          // Check if this is a compressed project and handle accordingly
          QStringList zipFiles = QgsZipUtils::files( filePath );
          const bool isCompressedProject = std::find_if( zipFiles.begin(),
                                                         zipFiles.end(),
                                                         []( const QString &zipFile ) {
                                                           return zipFile.toLower().endsWith( QLatin1String( ".qgs" ) ) || zipFile.toLower().endsWith( QLatin1String( ".qgz" ) );
                                                         } )
                                           != zipFiles.end();
          if ( isCompressedProject )
          {
            QString zipDirectory = QStringLiteral( "%1/Imported Projects/%2" ).arg( applicationDirectory, fileInfo.baseName() );
            {
              int i = 0;
              while ( QFileInfo::exists( zipDirectory ) )
              {
                zipDirectory = QStringLiteral( "%1/Imported Projects/%2_%3" ).arg( applicationDirectory, fileInfo.baseName(), QString::number( ++i ) );
              }
            }
            QDir( zipDirectory ).mkpath( "." );

            if ( FileUtils::unzip( filePath, zipDirectory, zipFiles, false ) )
            {
              // we need to close the project to safely flush the gpkg files and avoid file lock on Windows
              QDirIterator it( zipDirectory, { QStringLiteral( "*.qgs" ), QStringLiteral( "*.qgz" ) }, QDir::Filter::Files, QDirIterator::Subdirectories );
              QStringList projectFilePaths;
              while ( it.hasNext() )
              {
                projectFilePaths << it.nextFileInfo().absoluteFilePath();
              }

              // Project archive successfully imported
              emit importEnded( loadOnImport && projectFilePaths.size() == 1 ? projectFilePaths.at( 0 ) : zipDirectory, url );
              return;
            }
            else
            {
              // Broken project archive, bail out
              QDir dir( zipDirectory );
              dir.removeRecursively();
              filePath.clear();
              emit importEnded();
              return;
            }
          }
        }

        // Dataset successfully imported
        QFileInfo fi( filePath );
        emit importEnded( loadOnImport ? fi.absoluteFilePath() : fi.isFile() ? fi.absolutePath()
                                                                             : fi.absoluteFilePath(),
                          url );
        return;
      }
    }

    emit importEnded();
  } );
}
