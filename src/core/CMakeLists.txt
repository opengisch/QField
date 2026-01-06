set(QFIELD_CORE_SRCS
    cogo/cogoexecutor.cpp
    cogo/cogooperation.cpp
    cogo/cogooperationsmodel.cpp
    cogo/cogoregistry.cpp
    platforms/platformutilities.cpp
    utils/coordinatereferencesystemutils.cpp
    utils/expressioncontextutils.cpp
    utils/featureutils.cpp
    utils/fileutils.cpp
    utils/geometryutils.cpp
    utils/layerutils.cpp
    utils/positioningutils.cpp
    utils/processingutils.cpp
    utils/projectutils.cpp
    utils/qfieldcloudutils.cpp
    utils/relationutils.cpp
    utils/snappingutils.cpp
    utils/stringutils.cpp
    utils/urlutils.cpp
    qgsquick/qgsquickcoordinatetransformer.cpp
    qgsquick/qgsquickmapcanvasmap.cpp
    qgsquick/qgsquickelevationprofilecanvas.cpp
    qgsquick/qgsquickmapsettings.cpp
    qgsquick/qgsquickmaptransform.cpp
    locator/activelayerfeatureslocatorfilter.cpp
    locator/bookmarklocatorfilter.cpp
    locator/expressioncalculatorlocatorfilter.cpp
    locator/featureslocatorfilter.cpp
    locator/finlandlocatorfilter.cpp
    locator/gotolocatorfilter.cpp
    locator/helplocatorfilter.cpp
    locator/qfieldlocatorfilter.cpp
    locator/locatormodelsuperbridge.cpp
    positioning/abstractgnssreceiver.cpp
    positioning/gnsspositioninformation.cpp
    positioning/internalgnssreceiver.cpp
    positioning/nmeagnssreceiver.cpp
    positioning/egenioussreceiver.cpp
    positioning/filereceiver.cpp
    positioning/tcpreceiver.cpp
    positioning/udpreceiver.cpp
    positioning/positioning.cpp
    positioning/positioningsource.cpp
    positioning/positioningdevicemodel.cpp
    positioning/geofencer.cpp
    positioning/positioninginformationmodel.cpp
    processing/processingalgorithm.cpp
    processing/processingalgorithmparametersmodel.cpp
    processing/processingalgorithmsmodel.cpp
    qfieldcloud/deltafilewrapper.cpp
    qfieldcloud/deltalistmodel.cpp
    qfieldcloud/layerobserver.cpp
    qfieldcloud/networkmanager.cpp
    qfieldcloud/networkreply.cpp
    qfieldcloud/qfieldcloudconnection.cpp
    qfieldcloud/qfieldcloudproject.cpp
    qfieldcloud/qfieldcloudprojectsmodel.cpp
    appcoordinateoperationhandlers.cpp
    appexpressioncontextscopesgenerator.cpp
    appinterface.cpp
    attributeformmodel.cpp
    attributeformmodelbase.cpp
    audiorecorder.cpp
    barcodedecoder.cpp
    barcodeimageprovider.cpp
    badlayerhandler.cpp
    bookmarkmodel.cpp
    clipboardmanager.cpp
    changelogcontents.cpp
    digitizinglogger.cpp
    distancearea.cpp
    drawingcanvas.cpp
    drawingtemplatemodel.cpp
    expressionevaluator.cpp
    expressionvariablemodel.cpp
    externalstorage.cpp
    featurechecklistmodel.cpp
    featurelistextentcontroller.cpp
    featurelistmodel.cpp
    featurelistmodelselection.cpp
    featuremodel.cpp
    focusstack.cpp
    geometry.cpp
    geometryeditorsmodel.cpp
    gridmodel.cpp
    identifytool.cpp
    featurehistory.cpp
    layerresolver.cpp
    layertreemapcanvasbridge.cpp
    layertreemodel.cpp
    legendimageprovider.cpp
    linepolygonshape.cpp
    localfilesimageprovider.cpp
    localfilesmodel.cpp
    maplayermodel.cpp
    maptoscreen.cpp
    messagelogmodel.cpp
    multifeaturelistmodel.cpp
    multifeaturelistmodelbase.cpp
    navigation.cpp
    navigationmodel.cpp
    nearfieldreader.cpp
    orderedrelationmodel.cpp
    parametizedimage.cpp
    peliasgeocoder.cpp
    pluginmanager.cpp
    pluginmodel.cpp
    resourcesource.cpp
    translatormanager.cpp
    printlayoutlistmodel.cpp
    projectinfo.cpp
    projectsource.cpp
    projectsimageprovider.cpp
    qfieldappauthrequesthandler.cpp
    qfieldurlhandler.cpp
    qgismobileapp.cpp
    qgsgeometrywrapper.cpp
    qgsgpkgflusher.cpp
    recentprojectlistmodel.cpp
    referencingfeaturelistmodel.cpp
    rubberbandshape.cpp
    rubberbandmodel.cpp
    scalebarmeasurement.cpp
    screendimmer.cpp
    sensorlistmodel.cpp
    settings.cpp
    snappingresult.cpp
    submodel.cpp
    tracker.cpp
    trackingmodel.cpp
    valuemapmodel.cpp
    valuemapmodelbase.cpp
    vertexmodel.cpp
    viewstatus.cpp
    webdavconnection.cpp)

