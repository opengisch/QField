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
  qgssggeometry.cpp
  referencingfeaturelistmodel.cpp
  rubberband.cpp
  rubberbandmodel.cpp
  settings.cpp
  sgrubberband.cpp
  snappingresult.cpp
  snappingutils.cpp
  submodel.cpp
  valuemapmodel.cpp
  vertexmodel.cpp
)

SET(QFIELD_CORE_HDRS
)

FIND_PACKAGE(Qt5 COMPONENTS Qml Core Gui Xml Positioning Widgets PrintSupport Network Quick REQUIRED)

INCLUDE_DIRECTORIES(SYSTEM
  ${QGIS_INCLUDE_DIR}
)

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src/qgsquick
  ${CMAKE_SOURCE_DIR}/3rdparty/tessellate
)

SET(QFIELD_CORE_HDRS
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
)
