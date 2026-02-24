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
#include "gdal_version.h"

#ifdef WITH_BLUETOOTH
#include "bluetoothdevicemodel.h"
#include "bluetoothreceiver.h"
#endif
#ifdef WITH_SERIALPORT
#include "serialportmodel.h"
#include "serialportreceiver.h"
#endif
#include "appexpressioncontextscopesgenerator.h"
#include "appinterface.h"
#include "attributeformmodel.h"
#include "audiorecorder.h"
#include "badlayerhandler.h"
#include "barcodedecoder.h"
#include "barcodeimageprovider.h"
#include "changelogcontents.h"
#include "cogoexecutor.h"
#include "cogooperation.h"
#include "cogooperationsmodel.h"
#include "cogoregistry.h"
#include "coordinatereferencesystemutils.h"
#include "deltafilewrapper.h"
#include "deltalistmodel.h"
#include "digitizinglogger.h"
#include "distancearea.h"
#include "drawingcanvas.h"
#include "expressioncontextutils.h"
#include "expressionevaluator.h"
#include "expressionvariablemodel.h"
#include "externalstorage.h"
#include "featurechecklistmodel.h"
#include "featurehistory.h"
#include "featurelistextentcontroller.h"
#include "featurelistmodel.h"
#include "featurelistmodelselection.h"
#include "featuremodel.h"
#include "featureutils.h"
#include "fileutils.h"
#include "focusstack.h"
#include "geofencer.h"
#include "geometry.h"
#include "geometryeditorsmodel.h"
#include "geometryutils.h"
#include "gnsspositioninformation.h"
#include "gridmodel.h"
#include "identifytool.h"
#include "layerobserver.h"
#include "layerresolver.h"
#include "layertreemapcanvasbridge.h"
#include "layertreemodel.h"
#include "layerutils.h"
#include "legendimageprovider.h"
#include "linepolygonshape.h"
#include "localfilesimageprovider.h"
#include "localfilesmodel.h"
#include "locatormodelsuperbridge.h"
#include "maplayermodel.h"
#include "maptoscreen.h"
#include "messagelogmodel.h"
#include "navigation.h"
#include "navigationmodel.h"
#include "nearfieldreader.h"
#include "orderedrelationmodel.h"
#include "parametizedimage.h"
#include "permissions.h"
#include "platformutilities.h"
#include "pluginmodel.h"
#include "positioning.h"
#include "positioningdevicemodel.h"
#include "positioninginformationmodel.h"
#include "positioningutils.h"
#include "printlayoutlistmodel.h"
#include "processingalgorithm.h"
#include "processingalgorithmparametersmodel.h"
#include "processingalgorithmsmodel.h"
#include "processingutils.h"
#include "projectinfo.h"
#include "projectsimageprovider.h"
#include "projectsource.h"
#include "projectutils.h"
#include "qfield.h"
#include "qfieldcloudconnection.h"
#include "qfieldcloudproject.h"
#include "qfieldcloudprojectsmodel.h"
#include "qfieldcloudutils.h"
#include "qfieldlocatorfilter.h"
#include "qfieldurlhandler.h"
#include "qgismobileapp.h"
#include "qgsgeometrywrapper.h"
#include "qgsproviderregistry.h"
#include "qgsprovidersublayerdetails.h"
#include "qgsquickcoordinatetransformer.h"
#include "qgsquickelevationprofilecanvas.h"
#include "qgsquickmapcanvasmap.h"
#include "qgsquickmapsettings.h"
#include "qgsquickmaptransform.h"
#include "quick3dmaptexturedata.h"
#include "quick3drubberbandgeometry.h"
#include "quick3dterraingeometry.h"
#include "quick3dterrainprovider.h"
#include "recentprojectlistmodel.h"
#include "referencingfeaturelistmodel.h"
#include "relationutils.h"
#include "resourcesource.h"
#include "rubberbandmodel.h"
#include "rubberbandshape.h"
#include "scalebarmeasurement.h"
#include "sensorlistmodel.h"
#include "snappingresult.h"
#include "snappingutils.h"
#include "stringutils.h"
#include "submodel.h"
#include "theme.h"
#include "trackingmodel.h"
#include "urlutils.h"
#include "valuemapmodel.h"
#include "vertexmodel.h"
#include "webdavconnection.h"

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
#include <QStyleHints>
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
#include <qgslocatorcontext.h>
#include <qgslocatormodel.h>
#include <qgsmaplayer.h>
#include <qgsmaplayerstyle.h>
#include <qgsmapthemecollection.h>
#include <qgsmessagelog.h>
#include <qgsnetworkaccessmanager.h>
#include <qgsofflineediting.h>
#include <qgsprintlayout.h>
#include <qgsproject.h>
#include <qgsprojectdisplaysettings.h>
#include <qgsprojectelevationproperties.h>
#include <qgsprojectstorage.h>
#include <qgsprojectstorageregistry.h>
#include <qgsprojectstylesettings.h>
#include <qgsprojectviewsettings.h>
#include <qgsrasterlayer.h>
#include <qgsrasterresamplefilter.h>
#include <qgsrelationmanager.h>
#include <qgssettingsregistrycore.h>
#include <qgssinglesymbolrenderer.h>
#include <qgssnappingutils.h>
#include <qgstemporalutils.h>
#include <qgsterrainprovider.h>
#include <qgsunittypes.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayereditbuffer.h>
#include <qgsvectorlayertemporalproperties.h>