set(QFIELD_CORE_HDRS
    cogo/cogoexecutor.h
    cogo/cogooperation.h
    cogo/cogooperationsmodel.h
    cogo/cogoregistry.h
    platforms/platformutilities.h
    utils/coordinatereferencesystemutils.h
    utils/expressioncontextutils.h
    utils/featureutils.h
    utils/fileutils.h
    utils/geometryutils.h
    utils/layerutils.h
    utils/positioningutils.h
    utils/processingutils.h
    utils/projectutils.h
    utils/qfieldcloudutils.h
    utils/relationutils.h
    utils/snappingutils.h
    utils/stringutils.h
    utils/urlutils.h
    qgsquick/qgsquickcoordinatetransformer.h
    qgsquick/qgsquickmapcanvasmap.h
    qgsquick/qgsquickelevationprofilecanvas.h
    qgsquick/qgsquickmapsettings.h
    qgsquick/qgsquickmaptransform.h
    locator/activelayerfeatureslocatorfilter.h
    locator/bookmarklocatorfilter.h
    locator/expressioncalculatorlocatorfilter.h
    locator/featureslocatorfilter.h
    locator/finlandlocatorfilter.h
    locator/gotolocatorfilter.h
    locator/helplocatorfilter.h
    locator/qfieldlocatorfilter.h
    locator/locatormodelsuperbridge.h
    positioning/abstractgnssreceiver.h
    positioning/gnsspositioninformation.h
    positioning/positioning.h
    positioning/positioningsource.h
    positioning/positioningdevicemodel.h
    positioning/internalgnssreceiver.h
    positioning/nmeagnssreceiver.h
    positioning/egenioussreceiver.h
    positioning/filereceiver.h
    positioning/tcpreceiver.h
    positioning/udpreceiver.h
    positioning/geofencer.h
    positioning/positioninginformationmodel.h
    processing/processingalgorithm.h
    processing/processingalgorithmparametersmodel.h
    processing/processingalgorithmsmodel.h
    qfieldcloud/deltafilewrapper.h
    qfieldcloud/deltalistmodel.h
    qfieldcloud/layerobserver.h
    qfieldcloud/networkmanager.h
    qfieldcloud/networkreply.h
    qfieldcloud/qfieldcloudconnection.h
    qfieldcloud/qfieldcloudproject.h
    qfieldcloud/qfieldcloudprojectsmodel.h
    appcoordinateoperationhandlers.h
    appexpressioncontextscopesgenerator.h
    appinterface.h
    attributeformmodel.h
    attributeformmodelbase.h
    audiorecorder.h
    barcodedecoder.h
    barcodeimageprovider.h
    badlayerhandler.h
    bookmarkmodel.h
    clipboardmanager.h
    changelogcontents.h
    digitizinglogger.h
    distancearea.h
    drawingcanvas.h
    drawingtemplatemodel.h
    expressionevaluator.h
    expressionvariablemodel.h
    externalstorage.h
    featurechecklistmodel.h
    featureexpressionvaluesgatherer.h
    featurelistextentcontroller.h
    featurelistmodel.h
    featurelistmodelselection.h
    featuremodel.h
    focusstack.h
    geometry.h
    geometryeditorsmodel.h
    gridmodel.h
    identifytool.h
    featurehistory.h
    layerresolver.h
    layertreemapcanvasbridge.h
    layertreemodel.h
    legendimageprovider.h
    linepolygonshape.h
    localfilesimageprovider.h
    localfilesmodel.h
    maplayermodel.h
    maptoscreen.h
    messagelogmodel.h
    multifeaturelistmodel.h
    multifeaturelistmodelbase.h
    navigation.h
    navigationmodel.h
    nearfieldreader.h
    orderedrelationmodel.h
    parametizedimage.h
    peliasgeocoder.h
    pluginmanager.h
    pluginmodel.h
    resourcesource.h
    translatormanager.h
    printlayoutlistmodel.h
    projectinfo.h
    projectsource.h
    projectsimageprovider.h
    qfieldappauthrequesthandler.h
    qfieldurlhandler.h
    qgismobileapp.h
    qgsgeometrywrapper.h
    qgsgpkgflusher.h
    recentprojectlistmodel.h
    referencingfeaturelistmodel.h
    rubberbandshape.h
    rubberbandmodel.h
    scalebarmeasurement.h
    screendimmer.h
    sensorlistmodel.h
    settings.h
    snappingresult.h
    submodel.h
    tracker.h
    trackingmodel.h
    valuemapmodel.h
    valuemapmodelbase.h
    vertexmodel.h
    viewstatus.h
    webdavconnection.h
    ${CMAKE_CURRENT_BINARY_DIR}/qfield.h)

