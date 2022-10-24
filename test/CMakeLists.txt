find_package(Catch2 3 REQUIRED)

include(CTest)
include(Catch)

# Set WITH_CATCH2_MAIN to TRUE when qgis or qfield specific initialization is NOT needed
function(ADD_CATCH2_TEST TESTNAME TESTSRC WITH_CATCH2_MAIN)
  add_executable(${TESTNAME} ${TESTSRC})
  set_target_properties(${TESTNAME} PROPERTIES AUTOMOC TRUE)
  target_link_libraries(${TESTNAME} PRIVATE
    qfield_core
    ${QGIS_CORE_LIBRARY}
    ${QGIS_ANALYSIS_LIBRARY}
    ${QT_PKG}::Test
    ${QT_PKG}::Core
    ${QT_PKG}::Gui
    ${QT_PKG}::Widgets
    ${QT_PKG}::Xml
  )
  if(WITH_CATCH2_MAIN)
    target_link_libraries(${TESTNAME} PRIVATE
      Catch2::Catch2WithMain
    )
  else()
    target_link_libraries(${TESTNAME} PRIVATE
      Catch2::Catch2
    )
  endif()
  catch_discover_tests(${TESTNAME})
endfunction()

macro (ADD_QFIELD_TEST TESTNAME TESTSRC)
  add_executable(${TESTNAME} ${TESTSRC})
  set_target_properties(${TESTNAME} PROPERTIES AUTOMOC TRUE)
  target_link_libraries(${TESTNAME} PRIVATE
    qfield_core
    ${QGIS_CORE_LIBRARY}
    ${QGIS_ANALYSIS_LIBRARY}
    ${QT_PKG}::Test
    ${QT_PKG}::Core
    ${QT_PKG}::Gui
    ${QT_PKG}::Widgets
    ${QT_PKG}::Xml
  )
endmacro (ADD_QFIELD_TEST)

macro (ADD_QFIELD_QML_TEST TESTNAME TESTSRC)
  add_executable(${TESTNAME} ${TESTSRC})
  # This should be in src/qml/CMakeLists.txt but that's not possible with Qt5. Retry with Qt6
  # Same is in src/app/CMakeLists.txt
  # https://bugreports.qt.io/browse/QTBUG-80847
  get_target_property(QT_TARGET_TYPE ${QT_PKG}::Core TYPE)
  if(${QT_TARGET_TYPE} STREQUAL "STATIC_LIBRARY")
    include("${CMAKE_SOURCE_DIR}/cmake/QtQmlImportScanner.cmake")

    file(GLOB test_qmlfiles "${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml")
    file(COPY ${test_qmlfiles}
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/qmldrop/test")
    get_target_property(qfield_qmlfiles qfield_qml QML_FILES)
    file(COPY ${qfield_qmlfiles}
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/qmldrop/app")

    if(NOT WITH_BLUETOOTH)
      file(REMOVE
           "${CMAKE_CURRENT_BINARY_DIR}/qmldrop/app/BluetoothDeviceChooser.qml")
    endif()

    qt_import_qml_plugins(${TESTNAME}
                           PATH_TO_SCAN "${CMAKE_CURRENT_BINARY_DIR}/qmldrop")
  endif()

  set_target_properties(${TESTNAME} PROPERTIES AUTOMOC TRUE)
  target_link_libraries(${TESTNAME} PRIVATE
    qfield_core
    qfield_qml
    ${QGIS_CORE_LIBRARY}
    ${QGIS_ANALYSIS_LIBRARY}
    ${QT_PKG}::Test
    ${QT_PKG}::Core
    ${QT_PKG}::Gui
    ${QT_PKG}::Widgets
    ${QT_PKG}::Xml
    ${QT_PKG}::QuickTest
  )
  add_test(NAME ${TESTNAME} COMMAND ${TESTNAME} -import ${CMAKE_SOURCE_DIR}/src/qml/imports -input ${CMAKE_SOURCE_DIR}/test/qml)
endmacro (ADD_QFIELD_QML_TEST)

if (WITH_SPIX)
  add_subdirectory(spix)
endif ()

ADD_CATCH2_TEST(layerobservertest test_layerobserver.cpp FALSE)
ADD_CATCH2_TEST(featureutilstest test_featureutils.cpp TRUE)
ADD_CATCH2_TEST(featuremodeltest test_featuremodel.cpp TRUE)
ADD_CATCH2_TEST(vertexmodeltest test_vertexmodel.cpp TRUE)
ADD_CATCH2_TEST(deltafilewrappertest test_deltafilewrapper.cpp FALSE)
ADD_CATCH2_TEST(fileutilstest test_fileutils.cpp TRUE)
ADD_CATCH2_TEST(geometryutilstest test_geometryutils.cpp TRUE)
ADD_CATCH2_TEST(sggeometry test_sggeometry.cpp TRUE)
ADD_CATCH2_TEST(stringutilstest test_stringutils.cpp TRUE)
ADD_CATCH2_TEST(urlutilstest test_urlutils.cpp TRUE)
ADD_CATCH2_TEST(digitizingloggertest test_digitizinglogger.cpp FALSE)
ADD_CATCH2_TEST(attributeformmodeltest test_attributeformmodel.cpp FALSE)
ADD_CATCH2_TEST(orderedrelationmodeltest test_orderedrelationmodel.cpp FALSE)
ADD_CATCH2_TEST(referencingfeaturelistmodeltest test_referencingfeaturelistmodel.cpp FALSE)

ADD_QFIELD_QML_TEST(qmltest test_qml_editorwidgets.cpp)
