set(QFIELD_CORE_SRCS
    platforms/platformutilities.cpp
    utils/expressioncontextutils.cpp
    utils/featureutils.cpp
    utils/fileutils.cpp
    utils/geometryutils.cpp
    utils/layerutils.cpp
    utils/qfieldcloudutils.cpp
    utils/snappingutils.cpp
    utils/stringutils.cpp
    utils/urlutils.cpp
    qgsquick/qgsquickcoordinatetransformer.cpp
    qgsquick/qgsquickmapcanvasmap.cpp
    qgsquick/qgsquickmapsettings.cpp
    qgsquick/qgsquickmaptransform.cpp
    locator/featureslocatorfilter.cpp
    locator/finlandlocatorfilter.cpp
    locator/gotolocatorfilter.cpp
    locator/locatormodelsuperbridge.cpp
    appcoordinateoperationhandlers.cpp
    appinterface.cpp
    attributeformmodel.cpp
    attributeformmodelbase.cpp
    badlayerhandler.cpp
    bluetoothdevicemodel.cpp
    bluetoothreceiver.cpp
    changelogcontents.cpp
    deltafilewrapper.cpp
    deltalistmodel.cpp
    digitizinglogger.cpp
    distancearea.cpp
    expressionevaluator.cpp
    expressionvariablemodel.cpp
    featurechecklistmodel.cpp
    featurelistextentcontroller.cpp
    featurelistmodel.cpp
    featurelistmodelselection.cpp
    featuremodel.cpp
    feedback.cpp
    focusstack.cpp
    geometry.cpp
    geometryeditorsmodel.cpp
    gnsspositioninformation.cpp
    identifytool.cpp
    layerobserver.cpp
    layerresolver.cpp
    layertreemapcanvasbridge.cpp
    layertreemodel.cpp
    legendimageprovider.cpp
    linepolygonhighlight.cpp
    maptoscreen.cpp
    messagelogmodel.cpp
    modelhelper.cpp
    multifeaturelistmodel.cpp
    multifeaturelistmodelbase.cpp
    networkmanager.cpp
    networkreply.cpp
    orderedrelationmodel.cpp
    peliasgeocoder.cpp
    picturesource.cpp
    printlayoutlistmodel.cpp
    projectinfo.cpp
    projectsource.cpp
    qfieldappauthrequesthandler.cpp
    qfieldcloudconnection.cpp
    qfieldcloudprojectsmodel.cpp
    qgismobileapp.cpp
    qgsgeometrywrapper.cpp
    qgsgpkgflusher.cpp
    qgssggeometry.cpp
    recentprojectlistmodel.cpp
    referencingfeaturelistmodel.cpp
    rubberband.cpp
    rubberbandmodel.cpp
    scalebarmeasurement.cpp
    screendimmer.cpp
    settings.cpp
    sgrubberband.cpp
    snappingresult.cpp
    submodel.cpp
    tracker.cpp
    trackingmodel.cpp
    valuemapmodel.cpp
    vertexmodel.cpp
    viewstatus.cpp)

set(QFIELD_CORE_HDRS
    platforms/platformutilities.h
    utils/expressioncontextutils.cpp
    utils/featureutils.cpp
    utils/fileutils.cpp
    utils/geometryutils.cpp
    utils/layerutils.cpp
    utils/qfieldcloudutils.cpp
    utils/snappingutils.cpp
    utils/stringutils.cpp
    utils/urlutils.cpp
    qgsquick/qgsquickcoordinatetransformer.cpp
    qgsquick/qgsquickmapcanvasmap.cpp
    qgsquick/qgsquickmapsettings.cpp
    qgsquick/qgsquickmaptransform.cpp
    locator/featureslocatorfilter.cpp
    locator/finlandlocatorfilter.cpp
    locator/gotolocatorfilter.cpp
    locator/locatormodelsuperbridge.cpp
    appcoordinateoperationhandlers.h
    appinterface.h
    attributeformmodel.h
    attributeformmodelbase.h
    badlayerhandler.h
    bluetoothdevicemodel.h
    bluetoothreceiver.h
    changelogcontents.h
    deltafilewrapper.h
    deltalistmodel.h
    digitizinglogger.h
    distancearea.h
    expressionevaluator.h
    expressionvariablemodel.h
    featurechecklistmodel.h
    featurelistextentcontroller.h
    featurelistmodel.h
    featurelistmodelselection.h
    featuremodel.h
    feedback.h
    fieldexpressionvaluesgatherer.h
    focusstack.h
    geometry.h
    geometryeditorsmodel.h
    gnsspositioninformation.h
    identifytool.h
    layerobserver.h
    layerresolver.h
    layertreemapcanvasbridge.h
    layertreemodel.h
    legendimageprovider.h
    linepolygonhighlight.h
    maptoscreen.h
    messagelogmodel.h
    modelhelper.h
    multifeaturelistmodel.h
    multifeaturelistmodelbase.h
    networkmanager.h
    networkreply.h
    orderedrelationmodel.h
    peliasgeocoder.h
    picturesource.h
    printlayoutlistmodel.h
    projectinfo.h
    projectsource.h
    qfieldappauthrequesthandler.h
    qfieldcloudconnection.h
    qfieldcloudprojectsmodel.h
    qgismobileapp.h
    qgsgeometrywrapper.h
    qgsgpkgflusher.h
    qgssggeometry.h
    recentprojectlistmodel.h
    referencingfeaturelistmodel.h
    rubberband.h
    rubberbandmodel.h
    scalebarmeasurement.h
    screendimmer.h
    settings.h
    sgrubberband.h
    snappingresult.h
    submodel.h
    tracker.h
    trackingmodel.h
    valuemapmodel.h
    vertexmodel.h
    viewstatus.h
    ${CMAKE_CURRENT_BINARY_DIR}/qfield.h)

