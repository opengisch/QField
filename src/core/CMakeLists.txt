SET(QFIELD_CORE_SRCS
  appinterface.cpp
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
  focusstack.cpp
  geometry.cpp
  geometryeditorsmodel.cpp
  identifytool.cpp
  layertreemapcanvasbridge.cpp
  layertreemodel.cpp
  legendimageprovider.cpp
  linepolygonhighlight.cpp
  locatormodelsuperbridge.cpp
  maptoscreen.cpp
  messagelogmodel.cpp
  modelhelper.cpp
  multifeaturelistmodel.cpp
  picturesource.cpp
  platformutilities.cpp
  printlayoutlistmodel.cpp
  projectsource.cpp
  qfieldappauthrequesthandler.cpp
  qgismobileapp.cpp
  qgsgeometrywrapper.cpp
  qgsgpkgflusher.cpp
  qgssggeometry.cpp
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
  utils/fileutils.cpp
  utils/geometryutils.cpp
  utils/featureutils.cpp
)

SET(QFIELD_CORE_HDRS
  appinterface.h
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
  focusstack.h
  geometry.h
  geometryeditorsmodel.h
  identifytool.h
  layertreemapcanvasbridge.h
  layertreemodel.h
  legendimageprovider.h
  linepolygonhighlight.h
  locatormodelsuperbridge.h
  maptoscreen.h
  messagelogmodel.h
  modelhelper.h
  multifeaturelistmodel.h
  picturesource.h
  platformutilities.h
  printlayoutlistmodel.h
  projectsource.h
  qfieldappauthrequesthandler.h
  qgismobileapp.h
  qgsgeometrywrapper.h
  qgsgpkgflusher.h
  qgssggeometry.h
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
  tracker.h
  viewstatus.h
  utils/fileutils.h
  utils/geometryutils.h
  utils/featureutils.h
)

FIND_PACKAGE(Qt5 COMPONENTS Qml Core Gui Xml Positioning Widgets PrintSupport Network Quick REQUIRED)

INCLUDE_DIRECTORIES(SYSTEM
  ${QGIS_INCLUDE_DIR}
)

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src/qgsquick
  ${CMAKE_SOURCE_DIR}/3rdparty/tessellate
)

ADD_LIBRARY(qfield_core SHARED ${QFIELD_CORE_SRCS} ${QFILED_CORE_HDRS})
TARGET_LINK_LIBRARIES(qfield_core
  tess
  qfield_qgsquick
  Qt5::Core
  Qt5::Gui
  Qt5::Xml
  Qt5::Positioning
  Qt5::Widgets
  Qt5::PrintSupport
  Qt5::Network
  Qt5::Quick
  ${QGIS_CORE_LIBRARY}
  ${QGIS_ANALYSIS_LIBRARY}
)

INSTALL(FILES ${QFIELD_CORE_HDRS} DESTINATION ${QFIELD_INCLUDE_DIR})
INSTALL(TARGETS qfield_core
  RUNTIME DESTINATION ${QFIELD_BIN_DIR}
  LIBRARY DESTINATION ${QFIELD_LIB_DIR}
  ARCHIVE DESTINATION ${QFIELD_LIB_DIR}
  FRAMEWORK DESTINATION ${QFIELD_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QFIELD_INCLUDE_DIR}
)

