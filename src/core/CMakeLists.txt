set(QFIELD_CORE_SRCS
  appinterface.cpp
  appcoordinateoperationhandlers.cpp
  attributeformmodelbase.cpp
  attributeformmodel.cpp
  badlayerhandler.cpp
  distancearea.cpp
  expressioncontextutils.cpp
  expressionvariablemodel.cpp
  featurechecklistmodel.cpp
  featurelistextentcontroller.cpp
  featurelistmodel.cpp
  featurelistmodelselection.cpp
  featuremodel.cpp
  featureslocatorfilter.cpp
  feedback.cpp
  focusstack.cpp
  geometry.cpp
  geometryeditorsmodel.cpp
  gotolocatorfilter.cpp
  identifytool.cpp
  layertreemapcanvasbridge.cpp
  layertreemodel.cpp
  layerresolver.cpp
  legendimageprovider.cpp
  linepolygonhighlight.cpp
  locatormodelsuperbridge.cpp
  maptoscreen.cpp
  messagelogmodel.cpp
  modelhelper.cpp
  multifeaturelistmodelbase.cpp
  multifeaturelistmodel.cpp
  peliasgeocoder.cpp
  finlandlocatorfilter.cpp
  picturesource.cpp
  platformutilities.cpp
  printlayoutlistmodel.cpp
  projectinfo.cpp
  projectsource.cpp
  qfieldappauthrequesthandler.cpp
  qgismobileapp.cpp
  qgsgeometrywrapper.cpp
  qgsgpkgflusher.cpp
  qgssggeometry.cpp
  gnsspositioninformation.cpp
  referencingfeaturelistmodel.cpp
  recentprojectlistmodel.cpp
  rubberband.cpp
  rubberbandmodel.cpp
  settings.cpp
  sgrubberband.cpp
  snappingresult.cpp
  snappingutils.cpp
  submodel.cpp
  valuemapmodel.cpp
  vertexmodel.cpp
  trackingmodel.cpp
  tracker.cpp
  viewstatus.cpp
  bluetoothreceiver.cpp
  bluetoothdevicemodel.cpp
  changelogcontents.cpp
  utils/fileutils.cpp
  utils/geometryutils.cpp
  utils/featureutils.cpp
  utils/layerutils.cpp
  utils/stringutils.cpp
  utils/urlutils.cpp
  utils/qfieldcloudutils.cpp
  expressionevaluator.cpp
  qgsquick/qgsquickcoordinatetransformer.cpp
  qgsquick/qgsquickmapcanvasmap.cpp
  qgsquick/qgsquickmapsettings.cpp
  qgsquick/qgsquickmaptransform.cpp
  qfieldcloudconnection.cpp
  qfieldcloudprojectsmodel.cpp
  deltafilewrapper.cpp
  deltalistmodel.cpp
  layerobserver.cpp
  networkreply.cpp
  networkmanager.cpp
)

set(QFIELD_CORE_HDRS
  qfield.h
  appinterface.h
  appcoordinateoperationhandlers.h
  attributeformmodelbase.h
  attributeformmodel.h
  badlayerhandler.h
  distancearea.h
  expressioncontextutils.h
  expressionvariablemodel.h
  featurechecklistmodel.h
  featurelistextentcontroller.h
  featurelistmodel.h
  featurelistmodelselection.h
  featuremodel.h
  featureslocatorfilter.h
  feedback.h
  fieldexpressionvaluesgatherer.h
  focusstack.h
  geometry.h
  geometryeditorsmodel.h
  gotolocatorfilter.h
  identifytool.h
  layertreemapcanvasbridge.h
  layertreemodel.h
  layerresolver.h
  legendimageprovider.h
  linepolygonhighlight.h
  locatormodelsuperbridge.h
  maptoscreen.h
  messagelogmodel.h
  modelhelper.h
  multifeaturelistmodelbase.h
  multifeaturelistmodel.h
  peliasgeocoder.h
  finlandlocatorfilter.h
  picturesource.h
  platformutilities.h
  printlayoutlistmodel.h
  projectinfo.h
  projectsource.h
  qfieldappauthrequesthandler.h
  qgismobileapp.h
  qgsgeometrywrapper.h
  qgsgpkgflusher.h
  qgssggeometry.h
  gnsspositioninformation.h
  referencingfeaturelistmodel.h
  recentprojectlistmodel.h
  rubberband.h
  rubberbandmodel.h
  settings.h
  sgrubberband.h
  snappingresult.h
  snappingutils.h
  submodel.h
  valuemapmodel.h
  vertexmodel.h
  trackingmodel.h
  bluetoothreceiver.h
  bluetoothdevicemodel.h
  tracker.h
  viewstatus.h
  changelogcontents.h
  utils/fileutils.h
  utils/geometryutils.h
  utils/featureutils.h
  utils/layerutils.h
  utils/stringutils.h
  utils/urlutils.h
  utils/qfieldcloudutils.h
  expressionevaluator.h
  qgsquick/qgsquickcoordinatetransformer.h
  qgsquick/qgsquickmapcanvasmap.h
  qgsquick/qgsquickmapsettings.h
  qgsquick/qgsquickmaptransform.h
  qfieldcloudconnection.h
  qfieldcloudprojectsmodel.h
  deltafilewrapper.h
  deltalistmodel.h
  layerobserver.h
  networkreply.h
  networkmanager.h
)

