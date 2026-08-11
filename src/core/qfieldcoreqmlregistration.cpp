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

#include "cogo/qfcogoexecutor.h"
#include "cogo/qfcogooperation.h"
#include "cogo/qfcogooperationsmodel.h"
#include "locator/qflocatorfilter.h"
#include "locator/qflocatormodelsuperbridge.h"
#include "platforms/qfplatformutilities.h"
#include "positioning/qfabstractgnssreceiver.h"
#include "positioning/qfgeofencer.h"
#include "positioning/qfgnsspositioninformation.h"
#include "positioning/qfntripsettings.h"
#include "positioning/qfntripsourcetablefetcher.h"
#include "positioning/qfpositioning.h"
#include "positioning/qfpositioningdevicemodel.h"
#include "positioning/qfpositioninginformationmodel.h"
#include "processing/qfprocessingalgorithm.h"
#include "processing/qfprocessingalgorithmparametersmodel.h"
#include "processing/qfprocessingalgorithmsmodel.h"
#include "qfappexpressioncontextscopesgenerator.h"
#include "qfappinterface.h"
#include "qfaudioanalyzer.h"
#include "qfaudiorecorder.h"
#include "qfbarcodedecoder.h"
#include "qfbookmarkmodel.h"
#include "qfcameraorientationnormalizer.h"
#include "qfdigitizinglogger.h"
#include "qfdistancearea.h"
#include "qfdrawingcanvas.h"
#include "qfexternalstorage.h"
#include "qffeaturelistextentcontroller.h"
#include "qffeaturelistmodel.h"
#include "qffeaturelistmodelselection.h"
#include "qffeaturemodel.h"
#include "qfgeometry.h"
#include "qfgridmodel.h"
#include "qfidentifytool.h"
#include "qfieldcloud/qfcloudconnection.h"
#include "qfieldcloud/qfcloudproject.h"
#include "qfieldcloud/qfcloudprojectsmodel.h"
#include "qfieldcloud/qfcloudstatus.h"
#include "qfieldcloud/qfdeltafilewrapper.h"
#include "qfieldcloud/qfdeltalistmodel.h"
#include "qfieldcloud/qflayerobserver.h"
#include "qfieldcoreqmlregistration.h"
#include "qflayerresolver.h"
#include "qflayertreemodel.h"
#include "qflinepolygonshape.h"
#include "qfmaplayermodel.h"
#include "qfmaptoscreen.h"
#include "qfmultifeaturelistmodel.h"
#include "qfnavigation.h"
#include "qfnavigationmodel.h"
#include "qfnearfieldreader.h"
#include "qfprojectinfo.h"
#include "qfprojectsource.h"
#include "qfresourcesource.h"
#include "qfrubberbandmodel.h"
#include "qfrubberbandshape.h"
#include "qfscalebarmeasurement.h"
#include "qfsettings.h"
#include "qfsubmodel.h"
#include "qftracker.h"
#include "qftrackingmodel.h"
#include "qfvertexmodel.h"
#include "qfvideosinkcapture.h"
#include "qfviewstatus.h"
#include "qfwebdavconnection.h"
#include "qgsgeometrywrapper.h"
#include "qgsgpkgflusher.h"
#include "qgsquick/qgsquickcoordinatetransformer.h"
#include "qgsquick/qgsquickelevationprofilecanvas.h"
#include "qgsquick/qgsquickmapcanvasmap.h"
#include "qgsquick/qgsquickmapsettings.h"
#include "qgsquick/qgsquickmaptransform.h"
#include "utils/qfauthutils.h"
#include "utils/qfcloudutils.h"
#include "utils/qfcoordinatereferencesystemutils.h"
#include "utils/qfexpressioncontextutils.h"
#include "utils/qffeatureutils.h"
#include "utils/qffileutils.h"
#include "utils/qfgeometryutils.h"
#include "utils/qflayerutils.h"
#include "utils/qfpositioningutils.h"
#include "utils/qfprocessingutils.h"
#include "utils/qfprojectutils.h"
#include "utils/qfrelationutils.h"
#include "utils/qfsnappingutils.h"
#include "utils/qfstringutils.h"
#include "utils/qfurlutils.h"

