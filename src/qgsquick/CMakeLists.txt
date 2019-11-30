SET(QFIELD_QGSQUICK_SRCS
  qgsquickcoordinatetransformer.cpp
  qgsquickfeaturelayerpair.cpp
  qgsquickmapcanvasmap.cpp
  qgsquickmapsettings.cpp
  qgsquickmaptransform.cpp
  qgsquickutils.cpp
)

SET(QFIELD_QGSQUICK_HDRS
  qgsquickcoordinatetransformer.h
  qgsquickfeaturelayerpair.h
  qgsquickmapcanvasmap.h
  qgsquickmapsettings.h
  qgsquickmaptransform.h
  qgsquickutils.h
)
FIND_PACKAGE(Qt5 COMPONENTS Core Gui Xml Widgets Quick Positioning REQUIRED)

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
  Qt5::Positioning
  ${QGIS_CORE_LIBRARY}
)

IF(MXE)
    FIND_PACKAGE(Qt5 COMPONENTS Positioning REQUIRED)
    TARGET_LINK_LIBRARIES(qfield_qgsquick
        Qt5::Positioning
    )
ENDIF(MXE)

INSTALL(FILES ${QFIELD_QGSQUICK_HDRS} DESTINATION ${QFIELD_INCLUDE_DIR})
INSTALL(TARGETS qfield_qgsquick
  RUNTIME DESTINATION ${QFIELD_BIN_DIR}
  LIBRARY DESTINATION ${QFIELD_LIB_DIR}
  ARCHIVE DESTINATION ${QFIELD_LIB_DIR}
  FRAMEWORK DESTINATION ${QFIELD_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QFIELD_INCLUDE_DIR}
)
