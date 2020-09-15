#
# Copyright (c) 2019 Olivier Le Doeuff <olivier.ldff@gmail.com>
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

cmake_minimum_required(VERSION 3.0)

# ┌──────────────────────────────────────────────────────────────────┐
# │                       ENVIRONMENT                                │
# └──────────────────────────────────────────────────────────────────┘

# find the Qt root directory
if(NOT Qt5Core_DIR)
  find_package(Qt5Core REQUIRED)
endif()
get_filename_component(QT_STATIC_QT_ROOT "${Qt5Core_DIR}/../../.." ABSOLUTE)
message(STATUS "Found Qt SDK Root: ${QT_STATIC_QT_ROOT}")

set(QT_STATIC_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

# ┌──────────────────────────────────────────────────────────────────┐
# │                    GENERATE QML PLUGIN                           │
# └──────────────────────────────────────────────────────────────────┘

# We need to parse some arguments
include(CMakeParseArguments)

# Usage:
# qt_generate_qml_plugin_import(YourApp
#   QML_DIR "/path/to/qtsdk"
#   QML_SRC "/path/to/yourApp/qml"
#   OUTPUT "YourApp_qml_plugin_import.cpp"
#   OUTPUT_DIR "/path/to/generate"
#   EXTRA_PLUGIN
#     QtQuickVirtualKeyboardPlugin
#     QtQuickVirtualKeyboardSettingsPlugin
#     QtQuickVirtualKeyboardStylesPlugin
#     QmlFolderListModelPlugin
#     QQuickLayoutsPlugin
#   VERBOSE
#)
macro(qt_generate_qml_plugin_import TARGET)

  set(QT_STATIC_OPTIONS VERBOSE )
  set(QT_STATIC_ONE_VALUE_ARG QML_DIR
    QML_SRC
    OUTPUT
    OUTPUT_DIR
    )
  set(QT_STATIC_MULTI_VALUE_ARG
    EXTRA_PLUGIN)

  # parse the macro arguments
  cmake_parse_arguments(ARGSTATIC "${QT_STATIC_OPTIONS}" "${QT_STATIC_ONE_VALUE_ARG}" "${QT_STATIC_MULTI_VALUE_ARG}" ${ARGN})

  # Copy arg variables to local variables
  set(QT_STATIC_TARGET ${TARGET})
  set(QT_STATIC_QML_DIR ${ARGSTATIC_QML_DIR})
  set(QT_STATIC_QML_SRC ${ARGSTATIC_QML_SRC})
  set(QT_STATIC_OUTPUT ${ARGSTATIC_OUTPUT})
  set(QT_STATIC_OUTPUT_DIR ${ARGSTATIC_OUTPUT_DIR})
  set(QT_STATIC_VERBOSE ${ARGSTATIC_VERBOSE})

  # Default to QtSdk/qml
  if(NOT QT_STATIC_QML_DIR)
    set(QT_STATIC_QML_DIR "${QT_STATIC_QT_ROOT}/qml")
    if(QT_STATIC_VERBOSE)
      message(STATUS "QML_DIR not specified, default to ${QT_STATIC_QML_DIR}")
    endif()
  endif()

  # Default to ${QT_STATIC_TARGET}_qml_plugin_import.cpp
  if(NOT QT_STATIC_OUTPUT)
    set(QT_STATIC_OUTPUT ${QT_STATIC_TARGET}_qml_plugin_import.cpp)
    if(QT_STATIC_VERBOSE)
      message(STATUS "OUTPUT not specified, default to ${QT_STATIC_OUTPUT}")
    endif()
  endif()

  # Default to project build directory
  if(NOT QT_STATIC_OUTPUT_DIR)
    set(QT_STATIC_OUTPUT_DIR ${PROJECT_BINARY_DIR})
    if(QT_STATIC_VERBOSE)
      message(STATUS "OUTPUT not specified, default to ${QT_STATIC_OUTPUT_DIR}")
    endif()
  endif()

  # Print config
  if(QT_STATIC_VERBOSE)
    message(STATUS "------ QtStaticCMake Qml Generate Configuration ------")
    message(STATUS "TARGET          : ${QT_STATIC_TARGET}")
    message(STATUS "QML_DIR         : ${QT_STATIC_QML_DIR}")
    message(STATUS "QML_SRC         : ${QT_STATIC_QML_SRC}")
    message(STATUS "OUTPUT          : ${QT_STATIC_OUTPUT}")
    message(STATUS "OUTPUT_DIR      : ${QT_STATIC_OUTPUT_DIR}")
    message(STATUS "EXTRA_PLUGIN    : ${ARGSTATIC_EXTRA_PLUGIN}")
    message(STATUS "------ QtStaticCMake Qml Generate End Configuration ------")
  endif()

  if(QT_STATIC_QML_SRC)
    # Debug
    if(QT_STATIC_VERBOSE)
      message(STATUS "Get Qml Plugin dependencies for ${QT_STATIC_TARGET}. qmlimportscanner path is ${QT_STATIC_QT_ROOT}/bin/qmlimportscanner. RootPath is ${QT_STATIC_QML_SRC} and importPath is ${QT_STATIC_QML_DIR}.")
    endif()

    # Get Qml Plugin dependencies
    execute_process(
      COMMAND ${QT_STATIC_QT_ROOT}/bin/qmlimportscanner -rootPath ${QT_STATIC_QML_SRC} -importPath ${QT_STATIC_QML_DIR}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      OUTPUT_VARIABLE QT_STATIC_QML_DEPENDENCIES_JSON
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )

    # Dump Json File for debug
    if(QT_STATIC_VERBOSE)
      message(STATUS "qmlimportscanner result: ${QT_STATIC_QML_DEPENDENCIES_JSON}")
    endif()

    # Object look like this :
    # {
    #     "classname": "QtQuick2Plugin",
    #     "name": "QtQuick",
    #     "path": "C:/Path/To/qtbase/qml/QtQuick.2",
    #     "plugin": "qtquick2plugin",
    #     "relativePath": "QtQuick.2",
    #     "type": "module",
    #     "version": "2.14"
    # }

    # Extract everything inside { ... }
    if(QT_STATIC_QML_DEPENDENCIES_JSON)
      string(REGEX MATCHALL "\\{([^\\}]*)\\}"
        QT_STATIC_QML_IMPORT_JSON_OBJ ${QT_STATIC_QML_DEPENDENCIES_JSON})
    else()
      message(WARNING "qmlimportscanner didn't find any dependencies")
    endif()

    # message(STATUS "QT_STATIC_QML_IMPORT_JSON_OBJ  : ${QT_STATIC_QML_IMPORT_JSON_OBJ}")

    # Now simply loop over objects and extract "classname" for Q_IMPORT_PLUGIN macro
    # And "plugin" for find_library
    # Also extract path as hint if available
    foreach(JSON_OBJECT ${QT_STATIC_QML_IMPORT_JSON_OBJ})
      string(REGEX MATCH "\"classname\"\\: \"([a-zA-Z0-9]*)\"" CLASS_NAME ${JSON_OBJECT})
      #message(STATUS "CLASS_NAME : ${CLASS_NAME}")
      if(CMAKE_MATCH_1 AND NOT ${CMAKE_MATCH_1} STREQUAL "")

        set(_TEMP_CLASSNAME ${CMAKE_MATCH_1})
        set(${_TEMP_CLASSNAME}_CLASSNAME ${CMAKE_MATCH_1})
        list(FIND QT_STATIC_QML_DEPENDENCIES_PLUGINS ${CMAKE_MATCH_1} _PLUGIN_INDEX)
        if(_PLUGIN_INDEX EQUAL -1)
          list(APPEND QT_STATIC_QML_DEPENDENCIES_PLUGINS ${CMAKE_MATCH_1})

          string(REGEX MATCH "\"plugin\"\\: \"([a-zA-Z0-9_]*)\"" PLUGIN_NAME ${JSON_OBJECT})
          set(${_TEMP_CLASSNAME}_PLUGIN ${CMAKE_MATCH_1})
          #message(STATUS "PLUGIN_NAME : ${PLUGIN_NAME}")
          string(REGEX MATCH "\"path\"\\: \"([^\"]*)\"" PLUGIN_PATH ${JSON_OBJECT})
          #message(STATUS "PLUGIN_PATH : ${PLUGIN_PATH}")
          set(${_TEMP_CLASSNAME}_PATH ${CMAKE_MATCH_1})
        endif()
      endif()
    endforeach()

    # Print dependencies
    # if(QT_STATIC_VERBOSE)
    # message(STATUS "${QT_STATIC_TARGET} qml plugin dependencies:")
    # foreach(PLUGIN ${QT_STATIC_QML_DEPENDENCIES_PLUGINS})
    #     message(STATUS "${PLUGIN}")
    # endforeach()
    # endif()

    if(QT_STATIC_VERBOSE)
      message(STATUS "Generate ${QT_STATIC_OUTPUT} in ${QT_STATIC_OUTPUT_DIR}")
    endif()

    # Build file path
    set(QT_STATIC_QML_PLUGIN_SRC_FILE "${QT_STATIC_OUTPUT_DIR}/${QT_STATIC_OUTPUT}")

    # Write file header
    file(WRITE ${QT_STATIC_QML_PLUGIN_SRC_FILE} "// File Generated via CMake script during build time.\n"
      "// The purpose of this file is to force the static load of qml plugin during static build\n"
      "// Please rerun CMake to update this file.\n"
      "// File will be overwrite at each CMake run.\n"
      "\n#include <QtPlugin>\n\n")

    function(qt_static_generate_path_suffix result parentDir)
      file(GLOB_RECURSE children LIST_DIRECTORIES TRUE RELATIVE ${parentDir} ${parentDir}/*)
      set(dirlist "")
      foreach(child ${children})
        if(IS_DIRECTORY ${parentDir}/${child})
          list(APPEND dirlist ${child})
        endif()
      endforeach()
      set(${result} ${dirlist} PARENT_SCOPE)
    endfunction()

    # Generate PLUGIN_PATH_SUFFIXES for find_library PATH_SUFFIXES for plugin that do not have path set by qmlimportscanner
    qt_static_generate_path_suffix(PLUGIN_PATH_SUFFIXES ${QT_STATIC_QML_DIR})

    # Write Q_IMPORT_PLUGIN for each plugin
    foreach(PLUGIN ${QT_STATIC_QML_DEPENDENCIES_PLUGINS})
      # todo : in the future if any exclude plugin is added, filtering need to occur here.
      file(APPEND ${QT_STATIC_QML_PLUGIN_SRC_FILE} "Q_IMPORT_PLUGIN(${PLUGIN});\n")
    endforeach()

    # Add the file to the target sources
    if(QT_STATIC_VERBOSE)
      message(STATUS "Add ${QT_STATIC_QML_PLUGIN_SRC_FILE} to ${QT_STATIC_TARGET} sources")
    endif()
    target_sources(${QT_STATIC_TARGET} PRIVATE ${QT_STATIC_QML_PLUGIN_SRC_FILE})

    set(QT_STATIC_ALL_PLUGINS ${QT_STATIC_QML_DEPENDENCIES_PLUGINS} ${ARGSTATIC_EXTRA_PLUGIN})

    # Find the library associated with the plugin
    foreach(PLUGIN ${QT_STATIC_ALL_PLUGINS})

      # Try to use plugin name from qmlimportscanner
      set(_PLUGIN_LIBRARY_NAME ${${PLUGIN}_PLUGIN})
      if(NOT _PLUGIN_LIBRARY_NAME)
        #message(WARNING "Plugin ${PLUGIN} doesn't have a \"plugin\" field, use plugin name")
        set(_PLUGIN_LIBRARY_NAME ${PLUGIN})
      endif()

      string(TOLOWER ${_PLUGIN_LIBRARY_NAME} _PLUGIN_LIBRARY_NAME_LOWER)

      # Try to use path from qmlimportscanner
      if(${${PLUGIN}_PATH})
        find_library(${PLUGIN}_plugin "${_PLUGIN_LIBRARY_NAME_LOWER}"
          HINTS ${${PLUGIN}_PATH})

        find_library(${PLUGIN}_plugind "${_PLUGIN_LIBRARY_NAME_LOWER}d"
          HINTS ${${PLUGIN}_PATH})
      else()
        find_library(${PLUGIN}_plugin NAMES "${_PLUGIN_LIBRARY_NAME_LOWER}" "lib${_PLUGIN_LIBRARY_NAME_LOWER}"
          HINTS ${QT_STATIC_QML_DIR}
          PATH_SUFFIXES ${PLUGIN_PATH_SUFFIXES})

        find_library(${PLUGIN}_plugind NAMES "${_PLUGIN_LIBRARY_NAME_LOWER}d" "lib${_PLUGIN_LIBRARY_NAME_LOWER}d"
          HINTS ${QT_STATIC_QML_DIR}
          PATH_SUFFIXES ${PLUGIN_PATH_SUFFIXES})
      endif()

      # Use release library for release if available, otherwise debug library, otherwise produce warning
      if(${${PLUGIN}_plugin} STREQUAL "${PLUGIN}_plugin-NOTFOUND")
        # Fallback on debug library
        if(${${PLUGIN}_plugind} STREQUAL "${PLUGIN}_plugind-NOTFOUND")
          message(WARNING "Fail to find ${PLUGIN} for release build (release or debug library not available)")
        else()
          target_link_libraries(${QT_STATIC_TARGET} PRIVATE ${${PLUGIN}_plugind})
          if(QT_STATIC_VERBOSE)
            message(STATUS "${PLUGIN} Release : Found ${${PLUGIN}_plugind} (release library not available, use debug as fallback)")
          endif()
        endif()
      else()
        target_link_libraries(${QT_STATIC_TARGET} PRIVATE $<$<NOT:$<CONFIG:Debug>>:${${PLUGIN}_plugin}>)
        if(QT_STATIC_VERBOSE)
          message(STATUS "${PLUGIN} Release : Found ${${PLUGIN}_plugin}")
        endif()
      endif()

      # Use debug library for debug if available, otherwise release library, otherwise produce warning
      if(${${PLUGIN}_plugind} STREQUAL "${PLUGIN}_plugind-NOTFOUND")
        # Fallback on release library
        if(${${PLUGIN}_plugin} STREQUAL "${PLUGIN}_plugin-NOTFOUND")
          message(WARNING "Fail to find ${PLUGIN} for debug build (release or debug library not available)")
        else()
          target_link_libraries(${QT_STATIC_TARGET} PRIVATE ${${PLUGIN}_plugin})
          if(QT_STATIC_VERBOSE)
            message(STATUS "${PLUGIN} Debug : Found ${${PLUGIN}_plugin} (debug library not available, use release as fallback)")
          endif()
        endif()
      else()
        target_link_libraries(${QT_STATIC_TARGET} PRIVATE $<$<CONFIG:Debug>:${${PLUGIN}_plugind}>)
        if(QT_STATIC_VERBOSE)
          message(STATUS "${PLUGIN} Debug : Found ${${PLUGIN}_plugind}")
        endif()
      endif()
    endforeach()
  else()
    message(WARNING "QT_STATIC_QML_SRC not specified. Can't generate Q_IMPORT_PLUGIN for qml plugin")
  endif()

  get_target_property(CURRENT_INTERFACE_SOURCES ${QT_STATIC_TARGET} INTERFACE_SOURCES)
  if(${CURRENT_INTERFACE_SOURCES} STREQUAL "CURRENT_INTERFACE_SOURCES-NOTFOUND")
    set(CURRENT_INTERFACE_SOURCES ${QT_STATIC_QML_PLUGIN_SRC_FILE})
  else()
    set(CURRENT_INTERFACE_SOURCES ${CURRENT_INTERFACE_SOURCES} ${QT_STATIC_QML_PLUGIN_SRC_FILE})
  endif()
  set_target_properties(${QT_STATIC_TARGET} PROPERTIES INTERFACE_SOURCES "${CURRENT_INTERFACE_SOURCES}")
endmacro()

# ┌──────────────────────────────────────────────────────────────────┐
# │                     GENERATE QT PLUGIN                           │
# └──────────────────────────────────────────────────────────────────┘

# Usage:
# qt_generate_plugin_import(YourApp
#   OUTPUT "YourApp_plugin_import.cpp"
#   OUTPUT_DIR "/path/to/generate"
#   VERBOSE
#)
macro(qt_generate_plugin_import TARGET)

  set(QT_STATIC_OPTIONS VERBOSE )
  set(QT_STATIC_ONE_VALUE_ARG OUTPUT
    OUTPUT_DIR
    )

  set(QT_STATIC_MULTI_VALUE_ARG)

  # parse the macro arguments
  cmake_parse_arguments(ARGSTATIC "${QT_STATIC_OPTIONS}" "${QT_STATIC_ONE_VALUE_ARG}" "${QT_STATIC_MULTI_VALUE_ARG}" ${ARGN})

  # Copy arg variables to local variables
  set(QT_STATIC_TARGET ${TARGET})
  set(QT_STATIC_OUTPUT ${ARGSTATIC_OUTPUT})
  set(QT_STATIC_OUTPUT_DIR ${ARGSTATIC_OUTPUT_DIR})
  set(QT_STATIC_VERBOSE ${ARGSTATIC_VERBOSE})

  # Default to ${QT_STATIC_TARGET}_qml_plugin_import.cpp
  if(NOT QT_STATIC_OUTPUT)
    set(QT_STATIC_OUTPUT ${QT_STATIC_TARGET}_plugin_import.cpp)
    if(QT_STATIC_VERBOSE)
      message(STATUS "OUTPUT not specified, default to ${QT_STATIC_OUTPUT}")
    endif()
  endif()

  # Default to project build directory
  if(NOT QT_STATIC_OUTPUT_DIR)
    set(QT_STATIC_OUTPUT_DIR ${PROJECT_BINARY_DIR})
    if(QT_STATIC_VERBOSE)
      message(STATUS "OUTPUT not specified, default to ${QT_STATIC_OUTPUT_DIR}")
    endif()
  endif()

  # Print config
  if(QT_STATIC_VERBOSE)
    message(STATUS "------ QtStaticCMake Plugin Generate Configuration ------")
    message(STATUS "TARGET   : ${QT_STATIC_TARGET}")
    message(STATUS "OUTPUT      : ${QT_STATIC_OUTPUT}")
    message(STATUS "OUTPUT_DIR  : ${QT_STATIC_OUTPUT_DIR}")
    message(STATUS "------ QtStaticCMake Plugin Generate End Configuration ------")
  endif()

  if(QT_STATIC_VERBOSE)
    message(STATUS "Generate ${QT_STATIC_OUTPUT} in ${QT_STATIC_OUTPUT_DIR}")
  endif()

  set(QT_STATIC_PLUGIN_SRC_FILE "${QT_STATIC_OUTPUT_DIR}/${QT_STATIC_OUTPUT}")

  # Write the file header
  file(WRITE ${QT_STATIC_PLUGIN_SRC_FILE} "// File Generated via CMake script during build time.\n"
    "// The purpose of this file is to force the static load of qml plugin during static build\n"
    "// Please rerun CMake to update this file.\n"
    "// File will be overwrite at each CMake run.\n"
    "\n#include <QtPlugin>\n")

  # Get all available Qt5 module
  file(GLOB QT_STATIC_AVAILABLES_QT_DIRECTORIES
    LIST_DIRECTORIES true
    RELATIVE ${QT_STATIC_QT_ROOT}/lib/cmake
    ${QT_STATIC_QT_ROOT}/lib/cmake/Qt5*)
  foreach(DIR ${QT_STATIC_AVAILABLES_QT_DIRECTORIES})
    set(DIR_PRESENT ${${DIR}_DIR})
    if(DIR_PRESENT)
      set(DIR_PLUGIN_CONTENT ${${DIR}_PLUGINS})
      # Only print if there are some plugin
      if(DIR_PLUGIN_CONTENT)
        # Comment for help dubugging
        file(APPEND ${QT_STATIC_PLUGIN_SRC_FILE} "\n// ${DIR}\n")
        # Parse Plugin to append to the list only if unique
        foreach(PLUGIN ${DIR_PLUGIN_CONTENT})
          # List form is Qt5::NameOfPlugin, we just need NameOfPlugin
          string(REGEX MATCH ".*\\:\\:(.*)" PLUGIN_MATCH ${PLUGIN})
          set(PLUGIN_NAME ${CMAKE_MATCH_1})
          # Should be NameOfPlugin
          if(PLUGIN_NAME)
            # Keep track to only write once each plugin
            list(FIND QT_STATIC_DEPENDENCIES_PLUGINS ${PLUGIN_NAME} _PLUGIN_INDEX)
            # Only Write/Keep track if the plugin isn't already present
            if(_PLUGIN_INDEX EQUAL -1)
              list(APPEND QT_STATIC_DEPENDENCIES_PLUGINS ${PLUGIN_NAME})
              file(APPEND ${QT_STATIC_PLUGIN_SRC_FILE} "Q_IMPORT_PLUGIN(${PLUGIN_NAME});\n")

              set(PLUGIN_LIBRARY Qt5::${PLUGIN_NAME})
              if(QT_STATIC_VERBOSE)
                message(STATUS "Force link to qml plugin ${PLUGIN_LIBRARY}")
              endif()
              target_link_libraries(${QT_STATIC_TARGET} PRIVATE ${PLUGIN_LIBRARY})
            endif()
          endif()
        endforeach()
      endif()
    endif()
  endforeach()

  # Print dependencies
  if(QT_STATIC_VERBOSE)
    message(STATUS "${QT_STATIC_TARGET} plugin dependencies:")
    foreach(PLUGIN ${QT_STATIC_DEPENDENCIES_PLUGINS})
      message(STATUS "${PLUGIN}")
    endforeach()
  endif()

  # Add the generated file into source of the application
  if(QT_STATIC_VERBOSE)
    message(STATUS "Add ${QT_STATIC_PLUGIN_SRC_FILE} to ${QT_STATIC_TARGET} sources")
  endif()
  target_sources(${QT_STATIC_TARGET} PRIVATE ${QT_STATIC_PLUGIN_SRC_FILE})

  if(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
    # Link to the platform library
    if(QT_STATIC_VERBOSE)
      message(STATUS "Add -u _qt_registerPlatformPlugin linker flag to ${QT_STATIC_TARGET} in order to force load qios library")
    endif()
    target_link_libraries(${QT_STATIC_TARGET} PUBLIC "-u _qt_registerPlatformPlugin")
  endif()

  get_target_property(CURRENT_INTERFACE_SOURCES ${QT_STATIC_TARGET} INTERFACE_SOURCES)
  if(${CURRENT_INTERFACE_SOURCES} STREQUAL "CURRENT_INTERFACE_SOURCES-NOTFOUND")
    set(CURRENT_INTERFACE_SOURCES ${QT_STATIC_PLUGIN_SRC_FILE})
  else()
    set(CURRENT_INTERFACE_SOURCES ${CURRENT_INTERFACE_SOURCES} ${QT_STATIC_PLUGIN_SRC_FILE})
  endif()
  set_target_properties(${QT_STATIC_TARGET} PROPERTIES INTERFACE_SOURCES "${CURRENT_INTERFACE_SOURCES}")
endmacro()