#define QUOTE( string ) _QUOTE( string )
#define _QUOTE( string ) #string

QgisMobileapp::QgisMobileapp( QgsApplication *app, QObject *parent )
  : QQmlApplicationEngine( parent )
  , mIface( new AppInterface( this ) )
  , mFirstRenderingFlag( true )
  , mApp( app )
{
  // Set QGIS-specific core settings
  QgsSettingsRegistryCore::settingsEnableWMSTilePrefetching->setValue( true );

  // Increase maximum concurrent connections allowed
  QgsApplication::settingsConnectionPoolMaximumConcurrentConnections->setValue( 10 );

  // Set a nicer default hyperlink color to be used in QML Text items
  QPalette palette = app->palette();
  palette.setColor( QPalette::Link, QColor( 128, 204, 40 ) );
  palette.setColor( QPalette::LinkVisited, QColor( 128, 204, 40 ) );
  app->setPalette( palette );

  mUrlHandler.reset( new QFieldUrlHandler( mIface, this ) );
  QDesktopServices::setUrlHandler( QStringLiteral( "qfield" ), mUrlHandler.get(), "handleUrl" );

  mMessageLogModel = new MessageLogModel( this );

  mCogoRegistry.reset( new CogoRegistry() );
  CogoRegistry::setInstance( mCogoRegistry.get() );

  QSettings settings;
  if ( PlatformUtilities::instance()->capabilities() & PlatformUtilities::AdjustBrightness )
  {
    mScreenDimmer = std::make_unique<ScreenDimmer>( app );
    mScreenDimmer->setTimeout( settings.value( QStringLiteral( "dimTimeoutSeconds" ), 40 ).toInt() );
  }

  QgsNetworkAccessManager::settingsNetworkTimeout->setValue( 60 * 1000 );

  // we cannot use "/" as separator, since QGIS puts a suffix QGIS/31700 anyway
  const QString userAgent = QStringLiteral( "qfield|%1|%2|%3|" ).arg( qfield::appVersion, qfield::appVersionStr.normalized( QString::NormalizationForm_KD ), qfield::gitRev );
  settings.setValue( QStringLiteral( "/qgis/networkAndProxy/userAgent" ), userAgent );

  AppInterface::setInstance( mIface );

  //set the authHandler to qfield-handler
  std::unique_ptr<QgsNetworkAuthenticationHandler> handler;
  mAuthRequestHandler = new QFieldAppAuthRequestHandler();
  handler.reset( mAuthRequestHandler );
  QgsNetworkAccessManager::instance()->setAuthHandler( std::move( handler ) );

  QStringList dataDirs = PlatformUtilities::instance()->appDataDirs();
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

  QgsApplication::fontManager()->enableFontDownloadsForSession();

  mProject = QgsProject::instance();
  connect( mProject, &QgsProject::aboutToBeCleared, this, [this] {
    if ( !mProjectFilePath.isEmpty() )
    {
      mPluginManager->unloadPlugin( PluginManager::findProjectPlugin( mProjectFilePath ) );
    }
  } );

  mTrackingModel = new TrackingModel();
  mGpkgFlusher = std::make_unique<QgsGpkgFlusher>( mProject );
  mLayerObserver = std::make_unique<LayerObserver>( mProject );
  mFeatureHistory = std::make_unique<FeatureHistory>( mProject, mTrackingModel );
  mClipboardManager = std::make_unique<ClipboardManager>( this );
  mFlatLayerTree = new FlatLayerTreeModel( mProject->layerTreeRoot(), mProject, this );
  mLegendImageProvider = new LegendImageProvider( mFlatLayerTree->layerTreeModel() );
  mAsyncLegendImageProvider = new AsyncLegendImageProvider( mFlatLayerTree->layerTreeModel() );
  mLocalFilesImageProvider = new LocalFilesImageProvider();
  mProjectsImageProvider = new ProjectsImageProvider();
  mBarcodeImageProvider = new BarcodeImageProvider();

  mBookmarkModel = new BookmarkModel( QgsApplication::bookmarkManager(), mProject->bookmarkManager(), this );
  mDrawingTemplateModel = new DrawingTemplateModel( this );

  mPluginManager = new PluginManager( this );

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

  PlatformUtilities::instance()->setScreenLockPermission( false );

  load( QUrl( "qrc:/qml/qgismobileapp.qml" ) );
  mMapCanvas = rootObjects().first()->findChild<QgsQuickMapCanvasMap *>();
  Q_ASSERT_X( mMapCanvas, "QML Init", "QgsQuickMapCanvasMap not found. It is likely that we failed to load the QML files. Check debug output for related messages." );
  mMapCanvas->mapSettings()->setProject( mProject );
  mBookmarkModel->setMapSettings( mMapCanvas->mapSettings() );
  mAsyncLegendImageProvider->setMapSettings( mMapCanvas->mapSettings() );

  mFlatLayerTree->layerTreeModel()->setLegendMapViewData( mMapCanvas->mapSettings()->mapSettings().mapUnitsPerPixel(),
                                                          static_cast<int>( std::round( mMapCanvas->mapSettings()->outputDpi() ) ), mMapCanvas->mapSettings()->mapSettings().scale() );

  mLayerTreeCanvasBridge = new LayerTreeMapCanvasBridge( mFlatLayerTree, mMapCanvas->mapSettings(), mTrackingModel, this );

  connect( this, &QgisMobileapp::loadProjectTriggered, mIface, &AppInterface::loadProjectTriggered );
  connect( this, &QgisMobileapp::loadProjectEnded, mIface, &AppInterface::loadProjectEnded );
  connect( this, &QgisMobileapp::setMapExtent, mIface, &AppInterface::setMapExtent );

  QTimer::singleShot( 1, this, &QgisMobileapp::onAfterFirstRendering );

  mOfflineEditing = new QgsOfflineEditing();

  mSettings.setValue( "/Map/searchRadiusMM", 5 );

  mAppMissingGridHandler = new AppMissingGridHandler( this );

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
  engine->addImportPath( QStringLiteral( "qrc:/qml/imports" ) );

  qRegisterMetaType<QMetaType::Type>( "QMetaType::Type" );

  // Register QGIS QML types
  qmlRegisterType<QgsSnappingUtils>( "org.qgis", 1, 0, "SnappingUtils" );
  qmlRegisterType<QgsMapLayerProxyModel>( "org.qgis", 1, 0, "MapLayerModel" );
  qmlRegisterType<QgsVectorLayer>( "org.qgis", 1, 0, "VectorLayer" );
  qmlRegisterType<QgsMapThemeCollection>( "org.qgis", 1, 0, "MapThemeCollection" );
  qmlRegisterType<QgsLocatorProxyModel>( "org.qgis", 1, 0, "QgsLocatorProxyModel" );
  qmlRegisterType<QgsVectorLayerEditBuffer>( "org.qgis", 1, 0, "QgsVectorLayerEditBuffer" );

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

  qRegisterMetaType<Qgis::GeometryType>( "Qgis::GeometryType" );
  qRegisterMetaType<Qgis::WkbType>( "Qgis::WkbType" );
  qRegisterMetaType<Qgis::LayerType>( "Qgis::LayerType" );
  qRegisterMetaType<Qgis::LayerFilters>( "Qgis::LayerFilters" );
  qRegisterMetaType<Qgis::DistanceUnit>( "Qgis::DistanceUnit" );
  qRegisterMetaType<Qgis::AreaUnit>( "Qgis::AreaUnit" );
  qRegisterMetaType<Qgis::AngleUnit>( "Qgis::AngleUnit" );
  qRegisterMetaType<Qgis::DeviceConnectionStatus>( "Qgis::DeviceConnectionStatus" );
  qRegisterMetaType<Qgis::SnappingMode>( "Qgis::SnappingMode" );

  qmlRegisterUncreatableType<Qgis>( "org.qgis", 1, 0, "Qgis", "" );

  qmlRegisterUncreatableType<QgsProject>( "org.qgis", 1, 0, "Project", "" );
  qmlRegisterUncreatableType<QgsProjectDisplaySettings>( "org.qgis", 1, 0, "ProjectDisplaySettings", "" );
  qmlRegisterUncreatableType<QgsCoordinateReferenceSystem>( "org.qgis", 1, 0, "CoordinateReferenceSystem", "" );
  qmlRegisterUncreatableType<QgsUnitTypes>( "org.qgis", 1, 0, "QgsUnitTypes", "" );
  qmlRegisterUncreatableType<QgsRelationManager>( "org.qgis", 1, 0, "RelationManager", "The relation manager is available from the QgsProject. Try `qgisProject.relationManager`" );
  qmlRegisterUncreatableType<QgsWkbTypes>( "org.qgis", 1, 0, "QgsWkbTypes", "" );
  qmlRegisterUncreatableType<QgsMapLayer>( "org.qgis", 1, 0, "MapLayer", "" );
  qmlRegisterUncreatableType<QgsRasterLayer>( "org.qgis", 1, 0, "RasterLayer", "" );
  qmlRegisterUncreatableType<QgsVectorLayer>( "org.qgis", 1, 0, "VectorLayerStatic", "" );

  // Register QgsQuick QML types
  qmlRegisterType<QgsQuickMapCanvasMap>( "org.qgis", 1, 0, "MapCanvasMap" );
  qmlRegisterType<QgsQuickMapSettings>( "org.qgis", 1, 0, "MapSettings" );
  qmlRegisterType<QgsQuickCoordinateTransformer>( "org.qfield", 1, 0, "CoordinateTransformer" );
  qmlRegisterType<QgsQuickElevationProfileCanvas>( "org.qgis", 1, 0, "ElevationProfileCanvas" );
  qmlRegisterType<QgsQuickMapTransform>( "org.qgis", 1, 0, "MapTransform" );

  // Register 3D QML types
  qmlRegisterType<Quick3DRubberbandGeometry>( "org.qfield", 1, 0, "Quick3DRubberbandGeometry" );
  qmlRegisterType<Quick3DTerrainGeometry>( "org.qfield", 1, 0, "Quick3DTerrainGeometry" );
  qmlRegisterType<Quick3DTerrainProvider>( "org.qfield", 1, 0, "Quick3DTerrainProvider" );
  qmlRegisterType<Quick3DMapTextureData>( "org.qfield", 1, 0, "Quick3DMapTextureData" );

  // Register QField QML types
  qRegisterMetaType<PlatformUtilities::Capabilities>( "PlatformUtilities::Capabilities" );
  qRegisterMetaType<GeometryUtils::GeometryOperationResult>( "GeometryOperationResult" );
  qRegisterMetaType<QFieldCloudConnection::ConnectionStatus>( "QFieldCloudConnection::ConnectionStatus" );
  qRegisterMetaType<CloudUserInformation>( "CloudUserInformation" );
  qRegisterMetaType<QFieldCloudProject::ProjectStatus>( "QFieldCloudProject::ProjectStatus" );
  qRegisterMetaType<QFieldCloudProject::ProjectCheckout>( "QFieldCloudProject::ProjectCheckout" );
  qRegisterMetaType<QFieldCloudProject::ProjectModification>( "QFieldCloudProject::ProjectModification" );
  qRegisterMetaType<Tracker::MeasureType>( "Tracker::MeasureType" );
  qRegisterMetaType<PositioningSource::ElevationCorrectionMode>( "PositioningSource::ElevationCorrectionMode" );

  qmlRegisterType<MultiFeatureListModel>( "org.qfield", 1, 0, "MultiFeatureListModel" );
  qmlRegisterType<FeatureIterator>( "org.qfield", 1, 0, "FeatureIterator" );
  qmlRegisterType<FeatureListModel>( "org.qfield", 1, 0, "FeatureListModel" );
  qmlRegisterType<FeatureListModelSelection>( "org.qfield", 1, 0, "FeatureListModelSelection" );
  qmlRegisterType<FeatureListExtentController>( "org.qfield", 1, 0, "FeaturelistExtentController" );
  qmlRegisterType<Geometry>( "org.qfield", 1, 0, "Geometry" );
  qmlRegisterType<RubberbandShape>( "org.qfield", 1, 0, "RubberbandShape" );
  qmlRegisterType<RubberbandModel>( "org.qfield", 1, 0, "RubberbandModel" );
  qmlRegisterType<ResourceSource>( "org.qfield", 1, 0, "ResourceSource" );
  qmlRegisterType<ProjectInfo>( "org.qfield", 1, 0, "ProjectInfo" );
  qmlRegisterType<ProjectSource>( "org.qfield", 1, 0, "ProjectSource" );
  qmlRegisterType<ViewStatus>( "org.qfield", 1, 0, "ViewStatus" );
  qmlRegisterType<GridModel>( "org.qfield", 1, 0, "GridModel" );
  qmlRegisterUncreatableType<GridAnnotation>( "org.qfield", 1, 0, "GridAnnotation", "" );

  qmlRegisterType<CogoExecutor>( "org.qfield", 1, 0, "CogoExecutor" );
  qmlRegisterType<CogoOperationsModel>( "org.qfield", 1, 0, "CogoOperationsModel" );
  qmlRegisterUncreatableType<CogoParameter>( "org.qfield", 1, 0, "CogoParameter", "" );
  qmlRegisterUncreatableType<CogoVisualGuide>( "org.qfield", 1, 0, "CogoVisualGuide", "" );

  qmlRegisterType<Geofencer>( "org.qfield", 1, 0, "Geofencer" );
  qmlRegisterType<DigitizingLogger>( "org.qfield", 1, 0, "DigitizingLogger" );
  qmlRegisterType<AttributeFormModel>( "org.qfield", 1, 0, "AttributeFormModel" );
  qmlRegisterType<FeatureModel>( "org.qfield", 1, 0, "FeatureModel" );
  qmlRegisterType<IdentifyTool>( "org.qfield", 1, 0, "IdentifyTool" );
  qmlRegisterType<DrawingCanvas>( "org.qfield", 1, 0, "DrawingCanvas" );
  qmlRegisterType<SubModel>( "org.qfield", 1, 0, "SubModel" );
  qmlRegisterType<ExpressionVariableModel>( "org.qfield", 1, 0, "ExpressionVariableModel" );
  qmlRegisterType<ExternalStorage>( "org.qfield", 1, 0, "ExternalStorage" );
  qmlRegisterType<BadLayerHandler>( "org.qfield", 1, 0, "BadLayerHandler" );
  qmlRegisterType<SnappingUtils>( "org.qfield", 1, 0, "SnappingUtils" );
  qmlRegisterType<DistanceArea>( "org.qfield", 1, 0, "DistanceArea" );
  qmlRegisterType<FocusStack>( "org.qfield", 1, 0, "FocusStack" );
  qmlRegisterType<ParametizedImage>( "org.qfield", 1, 0, "ParametizedImage" );
  qmlRegisterType<PrintLayoutListModel>( "org.qfield", 1, 0, "PrintLayoutListModel" );
  qmlRegisterType<VertexModel>( "org.qfield", 1, 0, "VertexModel" );
  qmlRegisterType<MapLayerModel>( "org.qfield", 1, 0, "MapLayerModel" );
  qmlRegisterType<MapToScreen>( "org.qfield", 1, 0, "MapToScreen" );
  qmlRegisterType<LocatorModelSuperBridge>( "org.qfield", 1, 0, "LocatorModelSuperBridge" );
  qmlRegisterType<LocatorActionsModel>( "org.qfield", 1, 0, "LocatorActionsModel" );
  qmlRegisterType<LocatorFiltersModel>( "org.qfield", 1, 0, "LocatorFiltersModel" );
  qmlRegisterType<LinePolygonShape>( "org.qfield", 1, 0, "LinePolygonShape" );
  qmlRegisterType<LocalFilesModel>( "org.qfield", 1, 0, "LocalFilesModel" );
  qmlRegisterType<QgsGeometryWrapper>( "org.qfield", 1, 0, "QgsGeometryWrapper" );
  qmlRegisterType<ValueMapModel>( "org.qfield", 1, 0, "ValueMapModel" );
  qmlRegisterType<RecentProjectListModel>( "org.qfield", 1, 0, "RecentProjectListModel" );
  qmlRegisterType<ReferencingFeatureListModel>( "org.qfield", 1, 0, "ReferencingFeatureListModel" );
  qmlRegisterType<OrderedRelationModel>( "org.qfield", 1, 0, "OrderedRelationModel" );
  qmlRegisterType<FeatureCheckListModel>( "org.qfield", 1, 0, "FeatureCheckListModel" );
  qmlRegisterType<GeometryEditorsModel>( "org.qfield", 1, 0, "GeometryEditorsModel" );
  qmlRegisterType<ExpressionEvaluator>( "org.qfield", 1, 0, "ExpressionEvaluator" );
#ifdef WITH_BLUETOOTH
  qmlRegisterType<BluetoothDeviceModel>( "org.qfield", 1, 0, "BluetoothDeviceModel" );
  qmlRegisterType<BluetoothReceiver>( "org.qfield", 1, 0, "BluetoothReceiver" );
  engine->rootContext()->setContextProperty( "withBluetooth", QVariant( true ) );
#else
  engine->rootContext()->setContextProperty( "withBluetooth", QVariant( false ) );
#endif
#ifdef WITH_SERIALPORT
  qmlRegisterType<SerialPortModel>( "org.qfield", 1, 0, "SerialPortModel" );
  qmlRegisterType<SerialPortReceiver>( "org.qfield", 1, 0, "SerialPortReceiver" );
  engine->rootContext()->setContextProperty( "withSerialPort", QVariant( true ) );
#else
  engine->rootContext()->setContextProperty( "withSerialPort", QVariant( false ) );
#endif
  qmlRegisterType<NearFieldReader>( "org.qfield", 1, 0, "NearFieldReader" );
  engine->rootContext()->setContextProperty( "withNfc", QVariant( NearFieldReader::isSupported() ) );
  qmlRegisterType<ChangelogContents>( "org.qfield", 1, 0, "ChangelogContents" );
  qmlRegisterType<LayerResolver>( "org.qfield", 1, 0, "LayerResolver" );
  qmlRegisterType<QFieldCloudConnection>( "org.qfield", 1, 0, "QFieldCloudConnection" );
  qmlRegisterType<QFieldCloudProject>( "org.qfield", 1, 0, "QFieldCloudProject" );
  qmlRegisterType<QFieldCloudProjectsModel>( "org.qfield", 1, 0, "QFieldCloudProjectsModel" );
  qmlRegisterType<QFieldCloudProjectsFilterModel>( "org.qfield", 1, 0, "QFieldCloudProjectsFilterModel" );
  qmlRegisterType<DeltaListModel>( "org.qfield", 1, 0, "DeltaListModel" );
  qmlRegisterType<ScaleBarMeasurement>( "org.qfield", 1, 0, "ScaleBarMeasurement" );
  qmlRegisterType<SensorListModel>( "org.qfield", 1, 0, "SensorListModel" );
  qmlRegisterType<Navigation>( "org.qfield", 1, 0, "Navigation" );
  qmlRegisterType<NavigationModel>( "org.qfield", 1, 0, "NavigationModel" );
  qmlRegisterType<Positioning>( "org.qfield", 1, 0, "Positioning" );
  qmlRegisterType<PositioningInformationModel>( "org.qfield", 1, 0, "PositioningInformationModel" );
  qmlRegisterType<PositioningDeviceModel>( "org.qfield", 1, 0, "PositioningDeviceModel" );
  qmlRegisterType<WebdavConnection>( "org.qfield", 1, 0, "WebdavConnection" );
  qmlRegisterType<AppExpressionContextScopesGenerator>( "org.qfield", 1, 0, "AppExpressionContextScopesGenerator" );
  qmlRegisterType<AudioRecorder>( "org.qfield", 1, 0, "AudioRecorder" );
  qmlRegisterType<BarcodeDecoder>( "org.qfield", 1, 0, "BarcodeDecoder" );
  qmlRegisterType<CameraPermission>( "org.qfield", 1, 0, "QfCameraPermission" );
  qmlRegisterType<MicrophonePermission>( "org.qfield", 1, 0, "QfMicrophonePermission" );
  qmlRegisterUncreatableType<QAbstractSocket>( "org.qfield", 1, 0, "QAbstractSocket", "" );
  qmlRegisterUncreatableType<AbstractGnssReceiver>( "org.qfield", 1, 0, "AbstractGnssReceiver", "" );
  qmlRegisterUncreatableType<Tracker>( "org.qfield", 1, 0, "Tracker", "" );
  qmlRegisterUncreatableType<GnssPositionInformation>( "org.qfield", 1, 0, "GnssPositionInformation", "Access to enums and properties only; cannot instantiate in QML." );

  qRegisterMetaType<GnssPositionDetails>( "GnssPositionDetails" );
  qRegisterMetaType<PluginInformation>( "PluginInformation" );

  qmlRegisterType<PluginModel>( "org.qfield", 1, 0, "PluginModel" );
  qmlRegisterType<PluginProxyModel>( "org.qfield", 1, 0, "PluginProxyModel" );

  qmlRegisterType<ProcessingAlgorithm>( "org.qfield", 1, 0, "ProcessingAlgorithm" );
  qmlRegisterType<ProcessingAlgorithmParametersModel>( "org.qfield", 1, 0, "ProcessingAlgorithmParametersModel" );
  qmlRegisterType<ProcessingAlgorithmsModel>( "org.qfield", 1, 0, "ProcessingAlgorithmsModel" );

  qmlRegisterType<QgsLocatorContext>( "org.qgis", 1, 0, "QgsLocatorContext" );
  qmlRegisterType<QFieldLocatorFilter>( "org.qfield", 1, 0, "QFieldLocatorFilter" );

  QScreen *screen = QGuiApplication::primaryScreen();
  const qreal dpi = screen ? screen->logicalDotsPerInch() * screen->devicePixelRatio() : 96.0;
  const qreal systemFontPointSize = PlatformUtilities::instance()->systemFontPointSize();
  qmlRegisterSingletonType<Theme>( "org.qfield", 1, 0, "Theme", [dpi, systemFontPointSize]( QQmlEngine *, QJSEngine * ) -> QObject * {
    Theme *t = new Theme();
    t->setScreenPpi( dpi );
    t->setSystemFontPointSize( systemFontPointSize );
    return t;
  } );

  REGISTER_SINGLETON( "org.qfield", ExpressionContextUtils, "ExpressionContextUtils" );
  REGISTER_SINGLETON( "org.qfield", GeometryEditorsModel, "GeometryEditorsModelSingleton" );
  REGISTER_SINGLETON( "org.qfield", GeometryUtils, "GeometryUtils" );
  REGISTER_SINGLETON( "org.qfield", FeatureUtils, "FeatureUtils" );
  REGISTER_SINGLETON( "org.qfield", FileUtils, "FileUtils" );
  REGISTER_SINGLETON( "org.qfield", LayerUtils, "LayerUtils" );
  REGISTER_SINGLETON( "org.qfield", RelationUtils, "RelationUtils" );
  REGISTER_SINGLETON( "org.qfield", StringUtils, "StringUtils" );
  REGISTER_SINGLETON( "org.qfield", UrlUtils, "UrlUtils" );
  REGISTER_SINGLETON( "org.qfield", QFieldCloudUtils, "QFieldCloudUtils" );
  REGISTER_SINGLETON( "org.qfield", PositioningUtils, "PositioningUtils" );
  REGISTER_SINGLETON( "org.qfield", ProcessingUtils, "ProcessingUtils" );
  REGISTER_SINGLETON( "org.qfield", ProjectUtils, "ProjectUtils" );
  REGISTER_SINGLETON( "org.qfield", CoordinateReferenceSystemUtils, "CoordinateReferenceSystemUtils" );

  qmlRegisterUncreatableType<AppInterface>( "org.qfield", 1, 0, "AppInterface", "AppInterface is only provided by the environment and cannot be created ad-hoc" );
  qmlRegisterUncreatableType<Settings>( "org.qfield", 1, 0, "SettingsInterface", "" );
  qmlRegisterUncreatableType<PluginManager>( "org.qfield", 1, 0, "PluginManager", "" );
  qmlRegisterUncreatableType<PlatformUtilities>( "org.qfield", 1, 0, "PlatformUtilities", "" );
  qmlRegisterUncreatableType<FlatLayerTreeModel>( "org.qfield", 1, 0, "FlatLayerTreeModel", "The FlatLayerTreeModel is available as context property `flatLayerTree`." );
  qmlRegisterUncreatableType<TrackingModel>( "org.qfield", 1, 0, "TrackingModel", "The TrackingModel is available as context property `trackingModel`." );
  qmlRegisterUncreatableType<QgsGpkgFlusher>( "org.qfield", 1, 0, "QgsGpkgFlusher", "The gpkgFlusher is available as context property `gpkgFlusher`" );
  qmlRegisterUncreatableType<LayerObserver>( "org.qfield", 1, 0, "LayerObserver", "" );
  qmlRegisterUncreatableType<DeltaFileWrapper>( "org.qfield", 1, 0, "DeltaFileWrapper", "" );
  qmlRegisterUncreatableType<BookmarkModel>( "org.qfield", 1, 0, "BookmarkModel", "The BookmarkModel is available as context property `bookmarkModel`" );
  qmlRegisterUncreatableType<MessageLogModel>( "org.qfield", 1, 0, "MessageLogModel", "The MessageLogModel is available as context property `messageLogModel`." );

  qRegisterMetaType<SnappingResult>( "SnappingResult" );

  // Register some globally available variables
  engine->rootContext()->setContextProperty( "qVersion", qVersion() );
  engine->rootContext()->setContextProperty( "qgisVersion", Qgis::version() );
  engine->rootContext()->setContextProperty( "gdalVersion", GDAL_RELEASE_NAME );
  engine->rootContext()->setContextProperty( "withNfc", QVariant( NearFieldReader::isSupported() ) );
  engine->rootContext()->setContextProperty( "systemFontPointSize", PlatformUtilities::instance()->systemFontPointSize() );
  engine->rootContext()->setContextProperty( "mouseDoubleClickInterval", QApplication::styleHints()->mouseDoubleClickInterval() );
  engine->rootContext()->setContextProperty( "appVersion", qfield::appVersion );
  engine->rootContext()->setContextProperty( "appVersionStr", qfield::appVersionStr );
  engine->rootContext()->setContextProperty( "gitRev", qfield::gitRev );
  engine->rootContext()->setContextProperty( "platformUtilities", PlatformUtilities::instance() );
}

