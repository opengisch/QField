find_package(Catch2 3 REQUIRED)

include(CTest)
include(Catch)

set(CMAKE_AUTORCC ON)

# Set WITH_CATCH2_MAIN to TRUE when qgis or qfield specific initialization is NOT needed
function(ADD_CATCH2_TEST TESTNAME TESTSRC WITH_CATCH2_MAIN)
  set(TESTSOURCES
      ${TESTSRC} ${CMAKE_SOURCE_DIR}/images/images.qrc
  )
  add_executable(${TESTNAME} ${TESTSOURCES})
  set_target_properties(${TESTNAME} PROPERTIES AUTOMOC TRUE)
  target_link_libraries(${TESTNAME} PRIVATE
    qfield_core
    Qt::Test
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Xml
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

  # On macOS with Xcode generator, ad-hoc sign test executables so they can run
  if(APPLE AND CMAKE_GENERATOR STREQUAL "Xcode")
    add_custom_command(
      TARGET ${TESTNAME}
      POST_BUILD
      COMMAND /usr/bin/codesign --force --sign - $<TARGET_FILE:${TESTNAME}>
      COMMENT "Ad-hoc signing Catch2 test executable ${TESTNAME} on macOS/Xcode"
    )
  endif()

  # Use Catch2s safer PRE_TEST discovery mode (recommended upstream) - https://github.com/catchorg/Catch2/issues/2411
  catch_discover_tests(${TESTNAME}
    DISCOVERY_MODE PRE_TEST
  )
endfunction()

macro (ADD_QFIELD_TEST TESTNAME TESTSRC)
  set(TESTSOURCES
      ${TESTSRC} ${CMAKE_SOURCE_DIR}/images/images.qrc
  )
  add_executable(${TESTNAME} ${TESTSOURCES})
  set_target_properties(${TESTNAME} PROPERTIES AUTOMOC TRUE)
  target_link_libraries(${TESTNAME} PRIVATE
    qfield_core
    Qt::Test
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Xml
  )
endmacro (ADD_QFIELD_TEST)

macro (ADD_QFIELD_QML_TEST TESTNAME TESTSRC)
  set(TESTSOURCES
      ${TESTSRC} ${CMAKE_SOURCE_DIR}/images/images.qrc
  )
  file(GLOB test_qmlfiles "${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml")
  add_executable(${TESTNAME} ${TESTSOURCES} ${test_qmlfiles})
  # This should be in src/qml/CMakeLists.txt but that's not possible with Qt5. Retry with Qt6
  # Same is in src/app/CMakeLists.txt
  # https://bugreports.qt.io/browse/QTBUG-80847
  get_target_property(QT_TARGET_TYPE Qt::Core TYPE)
  if(${QT_TARGET_TYPE} STREQUAL "STATIC_LIBRARY")
    find_package(Qt6 COMPONENTS QmlImportScanner)

    file(COPY ${test_qmlfiles}
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/qmldrop/test")
    get_target_property(qfield_qmlfiles qfield_qml QML_FILES)
    file(COPY ${qfield_qmlfiles}
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/qmldrop/app")

    if(NOT WITH_BLUETOOTH)
      file(REMOVE
           "${CMAKE_CURRENT_BINARY_DIR}/qmldrop/app/BluetoothDeviceChooser.qml")
    endif()

    qt_import_qml_plugins(${TESTNAME} PATH_TO_SCAN "${CMAKE_CURRENT_BINARY_DIR}/qmldrop")
  endif()

  set_target_properties(${TESTNAME} PROPERTIES AUTOMOC TRUE)
  target_link_libraries(${TESTNAME} PRIVATE
    qfield_core
    qfield_qml
    Qt::Test
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Xml
    Qt::QuickTest
  )
  add_test(NAME ${TESTNAME} COMMAND ${TESTNAME} -import ${CMAKE_SOURCE_DIR}/src/qml/imports -input ${CMAKE_SOURCE_DIR}/test/qml)
endmacro (ADD_QFIELD_QML_TEST)

if (WITH_SPIX)
  add_subdirectory(spix)
endif()

add_definitions(-DTEST_DATA_DIR="${TEST_DATA_DIR}")

if (WITH_UNITTESTS)
  ADD_CATCH2_TEST(layerobservertest test_layerobserver.cpp FALSE)
  ADD_CATCH2_TEST(featureutilstest test_featureutils.cpp TRUE)
  ADD_CATCH2_TEST(featuremodeltest test_featuremodel.cpp TRUE)
  ADD_CATCH2_TEST(featurehistorytest test_featurehistory.cpp FALSE)
  ADD_CATCH2_TEST(vertexmodeltest test_vertexmodel.cpp TRUE)
  ADD_CATCH2_TEST(deltafilewrappertest test_deltafilewrapper.cpp FALSE)
  ADD_CATCH2_TEST(fileutilstest test_fileutils.cpp TRUE)
  ADD_CATCH2_TEST(geometryutilstest test_geometryutils.cpp TRUE)
  ADD_CATCH2_TEST(stringutilstest test_stringutils.cpp TRUE)
  ADD_CATCH2_TEST(urlutilstest test_urlutils.cpp TRUE)
  ADD_CATCH2_TEST(digitizingloggertest test_digitizinglogger.cpp FALSE)
  ADD_CATCH2_TEST(attributeformmodeltest test_attributeformmodel.cpp FALSE)
  ADD_CATCH2_TEST(orderedrelationmodeltest test_orderedrelationmodel.cpp FALSE)
  ADD_CATCH2_TEST(referencingfeaturelistmodeltest test_referencingfeaturelistmodel.cpp FALSE)
  ADD_CATCH2_TEST(expressionevaluatortest test_expressionevaluator.cpp TRUE)
  ADD_CATCH2_TEST(appinterfacetest test_appinterface.cpp TRUE)
  ADD_CATCH2_TEST(trackingtest test_tracking.cpp FALSE)
  ADD_CATCH2_TEST(identifytool test_identifytool.cpp TRUE)
  ADD_CATCH2_TEST(featurelistmodel test_featurelistmodel.cpp TRUE)

  ADD_QFIELD_QML_TEST(qmltest test_qml.cpp)
endif()
