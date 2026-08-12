set(QFIELD_CORE_SRCS
    cogo/qfcogoexecutor.cpp
    cogo/qfcogooperation.cpp
    cogo/qfcogooperationsmodel.cpp
    cogo/qfcogoregistry.cpp
    platforms/qfplatformutilities.cpp
    utils/qfauthutils.cpp
    utils/qfcoordinatereferencesystemutils.cpp
    utils/qfexpressioncontextutils.cpp
    utils/qffeatureutils.cpp
    utils/qffileutils.cpp
    utils/qfgeometryutils.cpp
    utils/qflayerutils.cpp
    utils/qfpositioningutils.cpp
    utils/qfprocessingutils.cpp
    utils/qfprojectutils.cpp
    utils/qfcloudutils.cpp
    utils/qfrelationutils.cpp
    utils/qfsnappingutils.cpp
    utils/qfstringutils.cpp
    utils/qfurlutils.cpp
    qgsquick/qgsquickcoordinatetransformer.cpp
    qgsquick/qgsquickmapcanvasmap.cpp
    qgsquick/qgsquickelevationprofilecanvas.cpp
    qgsquick/qgsquickmapsettings.cpp
    qgsquick/qgsquickmaptransform.cpp
    locator/qfactivelayerfeatureslocatorfilter.cpp
    locator/qfbookmarklocatorfilter.cpp
    locator/qfexpressioncalculatorlocatorfilter.cpp
    locator/qffeatureslocatorfilter.cpp
    locator/qffinlandlocatorfilter.cpp
    locator/qfgotolocatorfilter.cpp
    locator/qfhelplocatorfilter.cpp
    locator/qflocatorfilter.cpp
    locator/qflocatormodelsuperbridge.cpp
    positioning/qfabstractgnssreceiver.cpp
    positioning/qfgnsspositioninformation.cpp
    positioning/qfinternalgnssreceiver.cpp
    positioning/qfnmeagnssreceiver.cpp
    positioning/qfegenioussreceiver.cpp
    positioning/qffilereceiver.cpp
    positioning/qftcpreceiver.cpp
    positioning/qfudpreceiver.cpp
    positioning/qfpositioning.cpp
    positioning/qfpositioningsource.cpp
    positioning/qfntripclient.cpp
    positioning/qfntripsettings.cpp
    positioning/qfntripsourcetablefetcher.cpp
    positioning/qfpositioningdevicemodel.cpp
    positioning/qfgeofencer.cpp
    positioning/qfpositioninginformationmodel.cpp
    processing/qfprocessingalgorithm.cpp
    processing/qfprocessingalgorithmparametersmodel.cpp
    processing/qfprocessingalgorithmsmodel.cpp
    qfieldcloud/qfdeltafilewrapper.cpp
    qfieldcloud/qfdeltalistmodel.cpp
    qfieldcloud/qflayerobserver.cpp
    qfieldcloud/qfnetworkmanager.cpp
    qfieldcloud/qfnetworkreply.cpp
    qfieldcloud/qfcloudconnection.cpp
    qfieldcloud/qfcloudproject.cpp
    qfieldcloud/qfcloudprojectsmodel.cpp
    qfieldcloud/qfcloudstatus.cpp
    qfappexpressioncontextscopesgenerator.cpp
    qfappinterface.cpp
    qfapplicationinformation.cpp
    qfaudioanalyzer.cpp
    qfaudiorecorder.cpp
    qfbarcodedecoder.cpp
    qfbookmarkmodel.cpp
    qfcameraorientationnormalizer.cpp
    qfdigitizinglogger.cpp
    qfdistancearea.cpp
    qfdrawingcanvas.cpp
    qfexternalstorage.cpp
    qffeaturelistextentcontroller.cpp
    qffeaturelistmodel.cpp
    qffeaturelistmodelselection.cpp
    qffeaturemodel.cpp
    qfgeometry.cpp
    qfgridmodel.cpp
    qfidentifytool.cpp
    qffeaturehistory.cpp
    qflayerresolver.cpp
    qflayertreemapcanvasbridge.cpp
    qflayertreemodel.cpp
    qflinepolygonshape.cpp
    qfmaplayermodel.cpp
    qfmaptoscreen.cpp
    qfmultifeaturelistmodel.cpp
    qfmultifeaturelistmodelbase.cpp
    qfnavigation.cpp
    qfnavigationmodel.cpp
    qfnearfieldreader.cpp
    qfpeliasgeocoder.cpp
    qfresourcesource.cpp
    qftranslatormanager.cpp
    qfprojectinfo.cpp
    qfprojectsource.cpp
    qfcore.cpp
    qfxmlhttprequest.cpp
    qfgeometrywrapper.cpp
    qgsgpkgflusher.cpp
    qfrubberbandshape.cpp
    qfrubberbandmodel.cpp
    qfscalebarmeasurement.cpp
    qfsettings.cpp
    qfsnappingresult.cpp
    qfsubmodel.cpp
    qftracker.cpp
    qftrackingmodel.cpp
    qfvertexmodel.cpp
    qfvideosinkcapture.cpp
    qfviewstatus.cpp
    qfwebdavconnection.cpp)

