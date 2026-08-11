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
#include "qfgeometrywrapper.h"
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

    qmlRegisterType<QfAppExpressionContextScopesGenerator>( "org.qfield.core", 1, 0, "QfAppExpressionContextScopesGenerator" );
    qmlRegisterType<QfAudioAnalyzer>( "org.qfield.core", 1, 0, "QfAudioAnalyzer" );
    qmlRegisterType<QfAudioRecorder>( "org.qfield.core", 1, 0, "QfAudioRecorder" );
    qmlRegisterType<QfBarcodeDecoder>( "org.qfield.core", 1, 0, "QfBarcodeDecoder" );
    qmlRegisterType<QfCameraOrientationNormalizer>( "org.qfield.core", 1, 0, "QfCameraOrientationNormalizer" );
    qmlRegisterType<QfCogoExecutor>( "org.qfield.core", 1, 0, "QfCogoExecutor" );
    qmlRegisterType<QfCogoOperationsModel>( "org.qfield.core", 1, 0, "QfCogoOperationsModel" );
    qmlRegisterType<QfDeltaListModel>( "org.qfield.core", 1, 0, "QfDeltaListModel" );
    qmlRegisterType<QfDigitizingLogger>( "org.qfield.core", 1, 0, "QfDigitizingLogger" );
    qmlRegisterType<QfDistanceArea>( "org.qfield.core", 1, 0, "QfDistanceArea" );
    qmlRegisterType<QfDrawingCanvas>( "org.qfield.core", 1, 0, "QfDrawingCanvas" );
    qmlRegisterType<QfExternalStorage>( "org.qfield.core", 1, 0, "QfExternalStorage" );
    qmlRegisterType<QfFeatureListExtentController>( "org.qfield.core", 1, 0, "QfFeatureListExtentController" );
    qmlRegisterType<QfFeatureListModel>( "org.qfield.core", 1, 0, "QfFeatureListModel" );
    qmlRegisterType<QfFeatureListModelSelection>( "org.qfield.core", 1, 0, "QfFeatureListModelSelection" );
    qmlRegisterType<QfFeatureModel>( "org.qfield.core", 1, 0, "QfFeatureModel" );
    qmlRegisterType<QfGeofencer>( "org.qfield.core", 1, 0, "QfGeofencer" );
    qmlRegisterType<QfGeometry>( "org.qfield.core", 1, 0, "QfGeometry" );
    qmlRegisterType<QfGridModel>( "org.qfield.core", 1, 0, "QfGridModel" );
    qmlRegisterType<QfIdentifyTool>( "org.qfield.core", 1, 0, "QfIdentifyTool" );
    qmlRegisterType<QfLayerResolver>( "org.qfield.core", 1, 0, "QfLayerResolver" );
    qmlRegisterType<QfLinePolygonShape>( "org.qfield.core", 1, 0, "QfLinePolygonShape" );
    qmlRegisterType<QfLocatorModelSuperBridge>( "org.qfield.core", 1, 0, "QfLocatorModelSuperBridge" );
    qmlRegisterType<QfMapLayerModel>( "org.qfield.core", 1, 0, "QfMapLayerModel" );
    qmlRegisterType<QfMapToScreen>( "org.qfield.core", 1, 0, "QfMapToScreen" );
    qmlRegisterType<QfMultiFeatureListModel>( "org.qfield.core", 1, 0, "QfMultiFeatureListModel" );
    qmlRegisterType<QfNavigation>( "org.qfield.core", 1, 0, "QfNavigation" );
    qmlRegisterType<QfNavigationModel>( "org.qfield.core", 1, 0, "QfNavigationModel" );
    qmlRegisterType<QfNearFieldReader>( "org.qfield.core", 1, 0, "QfNearFieldReader" );
    qmlRegisterType<QfNtripSourceTableFetcher>( "org.qfield.core", 1, 0, "QfNtripSourceTableFetcher" );
    qmlRegisterType<QfPositioning>( "org.qfield.core", 1, 0, "QfPositioning" );
    qmlRegisterType<QfPositioningDeviceModel>( "org.qfield.core", 1, 0, "QfPositioningDeviceModel" );
    qmlRegisterType<QfPositioningInformationModel>( "org.qfield.core", 1, 0, "QfPositioningInformationModel" );
    qmlRegisterType<QfProcessingAlgorithm>( "org.qfield.core", 1, 0, "QfProcessingAlgorithm" );
    qmlRegisterType<QfProcessingAlgorithmParametersModel>( "org.qfield.core", 1, 0, "QfProcessingAlgorithmParametersModel" );
    qmlRegisterType<QfProcessingAlgorithmsModel>( "org.qfield.core", 1, 0, "QfProcessingAlgorithmsModel" );
    qmlRegisterType<QfProjectInfo>( "org.qfield.core", 1, 0, "QfProjectInfo" );
    qmlRegisterType<QfProjectSource>( "org.qfield.core", 1, 0, "QfProjectSource" );
    qmlRegisterType<QfCloudConnection>( "org.qfield.core", 1, 0, "QfCloudConnection" );
    qmlRegisterType<QfCloudProject>( "org.qfield.core", 1, 0, "QfCloudProject" );
    qmlRegisterType<QfCloudProjectsModel>( "org.qfield.core", 1, 0, "QfCloudProjectsModel" );
    qmlRegisterType<QfCloudStatus>( "org.qfield.core", 1, 0, "QfCloudStatus" );
    qmlRegisterType<QfLocatorFilter>( "org.qfield.core", 1, 0, "QfLocatorFilter" );
    qmlRegisterType<QfGeometryWrapper>( "org.qfield.core", 1, 0, "QfGeometryWrapper" );
    qmlRegisterType<QgsQuickCoordinateTransformer>( "org.qfield.core", 1, 0, "CoordinateTransformer" );
    qmlRegisterType<QfResourceSource>( "org.qfield.core", 1, 0, "QfResourceSource" );
    qmlRegisterType<QfRubberbandModel>( "org.qfield.core", 1, 0, "QfRubberbandModel" );
    qmlRegisterType<QfRubberbandShape>( "org.qfield.core", 1, 0, "QfRubberbandShape" );
    qmlRegisterType<QfScaleBarMeasurement>( "org.qfield.core", 1, 0, "QfScaleBarMeasurement" );
    qmlRegisterType<QfSnappingUtils>( "org.qfield.core", 1, 0, "QfSnappingUtils" );
    qmlRegisterType<QfSubModel>( "org.qfield.core", 1, 0, "QfSubModel" );
    qmlRegisterType<QfVertexModel>( "org.qfield.core", 1, 0, "QfVertexModel" );
    qmlRegisterType<QfVideoSinkCapture>( "org.qfield.core", 1, 0, "QfVideoSinkCapture" );
    qmlRegisterType<QfViewStatus>( "org.qfield.core", 1, 0, "QfViewStatus" );
    qmlRegisterType<QfWebdavConnection>( "org.qfield.core", 1, 0, "QfWebdavConnection" );

    REGISTER_SINGLETON( "org.qfield.core", QfAuthUtils, "QfAuthUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QfCoordinateReferenceSystemUtils, "QfCoordinateReferenceSystemUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QfExpressionContextUtils, "QfExpressionContextUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QfFeatureUtils, "QfFeatureUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QfFileUtils, "QfFileUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QfGeometryUtils, "QfGeometryUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QfLayerUtils, "QfLayerUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QfPositioningUtils, "QfPositioningUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QfProcessingUtils, "QfProcessingUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QfProjectUtils, "QfProjectUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QfCloudUtils, "QfCloudUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QfRelationUtils, "QfRelationUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QfStringUtils, "QfStringUtils" );
    REGISTER_SINGLETON( "org.qfield.core", QfUrlUtils, "QfUrlUtils" );

    qmlRegisterUncreatableType<QfAbstractGnssReceiver>( "org.qfield.core", 1, 0, "QfAbstractGnssReceiver", "" );
    qmlRegisterUncreatableType<QfAppInterface>( "org.qfield.core", 1, 0, "QfAppInterface", "AppInterface is only provided by the environment and cannot be created ad-hoc" );
    qmlRegisterUncreatableType<QfBookmarkModel>( "org.qfield.core", 1, 0, "QfBookmarkModel", "The BookmarkModel is available as context property `bookmarkModel`" );
    qmlRegisterUncreatableType<QfCogoParameter>( "org.qfield.core", 1, 0, "cogoParameter", "Used for property values" );
    qmlRegisterUncreatableType<QfCogoVisualGuide>( "org.qfield.core", 1, 0, "cogoVisualGuide", "Used for property values" );
    qmlRegisterUncreatableType<QfDeltaFileWrapper>( "org.qfield.core", 1, 0, "QfDeltaFileWrapper", "" );
    qmlRegisterUncreatableType<QfFlatLayerTreeModel>( "org.qfield.core", 1, 0, "QfFlatLayerTreeModel", "The FlatLayerTreeModel is available as context property `flatLayerTree`." );
    qmlRegisterUncreatableType<QfGnssPositionInformation>( "org.qfield.core", 1, 0, "gnssPositionInformation", "Used for property values" );
    qmlRegisterUncreatableType<QfGridAnnotation>( "org.qfield.core", 1, 0, "gridAnnotation", "Used for property values" );
    qmlRegisterUncreatableType<QfLayerObserver>( "org.qfield.core", 1, 0, "QfLayerObserver", "" );
    qmlRegisterUncreatableType<QfNtripSettings>( "org.qfield.core", 1, 0, "ntripSettings", "Used for property values" );
    qmlRegisterUncreatableType<QfPlatformUtilities>( "org.qfield.core", 1, 0, "QfPlatformUtilities", "" );
    qmlRegisterUncreatableType<QgsGpkgFlusher>( "org.qfield.core", 1, 0, "QgsGpkgFlusher", "The gpkgFlusher is available as context property `gpkgFlusher`" );
    qmlRegisterUncreatableType<QfSettings>( "org.qfield.core", 1, 0, "QfSettings", "" );
    qmlRegisterUncreatableType<QfTracker>( "org.qfield.core", 1, 0, "QfTracker", "" );
    qmlRegisterUncreatableType<QfTrackingModel>( "org.qfield.core", 1, 0, "QfTrackingModel", "The TrackingModel is available as context property `trackingModel`." );