list(APPEND QFIELD_CORE_SRCS permissions.cpp)
list(APPEND QFIELD_CORE_HDRS permissions.h)

if(WITH_BLUETOOTH)
  list(APPEND QFIELD_CORE_SRCS positioning/bluetoothdevicemodel.cpp
       positioning/bluetoothreceiver.cpp)
  list(APPEND QFIELD_CORE_HDRS positioning/bluetoothdevicemodel.h
       positioning/bluetoothreceiver.h)
endif()

if(WITH_SERIALPORT)
  list(APPEND QFIELD_CORE_SRCS positioning/serialportmodel.cpp
       positioning/serialportreceiver.cpp)
  list(APPEND QFIELD_CORE_HDRS positioning/serialportmodel.h
       positioning/serialportreceiver.h)
endif()

if(ANDROID)
  set(QFIELD_CORE_HDRS
      ${QFIELD_CORE_HDRS}
      platforms/android/androidplatformutilities.h
      platforms/android/androidresourcesource.h
      platforms/android/androidviewstatus.h
      platforms/android/androidprojectsource.h
      ${CMAKE_CURRENT_BINARY_DIR}/qfield_android.h)
  set(QFIELD_CORE_SRCS
      ${QFIELD_CORE_SRCS}
      platforms/android/androidplatformutilities.cpp
      platforms/android/androidresourcesource.cpp
      platforms/android/androidviewstatus.cpp
      platforms/android/androidprojectsource.cpp)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
  set(QFIELD_CORE_HDRS
      ${QFIELD_CORE_HDRS} platforms/ios/iosplatformutilities.h
      platforms/ios/iosresourcesource.h platforms/ios/iosprojectsource.h)
  set(QFIELD_CORE_SRCS
      ${QFIELD_CORE_SRCS} platforms/ios/iosplatformutilities.mm
      platforms/ios/iosresourcesource.mm platforms/ios/iosprojectsource.mm)
endif()

find_package(SQLite3 REQUIRED)
find_package(ZXing REQUIRED)
find_package(QtWebDAV REQUIRED)

add_library(qfield_core STATIC ${QFIELD_CORE_SRCS} ${QFIELD_CORE_HDRS})

string(SUBSTRING ${ZXing_VERSION} 0 1 ZXing_VERSION_MAJOR)
target_compile_definitions(qfield_core
                           PRIVATE ZXing_VERSION_MAJOR=${ZXing_VERSION_MAJOR})

if(WITH_BLUETOOTH)
  target_compile_definitions(qfield_core PRIVATE WITH_BLUETOOTH)
endif()
if(WITH_SERIALPORT)
  target_compile_definitions(qfield_core PRIVATE WITH_SERIALPORT)
endif()
if(WITH_NFC)
  target_compile_definitions(qfield_core PRIVATE WITH_NFC)
endif()

include(GenerateExportHeader)
generate_export_header(qfield_core)

configure_file(qfield.h.in ${CMAKE_CURRENT_BINARY_DIR}/qfield.h @ONLY)