set(QFIELD_CORE_HDRS
    cogo/qfcogoexecutor.h
    cogo/qfcogooperation.h
    cogo/qfcogooperationsmodel.h
    cogo/qfcogoregistry.h
    platforms/qfplatformutilities.h
    utils/qfauthutils.h
    utils/qfcoordinatereferencesystemutils.h
    utils/qfexpressioncontextutils.h
    utils/qffeatureutils.h
    utils/qffileutils.h
    utils/qfgeometryutils.h
    utils/qflayerutils.h
    utils/qfpositioningutils.h
    utils/qfprocessingutils.h
    utils/qfprojectutils.h
    utils/qfcloudutils.h
    utils/qfrelationutils.h
    utils/qfsnappingutils.h
    utils/qfstringutils.h
    utils/qfurlutils.h
    qgsquick/qgsquickcoordinatetransformer.h
    qgsquick/qgsquickmapcanvasmap.h
    qgsquick/qgsquickelevationprofilecanvas.h
    qgsquick/qgsquickmapsettings.h
    qgsquick/qgsquickmaptransform.h
    locator/qfactivelayerfeatureslocatorfilter.h
    locator/qfbookmarklocatorfilter.h
    locator/qfexpressioncalculatorlocatorfilter.h
    locator/qffeatureslocatorfilter.h
    locator/qffinlandlocatorfilter.h
    locator/qfgotolocatorfilter.h
    locator/qfhelplocatorfilter.h
    locator/qflocatorfilter.h
    locator/qflocatormodelsuperbridge.h
    positioning/qfabstractgnssreceiver.h
    positioning/qfgnsspositioninformation.h
    positioning/qfpositioning.h
    positioning/qfpositioningsource.h
    positioning/qfpositioningdevicemodel.h
    positioning/qfinternalgnssreceiver.h
    positioning/qfnmeagnssreceiver.h
    positioning/qfegenioussreceiver.h
    positioning/qffilereceiver.h
    positioning/qfntripclient.h
    positioning/qfntripsettings.h
    positioning/qfntripsourcetablefetcher.h
    positioning/qftcpreceiver.h
    positioning/qfudpreceiver.h
    positioning/qfgeofencer.h
    positioning/qfpositioninginformationmodel.h
    processing/qfprocessingalgorithm.h
    processing/qfprocessingalgorithmparametersmodel.h
    processing/qfprocessingalgorithmsmodel.h
    qfieldcloud/qfdeltafilewrapper.h
    qfieldcloud/qfdeltalistmodel.h
    qfieldcloud/qflayerobserver.h
    qfieldcloud/qfnetworkmanager.h
    qfieldcloud/qfnetworkreply.h
    qfieldcloud/qfcloudconnection.h
    qfieldcloud/qfcloudproject.h
    qfieldcloud/qfcloudprojectsmodel.h
    qfieldcloud/qfcloudstatus.h
    qfappcontroller.h
    qfappexpressioncontextscopesgenerator.h
    qfappinterface.h
    qfapplicationinformation.h
    qfaudioanalyzer.h
    qfaudiorecorder.h
    qfbarcodedecoder.h
    qfbookmarkmodel.h
    qfcameraorientationnormalizer.h
    qfdigitizinglogger.h
    qfdistancearea.h
    qfdrawingcanvas.h
    qfexternalstorage.h
    qffeatureexpressionvaluesgatherer.h
    qffeaturelistextentcontroller.h
    qffeaturelistmodel.h
    qffeaturelistmodelselection.h
    qffeaturemodel.h
    qfgeometry.h
    qfgridmodel.h
    qfidentifytool.h
    qffeaturehistory.h
    qflayerresolver.h
    qflayertreemapcanvasbridge.h
    qflayertreemodel.h
    qflinepolygonshape.h
    qfmaplayermodel.h
    qfmaptoscreen.h
    qfmultifeaturelistmodel.h
    qfmultifeaturelistmodelbase.h
    qfnavigation.h
    qfnavigationmodel.h
    qfnearfieldreader.h
    qfpeliasgeocoder.h
    qfresourcesource.h
    qftranslatormanager.h
    qfprojectinfo.h
    qfprojectsource.h
    qfcore.h
    qfxmlhttprequest.h
    qfgeometrywrapper.h
    qgsgpkgflusher.h
    qfrubberbandshape.h
    qfrubberbandmodel.h
    qfscalebarmeasurement.h
    qfsettings.h
    qfsnappingresult.h
    qfsubmodel.h
    qftracker.h
    qftrackingmodel.h
    qfvertexmodel.h
    qfvideosinkcapture.h
    qfviewstatus.h
    qfwebdavconnection.h
    ${CMAKE_CURRENT_BINARY_DIR}/qfield.h)

if(WITH_BLUETOOTH)
  list(APPEND QFIELD_CORE_SRCS positioning/qfbluetoothdevicemodel.cpp
       positioning/qfbluetoothreceiver.cpp
       positioning/qfbluetoothlowenergyreceiver.cpp)
  list(APPEND QFIELD_CORE_HDRS positioning/qfbluetoothdevicemodel.h
       positioning/qfbluetoothreceiver.h
       positioning/qfbluetoothlowenergyreceiver.h)