#ifdef WITH_BLUETOOTH
    qmlRegisterType<QfBluetoothDeviceModel>( "org.qfield.core", 1, 0, "QfBluetoothDeviceModel" );
    qmlRegisterType<QfBluetoothReceiver>( "org.qfield.core", 1, 0, "QfBluetoothReceiver" );
#endif
#ifdef WITH_SERIALPORT
    qmlRegisterType<QfSerialPortModel>( "org.qfield.core", 1, 0, "QfSerialPortModel" );
    qmlRegisterType<QfSerialPortReceiver>( "org.qfield.core", 1, 0, "QfSerialPortReceiver" );
#endif

    qmlRegisterType<QfFeatureIterator>( "org.qfield.core", 1, 0, "featureIterator" );
    qmlRegisterType<QfLocatorActionsModel>( "org.qfield.core", 1, 0, "QfLocatorActionsModel" );
    qmlRegisterType<QfLocatorFiltersModel>( "org.qfield.core", 1, 0, "QfLocatorFiltersModel" );
    qmlRegisterType<QfCloudProjectsFilterModel>( "org.qfield.core", 1, 0, "QfCloudProjectsFilterModel" );
    qmlRegisterUncreatableMetaObject( QfCogoVisualGuide::staticMetaObject, "org.qfield.core", 1, 0, "QfCogoVisualGuide", "Used to access enum values" );
    qmlRegisterUncreatableMetaObject( QfGnssPositionInformation::staticMetaObject, "org.qfield.core", 1, 0, "QfGnssPositionInformation", "Used to access to enum values" );
    qmlRegisterUncreatableMetaObject( QfGridAnnotation::staticMetaObject, "org.qfield.core", 1, 0, "QfGridAnnotation", "Used to access enum values" );
    qmlRegisterUncreatableMetaObject( QfNtripSettings::staticMetaObject, "org.qfield.core", 1, 0, "QfNtripSettings", "Used to access to enum values" );
    qmlRegisterUncreatableMetaObject( QfCloudDelta::staticMetaObject, "org.qfield.core", 1, 0, "QfCloudDelta", "Used to access to enum values" );
    qmlRegisterUncreatableType<QAbstractSocket>( "org.qfield.core", 1, 0, "QAbstractSocket", "" );
  }
} // namespace QFieldCore

#undef REGISTER_SINGLETON
