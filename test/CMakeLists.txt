FIND_PACKAGE(Qt5 COMPONENTS Test Core Gui Widgets Xml REQUIRED)

INCLUDE_DIRECTORIES(SYSTEM
  ${QGIS_INCLUDE_DIR}
)

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src/qgsquick
  ${CMAKE_SOURCE_DIR}/src/core
  ${CMAKE_SOURCE_DIR}/3rdparty/tessellate
)

MACRO (ADD_QFIELD_TEST TESTNAME TESTSRC)
  ADD_EXECUTABLE(${TESTNAME} ${TESTSRC})
  TARGET_LINK_LIBRARIES(${TESTNAME}
    qfield_core
    ${QGIS_CORE_LIBRARY}
    ${QGIS_ANALYSIS_LIBRARY}
    Qt5::Test
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Xml
  )
  ADD_TEST(${TESTNAME} ${TESTNAME})
ENDMACRO (ADD_QFIELD_TEST)

ADD_QFIELD_TEST(vertexmodeltest test_vertexmodel.cpp)
ADD_QFIELD_TEST(referencingfeaturelistmodeltest test_referencingfeaturelistmodel.cpp)
