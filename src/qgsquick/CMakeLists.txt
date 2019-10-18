SET(QFIELD_QGSQUICK_SRCS
  qgsquickcoordinatetransformer.cpp
  qgsquickfeaturelayerpair.cpp
  qgsquickmapcanvasmap.cpp
  qgsquickmapsettings.cpp
  qgsquickmaptransform.cpp
  qgsquickutils.cpp
)

FIND_PACKAGE(Qt5 COMPONENTS Core Gui Xml Widgets Quick REQUIRED)

INCLUDE_DIRECTORIES(SYSTEM
  ${QGIS_INCLUDE_DIR}
)

ADD_LIBRARY(qfield_qgsquick SHARED ${QFIELD_QGSQUICK_SRCS})
TARGET_LINK_LIBRARIES(qfield_qgsquick
  Qt5::Core
  Qt5::Gui
  Qt5::Xml
  Qt5::Widgets
  Qt5::Quick
  ${QGIS_CORE_LIBRARY}
)
