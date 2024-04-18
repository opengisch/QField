find_package(Qt6QuickCompiler QUIET)
find_package(Qt6 COMPONENTS Quick)

file(GLOB_RECURSE QML_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.qml")
if(QtQuickCompiler_FOUND
   AND NOT CMAKE_BUILD_TYPE MATCHES Debug
   AND NOT CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  qtquick_compiler_add_resources(QML_SOURCES
                                 ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc)
else()
  qt_add_resources(QML_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc)
endif()

add_library(qfield_qml STATIC ${QML_SOURCES})
target_link_libraries(qfield_qml PRIVATE Qt::Quick)

target_compile_definitions(
  qfield_qml
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
set_target_properties(qfield_qml PROPERTIES LINKER_LANGUAGE CXX)

# For QtCreator
set(QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/imports")

file(GLOB qfield_qmlfiles "${CMAKE_SOURCE_DIR}/src/qml/*.qml")
file(GLOB qfield_qmlfiles_geometryeditors
     "${CMAKE_SOURCE_DIR}/src/qml/geometry_editors/*.qml")
file(GLOB qfield_qmlfiles_editorwidgets
     "${CMAKE_SOURCE_DIR}/src/qml/editorwidgets/*.qml")
file(GLOB qfield_qmlfiles_theme
     "${CMAKE_SOURCE_DIR}/src/qml/imports/Theme/*.qml")

list(APPEND qfield_qmlfiles ${qfield_qmlfiles_geometryeditors}
     ${qfield_qmlfiles_editorwidgets} ${qfield_qmlfiles_theme}
     ${qfield_qmlfiles_theme_extra})

set_property(TARGET qfield_qml PROPERTY QML_FILES ${qfield_qmlfiles})

qt_import_qml_plugins(qfield_qml)
