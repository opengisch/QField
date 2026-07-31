/***************************************************************************
    qfieldcoreqmlregistration.cpp
    ---------------------
    begin                : July 2026
    copyright            : (C) 2026 by Mohsen Dehghanzadeh
    email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "3d/maptoview3d.h"
#include "3d/quick3dgeometry.h"
#include "3d/quick3dgeometryconfiguration.h"
#include "3d/quick3dmaptexturedata.h"
#include "3d/quick3drubberbandgeometry.h"
#include "3d/quick3dterraingeometry.h"
#include "3d/quick3dterrainprovider.h"
#include "appexpressioncontextscopesgenerator.h"
#include "appinterface.h"
#include "audioanalyzer.h"
#include "audiorecorder.h"
#include "barcodedecoder.h"
#include "bookmarkmodel.h"
#include "cameraorientationnormalizer.h"
#include "cogo/cogoexecutor.h"
#include "cogo/cogooperation.h"
#include "cogo/cogooperationsmodel.h"
#include "digitizinglogger.h"
#include "distancearea.h"
#include "drawingcanvas.h"
#include "externalstorage.h"
#include "featurelistextentcontroller.h"
#include "featurelistmodel.h"
#include "featurelistmodelselection.h"
#include "featuremodel.h"
#include "geometry.h"
#include "gridmodel.h"
#include "identifytool.h"
#include "layerresolver.h"
#include "layertreemodel.h"
#include "linepolygonshape.h"
#include "locator/locatormodelsuperbridge.h"
#include "locator/qfieldlocatorfilter.h"
#include "maplayermodel.h"
#include "maptoscreen.h"
#include "multifeaturelistmodel.h"
#include "navigation.h"
#include "navigationmodel.h"
#include "nearfieldreader.h"
#include "platforms/platformutilities.h"
#include "positioning/abstractgnssreceiver.h"
#include "positioning/geofencer.h"
#include "positioning/gnsspositioninformation.h"
#include "positioning/ntripsettings.h"
#include "positioning/ntripsourcetablefetcher.h"
#include "positioning/positioning.h"
#include "positioning/positioningdevicemodel.h"
#include "positioning/positioninginformationmodel.h"
#include "processing/processingalgorithm.h"
#include "processing/processingalgorithmparametersmodel.h"
#include "processing/processingalgorithmsmodel.h"
#include "projectinfo.h"
#include "projectsource.h"
#include "qfieldcloud/deltafilewrapper.h"
#include "qfieldcloud/deltalistmodel.h"
#include "qfieldcloud/layerobserver.h"
#include "qfieldcloud/qfieldcloudconnection.h"
#include "qfieldcloud/qfieldcloudproject.h"
#include "qfieldcloud/qfieldcloudprojectsmodel.h"
#include "qfieldcloud/qfieldcloudstatus.h"
#include "qfieldcoreqmlregistration.h"
#include "qgsgeometrywrapper.h"
#include "qgsgpkgflusher.h"
#include "qgsquick/qgsquickcoordinatetransformer.h"
#include "resourcesource.h"
#include "rubberbandmodel.h"
#include "rubberbandshape.h"
#include "scalebarmeasurement.h"
#include "settings.h"
#include "submodel.h"
#include "theme.h"
#include "tracker.h"
#include "trackingmodel.h"
#include "utils/authutils.h"
#include "utils/coordinatereferencesystemutils.h"
#include "utils/expressioncontextutils.h"
#include "utils/featureutils.h"
#include "utils/fileutils.h"
#include "utils/geometryutils.h"
#include "utils/layerutils.h"
#include "utils/positioningutils.h"
#include "utils/processingutils.h"
#include "utils/projectutils.h"
#include "utils/qfieldcloudutils.h"
#include "utils/relationutils.h"
#include "utils/snappingutils.h"
#include "utils/stringutils.h"
#include "utils/urlutils.h"
#include "vertexmodel.h"
#include "viewstatus.h"
#include "webdavconnection.h"

#include <QAbstractSocket>
#include <QGuiApplication>
#include <QScreen>
#include <QtQml>

#ifdef WITH_BLUETOOTH
#include "positioning/bluetoothdevicemodel.h"
#include "positioning/bluetoothreceiver.h"
#endif
#ifdef WITH_SERIALPORT
#include "positioning/serialportmodel.h"
#include "positioning/serialportreceiver.h"
#endif

#define REGISTER_SINGLETON( uri, _class, name ) qmlRegisterSingletonType<_class>( uri, 1, 0, name, []( QQmlEngine *engine, QJSEngine *scriptEngine ) -> QObject * { Q_UNUSED( engine ); Q_UNUSED( scriptEngine ); return new _class(); } )

namespace QFieldCore
{
  void registerQmlTypes()
  {
    qmlRegisterType<AppExpressionContextScopesGenerator>( "org.qfield.core", 1, 0, "AppExpressionContextScopesGenerator" );
    qmlRegisterType<AudioAnalyzer>( "org.qfield.core", 1, 0, "AudioAnalyzer" );
    qmlRegisterType<AudioRecorder>( "org.qfield.core", 1, 0, "AudioRecorder" );
    qmlRegisterType<BarcodeDecoder>( "org.qfield.core", 1, 0, "BarcodeDecoder" );
    qmlRegisterType<CameraOrientationNormalizer>( "org.qfield.core", 1, 0, "CameraOrientationNormalizer" );
    qmlRegisterType<CogoExecutor>( "org.qfield.core", 1, 0, "CogoExecutor" );
    qmlRegisterType<CogoOperationsModel>( "org.qfield.core", 1, 0, "CogoOperationsModel" );
    qmlRegisterType<DeltaListModel>( "org.qfield.core", 1, 0, "DeltaListModel" );
    qmlRegisterType<DigitizingLogger>( "org.qfield.core", 1, 0, "DigitizingLogger" );
    qmlRegisterType<DistanceArea>( "org.qfield.core", 1, 0, "DistanceArea" );
    qmlRegisterType<DrawingCanvas>( "org.qfield.core", 1, 0, "DrawingCanvas" );
    qmlRegisterType<ExternalStorage>( "org.qfield.core", 1, 0, "ExternalStorage" );
    qmlRegisterType<FeatureListExtentController>( "org.qfield.core", 1, 0, "FeaturelistExtentController" );
    qmlRegisterType<FeatureListModel>( "org.qfield.core", 1, 0, "FeatureListModel" );
    qmlRegisterType<FeatureListModelSelection>( "org.qfield.core", 1, 0, "FeatureListModelSelection" );
    qmlRegisterType<FeatureModel>( "org.qfield.core", 1, 0, "FeatureModel" );
    qmlRegisterType<Geofencer>( "org.qfield.core", 1, 0, "Geofencer" );
    qmlRegisterType<Geometry>( "org.qfield.core", 1, 0, "Geometry" );
    qmlRegisterType<GridModel>( "org.qfield.core", 1, 0, "GridModel" );
    qmlRegisterType<IdentifyTool>( "org.qfield.core", 1, 0, "IdentifyTool" );
    qmlRegisterType<LayerResolver>( "org.qfield.core", 1, 0, "LayerResolver" );
    qmlRegisterType<LinePolygonShape>( "org.qfield.core", 1, 0, "LinePolygonShape" );
    qmlRegisterType<LocatorModelSuperBridge>( "org.qfield.core", 1, 0, "LocatorModelSuperBridge" );
    qmlRegisterType<MapLayerModel>( "org.qfield.core", 1, 0, "MapLayerModel" );
    qmlRegisterType<MapToScreen>( "org.qfield.core", 1, 0, "MapToScreen" );
    qmlRegisterType<MapToView3D>( "org.qfield.core", 1, 0, "MapToView3D" );
    qmlRegisterType<MultiFeatureListModel>( "org.qfield.core", 1, 0, "MultiFeatureListModel" );
    qmlRegisterType<Navigation>( "org.qfield.core", 1, 0, "Navigation" );
    qmlRegisterType<NavigationModel>( "org.qfield.core", 1, 0, "NavigationModel" );
    qmlRegisterType<NearFieldReader>( "org.qfield.core", 1, 0, "NearFieldReader" );
    qmlRegisterType<NtripSourceTableFetcher>( "org.qfield.core", 1, 0, "NtripSourceTableFetcher" );
    qmlRegisterType<Positioning>( "org.qfield.core", 1, 0, "Positioning" );
    qmlRegisterType<PositioningDeviceModel>( "org.qfield.core", 1, 0, "PositioningDeviceModel" );
    qmlRegisterType<PositioningInformationModel>( "org.qfield.core", 1, 0, "PositioningInformationModel" );
    qmlRegisterType<ProcessingAlgorithm>( "org.qfield.core", 1, 0, "ProcessingAlgorithm" );
    qmlRegisterType<ProcessingAlgorithmParametersModel>( "org.qfield.core", 1, 0, "ProcessingAlgorithmParametersModel" );
    qmlRegisterType<ProcessingAlgorithmsModel>( "org.qfield.core", 1, 0, "ProcessingAlgorithmsModel" );
    qmlRegisterType<ProjectInfo>( "org.qfield.core", 1, 0, "ProjectInfo" );
    qmlRegisterType<ProjectSource>( "org.qfield.core", 1, 0, "ProjectSource" );
    qmlRegisterType<QFieldCloudConnection>( "org.qfield.core", 1, 0, "QFieldCloudConnection" );
    qmlRegisterType<QFieldCloudProject>( "org.qfield.core", 1, 0, "QFieldCloudProject" );
    qmlRegisterType<QFieldCloudProjectsModel>( "org.qfield.core", 1, 0, "QFieldCloudProjectsModel" );
    qmlRegisterType<QFieldCloudStatus>( "org.qfield.core", 1, 0, "QFieldCloudStatus" );
    qmlRegisterType<QFieldLocatorFilter>( "org.qfield.core", 1, 0, "QFieldLocatorFilter" );
    qmlRegisterType<QgsGeometryWrapper>( "org.qfield.core", 1, 0, "QgsGeometryWrapper" );
    qmlRegisterType<QgsQuickCoordinateTransformer>( "org.qfield.core", 1, 0, "CoordinateTransformer" );
    qmlRegisterType<Quick3DGeometry>( "org.qfield.core", 1, 0, "Quick3DGeometry" );
    qmlRegisterType<Quick3DGeometryConfiguration>( "org.qfield.core", 1, 0, "Quick3DGeometryConfiguration" );
    qmlRegisterType<Quick3DMapTextureData>( "org.qfield.core", 1, 0, "Quick3DMapTextureData" );
    qmlRegisterType<Quick3DRubberbandGeometry>( "org.qfield.core", 1, 0, "Quick3DRubberbandGeometry" );
    qmlRegisterType<Quick3DTerrainGeometry>( "org.qfield.core", 1, 0, "Quick3DTerrainGeometry" );
    qmlRegisterType<Quick3DTerrainProvider>( "org.qfield.core", 1, 0, "Quick3DTerrainProvider" );
    qmlRegisterType<ResourceSource>( "org.qfield.core", 1, 0, "ResourceSource" );
    qmlRegisterType<RubberbandModel>( "org.qfield.core", 1, 0, "RubberbandModel" );
    qmlRegisterType<RubberbandShape>( "org.qfield.core", 1, 0, "RubberbandShape" );
    qmlRegisterType<ScaleBarMeasurement>( "org.qfield.core", 1, 0, "ScaleBarMeasurement" );
    qmlRegisterType<SnappingUtils>( "org.qfield.core", 1, 0, "SnappingUtils" );
    qmlRegisterType<SubModel>( "org.qfield.core", 1, 0, "SubModel" );
    qmlRegisterType<VertexModel>( "org.qfield.core", 1, 0, "VertexModel" );
    qmlRegisterType<ViewStatus>( "org.qfield.core", 1, 0, "ViewStatus" );
    qmlRegisterType<WebdavConnection>( "org.qfield.core", 1, 0, "WebdavConnection" );

    REGISTER_SINGLETON( "org.qfield.core", AuthUtils, "AuthUtils" );
    REGISTER_SINGLETON( "org.qfield.core", CoordinateReferenceSystemUtils, "CoordinateReferenceSystemUtils" );
    REGISTER_SINGLETON( "org.qfield.core", ExpressionContextUtils, "ExpressionContextUtils" );
    REGISTER_SINGLETON( "org.qfield.core", FeatureUtils, "FeatureUtils" );
    REGISTER_SINGLETON( "org.qfield.core", FileUtils, "FileUtils" );
    REGISTER_SINGLETON( "org.qfield.core", GeometryUtils, "GeometryUtils" );
    REGISTER_SINGLETON( "org.qfield.core", LayerUtils, "LayerUtils" );
    REGISTER_SINGLETON( "org.qfield.core", PositioningUtils, "PositioningUtils" );
    REGISTER_SINGLETON( "org.qfield.core", ProcessingUtils, "ProcessingUtils" );
    REGISTER_SINGLETON( "org.qfield.core", ProjectUtils, "ProjectUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QFieldCloudUtils, "QFieldCloudUtils" );
    REGISTER_SINGLETON( "org.qfield.core", RelationUtils, "RelationUtils" );
    REGISTER_SINGLETON( "org.qfield.core", StringUtils, "StringUtils" );
    REGISTER_SINGLETON( "org.qfield.core", UrlUtils, "UrlUtils" );

    qmlRegisterSingletonType<Theme>( "org.qfield.core", 1, 0, "Theme", []( QQmlEngine *, QJSEngine * ) -> QObject * {
      QScreen *screen = QGuiApplication::primaryScreen();
      const qreal dpi = screen ? screen->logicalDotsPerInch() * screen->devicePixelRatio() : 96.0;
      Theme *theme = new Theme();
      theme->setScreenPpi( dpi );
      theme->setSystemFontPointSize( PlatformUtilities::instance()->systemFontPointSize() );
      return theme;
    } );

    qmlRegisterUncreatableType<AbstractGnssReceiver>( "org.qfield.core", 1, 0, "AbstractGnssReceiver", "" );
    qmlRegisterUncreatableType<AppInterface>( "org.qfield.core", 1, 0, "AppInterface", "AppInterface is only provided by the environment and cannot be created ad-hoc" );
    qmlRegisterUncreatableType<BookmarkModel>( "org.qfield.core", 1, 0, "BookmarkModel", "The BookmarkModel is available as context property `bookmarkModel`" );
    qmlRegisterUncreatableType<CogoParameter>( "org.qfield.core", 1, 0, "cogoParameter", "Used for property values" );
    qmlRegisterUncreatableType<CogoVisualGuide>( "org.qfield.core", 1, 0, "cogoVisualGuide", "Used for property values" );
    qmlRegisterUncreatableType<DeltaFileWrapper>( "org.qfield.core", 1, 0, "DeltaFileWrapper", "" );
    qmlRegisterUncreatableType<FlatLayerTreeModel>( "org.qfield.core", 1, 0, "FlatLayerTreeModel", "The FlatLayerTreeModel is available as context property `flatLayerTree`." );
    qmlRegisterUncreatableType<GnssPositionInformation>( "org.qfield.core", 1, 0, "gnssPositionInformation", "Used for property values" );
    qmlRegisterUncreatableType<GridAnnotation>( "org.qfield.core", 1, 0, "gridAnnotation", "Used for property values" );
    qmlRegisterUncreatableType<LayerObserver>( "org.qfield.core", 1, 0, "LayerObserver", "" );
    qmlRegisterUncreatableType<NtripSettings>( "org.qfield.core", 1, 0, "ntripSettings", "Used for property values" );
    qmlRegisterUncreatableType<PlatformUtilities>( "org.qfield.core", 1, 0, "PlatformUtilities", "" );
    qmlRegisterUncreatableType<QgsGpkgFlusher>( "org.qfield.core", 1, 0, "QgsGpkgFlusher", "The gpkgFlusher is available as context property `gpkgFlusher`" );
    qmlRegisterUncreatableType<Settings>( "org.qfield.core", 1, 0, "SettingsInterface", "" );
    qmlRegisterUncreatableType<Tracker>( "org.qfield.core", 1, 0, "Tracker", "" );
    qmlRegisterUncreatableType<TrackingModel>( "org.qfield.core", 1, 0, "TrackingModel", "The TrackingModel is available as context property `trackingModel`." );

#ifdef WITH_BLUETOOTH
    qmlRegisterType<BluetoothDeviceModel>( "org.qfield.core", 1, 0, "BluetoothDeviceModel" );
    qmlRegisterType<BluetoothReceiver>( "org.qfield.core", 1, 0, "BluetoothReceiver" );
#endif
#ifdef WITH_SERIALPORT
    qmlRegisterType<SerialPortModel>( "org.qfield.core", 1, 0, "SerialPortModel" );
    qmlRegisterType<SerialPortReceiver>( "org.qfield.core", 1, 0, "SerialPortReceiver" );
#endif

    qmlRegisterType<FeatureIterator>( "org.qfield.core", 1, 0, "featureIterator" );
    qmlRegisterType<LocatorActionsModel>( "org.qfield.core", 1, 0, "LocatorActionsModel" );
    qmlRegisterType<LocatorFiltersModel>( "org.qfield.core", 1, 0, "LocatorFiltersModel" );
    qmlRegisterType<QFieldCloudProjectsFilterModel>( "org.qfield.core", 1, 0, "QFieldCloudProjectsFilterModel" );
    qmlRegisterUncreatableMetaObject( CogoVisualGuide::staticMetaObject, "org.qfield.core", 1, 0, "CogoVisualGuide", "Used to access enum values" );
    qmlRegisterUncreatableMetaObject( GnssPositionInformation::staticMetaObject, "org.qfield.core", 1, 0, "GnssPositionInformation", "Used to access to enum values" );
    qmlRegisterUncreatableMetaObject( GridAnnotation::staticMetaObject, "org.qfield.core", 1, 0, "GridAnnotation", "Used to access enum values" );
    qmlRegisterUncreatableMetaObject( NtripSettings::staticMetaObject, "org.qfield.core", 1, 0, "NtripSettings", "Used to access to enum values" );
    qmlRegisterUncreatableMetaObject( QFieldCloudDelta::staticMetaObject, "org.qfield.core", 1, 0, "QFieldCloudDelta", "Used to access to enum values" );
    qmlRegisterUncreatableType<QAbstractSocket>( "org.qfield.core", 1, 0, "QAbstractSocket", "" );
  }
} // namespace QFieldCore

#undef REGISTER_SINGLETON
