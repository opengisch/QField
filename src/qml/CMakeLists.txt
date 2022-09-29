find_package(${QT_PKG}QuickCompiler QUIET)
find_package(${QT_PKG} COMPONENTS Quick)

file(GLOB_RECURSE QML_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.qml")
if(${QT_PKG}QuickCompiler_FOUND
   AND NOT CMAKE_BUILD_TYPE MATCHES Debug
   AND NOT CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  if(BUILD_WITH_QT6)
    qtquick_compiler_add_resources(
      QML_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc
      ${CMAKE_CURRENT_SOURCE_DIR}/qmlqt6.qrc)
  else()
    qtquick_compiler_add_resources(
      QML_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc
      ${CMAKE_CURRENT_SOURCE_DIR}/qmlqt5.qrc)
  endif()
else()
  if(BUILD_WITH_QT6)
    qt_add_resources(QML_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc)
    qt_add_resources(QML_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/qmlqt6.qrc)
  else()
    qt5_add_resources(QML_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc)
    qt5_add_resources(QML_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/qmlqt5.qrc)
  endif()
endif()

add_library(qfield_qml STATIC ${QML_SOURCES})
target_link_libraries(qfield_qml PRIVATE ${QT_PKG}::Quick)

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
if(BUILD_WITH_QT6)
  file(GLOB qfield_qmlfiles_theme_extra
       "${CMAKE_SOURCE_DIR}/src/qml/imports/Theme/+Qt6/*.qml")
  file(GLOB qfield_qmlfiles_qfieldcontrols
       "${CMAKE_SOURCE_DIR}/src/qml/imports/QFieldControls/+Qt6/*.qml")
else()
  file(GLOB qfield_qmlfiles_theme_extra
       "${CMAKE_SOURCE_DIR}/src/qml/imports/Theme/+Qt5/*.qml")
  file(GLOB qfield_qmlfiles_qfieldcontrols
       "${CMAKE_SOURCE_DIR}/src/qml/imports/QFieldControls/+Qt5/*.qml")
endif()

list(
  APPEND
  qfield_qmlfiles
  ${qfield_qmlfiles_geometryeditors}
  ${qfield_qmlfiles_editorwidgets}
  ${qfield_qmlfiles_theme}
  ${qfield_qmlfiles_theme_extra}
  ${qfield_qmlfiles_qfieldcontrols})

set_property(TARGET qfield_qml PROPERTY QML_FILES ${qfield_qmlfiles})

if(BUILD_WITH_QT6)
  qt_import_qml_plugins(qfield_qml)
endif()
