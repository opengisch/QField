find_package(${QT_PKG}QuickCompiler QUIET)
find_package(${QT_PKG} COMPONENTS Quick)

file(GLOB_RECURSE QML_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.qml")
if(${QT_PKG}QuickCompiler_FOUND
   AND NOT CMAKE_BUILD_TYPE MATCHES Debug
   AND NOT CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  qtquick_compiler_add_resources(QML_SOURCES
                                 ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc)
else()
  qt5_add_resources(QML_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc)
endif()

add_library(qfield_qml STATIC ${QML_SOURCES})
target_link_libraries(qfield_qml PRIVATE ${QT_PKG}::Quick)

target_compile_definitions(
  qfield_qml
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
set_target_properties(qfield_qml PROPERTIES LINKER_LANGUAGE CXX)

# For QtCreator
set(QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/imports")