if(ANDROID)
  set(QFIELD_CORE_HDRS
      ${QFIELD_CORE_HDRS}
      platforms/android/androidplatformutilities.h
      platforms/android/androidpicturesource.h
      platforms/android/androidviewstatus.h
      platforms/android/androidprojectsource.h
      ${CMAKE_CURRENT_BINARY_DIR}/qfield_android.h)
  set(QFIELD_CORE_SRCS
      ${QFIELD_CORE_SRCS}
      platforms/android/androidplatformutilities.cpp
      platforms/android/androidpicturesource.cpp
      platforms/android/androidviewstatus.cpp
      platforms/android/androidprojectsource.cpp)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
  set(QFIELD_CORE_HDRS ${QFIELD_CORE_HDRS} platforms/ios/iosplatformutilities.h
                       platforms/ios/iospicturesource.h)
  set(QFIELD_CORE_SRCS
      ${QFIELD_CORE_SRCS} platforms/ios/iosplatformutilities.mm
      platforms/ios/iospicturesource.mm)
endif()

find_package(Sqlite3)

add_library(qfield_core STATIC ${QFIELD_CORE_SRCS} ${QFIELD_CORE_HDRS})

include(GenerateExportHeader)
generate_export_header(qfield_core)

configure_file(qfield.h.in ${CMAKE_CURRENT_BINARY_DIR}/qfield.h @ONLY)

target_include_directories(qfield_core SYSTEM
                           PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(
  qfield_core SYSTEM
  PRIVATE ${PROJ_INCLUDE_DIR} ${GDAL_INCLUDE_DIR} ${Qca_INCLUDE_DIR}
          ${QTKEYCHAIN_INCLUDE_DIR} ${SQLITE3_INCLUDE_DIR})
if(IOS)
  # to include <qpa/qplatformnativeinterface.h>
  target_include_directories(qfield_core SYSTEM
                             PRIVATE ${Qt5Gui_PRIVATE_INCLUDE_DIRS})
endif()

target_include_directories(
  qfield_core
  PUBLIC ${CMAKE_SOURCE_DIR}/src/core
         ${CMAKE_SOURCE_DIR}/src/core/locator
         ${CMAKE_SOURCE_DIR}/src/core/platforms
         ${CMAKE_SOURCE_DIR}/src/core/qgsquick
         ${CMAKE_SOURCE_DIR}/src/core/utils
         ${CMAKE_SOURCE_DIR}/3rdparty/tessellate)
if(ANDROID)
  configure_file(platforms/android/qfield_android.h.in
                 ${CMAKE_CURRENT_BINARY_DIR}/qfield_android.h @ONLY)
  target_include_directories(
    qfield_core PUBLIC ${CMAKE_SOURCE_DIR}/src/core/platforms/android)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
  target_include_directories(qfield_core
                             PUBLIC ${CMAKE_SOURCE_DIR}/src/core/platforms/ios)
endif()

target_compile_features(qfield_core PUBLIC cxx_std_17)
set_target_properties(qfield_core PROPERTIES AUTOMOC TRUE)

if(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
  # debug symbols https://stackoverflow.com/a/49714128/1548052
  target_compile_options(qfield_core PRIVATE $<$<CXX_COMPILER_ID:Clang>:-g>)
endif()

target_link_libraries(
  qfield_core
  PUBLIC tess
         Qt5::Core
         Qt5::Gui
         Qt5::Xml
         Qt5::Positioning
         Qt5::Widgets
         Qt5::Network
         Qt5::Quick
         Qt5::Svg
         Qt5::OpenGL
         Qt5::Sensors
         Qt5::Positioning
         Qt5::Sql
         Qt5::Concurrent
         Qt5::WebView
         Qt5::Bluetooth
         QGIS::Core
         QGIS::Analysis
         ${PROJ_LIBRARY}
         ${GDAL_LIBRARIES})

if(ANDROID)
  target_link_libraries(qfield_core PUBLIC Qt5::AndroidExtras)
endif()

if(TARGET Qt5::PrintSupport)
  target_link_libraries(qfield_core PUBLIC Qt5::PrintSupport)
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