#include <QAbstractSocket>
#include <QtQml>
#include <qgis.h>
#include <qgslocatorcontext.h>
#include <qgslocatormodel.h>
#include <qgsmaplayer.h>
#include <qgsmapthemecollection.h>
#include <qgsproject.h>
#include <qgsprojectdisplaysettings.h>
#include <qgsrasterlayer.h>
#include <qgsrelationmanager.h>
#include <qgssnappingutils.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayereditbuffer.h>

#ifdef WITH_BLUETOOTH
#include "positioning/qfbluetoothdevicemodel.h"
#include "positioning/qfbluetoothreceiver.h"
#endif
#ifdef WITH_SERIALPORT
#include "positioning/qfserialportmodel.h"
#include "positioning/qfserialportreceiver.h"
#endif

#define REGISTER_SINGLETON( uri, _class, name ) qmlRegisterSingletonType<_class>( uri, 1, 0, name, []( QQmlEngine *engine, QJSEngine *scriptEngine ) -> QObject * { Q_UNUSED( engine ); Q_UNUSED( scriptEngine ); return new _class(); } )

namespace QFieldCore
{
  void registerQmlTypes()
  {
    qmlRegisterType<QgsLocatorProxyModel>( "org.qgis", 1, 0, "QgsLocatorProxyModel" );
    qmlRegisterType<QgsMapThemeCollection>( "org.qgis", 1, 0, "MapThemeCollection" );
    qmlRegisterType<QgsSnappingUtils>( "org.qgis", 1, 0, "SnappingUtils" );
    qmlRegisterType<QgsVectorLayer>( "org.qgis", 1, 0, "VectorLayer" );
    qmlRegisterType<QgsVectorLayerEditBuffer>( "org.qgis", 1, 0, "QgsVectorLayerEditBuffer" );

    qmlRegisterUncreatableMetaObject( Qgis::staticMetaObject, "org.qgis", 1, 0, "Qgis", "Used to access enum values" );

    qmlRegisterUncreatableType<QgsLocatorContext>( "org.qgis", 1, 0, "locatorContext", "Used as parameter type in invokable function" );
    qmlRegisterUncreatableType<QgsMapLayer>( "org.qgis", 1, 0, "MapLayer", "" );
    qmlRegisterUncreatableType<QgsProject>( "org.qgis", 1, 0, "Project", "" );
    qmlRegisterUncreatableType<QgsProjectDisplaySettings>( "org.qgis", 1, 0, "ProjectDisplaySettings", "" );
    qmlRegisterUncreatableType<QgsRasterLayer>( "org.qgis", 1, 0, "RasterLayer", "" );
    qmlRegisterUncreatableType<QgsRelationManager>( "org.qgis", 1, 0, "RelationManager", "The relation manager is available from the QgsProject. Try `qgisProject.relationManager`" );
    qmlRegisterUncreatableType<QgsVectorLayer>( "org.qgis", 1, 0, "VectorLayerStatic", "" );

    qmlRegisterType<QgsQuickElevationProfileCanvas>( "org.qgis", 1, 0, "ElevationProfileCanvas" );
    qmlRegisterType<QgsQuickMapCanvasMap>( "org.qgis", 1, 0, "MapCanvasMap" );
    qmlRegisterType<QgsQuickMapSettings>( "org.qgis", 1, 0, "MapSettings" );
    qmlRegisterType<QgsQuickMapTransform>( "org.qgis", 1, 0, "MapTransform" );

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
    qmlRegisterType<ResourceSource>( "org.qfield.core", 1, 0, "ResourceSource" );
    qmlRegisterType<RubberbandModel>( "org.qfield.core", 1, 0, "RubberbandModel" );
    qmlRegisterType<RubberbandShape>( "org.qfield.core", 1, 0, "RubberbandShape" );
    qmlRegisterType<ScaleBarMeasurement>( "org.qfield.core", 1, 0, "ScaleBarMeasurement" );
    qmlRegisterType<SnappingUtils>( "org.qfield.core", 1, 0, "SnappingUtils" );
    qmlRegisterType<SubModel>( "org.qfield.core", 1, 0, "SubModel" );
    qmlRegisterType<VertexModel>( "org.qfield.core", 1, 0, "VertexModel" );
    qmlRegisterType<VideoSinkCapture>( "org.qfield.core", 1, 0, "VideoSinkCapture" );
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
