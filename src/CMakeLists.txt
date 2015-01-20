SET(QGIS_MOBILE_APP_SRCS
  appinterface.cpp
  feature.cpp
  featurelistextentcontroller.cpp
  featurelistmodel.cpp
  featurelistmodelhighlight.cpp
  featurelistmodelselection.cpp
  featuremodel.cpp
  mapsettings.cpp
  maptransform.cpp
  qgismobileapp.cpp
  qgsquickmapcanvasmap.cpp
  qgssggeometry.cpp
  settings.cpp
)

SET(QGIS_MOBILE_APP_HDRS
  appinterface.h
  feature.h
  featurelistextentcontroller.h
  featurelistmodel.h
  featurelistmodelhighlight.h
  featurelistmodelselection.h
  featuremodel.h
  mapsettings.h
  maptransform.h
  qgismobileapp.h
  qgsquickmapcanvasmap.h
  qgssggeometry.h
  settings.h
)

SET(QGIS_MOBILE_APP_MOC_HDRS
  appinterface.h
  featurelistextentcontroller.h
  featurelistmodel.h
  featurelistmodelhighlight.h
  featurelistmodelselection.h
  featuremodel.h
  mapsettings.h
  maptransform.h
  qgismobileapp.h
  qgsquickmapcanvasmap.h
  settings.h
)

QT5_WRAP_CPP(QGIS_MOBILE_APP_MOC_SRCS ${QGIS_MOBILE_APP_MOC_HDRS})

INCLUDE_DIRECTORIES(
  ${QGIS_INCLUDE_DIR}
  ${QGIS_INCLUDE_DIR}/gui
  ${QGIS_INCLUDE_DIR}/gui/layertree
  ${QGIS_INCLUDE_DIR}/gui/attributetable
  ${QGIS_INCLUDE_DIR}/core
  ${QGIS_INCLUDE_DIR}/core/raster
  ${GEOS_INCLUDE_DIR}
  ${Qt5Declarative_INCLUDE_DIRS}
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Gui_INCLUDE_DIRS}
  ${Qt5Xml_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/tessellate
)

FIND_PACKAGE(Qt5Qml REQUIRED)
FIND_PACKAGE(Qt5Quick REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)

ADD_SUBDIRECTORY(qml)

SET(QML_RCCS qml/qml.qrc)
QT5_ADD_RESOURCES(QML_RCC_SRCS ${QML_RCCS})
FILE(GLOB QML_FILES qml/*.qml) # Makes qml files appear in QtCreator project pane
SET(IMAGE_RCCS ../images/images.qrc)
QT5_ADD_RESOURCES(IMAGE_RCC_SRCS ${IMAGE_RCCS})

ADD_SUBDIRECTORY(3rdparty)

ADD_LIBRARY(${QGIS_MOBILE_APP_NAME}_core SHARED ${QGIS_MOBILE_APP_SRCS} ${QGIS_MOBILE_APP_MOC_SRCS} ${QML_RCC_SRCS} ${IMAGE_RCC_SRCS} ${QML_FILES})

TARGET_LINK_LIBRARIES(${QGIS_MOBILE_APP_NAME}_core
  ${Qt5Qml_LIBRARIES}
  ${Qt5Quick_LIBRARIES}
  ${Qt5Widgets_LIBRARIES}
  ${QGIS_CORE_LIBRARY}
  ${QGIS_GUI_LIBRARY}
  tess
)

IF (ANDROID)
  ADD_LIBRARY(${QGIS_MOBILE_APP_NAME} SHARED main.cpp)
ELSE (ANDROID)
  ADD_EXECUTABLE(${QGIS_MOBILE_APP_NAME} MACOSX_BUNDLE WIN32 main.cpp)
ENDIF (ANDROID)

TARGET_LINK_LIBRARIES(${QGIS_MOBILE_APP_NAME}
  ${QGIS_MOBILE_APP_NAME}_core
)

INSTALL(TARGETS ${QGIS_MOBILE_APP_NAME}
  BUNDLE DESTINATION ${QGIS_MOBILE_INSTALL_PREFIX}
  RUNTIME DESTINATION ${QGIS_MOBILE_BIN_DIR}
  LIBRARY DESTINATION ${QGIS_MOBILE_LIB_DIR}
  ARCHIVE DESTINATION ${QGIS_MOBILE_LIB_DIR}
  FRAMEWORK DESTINATION ${QGIS_MOBILE_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QGIS_INCLUDE_DIR}
)

INSTALL(TARGETS ${QGIS_MOBILE_APP_NAME}_core
  RUNTIME DESTINATION ${QGIS_MOBILE_BIN_DIR}
  LIBRARY DESTINATION ${QGIS_MOBILE_LIB_DIR}
  ARCHIVE DESTINATION ${QGIS_MOBILE_LIB_DIR}
  FRAMEWORK DESTINATION ${QGIS_MOBILE_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QGIS_INCLUDE_DIR}
)