if (ANDROID)
  set(QFIELD_CORE_HDRS ${QFIELD_CORE_HDRS}
    androidplatformutilities.h
    androidpicturesource.h
    androidviewstatus.h
    androidprojectsource.h
  )
  set(QFIELD_CORE_SRCS ${QFIELD_CORE_SRCS}
    androidplatformutilities.cpp
    androidpicturesource.cpp
    androidviewstatus.cpp
    androidprojectsource.cpp
  )
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
  set(QFIELD_CORE_HDRS ${QFIELD_CORE_HDRS}
    ios/iosplatformutilities.h
  )
  set(QFIELD_CORE_SRCS ${QFIELD_CORE_SRCS}
    ios/iosplatformutilities.mm
  )
endif()

find_package(Sqlite3)

add_library(qfield_core STATIC ${QFIELD_CORE_SRCS} ${QFIELD_CORE_HDRS})

include(GenerateExportHeader)
generate_export_header(qfield_core)

target_include_directories(qfield_core SYSTEM PUBLIC
  ${QGIS_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_include_directories(qfield_core SYSTEM PRIVATE
  ${PROJ_INCLUDE_DIR}
  ${GDAL_INCLUDE_DIR}
  ${QCA_INCLUDE_DIR}
  ${QTKEYCHAIN_INCLUDE_DIR}
  ${SQLITE3_INCLUDE_DIR}
)

target_include_directories(qfield_core PUBLIC
  ${CMAKE_SOURCE_DIR}/src/core
  ${CMAKE_SOURCE_DIR}/src/core/qgsquick
  ${CMAKE_SOURCE_DIR}/src/core/utils
  ${CMAKE_SOURCE_DIR}/3rdparty/tessellate
)

target_compile_features(qfield_core PUBLIC cxx_std_17)
set_target_properties(qfield_core PROPERTIES AUTOMOC TRUE)

target_link_libraries(qfield_core PUBLIC
  tess
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
  ${QGIS_CORE_LIBRARY}
  ${QGIS_ANALYSIS_LIBRARY}
  ${PROJ_LIBRARY}
  ${GDAL_LIBRARY}
)

if (ANDROID)
  target_link_libraries(qfield_core PUBLIC
    Qt5::AndroidExtras
  )
endif()

if (TARGET Qt5::PrintSupport)
  target_link_libraries(qfield_core PUBLIC
    Qt5::PrintSupport
  )
endif()

install(FILES ${QFIELD_CORE_HDRS} DESTINATION ${QFIELD_INCLUDE_DIR})
install(TARGETS qfield_core
  BUNDLE DESTINATION ${QFIELD_BIN_DIR}
  RUNTIME DESTINATION ${QFIELD_BIN_DIR}
  LIBRARY DESTINATION ${QFIELD_LIB_DIR}
  ARCHIVE DESTINATION ${QFIELD_LIB_DIR}
  FRAMEWORK DESTINATION ${QFIELD_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QFIELD_INCLUDE_DIR}
)