target_include_directories(qfield_core SYSTEM
                           PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

if(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
  target_compile_options(qfield_core PRIVATE "-fobjc-arc")
endif()

target_include_directories(
  qfield_core
  PUBLIC ${CMAKE_SOURCE_DIR}/src/core
         ${CMAKE_SOURCE_DIR}/src/core/cogo
         ${CMAKE_SOURCE_DIR}/src/core/locator
         ${CMAKE_SOURCE_DIR}/src/core/platforms
         ${CMAKE_SOURCE_DIR}/src/core/qgsquick
         ${CMAKE_SOURCE_DIR}/src/core/utils
         ${CMAKE_SOURCE_DIR}/src/core/positioning
         ${CMAKE_SOURCE_DIR}/src/core/processing
         ${CMAKE_SOURCE_DIR}/src/core/qfieldcloud)
if(ANDROID)
  string(REPLACE "_" "_1" APP_PACKAGE_JNI_NAME "${APP_PACKAGE_NAME}")
  configure_file(platforms/android/qfield_android.h.in
                 ${CMAKE_CURRENT_BINARY_DIR}/qfield_android.h @ONLY)
  target_include_directories(
    qfield_core PUBLIC ${CMAKE_SOURCE_DIR}/src/core/platforms/android)
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  target_include_directories(qfield_core
                             PUBLIC ${CMAKE_SOURCE_DIR}/src/core/platforms/ios)
endif()

target_compile_features(qfield_core PUBLIC cxx_std_17)
set_target_properties(qfield_core PROPERTIES AUTOMOC TRUE)

target_link_libraries(
  qfield_core
  PUBLIC Qt::Core
         Qt::CorePrivate
         Qt::Gui
         Qt::GuiPrivate
         Qt::Xml
         Qt::Positioning
         Qt::Widgets
         Qt::Network
         Qt::Quick
         Qt::Svg
         Qt::Sensors
         Qt::Sql
         Qt::Concurrent
         Qt::WebView
         Qt::Multimedia
         Qt::RemoteObjects
         Qt::WebSockets
         Qt::Quick3D
         QGIS::Core
         QGIS::Analysis
         ZXing::ZXing
         PROJ::proj
         GDAL::GDAL
         SQLite::SQLite3
         Qca::qca
         libzip::zip
         QtWebDAV::QtWebDAV)

if(WITH_BLUETOOTH)
  find_package(
    Qt6
    COMPONENTS Bluetooth
    REQUIRED)
  target_link_libraries(qfield_core PUBLIC Qt6::Bluetooth)
endif()

if(WITH_SERIALPORT)
  find_package(
    Qt6
    COMPONENTS SerialPort
    REQUIRED)

  target_link_libraries(qfield_core PUBLIC Qt::SerialPort)
endif()

if(WITH_NFC)
  find_package(
    Qt6
    COMPONENTS Nfc
    REQUIRED)
  target_link_libraries(qfield_core PUBLIC Qt6::Nfc)
endif()

if(WITH_SENTRY)
  target_link_libraries(qfield_core PRIVATE qfield_sentry)
endif()

if(TARGET Qt::PrintSupport)
  target_link_libraries(qfield_core PUBLIC Qt::PrintSupport)
endif()

if(MSVC)
  find_package(FFMPEG REQUIRED)
  find_library(AVCODEC_LIBRARY avcodec)
  find_library(AVDEVICE_LIBRARY avdevice)
  find_library(AVFORMAT_LIBRARY avformat)
  find_library(AVUTIL_LIBRARY avutil)
  find_library(SWRESAMPLE_LIBRARY swresample)
  find_library(SWSCALE_LIBRARY swscale)
  find_library(OPENH264_LIBRARY openh264)
  target_link_libraries(
    qfield_core
    PUBLIC ${AVCODEC_LIBRARY}
           ${AVDEVICE_LIBRARY}
           ${AVFORMAT_LIBRARY}
           ${AVUTIL_LIBRARY}
           ${SWRESAMPLE_LIBRARY}
           ${SWSCALE_LIBRARY}
           ${OPENH264_LIBRARY})
endif()

install(FILES ${QFIELD_CORE_HDRS} DESTINATION ${QFIELD_INCLUDE_DIR})
install(
  TARGETS qfield_core
  BUNDLE DESTINATION ${QFIELD_BIN_DIR}
  RUNTIME DESTINATION ${QFIELD_BIN_DIR}
  LIBRARY DESTINATION ${QFIELD_LIB_DIR}
  ARCHIVE DESTINATION ${QFIELD_LIB_DIR}
  FRAMEWORK DESTINATION ${QFIELD_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QFIELD_INCLUDE_DIR})