endif()

if(WITH_SERIALPORT)
  list(APPEND QFIELD_CORE_SRCS positioning/qfserialportmodel.cpp
       positioning/qfserialportreceiver.cpp)
  list(APPEND QFIELD_CORE_HDRS positioning/qfserialportmodel.h
       positioning/qfserialportreceiver.h)
endif()

if(ANDROID)
  set(QFIELD_CORE_HDRS
      ${QFIELD_CORE_HDRS}
      platforms/android/qfandroidplatformutilities.h
      platforms/android/qfandroidresourcesource.h
      platforms/android/qfandroidviewstatus.h
      platforms/android/qfandroidprojectsource.h
      ${CMAKE_CURRENT_BINARY_DIR}/qfield_android.h)
  set(QFIELD_CORE_SRCS
      ${QFIELD_CORE_SRCS}
      platforms/android/qfandroidplatformutilities.cpp
      platforms/android/qfandroidresourcesource.cpp
      platforms/android/qfandroidviewstatus.cpp
      platforms/android/qfandroidprojectsource.cpp)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
  set(QFIELD_CORE_HDRS
      ${QFIELD_CORE_HDRS} platforms/ios/qfiosplatformutilities.h
      platforms/ios/qfiosresourcesource.h platforms/ios/qfiosprojectsource.h)
  set(QFIELD_CORE_SRCS
      ${QFIELD_CORE_SRCS} platforms/ios/qfiosplatformutilities.mm
      platforms/ios/qfiosresourcesource.mm platforms/ios/qfiosprojectsource.mm)
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

target_include_directories(
  qfield_core SYSTEM PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

if(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
  target_compile_options(qfield_core PRIVATE "-fobjc-arc")
endif()

# The public headers are installed flat into ${QFIELD_INCLUDE_SUBDIR}, so a
# consumer of the installed library needs that one directory, not the source
# tree. Without the BUILD_INTERFACE wrappers these absolute paths are baked into
# any exported target and only resolve on the build machine.
target_include_directories(
  qfield_core
  PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core>
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core/cogo>
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core/locator>
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core/platforms>
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core/qgsquick>
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core/3d>
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core/utils>
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core/positioning>
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core/processing>
         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core/qfieldcloud>
         $<INSTALL_INTERFACE:${QFIELD_INCLUDE_SUBDIR}>)
if(ANDROID)
  string(REPLACE "_" "_1" APP_PACKAGE_JNI_NAME "${APP_PACKAGE_NAME}")
  configure_file(platforms/android/qfield_android.h.in
                 ${CMAKE_CURRENT_BINARY_DIR}/qfield_android.h @ONLY)
  target_include_directories(
    qfield_core
    PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core/platforms/android>)
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  target_include_directories(
    qfield_core
    PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core/platforms/ios>)
endif()

target_compile_features(qfield_core PUBLIC cxx_std_20)
set_target_properties(qfield_core PROPERTIES AUTOMOC TRUE)

find_package(Qt6 REQUIRED COMPONENTS CorePrivate GuiPrivate)

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
         Qt::Multimedia
         Qt::RemoteObjects
         Qt::WebSockets
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

# generate_export_header() writes qfield_core_export.h into the build tree, and
# installed public headers (qgsquickmapsettings.h, qfplatformutilities.h, ...)
# include it, so it has to ship alongside them.
install(FILES ${QFIELD_CORE_HDRS}
              "${CMAKE_CURRENT_BINARY_DIR}/qfield_core_export.h"
        DESTINATION ${QFIELD_INCLUDE_DIR})
# Before the install() below, so QFIELD_CORE_QML_OUTPUT_TARGETS is set.
add_subdirectory(qml)

# Xcode cannot install the objects of OBJECT libraries ("may not be installed
# under Xcode with multiple architectures"), so the QML resource targets stay
# out of the install there; the package export is skipped for Xcode in the
# top-level CMakeLists.txt for the same reason.
if(CMAKE_GENERATOR STREQUAL "Xcode")
  set(QFIELD_CORE_QML_OUTPUT_TARGETS "")
endif()

# The OBJECTS destination is required: the QML module's generated *_resources_N
# targets are OBJECT libraries that qfield_core references as
# $<TARGET_OBJECTS:...>. Without it install(EXPORT) emits them as INTERFACE
# IMPORTED libraries with no object files, and consumers then fail at generate
# time with "Objects of target ... referenced but is not one of the allowed
# target types".
install(
  TARGETS qfield_core ${QFIELD_CORE_QML_OUTPUT_TARGETS}
  EXPORT qfield-targets
  BUNDLE DESTINATION ${QFIELD_BIN_DIR}
  RUNTIME DESTINATION ${QFIELD_BIN_DIR}
  LIBRARY DESTINATION ${QFIELD_LIB_DIR}
  ARCHIVE DESTINATION ${QFIELD_LIB_DIR}
  OBJECTS DESTINATION ${QFIELD_LIB_DIR}
  FRAMEWORK DESTINATION ${QFIELD_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QFIELD_INCLUDE_DIR})
