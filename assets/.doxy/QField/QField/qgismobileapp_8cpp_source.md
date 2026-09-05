

# File qgismobileapp.cpp

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qgismobileapp.cpp**](qgismobileapp_8cpp.md)

[Go to the documentation of this file](qgismobileapp_8cpp.md)


```C++
/***************************************************************************
                            qgismobileapp.cpp
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

#include <QApplication>

#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <stdlib.h>

// use GDAL VSI mechanism
#define CPL_SUPRESS_CPLUSPLUS //#spellok
#include "cpl_conv.h"
#include "cpl_string.h"
#include "cpl_vsi.h"

#ifdef WITH_BLUETOOTH
#endif
#ifdef WITH_SERIALPORT
#endif
#include "qf3d.h"
#include "qf3dterrainprovider.h"
#include "qfapp.h"
#include "qfappinterface.h"
#include "qfbarcodeimageprovider.h"
#include "qfcloudconnection.h"
#include "qfcloudproject.h"
#include "qfcloudprojectsmodel.h"
#include "qfcloudutils.h"
#include "qfcogoregistry.h"
#include "qfcore.h"
#include "qfdistancearea.h"
#include "qffeaturehistory.h"
#include "qffeaturelistmodel.h"
#include "qffeatureutils.h"
#include "qfgeometryutils.h"
#include "qfgnsspositioninformation.h"
#include "qfgui.h"
#include "qfidentifytool.h"
#include "qfield.h"
#include "qflayerobserver.h"
#include "qflayerresolver.h"
#include "qflayertreemapcanvasbridge.h"
#include "qflayertreemodel.h"
#include "qflayerutils.h"
#include "qflegendimageprovider.h"
#include "qflinepolygonshape.h"
#include "qflocalfilesimageprovider.h"
#include "qflocatormodelsuperbridge.h"
#include "qfmessagelogmodel.h"
#include "qfntripsourcetablefetcher.h"
#include "qfplatformutilities.h"
#include "qfpluginmodel.h"
#include "qfpositioningsource.h"
#include "qfprintlayoutlistmodel.h"
#include "qfprocessingalgorithm.h"
#include "qfprocessingalgorithmsmodel.h"
#include "qfprojectinfo.h"
#include "qfprojectsimageprovider.h"
#include "qfrecentprojectlistmodel.h"
#include "qfreferencingfeaturelistmodel.h"
#include "qfrubberbandmodel.h"
#include "qfrubberbandshape.h"
#include "qfscalebarmeasurement.h"
#include "qfsensorlistmodel.h"
#include "qfsnappingresult.h"
#include "qfsnappingutils.h"
#include "qftrackingmodel.h"
#include "qfurlhandler.h"
#include "qfvertexmodel.h"
#include "qgismobileapp.h"
#include "qgsproviderregistry.h"
#include "qgsprovidersublayerdetails.h"
#include "qgsquickmapcanvasmap.h"
#include "qgsquickmapsettings.h"

#include <QDateTime>
#include <QDesktopServices>
#include <QFileInfo>
#include <QFontDatabase>
#include <QPalette>
#include <QPermissions>
#include <QQmlFileSelector>
#include <QResource>
#include <QScreen>
#include <QSslConfiguration>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <qgsauthmanager.h>
#include <qgsbilinearrasterresampler.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsexpressionfunction.h>
#include <qgsfeature.h>
#include <qgsfield.h>
#include <qgsfieldconstraints.h>
#include <qgsfontmanager.h>
#include <qgsgeopackageprojectstorage.h>
#include <qgslayertree.h>
#include <qgslayertreemodel.h>
#include <qgslayertreeregistrybridge.h>
#include <qgslayoutatlas.h>
#include <qgslayoutexporter.h>
#include <qgslayoutitemlabel.h>
#include <qgslayoutitemmap.h>
#include <qgslayoutmanager.h>
#include <qgslayoutpagecollection.h>
#include <qgslocalizeddatapathregistry.h>
#include <qgslocator.h>
#include <qgsmaplayer.h>
#include <qgsmaplayerstyle.h>
#include <qgsmessagelog.h>
#include <qgsnetworkaccessmanager.h>
#include <qgsofflineediting.h>
#include <qgsprintlayout.h>
#include <qgsproject.h>
#include <qgsprojectelevationproperties.h>
#include <qgsprojectstorage.h>
#include <qgsprojectstorageregistry.h>
#include <qgsprojectstylesettings.h>
#include <qgsprojectviewsettings.h>
#include <qgsrasterlayer.h>
#include <qgsrasterresamplefilter.h>
#include <qgsscreenproperties.h>
#include <qgssettingsregistrycore.h>
#include <qgssinglesymbolrenderer.h>
#include <qgstemporalutils.h>
#include <qgsterrainprovider.h>
#include <qgsunittypes.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayertemporalproperties.h>
#include <qgsvectortilelayer.h>


#define QUOTE( string ) _QUOTE( string )
#define _QUOTE( string ) #string

QgisMobileapp::QgisMobileapp( QgsApplication *app, QObject *parent )
  : QQmlApplicationEngine( parent )
  , mIface( new QfAppInterface( this, this ) )
  , mFirstRenderingFlag( true )
  , mApp( app )
{
  // Set QGIS-specific core settings
  QgsSettingsRegistryCore::settingsEnableWMSTilePrefetching->setValue( true );

  // Increase maximum concurrent connections allowed
  QgsApplication::settingsConnectionPoolMaximumConcurrentConnections->setValue( 10 );

  mUrlHandler.reset( new QfUrlHandler( mIface, this ) );
  QDesktopServices::setUrlHandler( QStringLiteral( "qfield" ), mUrlHandler.get(), "handleUrl" );

  mMessageLogModel = new QfMessageLogModel( this );

  mCogoRegistry.reset( new QfCogoRegistry() );
  QfCogoRegistry::setInstance( mCogoRegistry.get() );

  QSettings settings;
  if ( QfPlatformUtilities::instance()->capabilities() & QfPlatformUtilities::AdjustBrightness )
  {
    mScreenDimmer = std::make_unique<QfScreenDimmer>( app );
    mScreenDimmer->setTimeout( settings.value( QStringLiteral( "dimTimeoutSeconds" ), 40 ).toInt() );
  }

  // we cannot use "/" as separator, since QGIS puts a suffix QGIS/31700 anyway
  const QString userAgent = QStringLiteral( "qfield|%1|%2|%3|" ).arg( Qfield::appVersion, Qfield::appVersionStr.normalized( QString::NormalizationForm_KD ), Qfield::gitRev );
  QgsNetworkAccessManager::settingsUserAgent->setValue( userAgent );
  QgsNetworkAccessManager::settingsNetworkTimeout->setValue( 60 * 1000 );
  QgsNetworkAccessManager::instance()->setupDefaultProxyAndCache();

  QfAppInterface::setInstance( mIface );

  //set the authHandler to qfield-handler
  std::unique_ptr<QgsNetworkAuthenticationHandler> handler;
  mAuthRequestHandler = new QfAppAuthRequestHandler();
  handler.reset( mAuthRequestHandler );
  QgsNetworkAccessManager::instance()->setAuthHandler( std::move( handler ) );

  QStringList dataDirs = QfPlatformUtilities::instance()->appDataDirs();
  if ( !dataDirs.isEmpty() )
  {
    //set localized data paths and register fonts
    QStringList localizedDataPaths;
    for ( const QString &dataDir : dataDirs )
    {
      localizedDataPaths << dataDir + QStringLiteral( "basemaps/" );

      // Add app-wide font(s)
      const QDir fontDir = QDir::cleanPath( QFileInfo( dataDir ).absoluteDir().path() + QDir::separator() + QStringLiteral( "fonts" ) );
      const QStringList fontExts = QStringList() << "*.ttf"
                                                 << "*.TTF"
                                                 << "*.otf"
                                                 << "*.OTF";
      const QStringList fontFiles = fontDir.entryList( fontExts, QDir::Files );
      for ( const QString &fontFile : fontFiles )
      {
        const int id = QFontDatabase::addApplicationFont( QDir::cleanPath( fontDir.path() + QDir::separator() + fontFile ) );
        qInfo() << QStringLiteral( "App-wide font registered: %1" ).arg( QDir::cleanPath( fontDir.path() + QDir::separator() + fontFile ) );
        if ( id == -1 )
        {
          QgsMessageLog::logMessage( tr( "Could not load font: %1" ).arg( fontFile ) );
        }
      }
    }
    QgsApplication::instance()->localizedDataPathRegistry()->setPaths( localizedDataPaths );
  }

  // Add app resource font(s)
  const QDir resourceFontDir = QStringLiteral( ":/fonts/" );
  const QStringList resourceFontExts = QStringList() << "*.ttf"
                                                     << "*.TTF"
                                                     << "*.otf"
                                                     << "*.OTF";
  const QStringList resourceFontFiles = resourceFontDir.entryList( resourceFontExts, QDir::Files );
  for ( const QString &resourceFontFile : resourceFontFiles )
  {
    const int id = QFontDatabase::addApplicationFont( QStringLiteral( ":/fonts/%1" ).arg( resourceFontFile ) );
    if ( id == -1 )
    {
      QgsMessageLog::logMessage( tr( "Could not load resource font: %1" ).arg( resourceFontFile ) );
    }
  }

  QgsFontManager::settingsDownloadMissingFonts->setValue( true );
  QgsApplication::fontManager()->enableFontDownloadsForSession();

  mProject = QgsProject::instance();
  connect( mProject, &QgsProject::aboutToBeCleared, this, [this] {
    if ( !mProjectFilePath.isEmpty() )
    {
      mPluginManager->unloadPlugin( QfPluginManager::findProjectPlugin( mProjectFilePath ) );
    }
  } );

  mTrackingModel = new QfTrackingModel( this );
  mFocusStack = std::make_unique<QfFocusStack>( this );
  mGpkgFlusher = std::make_unique<QgsGpkgFlusher>( mProject );
  mLayerObserver = std::make_unique<QfLayerObserver>( mProject );
  mFeatureHistory = std::make_unique<QfFeatureHistory>( mProject, mTrackingModel );
  mClipboardManager = std::make_unique<QfClipboardManager>( this );
  mFlatLayerTree = new QfFlatLayerTreeModel( mProject->layerTreeRoot(), mProject, this );

  QScreen *screen = QGuiApplication::primaryScreen();
  if ( screen )
  {
    mFlatLayerTree->layerTreeModel()->addTargetScreenProperties( QgsScreenProperties( screen ) );
  }

  mLegendImageProvider = new QfLegendImageProvider( mFlatLayerTree->layerTreeModel() );
  mAsyncLegendImageProvider = new QfAsyncLegendImageProvider( mFlatLayerTree->layerTreeModel() );
  mLocalFilesImageProvider = new QfLocalFilesImageProvider();
  mProjectsImageProvider = new QfProjectsImageProvider();
  mBarcodeImageProvider = new QfBarcodeImageProvider();

  mBookmarkModel = new QfBookmarkModel( QgsApplication::bookmarkManager(), mProject->bookmarkManager(), this );
  mDrawingTemplateModel = new QfDrawingTemplateModel( this );

  mPluginManager = new QfPluginManager( this );

  // cppcheck-suppress leakReturnValNotUsed
  initDeclarative( this );

  registerGlobalVariables();

  if ( !dataDirs.isEmpty() )
  {
    // import authentication method configurations
    for ( const QString &dataDir : dataDirs )
    {
      QDir configurationsDir( QStringLiteral( "%1/auth/" ).arg( dataDir ) );
      if ( configurationsDir.exists() )
      {
        const QStringList configurations = configurationsDir.entryList( QStringList() << QStringLiteral( "*.xml" ) << QStringLiteral( "*.XML" ), QDir::Files );
        for ( const QString &configuration : configurations )
        {
          QgsApplication::authManager()->importAuthenticationConfigsFromXml( configurationsDir.absoluteFilePath( configuration ), QString(), true );
        }
      }
    }
  }

  QfPlatformUtilities::instance()->setScreenLockPermission( false );

  loadFromModule( "org.qfield.app", "QgisMobileapp" );
  mMapCanvas = rootObjects().first()->findChild<QgsQuickMapCanvasMap *>();
  Q_ASSERT_X( mMapCanvas, "QML Init", "QgsQuickMapCanvasMap not found. It is likely that we failed to load the QML files. Check debug output for related messages." );
  mMapCanvas->mapSettings()->setProject( mProject );
  mBookmarkModel->setMapSettings( mMapCanvas->mapSettings() );
  mAsyncLegendImageProvider->setMapSettings( mMapCanvas->mapSettings() );

  mFlatLayerTree->layerTreeModel()->setLegendMapViewData( mMapCanvas->mapSettings()->mapSettings().mapUnitsPerPixel(),
                                                          static_cast<int>( std::round( mMapCanvas->mapSettings()->outputDpi() ) ), mMapCanvas->mapSettings()->mapSettings().scale() );

  mLayerTreeCanvasBridge = new QfLayerTreeMapCanvasBridge( mFlatLayerTree, mMapCanvas->mapSettings(), mTrackingModel, this );

  connect( this, &QgisMobileapp::loadProjectTriggered, mIface, &QfAppInterface::loadProjectTriggered );
  connect( this, &QgisMobileapp::loadProjectEnded, mIface, &QfAppInterface::loadProjectEnded );
  connect( this, &QgisMobileapp::setMapExtent, mIface, &QfAppInterface::setMapExtent );

  QTimer::singleShot( 1, this, &QgisMobileapp::onAfterFirstRendering );

  mOfflineEditing = new QgsOfflineEditing();

  mSettings.setValue( "/Map/searchRadiusMM", 5 );

  mAppMissingGridHandler = new QfAppMissingGridHandler( this );

  // Set GDAL option to fix loading of datasets within ZIP containers
  CPLSetConfigOption( "CPL_ZIP_ENCODING", "UTF-8" );

  connect( QgsApplication::instance(), &QGuiApplication::applicationStateChanged, this, []( Qt::ApplicationState state ) {
    switch ( state )
    {
      case Qt::ApplicationSuspended:
      case Qt::ApplicationHidden:
      case Qt::ApplicationInactive:
      {
        // Write settings to permanent storage
        QSettings().sync();
      }

      case Qt::ApplicationActive:
      {
        break;
      }
    }
  } );
}

void QgisMobileapp::initDeclarative( QQmlEngine *engine )
{
#if defined( Q_OS_ANDROID )
  QResource::registerResource( QStringLiteral( "assets:/android_rcc_bundle.rcc" ) );
#endif

  qRegisterMetaType<QMetaType::Type>( "QMetaType::Type" );

  qRegisterMetaType<QgsGeometry>( "QgsGeometry" );
  qRegisterMetaType<QgsFeature>( "QgsFeature" );
  qRegisterMetaType<QgsFeatureRequest>( "QgsFeatureRequest" );
  qRegisterMetaType<QgsFeatureIterator>( "QgsFeatureIterator" );
  qRegisterMetaType<QgsPoint>( "QgsPoint" );
  qRegisterMetaType<QgsPointXY>( "QgsPointXY" );
  qRegisterMetaType<QgsPointSequence>( "QgsPointSequence" );
  qRegisterMetaType<QgsCoordinateTransformContext>( "QgsCoordinateTransformContext" );
  qRegisterMetaType<QgsFeatureId>( "QgsFeatureId" );
  qRegisterMetaType<QgsFeatureIds>( "QgsFeatureIds" );
  qRegisterMetaType<QgsAttributes>( "QgsAttributes" );
  qRegisterMetaType<QgsSnappingConfig>( "QgsSnappingConfig" );
  qRegisterMetaType<QgsRelation>( "QgsRelation" );
  qRegisterMetaType<QgsPolymorphicRelation>( "QgsPolymorphicRelation" );
  qRegisterMetaType<QgsFields>( "QgsFields" );
  qRegisterMetaType<QgsField>( "QgsField" );
  qRegisterMetaType<QgsDefaultValue>( "QgsDefaultValue" );
  qRegisterMetaType<QgsFieldConstraints>( "QgsFieldConstraints" );
  qRegisterMetaType<QgsCoordinateReferenceSystem>( "QgsCoordinateReferenceSystem" );
  qRegisterMetaType<QgsUnitTypes>( "QgsUnitTypes" );
  qRegisterMetaType<QgsWkbTypes>( "QgsWkbTypes" );

  qRegisterMetaType<Qgis::GeometryType>( "Qgis::GeometryType" );
  qRegisterMetaType<Qgis::WkbType>( "Qgis::WkbType" );
  qRegisterMetaType<Qgis::LayerType>( "Qgis::LayerType" );
  qRegisterMetaType<Qgis::LayerFilters>( "Qgis::LayerFilters" );
  qRegisterMetaType<Qgis::DistanceUnit>( "Qgis::DistanceUnit" );
  qRegisterMetaType<Qgis::AreaUnit>( "Qgis::AreaUnit" );
  qRegisterMetaType<Qgis::AngleUnit>( "Qgis::AngleUnit" );
  qRegisterMetaType<Qgis::DeviceConnectionStatus>( "Qgis::DeviceConnectionStatus" );
  qRegisterMetaType<Qgis::SnappingMode>( "Qgis::SnappingMode" );

  qRegisterMetaType<QfPlatformUtilities::Capabilities>( "QfPlatformUtilities::Capabilities" );
  qRegisterMetaType<QfGeometryUtils::GeometryOperationResult>( "GeometryOperationResult" );
  qRegisterMetaType<QfCloudConnection::ConnectionStatus>( "QfCloudConnection::ConnectionStatus" );
  qRegisterMetaType<QfCloudUserInformation>( "QfCloudUserInformation" );
  qRegisterMetaType<QfCloudProject::ProjectStatus>( "QfCloudProject::ProjectStatus" );
  qRegisterMetaType<QfCloudProject::ProjectCheckout>( "QfCloudProject::ProjectCheckout" );
  qRegisterMetaType<QfCloudProject::ProjectModification>( "QfCloudProject::ProjectModification" );
  qRegisterMetaType<QfTracker::MeasureType>( "QfTracker::MeasureType" );
  qRegisterMetaType<QfPositioningSource::ElevationCorrectionMode>( "QfPositioningSource::ElevationCorrectionMode" );
  qRegisterMetaType<QfPositioningSource::NtripState>( "QfPositioningSource::NtripState" );

  QfCore::registerQmlTypes();
  Qf3D::registerQmlTypes();
  QfGui::registerQmlTypes();
  QfApp::registerQmlTypes();


  qRegisterMetaType<QfNtripMountPoint>( "QfNtripMountPoint" );


  qRegisterMetaType<QfGnssPositionDetails>( "QfGnssPositionDetails" );

  qRegisterMetaType<QfPluginInformation>( "QfPluginInformation" );

  qRegisterMetaType<QfSnappingResult>( "QfSnappingResult" );

  engine->rootContext()->setContextProperty( "platformUtilities", QfPlatformUtilities::instance() );
}

void QgisMobileapp::registerGlobalVariables()
{
  // Calculate device pixels
  rootContext()->setContextProperty( "qgisProject", mProject );
  rootContext()->setContextProperty( "iface", mIface );
  rootContext()->setContextProperty( "pluginManager", mPluginManager );
  rootContext()->setContextProperty( "settings", &mSettings );
  rootContext()->setContextProperty( "flatLayerTree", mFlatLayerTree );
  rootContext()->setContextProperty( "focusstack", mFocusStack.get() );
  rootContext()->setContextProperty( "WkbTypes", QVariant::fromValue<QgsWkbTypes>( mWkbTypes ) );
  rootContext()->setContextProperty( "UnitTypes", QVariant::fromValue<QgsUnitTypes>( mUnitTypes ) );
  rootContext()->setContextProperty( "ExifTools", QVariant::fromValue<QgsExifTools>( mExifTools ) );
  rootContext()->setContextProperty( "bookmarkModel", mBookmarkModel );
  rootContext()->setContextProperty( "gpkgFlusher", mGpkgFlusher.get() );
  rootContext()->setContextProperty( "layerObserver", mLayerObserver.get() );
  rootContext()->setContextProperty( "featureHistory", mFeatureHistory.get() );
  rootContext()->setContextProperty( "clipboardManager", mClipboardManager.get() );
  rootContext()->setContextProperty( "messageLogModel", mMessageLogModel );
  rootContext()->setContextProperty( "drawingTemplateModel", mDrawingTemplateModel );
  rootContext()->setContextProperty( "qfieldAuthRequestHandler", mAuthRequestHandler );
  rootContext()->setContextProperty( "trackingModel", mTrackingModel );
  addImageProvider( QLatin1String( "legend" ), mLegendImageProvider );
  addImageProvider( QLatin1String( "asynclegend" ), mAsyncLegendImageProvider );
  addImageProvider( QLatin1String( "localfiles" ), mLocalFilesImageProvider );
  addImageProvider( QLatin1String( "projects" ), mProjectsImageProvider );
  addImageProvider( QLatin1String( "barcode" ), mBarcodeImageProvider );
}


void QgisMobileapp::loadProjectQuirks()
{
  // force update of canvas, without automatic changes to extent and OTF projections
  bool autoEnableCrsTransform = mLayerTreeCanvasBridge->autoEnableCrsTransform();
  bool autoSetupOnFirstLayer = mLayerTreeCanvasBridge->autoSetupOnFirstLayer();
  mLayerTreeCanvasBridge->setAutoEnableCrsTransform( false );
  mLayerTreeCanvasBridge->setAutoSetupOnFirstLayer( false );

  mLayerTreeCanvasBridge->setCanvasLayers();

  if ( autoEnableCrsTransform )
    mLayerTreeCanvasBridge->setAutoEnableCrsTransform( true );

  if ( autoSetupOnFirstLayer )
    mLayerTreeCanvasBridge->setAutoSetupOnFirstLayer( true );
}

void QgisMobileapp::onAfterFirstRendering()
{
  // This should get triggered exactly once, so we disconnect it right away
  // disconnect( this, &QgisMobileapp::afterRendering, this, &QgisMobileapp::onAfterFirstRendering );
  if ( mFirstRenderingFlag )
  {
    mPluginManager->restoreAppPlugins();
    if ( QfPlatformUtilities::instance()->hasQfAction() )
    {
      QfPlatformUtilities::instance()->executeQfAction();
    }
    else if ( QfPlatformUtilities::instance()->hasQgsProject() )
    {
      QfPlatformUtilities::instance()->loadQgsProject();
    }
    else
    {
      if ( QSettings().value( "/QField/loadProjectOnLaunch", true ).toBool() )
      {
        QSettings settings;
        const QString defaultProject = settings.value( QStringLiteral( "QField/defaultProject" ), QString() ).toString();
        if ( !defaultProject.isEmpty() && QFileInfo::exists( defaultProject ) )
        {
          loadProjectFile( defaultProject );
        }
        else
        {
          const QString lastProjectFilePath = settings.value( QStringLiteral( "QField/lastProjectFilePath" ), QString() ).toString();
          if ( !lastProjectFilePath.isEmpty() && QFileInfo::exists( lastProjectFilePath ) )
          {
            loadProjectFile( lastProjectFilePath );
          }
        }
      }
    }
    rootObjects().first()->setProperty( "sceneLoaded", true );
    mFirstRenderingFlag = false;
  }
}

void QgisMobileapp::onMapCanvasRefreshed()
{
  disconnect( mMapCanvas, &QgsQuickMapCanvasMap::mapCanvasRefreshed, this, &QgisMobileapp::onMapCanvasRefreshed );
  if ( !mProjectFilePath.isEmpty() )
  {
    if ( !QFileInfo::exists( QStringLiteral( "%1.png" ).arg( mProjectFilePath ) ) )
    {
      saveProjectPreviewImage();
    }
  }
}

bool QgisMobileapp::loadProjectFile( const QString &path, const QString &name )
{
  QFileInfo fi( path );
  if ( !fi.exists() )
  {
    QgsMessageLog::logMessage( tr( "Can't load project, file \"%1\" does not exist" ).arg( path ), QStringLiteral( "QField" ), Qgis::Warning );
    return false;
  }

  const QString suffix = fi.suffix().toLower();
  if ( SUPPORTED_PROJECT_EXTENSIONS.contains( suffix ) || SUPPORTED_VECTOR_EXTENSIONS.contains( suffix ) || SUPPORTED_RASTER_EXTENSIONS.contains( suffix ) )
  {
    saveProjectPreviewImage();

    if ( !mProjectFilePath.isEmpty() )
    {
      mPluginManager->unloadPlugin( QfPluginManager::findProjectPlugin( mProjectFilePath ) );
    }
    mAuthRequestHandler->clearStoredRealms();

    mProjectFilePath = path;
    mProjectFileName = !name.isEmpty() ? name : fi.completeBaseName();

    emit loadProjectTriggered( mProjectFilePath, mProjectFileName );
    return true;
  }

  return false;
}

void QgisMobileapp::reloadProjectFile()
{
  if ( mProjectFilePath.isEmpty() )
    QgsMessageLog::logMessage( tr( "No project file currently opened" ), QStringLiteral( "QField" ), Qgis::Warning );

  emit loadProjectTriggered( mProjectFilePath, mProjectFileName );
}

void QgisMobileapp::prepareLocalizedDataPaths( const QString &projectFilePath )
{
  const QString cloudProjectId = QfCloudUtils::getProjectId( projectFilePath );
  QString cloudLocalizedDataPath;
  if ( !cloudProjectId.isEmpty() )
  {
    const QString cloudSharedDatasetsProjectId = QfCloudUtils::projectSetting( cloudProjectId, QStringLiteral( "sharedDatasetsProjectId" ) ).toString();
    if ( !cloudSharedDatasetsProjectId.isEmpty() )
    {
      const QString cloudUsername = QSettings().value( QStringLiteral( "/QFieldCloud/username" ) ).toString();
      cloudLocalizedDataPath = QStringLiteral( "%1/%2/%3" ).arg( QfCloudUtils::localCloudDirectory(), cloudUsername, cloudSharedDatasetsProjectId );
    }
  }

  QStringList localizedDataPaths = QgsApplication::instance()->localizedDataPathRegistry()->paths();
  localizedDataPaths.erase( std::remove_if( localizedDataPaths.begin(),
                                            localizedDataPaths.end(),
                                            []( const QString &path ) { return path.startsWith( QfCloudUtils::localCloudDirectory() ); } ),
                            localizedDataPaths.end() );
  if ( !cloudLocalizedDataPath.isEmpty() )
  {
    localizedDataPaths << cloudLocalizedDataPath;
  }
  QgsApplication::instance()->localizedDataPathRegistry()->setPaths( localizedDataPaths );
}

void QgisMobileapp::readProjectFile()
{
  QFileInfo fi( mProjectFilePath );
  if ( !fi.exists() )
    QgsMessageLog::logMessage( tr( "Can't read project, file \"%1\" does not exist" ).arg( mProjectFilePath ), QStringLiteral( "QField" ), Qgis::Warning );

  QSettings().setValue( QStringLiteral( "QField/lastProjectFilePath" ), mProjectFilePath );

  const QString suffix = fi.suffix().toLower();

  mProject->clear();
  mProject->layerTreeRegistryBridge()->setLayerInsertionMethod( Qgis::LayerTreeInsertionMethod::OptimalInInsertionGroup );

  mTrackingModel->reset();

  // load project file fonts if present
  const QStringList fontDirNames = QStringList() << QStringLiteral( ".fonts" ) << QStringLiteral( "fonts" );
  for ( const QString &fontDirName : fontDirNames )
  {
    const QDir fontDir = QDir::cleanPath( QFileInfo( mProjectFilePath ).absoluteDir().path() + QDir::separator() + fontDirName );
    const QStringList fontExts = QStringList() << "*.ttf"
                                               << "*.TTF"
                                               << "*.otf"
                                               << "*.OTF";
    const QStringList fontFiles = fontDir.entryList( fontExts, QDir::Files );
    for ( const QString &fontFile : fontFiles )
    {
      const int id = QFontDatabase::addApplicationFont( QDir::cleanPath( fontDir.path() + QDir::separator() + fontFile ) );
      qInfo() << QStringLiteral( "Project font registered: %1" ).arg( QDir::cleanPath( fontDir.path() + QDir::separator() + fontFile ) );
      if ( id == -1 )
      {
        QgsMessageLog::logMessage( tr( "Could not load font: %1" ).arg( fontFile ) );
      }
    }
  }

  // Load project file
  bool projectLoaded = false;
  if ( SUPPORTED_PROJECT_EXTENSIONS.contains( suffix ) )
  {
    prepareLocalizedDataPaths( mProjectFilePath );

    mProject->read( mProjectFilePath, Qgis::ProjectReadFlag::DontLoadProjectStyles | Qgis::ProjectReadFlag::DontLoad3DViews );
    projectLoaded = true;
  }
  else if ( suffix == QStringLiteral( "gpkg" ) )
  {
    QgsProjectStorage *storage = QgsApplication::projectStorageRegistry()->projectStorageFromType( "geopackage" );
    if ( storage )
    {
      const QStringList projectNames = storage->listProjects( mProjectFilePath );
      if ( !projectNames.isEmpty() )
      {
        QgsGeoPackageProjectUri projectUri { true, mProjectFilePath, projectNames.at( 0 ) };
        mProject->read( QgsGeoPackageProjectStorage::encodeUri( projectUri ), Qgis::ProjectReadFlag::DontLoadProjectStyles | Qgis::ProjectReadFlag::DontLoad3DViews );
        projectLoaded = true;
      }
    }
  }

  if ( projectLoaded )
  {
    mProject->writeEntry( QStringLiteral( "QField" ), QStringLiteral( "isDataset" ), false );
    if ( !QFileInfo::exists( mProject->homePath() ) )
    {
      // When a custom home path is not available, revert to the project file path
      QFileInfo projectFileInfo( mProjectFilePath );
      mProject->setPresetHomePath( projectFileInfo.absolutePath() );
    }

    if ( !mProject->error().isEmpty() )
    {
      QgsMessageLog::logMessage( mProject->error() );
    }
  }

  QString title;
  if ( mProject->fileName().startsWith( QfCloudUtils::localCloudDirectory() ) )
  {
    // Overwrite the title to match what is used in QFieldCloud
    const QString projectId = fi.dir().dirName();
    title = QSettings().value( QStringLiteral( "QFieldCloud/projects/%1/name" ).arg( projectId ), fi.fileName() ).toString();
  }
  else
  {
    title = mProject->title().isEmpty() ? mProjectFileName : mProject->title();
  }

  QList<QfRecentProjectListModel::RecentProject> projects = QfRecentProjectListModel::recentProjects();
  for ( int idx = 0; idx < projects.count(); idx++ )
  {
    if ( projects.at( idx ).path == mProjectFilePath )
    {
      projects.removeAt( idx );
      break;
    }
  }
  projects.insert( 0, QfRecentProjectListModel::RecentProject( QfRecentProjectListModel::LocalProject, title, mProjectFilePath ) );
  QfRecentProjectListModel::saveRecentProjects( projects );

  QList<QgsMapLayer *> vectorLayers;
  QList<QgsMapLayer *> vectorTileLayers;
  QList<QgsMapLayer *> rasterLayers;
  QgsCoordinateReferenceSystem crs;
  QgsRectangle extent;

  QStringList files;
  if ( suffix == QStringLiteral( "zip" ) || suffix == QStringLiteral( "7z" ) || suffix == QStringLiteral( "rar" ) )
  {
    // get list of files inside zip file
    QString tmpPath;
    char **papszSiblingFiles = VSIReadDirRecursive( QStringLiteral( "/vsi%1/%2" ).arg( suffix, mProjectFilePath ).toLocal8Bit().constData() );
    if ( papszSiblingFiles )
    {
      for ( int i = 0; papszSiblingFiles[i]; i++ )
      {
        tmpPath = papszSiblingFiles[i];
        // skip directories (files ending with /)
        if ( tmpPath.right( 1 ) != QLatin1String( "/" ) )
        {
          const QFileInfo tmpFi( tmpPath );
          if ( SUPPORTED_VECTOR_EXTENSIONS.contains( tmpFi.suffix().toLower() ) || SUPPORTED_RASTER_EXTENSIONS.contains( tmpFi.suffix().toLower() ) )
            files << QStringLiteral( "/vsi%1/%2/%3" ).arg( suffix, mProjectFilePath, tmpPath );
        }
      }
      CSLDestroy( papszSiblingFiles );
    }
  }
  else if ( !projectLoaded )
  {
    files << mProjectFilePath;
  }

  QgsProviderSublayerDetails::LayerOptions options( QgsProject::instance()->transformContext() );
  options.loadDefaultStyle = true;

  for ( auto filePath : std::as_const( files ) )
  {
    const QString fileSuffix = QFileInfo( filePath ).suffix().toLower();

    if ( fileSuffix == QLatin1String( "kmz" ) )
    {
      // GDAL's internal KML driver doesn't support KMZ, work around this limitation
      filePath = QStringLiteral( "/vsizip/%1/doc.kml" ).arg( mProjectFilePath );
    }
    else if ( fileSuffix == QLatin1String( "pdf" ) )
    {
      // Hardcode a DPI value of 300 for PDFs as most PDFs fail to register their proper resolution
      filePath += QStringLiteral( "|option:DPI=300" );
    }

    const QList<QgsProviderSublayerDetails> sublayers = QgsProviderRegistry::instance()->querySublayers( filePath, Qgis::SublayerQueryFlags() | Qgis::SublayerQueryFlag::ResolveGeometryType );
    for ( const QgsProviderSublayerDetails &sublayer : sublayers )
    {
      std::unique_ptr<QgsMapLayer> layer( sublayer.toLayer( options ) );
      if ( !layer || !layer->isValid() )
        continue;

      if ( layer->crs().isValid() )
      {
        if ( !crs.isValid() )
          crs = layer->crs();

        if ( !layer->extent().isEmpty() )
        {
          if ( crs != layer->crs() )
          {
            QgsCoordinateTransform transform( layer->crs(), crs, mProject->transformContext() );
            try
            {
              if ( extent.isEmpty() )
                extent = transform.transformBoundingBox( layer->extent() );
              else
                extent.combineExtentWith( transform.transformBoundingBox( layer->extent() ) );
            }
            catch ( const QgsCsException &exp )
            {
              Q_UNUSED( exp )
              // Ignore extent if it can't be transformed
            }
          }
          else
          {
            if ( extent.isEmpty() )
              extent = layer->extent();
            else
              extent.combineExtentWith( layer->extent() );
          }
        }
      }

      switch ( sublayer.type() )
      {
        case Qgis::LayerType::Vector:
          vectorLayers << layer.release();
          break;
        case Qgis::LayerType::Raster:
          rasterLayers << layer.release();
          break;
        case Qgis::LayerType::VectorTile:
          vectorTileLayers << layer.release();
          break;
        case Qgis::LayerType::Mesh:
        case Qgis::LayerType::Annotation:
        case Qgis::LayerType::PointCloud:
        case Qgis::LayerType::Group:
        case Qgis::LayerType::Plugin:
        case Qgis::LayerType::TiledScene:
          continue;
          break;
      }
    }
  }

  if ( vectorLayers.size() > 1 )
  {
    std::sort( vectorLayers.begin(), vectorLayers.end(), []( QgsMapLayer *a, QgsMapLayer *b ) {
      QgsVectorLayer *alayer = qobject_cast<QgsVectorLayer *>( a );
      QgsVectorLayer *blayer = qobject_cast<QgsVectorLayer *>( b );
      if ( alayer->geometryType() == Qgis::GeometryType::Point && blayer->geometryType() != Qgis::GeometryType::Point )
      {
        return true;
      }
      else if ( alayer->geometryType() == Qgis::GeometryType::Line && blayer->geometryType() == Qgis::GeometryType::Polygon )
      {
        return true;
      }
      else
      {
        return false;
      }
    } );
  }

  if ( vectorLayers.size() > 0 || rasterLayers.size() > 0 || vectorTileLayers.size() > 0 )
  {
    if ( crs.isValid() )
    {
      QSettings settings;
      const QString fileAssociationProject = settings.value( QStringLiteral( "QField/baseMapProject" ), QString() ).toString();
      if ( !fileAssociationProject.isEmpty() && QFile::exists( fileAssociationProject ) )
      {
        prepareLocalizedDataPaths( fileAssociationProject );
        mProject->read( fileAssociationProject, Qgis::ProjectReadFlag::DontLoadProjectStyles | Qgis::ProjectReadFlag::DontLoad3DViews );
      }
      else
      {
        const QStringList dataDirs = QfPlatformUtilities::instance()->appDataDirs();
        bool projectFound = false;
        for ( const QString &dataDir : dataDirs )
        {
          if ( QFile::exists( dataDir + QStringLiteral( "basemap.qgs" ) ) )
          {
            projectFound = true;
            mProject->read( dataDir + QStringLiteral( "basemap.qgs" ), Qgis::ProjectReadFlag::DontLoadProjectStyles | Qgis::ProjectReadFlag::DontLoad3DViews );
            break;
          }
          else if ( QFile::exists( dataDir + QStringLiteral( "basemap.qgz" ) ) )
          {
            projectFound = true;
            mProject->read( dataDir + QStringLiteral( "basemap.qgs" ), Qgis::ProjectReadFlag::DontLoadProjectStyles | Qgis::ProjectReadFlag::DontLoad3DViews );
            break;
          }
        }
        if ( !projectFound )
        {
          mProject->clear();

          // Add a default basemap
          mProject->addMapLayers( QList<QgsMapLayer *>() << QfLayerUtils::createBasemap() );
        }
      }

      if ( !mProject->error().isEmpty() )
      {
        QgsMessageLog::logMessage( mProject->error() );
      }
    }
    else
    {
      mProject->clear();
    }

    mProject->setCrs( crs );
    mProject->setEllipsoid( crs.ellipsoidAcronym() );
    mProject->setTitle( mProjectFileName );
    mProject->setPresetHomePath( fi.absolutePath() );
    mProject->writeEntry( QStringLiteral( "QField" ), QStringLiteral( "isDataset" ), true );

    for ( QgsMapLayer *l : std::as_const( vectorTileLayers ) )
    {
      QgsVectorTileLayer *vtlayer = qobject_cast<QgsVectorTileLayer *>( l );
      bool ok;
      vtlayer->loadDefaultStyle( ok );
    }
    mProject->addMapLayers( vectorTileLayers );

    for ( QgsMapLayer *l : std::as_const( rasterLayers ) )
    {
      QgsRasterLayer *rlayer = qobject_cast<QgsRasterLayer *>( l );
      bool ok;
      rlayer->loadDefaultStyle( ok );
      if ( !ok && fi.size() < 50000000 )
      {
        // If the raster size is reasonably small, apply nicer resampling settings
        rlayer->resampleFilter()->setZoomedInResampler( new QgsBilinearRasterResampler() );
        rlayer->resampleFilter()->setZoomedOutResampler( new QgsBilinearRasterResampler() );
        rlayer->resampleFilter()->setMaxOversampling( 2.0 );
      }
    }
    mProject->addMapLayers( rasterLayers );

    bool hasTemporalLayers = false;
    for ( QgsMapLayer *l : std::as_const( vectorLayers ) )
    {
      QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( l );
      bool ok;
      vlayer->loadDefaultStyle( ok );
      if ( !ok )
      {
        QfLayerUtils::setDefaultRenderer( vlayer, mProject );
      }

      if ( !vlayer->labeling() )
      {
        QfLayerUtils::setDefaultLabeling( vlayer, mProject );
      }

      const QgsFields fields = vlayer->fields();
      int temporalFieldIndex = -1;
      for ( int i = 0; i < fields.size(); i++ )
      {
        if ( fields[i].type() == QMetaType::QDateTime || fields[i].type() == QMetaType::QDate )
        {
          if ( temporalFieldIndex == -1 )
          {
            temporalFieldIndex = i;
          }
          else
          {
            // Be super conservative, if more than one temporal field is present, don't auto setup
            temporalFieldIndex = -1;
            break;
          }
        }
      }
      if ( temporalFieldIndex > 0 )
      {
        hasTemporalLayers = true;
        QgsVectorLayerTemporalProperties *temporalProperties = static_cast<QgsVectorLayerTemporalProperties *>( vlayer->temporalProperties() );
        temporalProperties->setStartField( fields[temporalFieldIndex].name() );
        temporalProperties->setMode( Qgis::VectorTemporalMode::FeatureDateTimeInstantFromField );
        temporalProperties->setLimitMode( Qgis::VectorTemporalLimitMode::IncludeBeginIncludeEnd );
        temporalProperties->setAccumulateFeatures( false );
        temporalProperties->setIsActive( true );
      }
    }
    mProject->addMapLayers( vectorLayers );

    if ( hasTemporalLayers )
    {
      const QgsDateTimeRange range = QgsTemporalUtils::calculateTemporalRangeForProject( mProject );
      mMapCanvas->mapSettings()->setTemporalBegin( range.begin() );
      mMapCanvas->mapSettings()->setTemporalEnd( range.end() );
      mMapCanvas->mapSettings()->setIsTemporal( false );
    }

    if ( suffix.compare( QLatin1String( "pdf" ) ) == 0 )
    {
      // Geospatial PDFs should have vector layers hidden by default
      for ( QgsMapLayer *layer : vectorLayers )
      {
        mProject->layerTreeRoot()->findLayer( layer->id() )->setItemVisibilityChecked( false );
      }
    }
  }

  if ( mProject->elevationProperties()->terrainProvider()->type() == QStringLiteral( "flat" ) && qgsDoubleNear( mProject->elevationProperties()->terrainProvider()->offset(), 0.0 ) && qgsDoubleNear( mProject->elevationProperties()->terrainProvider()->scale(), 1.0 ) )
  {
    QgsRasterLayer *elevationLayer = QfLayerUtils::createOnlineElevationLayer();
    mProject->addMapLayer( elevationLayer, false, true );
    QgsRasterDemTerrainProvider *terrainProvider = new QgsRasterDemTerrainProvider();
    terrainProvider->setLayer( elevationLayer );
    mProject->elevationProperties()->setTerrainProvider( terrainProvider );
  }

  loadProjectQuirks();

  // Restore project information (extent, customized style, layer visibility, etc.)
  QSettings settings;
  const QStringList parts = settings.value( QStringLiteral( "/qgis/projectInfo/%1/extent" ).arg( mProjectFilePath ), QString() ).toString().split( '|' );
  if ( parts.size() == 4 )
  {
    extent.setXMinimum( parts[0].toDouble() );
    extent.setXMaximum( parts[1].toDouble() );
    extent.setYMinimum( parts[2].toDouble() );
    extent.setYMaximum( parts[3].toDouble() );
    mMapCanvas->mapSettings()->setExtent( extent );
  }
  else if ( !extent.isNull() )
  {
    if ( extent.width() == 0.0 || extent.height() == 0.0 )
    {
      // If all of the features are at the one point, buffer the
      // rectangle a bit. If they are all at zero, do something a bit
      // more crude.
      if ( extent.xMinimum() == 0.0 && extent.xMaximum() == 0.0 && extent.yMinimum() == 0.0 && extent.yMaximum() == 0.0 )
      {
        extent.set( -1.0, -1.0, 1.0, 1.0 );
      }
      else
      {
        const double padFactor = 1e-8;
        const double widthPad = extent.xMinimum() * padFactor;
        const double heightPad = extent.yMinimum() * padFactor;
        const double xmin = extent.xMinimum() - widthPad;
        const double xmax = extent.xMaximum() + widthPad;
        const double ymin = extent.yMinimum() - heightPad;
        const double ymax = extent.yMaximum() + heightPad;
        extent.set( xmin, ymin, xmax, ymax );
      }
    }

    // Add a bit of buffer so datasets don't touch the very edge of the map on the screen
    mMapCanvas->mapSettings()->setExtent( extent.buffered( extent.width() * 0.02 ) );
  }

  QfProjectInfo::restoreSettings( mProjectFilePath, mProject, mMapCanvas, mFlatLayerTree );
  emit loadProjectEnded( mProjectFilePath, mProjectFileName );
  mTrackingModel->createProjectTrackers( mProject );

  connect( mMapCanvas, &QgsQuickMapCanvasMap::mapCanvasRefreshed, this, &QgisMobileapp::onMapCanvasRefreshed );

  const QString projectPluginPath = QfPluginManager::findProjectPlugin( mProjectFilePath );
  if ( !projectPluginPath.isEmpty() )
  {
    bool skipPermission = false;

    const QString cloudProjectId = QfCloudUtils::getProjectId( projectPluginPath );
    if ( !cloudProjectId.isEmpty() )
    {
      skipPermission = QfCloudUtils::projectSetting( cloudProjectId, QStringLiteral( "userRoleOrigin" ), QStringLiteral( "public" ) ).toString() != QStringLiteral( "public" );
    }

    mPluginManager->loadPlugin( projectPluginPath, tr( "Project Plugin" ), skipPermission, true );
  }
}

QString QgisMobileapp::readProjectEntry( const QString &scope, const QString &key, const QString &def ) const
{
  if ( !mProject )
    return def;

  return mProject->readEntry( scope, key, def );
}

int QgisMobileapp::readProjectNumEntry( const QString &scope, const QString &key, int def ) const
{
  if ( !mProject )
    return def;

  return mProject->readNumEntry( scope, key, def );
}

double QgisMobileapp::readProjectDoubleEntry( const QString &scope, const QString &key, double def ) const
{
  if ( !mProject )
    return def;

  return mProject->readDoubleEntry( scope, key, def );
}

bool QgisMobileapp::readProjectBoolEntry( const QString &scope, const QString &key, bool def ) const
{
  if ( !mProject )
    return def;

  return mProject->readBoolEntry( scope, key, def );
}

bool QgisMobileapp::print( const QString &layoutName )
{
  const QList<QgsPrintLayout *> printLayouts = mProject->layoutManager()->printLayouts();
  QgsPrintLayout *layoutToPrint = nullptr;
  std::unique_ptr<QgsPrintLayout> templateLayout;
  if ( layoutName.isEmpty() && printLayouts.isEmpty() )
  {
    QFile templateFile( QStringLiteral( ":/templates/layout.qpt" ) );
    QDomDocument templateDoc;
    templateDoc.setContent( &templateFile );

    templateLayout = std::make_unique<QgsPrintLayout>( QgsProject::instance() );
    bool loadedOK = false;
    QList<QgsLayoutItem *> items = templateLayout->loadFromTemplate( templateDoc, QgsReadWriteContext(), true, &loadedOK );
    if ( !loadedOK )
    {
      return false;
    }

    for ( QgsLayoutItem *item : items )
    {
      if ( item->type() == QgsLayoutItemRegistry::LayoutLabel && item->id() == QStringLiteral( "Title" ) )
      {
        QgsLayoutItemLabel *labelItem = qobject_cast<QgsLayoutItemLabel *>( item );
        labelItem->setText( tr( "Map printed on %1 using QField" ).arg( "[%format_date(now(), 'yyyy-MM-dd @ hh:mm')%]" ) );
      }
    }
    layoutToPrint = templateLayout.get();
  }
  else
  {
    auto match = std::find_if( printLayouts.begin(), printLayouts.end(), [&layoutName]( QgsPrintLayout *layout ) { return layout->name() == layoutName || layoutName.isEmpty(); } );
    if ( match != printLayouts.end() )
    {
      layoutToPrint = *match;
    }
  }

  if ( !layoutToPrint || layoutToPrint->pageCollection()->pageCount() == 0 )
    return false;

  const QString destination = QStringLiteral( "%1/layouts/%2-%3.pdf" ).arg( mProject->homePath(), layoutToPrint->name(), QDateTime::currentDateTime().toString( QStringLiteral( "yyyyMMdd_hhmmss" ) ) );

  if ( !layoutToPrint->atlas() || !layoutToPrint->atlas()->enabled() )
  {
    if ( layoutToPrint->referenceMap() )
      layoutToPrint->referenceMap()->zoomToExtent( mMapCanvas->mapSettings()->visibleExtent() );
    layoutToPrint->refresh();

    QgsLayoutExporter exporter = QgsLayoutExporter( layoutToPrint );

    QgsLayoutExporter::PdfExportSettings pdfSettings;
    pdfSettings.rasterizeWholeImage = layoutToPrint->customProperty( QStringLiteral( "rasterize" ), false ).toBool();
    pdfSettings.dpi = layoutToPrint->renderContext().dpi();
    pdfSettings.appendGeoreference = true;
    pdfSettings.exportMetadata = true;
    pdfSettings.simplifyGeometries = true;
    QgsLayoutExporter::ExportResult result = exporter.exportToPdf( destination, pdfSettings );

    if ( result == QgsLayoutExporter::Success )
      QfPlatformUtilities::instance()->open( destination );

    return result == QgsLayoutExporter::Success ? true : false;
  }
  else
  {
    bool success = printAtlas( layoutToPrint, destination );
    if ( success )
    {
      if ( layoutToPrint->customProperty( QStringLiteral( "singleFile" ), true ).toBool() )
      {
        QfPlatformUtilities::instance()->open( destination );
      }
      else
      {
        QfPlatformUtilities::instance()->open( mProject->homePath() );
      }
    }
    return success;
  }
}

bool QgisMobileapp::printAtlasFeatures( const QString &layoutName, const QList<long long> &featureIds )
{
  const QList<QgsPrintLayout *> printLayouts = mProject->layoutManager()->printLayouts();
  QgsPrintLayout *layoutToPrint = nullptr;
  auto match = std::find_if( printLayouts.begin(), printLayouts.end(), [&layoutName]( QgsPrintLayout *layout ) { return layout->name() == layoutName; } );
  if ( match != printLayouts.end() )
  {
    layoutToPrint = *match;
  }

  if ( !layoutToPrint || !layoutToPrint->atlas() )
    return false;

  QStringList ids;
  for ( const auto id : featureIds )
  {
    ids << QString::number( id );
  }

  QString error;
  const QString priorFilterExpression = layoutToPrint->atlas()->filterExpression();
  const bool priorFilterFeatures = layoutToPrint->atlas()->filterFeatures();

  layoutToPrint->atlas()->setFilterExpression( QStringLiteral( "@id IN (%1)" ).arg( ids.join( ',' ) ), error );
  layoutToPrint->atlas()->setFilterFeatures( true );
  layoutToPrint->atlas()->updateFeatures();

  const QString destination = QStringLiteral( "%1/layouts/%2-%3.pdf" ).arg( mProject->homePath(), layoutToPrint->name(), QDateTime::currentDateTime().toString( QStringLiteral( "yyyyMMdd_hhmmss" ) ) );
  QString finalDestination;
  const bool destinationSingleFile = layoutToPrint->customProperty( QStringLiteral( "singleFile" ), true ).toBool();
  if ( !destinationSingleFile && ids.size() == 1 )
  {
    layoutToPrint->atlas()->first();
    finalDestination = mProject->homePath() + '/' + layoutToPrint->atlas()->currentFilename() + QStringLiteral( ".pdf" );
  }
  else
  {
    finalDestination = destination;
  }
  const bool success = printAtlas( layoutToPrint, destination );

  layoutToPrint->atlas()->setFilterExpression( priorFilterExpression, error );
  layoutToPrint->atlas()->setFilterFeatures( priorFilterFeatures );

  if ( success )
  {
    if ( destinationSingleFile || ids.size() == 1 )
    {
      QfPlatformUtilities::instance()->open( finalDestination );
    }
    else
    {
      QfPlatformUtilities::instance()->open( mProject->homePath() );
    }
  }
  return success;
}

bool QgisMobileapp::printAtlas( QgsPrintLayout *layoutToPrint, const QString &destination )
{
  QString error;

  QVector<double> mapScales = layoutToPrint->project()->viewSettings()->mapScales();
  bool hasProjectScales( layoutToPrint->project()->viewSettings()->useProjectScales() );
  if ( !hasProjectScales || mapScales.isEmpty() )
  {
    // default to global map tool scales
    const QStringList scales = Qgis::defaultProjectScales().split( ',' );
    for ( const QString &scale : scales )
    {
      QStringList parts( scale.split( ':' ) );
      if ( parts.size() == 2 )
      {
        mapScales.push_back( parts[1].toDouble() );
      }
    }
  }

  QgsLayoutExporter::PdfExportSettings pdfSettings;
  pdfSettings.rasterizeWholeImage = layoutToPrint->customProperty( QStringLiteral( "rasterize" ), false ).toBool();
  pdfSettings.dpi = layoutToPrint->renderContext().dpi();
  pdfSettings.appendGeoreference = true;
  pdfSettings.exportMetadata = true;
  pdfSettings.simplifyGeometries = true;
  pdfSettings.predefinedMapScales = mapScales;

  if ( layoutToPrint->atlas()->updateFeatures() )
  {
    QgsLayoutExporter exporter = QgsLayoutExporter( layoutToPrint );
    QgsLayoutExporter::ExportResult result;

    if ( layoutToPrint->customProperty( QStringLiteral( "singleFile" ), true ).toBool() )
    {
      result = exporter.exportToPdf( layoutToPrint->atlas(), destination, pdfSettings, error );
    }
    else
    {
      result = exporter.exportToPdfs( layoutToPrint->atlas(), destination, pdfSettings, error );
    }

    return result == QgsLayoutExporter::Success ? true : false;
  }

  return false;
}

void QgisMobileapp::setScreenDimmerTimeout( int timeoutSeconds )
{
  if ( mScreenDimmer )
  {
    mScreenDimmer->setTimeout( timeoutSeconds );
  }
}

bool QgisMobileapp::event( QEvent *event )
{
  if ( event->type() == QEvent::Close )
  {
    quit();
  }

  return QQmlApplicationEngine::event( event );
}

void QgisMobileapp::clearProject()
{
  mAuthRequestHandler->clearStoredRealms();

  mProject->clear();
  mProjectFileName = QString();
  mProjectFilePath = QString();
}

void QgisMobileapp::saveProjectPreviewImage()
{
  if ( !mProjectFilePath.isEmpty() && mMapCanvas && !mMapCanvas->isRendering() )
  {
    const QImage grab = mMapCanvas->image();
    if ( !grab.isNull() )
    {
      const int pixels = std::min( grab.width(), grab.height() );
      const QRect rect( ( grab.width() - pixels ) / 2, ( grab.height() - pixels ) / 2, pixels, pixels );
      const QImage img = grab.copy( rect );
      img.save( QStringLiteral( "%1.png" ).arg( mProjectFilePath ) );
    }
  }
}

QgisMobileapp::~QgisMobileapp()
{
  QfPlatformUtilities::instance()->stopPositioningService();

  saveProjectPreviewImage();

  mPluginManager->unloadPlugins();

  delete mOfflineEditing;
  mProject->clear();

  QgsApplication::taskManager()->cancelAll();

  mApp->exitQgis();
  QMetaObject::invokeMethod( mApp, &QgsApplication::quit, Qt::QueuedConnection );
}
```


