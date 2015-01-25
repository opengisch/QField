# Fetch test data
IF (EXISTS "${CMAKE_SOURCE_DIR}/.gitmodules")
  FIND_PACKAGE(Git REQUIRED)
  EXECUTE_PROCESS (COMMAND ${GIT_EXECUTABLE} submodule update
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test/QGIS-Sampledata)
ENDIF (EXISTS "${CMAKE_SOURCE_DIR}/.gitmodules")

# Find Qt packages
FIND_PACKAGE(Qt5Test REQUIRED)
FIND_PACKAGE(Qt5Xml REQUIRED)
FIND_PACKAGE(Qt5Qml REQUIRED)
FIND_PACKAGE(Qt5Quick REQUIRED)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Test_INCLUDE_DIRS}
  ${Qt5Xml_INCLUDE_DIRS}
  ../src
  ${QGIS_INCLUDE_DIR}/core
  ${QGIS_INCLUDE_DIR}
)

# PIC. Because Qt is/may be compiled with -reduce-relocation
SET (CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# Test data directory
ADD_DEFINITIONS ("-DTESTDATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/QGIS-Sampledata\"")

ADD_SUBDIRECTORY(qt_modeltest)

# Add unit test macro
MACRO (ADD_UNIT_TEST testname testsrc)
  SET(test_${testname}_SRCS ${testsrc})
  SET(test_${testname}_MOC_CPPS ${testsrc})
  ADD_EXECUTABLE(test_${testname} ${test_${testname}_SRCS})
  SET_TARGET_PROPERTIES(test_${testname} PROPERTIES AUTOMOC TRUE)

  TARGET_LINK_LIBRARIES(test_${testname}
    ${Qt5Core_LIBRARIES}
    ${Qt5Test_LIBRARIES}
    ${Qt5Qml_LIBRARIES}
    ${Qt5Quick_LIBRARIES}
    ${QGIS_CORE_LIBRARY}
    ${QGIS_MOBILE_APP_NAME}_core
    qt_modeltest
  )

  ADD_TEST(qgis_${testname} ${CMAKE_CURRENT_BINARY_DIR}/../output/bin/test_${testname})
ENDMACRO (ADD_UNIT_TEST)

# List of unit tests
ADD_UNIT_TEST(featuremodel test_featuremodel.cpp)
ADD_UNIT_TEST(featurelistmodel test_featurelistmodel.cpp)