void QgisMobileapp::registerGlobalVariables()
{
  // Calculate device pixels
  rootContext()->setContextProperty( "qgisProject", mProject );
  rootContext()->setContextProperty( "iface", mIface );
  rootContext()->setContextProperty( "pluginManager", mPluginManager );
  rootContext()->setContextProperty( "settings", &mSettings );
  rootContext()->setContextProperty( "flatLayerTree", mFlatLayerTree );
  rootContext()->setContextProperty( "CrsFactory", QVariant::fromValue<QgsCoordinateReferenceSystem>( mCrsFactory ) );
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

void QgisMobileapp::removeRecentProject( const QString &path )
{
  QList<QPair<QString, QString>> projects = recentProjects();
  for ( int idx = 0; idx < projects.count(); idx++ )
  {
    if ( projects.at( idx ).second == path )
    {
      projects.removeAt( idx );
      break;
    }
  }
  saveRecentProjects( projects );
}

QList<QPair<QString, QString>> QgisMobileapp::recentProjects()
{
  QSettings settings;
  QList<QPair<QString, QString>> projects;

  settings.beginGroup( "/qgis/recentProjects" );
  const QStringList projectKeysList = settings.childGroups();
  QList<int> projectKeys;
  // This is overdoing it since we're clipping the recent projects list to five items at the moment, but might as well be futureproof
  for ( const QString &key : projectKeysList )
  {
    projectKeys.append( key.toInt() );
  }
  for ( int i = 0; i < projectKeys.count(); i++ )
  {
    settings.beginGroup( QString::number( projectKeys.at( i ) ) );
    projects << qMakePair( settings.value( QStringLiteral( "title" ) ).toString(), settings.value( QStringLiteral( "path" ) ).toString() );
    settings.endGroup();
  }
  settings.endGroup();
  return projects;
}

void QgisMobileapp::saveRecentProjects( const QList<QPair<QString, QString>> &projects )
{
  QSettings settings;
  settings.remove( QStringLiteral( "/qgis/recentProjects" ) );
  for ( int idx = 0; idx < projects.count() && idx < 5; idx++ )
  {
    settings.beginGroup( QStringLiteral( "/qgis/recentProjects/%1" ).arg( idx ) );
    settings.setValue( QStringLiteral( "title" ), projects.at( idx ).first );
    settings.setValue( QStringLiteral( "path" ), projects.at( idx ).second );
    settings.endGroup();
  }
}

void QgisMobileapp::onAfterFirstRendering()
{
  // This should get triggered exactly once, so we disconnect it right away
  // disconnect( this, &QgisMobileapp::afterRendering, this, &QgisMobileapp::onAfterFirstRendering );
  if ( mFirstRenderingFlag )
  {
    mPluginManager->restoreAppPlugins();
    if ( PlatformUtilities::instance()->hasQfAction() )
    {
      PlatformUtilities::instance()->executeQfAction();
    }
    else if ( PlatformUtilities::instance()->hasQgsProject() )
    {
      PlatformUtilities::instance()->loadQgsProject();
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
      mPluginManager->unloadPlugin( PluginManager::findProjectPlugin( mProjectFilePath ) );
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
  if ( mProject->fileName().startsWith( QFieldCloudUtils::localCloudDirectory() ) )
  {
    // Overwrite the title to match what is used in QFieldCloud
    const QString projectId = fi.dir().dirName();
    title = QSettings().value( QStringLiteral( "QFieldCloud/projects/%1/name" ).arg( projectId ), fi.fileName() ).toString();
  }
  else
  {
    title = mProject->title().isEmpty() ? mProjectFileName : mProject->title();
  }

  QList<QPair<QString, QString>> projects = recentProjects();
  for ( int idx = 0; idx < projects.count(); idx++ )
  {
    if ( projects.at( idx ).second == mProjectFilePath )
    {
      projects.removeAt( idx );
      break;
    }
  }
  QPair<QString, QString> project = qMakePair( title, mProjectFilePath );
  projects.insert( 0, project );
  saveRecentProjects( projects );

  QList<QgsMapLayer *> vectorLayers;
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
        case Qgis::LayerType::Mesh:
        case Qgis::LayerType::VectorTile:
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

  if ( vectorLayers.size() > 0 || rasterLayers.size() > 0 )
  {
    if ( crs.isValid() )
    {
      QSettings settings;
      const QString fileAssociationProject = settings.value( QStringLiteral( "QField/baseMapProject" ), QString() ).toString();
      if ( !fileAssociationProject.isEmpty() && QFile::exists( fileAssociationProject ) )
      {
        mProject->read( fileAssociationProject, Qgis::ProjectReadFlag::DontLoadProjectStyles | Qgis::ProjectReadFlag::DontLoad3DViews );
      }
      else
      {
        const QStringList dataDirs = PlatformUtilities::instance()->appDataDirs();
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
          mProject->addMapLayers( QList<QgsMapLayer *>() << LayerUtils::createBasemap() );
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
        LayerUtils::setDefaultRenderer( vlayer, mProject );
      }

      if ( !vlayer->labeling() )
      {
        LayerUtils::setDefaultLabeling( vlayer, mProject );
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
    QgsRasterLayer *elevationLayer = LayerUtils::createOnlineElevationLayer();
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

  ProjectInfo::restoreSettings( mProjectFilePath, mProject, mMapCanvas, mFlatLayerTree );
  emit loadProjectEnded( mProjectFilePath, mProjectFileName );
  mTrackingModel->createProjectTrackers( mProject );

  connect( mMapCanvas, &QgsQuickMapCanvasMap::mapCanvasRefreshed, this, &QgisMobileapp::onMapCanvasRefreshed );

  const QString projectPluginPath = PluginManager::findProjectPlugin( mProjectFilePath );
  if ( !projectPluginPath.isEmpty() )
  {
    mPluginManager->loadPlugin( projectPluginPath, tr( "Project Plugin" ), false, true );
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
      PlatformUtilities::instance()->open( destination );

    return result == QgsLayoutExporter::Success ? true : false;
  }
  else
  {
    bool success = printAtlas( layoutToPrint, destination );
    if ( success )
    {
      if ( layoutToPrint->customProperty( QStringLiteral( "singleFile" ), true ).toBool() )
      {
        PlatformUtilities::instance()->open( destination );
      }
      else
      {
        PlatformUtilities::instance()->open( mProject->homePath() );
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
      PlatformUtilities::instance()->open( finalDestination );
    }
    else
    {
      PlatformUtilities::instance()->open( mProject->homePath() );
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
  PlatformUtilities::instance()->stopPositioningService();

  saveProjectPreviewImage();

  mPluginManager->unloadPlugins();

  delete mOfflineEditing;
  mProject->clear();
  delete mProject;
  delete mAppMissingGridHandler;

  QgsApplication::taskManager()->cancelAll();
  mApp->exitQgis();
  QMetaObject::invokeMethod( mApp, &QgsApplication::quit, Qt::QueuedConnection );
}
