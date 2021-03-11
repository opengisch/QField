MACRO (ADD_QFIELD_TEST TESTNAME TESTSRC)
  ADD_EXECUTABLE(${TESTNAME} ${TESTSRC})
  SET_TARGET_PROPERTIES(${TESTNAME} PROPERTIES AUTOMOC TRUE)
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
  ADD_TEST(NAME ${TESTNAME} COMMAND ${TESTNAME})
ENDMACRO (ADD_QFIELD_TEST)

ADD_QFIELD_TEST(vertexmodeltest test_vertexmodel.cpp)
ADD_QFIELD_TEST(referencingfeaturelistmodeltest test_referencingfeaturelistmodel.cpp)
ADD_QFIELD_TEST(deltafilewrappertest test_deltafilewrapper.cpp)
ADD_QFIELD_TEST(layerobservertest test_layerobserver.cpp)
ADD_QFIELD_TEST(featureutilstest test_featureutils.cpp)
ADD_QFIELD_TEST(fileutilstest test_fileutils.cpp)
ADD_QFIELD_TEST(geometryutilstest test_geometryutils.cpp)
ADD_QFIELD_TEST(stringutilstest test_stringutils.cpp)
ADD_QFIELD_TEST(urlutilstest test_urlutils.cpp)
