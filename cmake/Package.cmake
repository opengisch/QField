

######################################################################################################################
# Packaging
######################################################################################################################

find_program(LINUXDEPLOY_EXECUTABLE linuxdeploy linuxdeploy-x86_64.AppImage)

get_target_property(qmake_executable Qt5::qmake IMPORTED_LOCATION)

set(CPACK_OUTPUT_CONFIG_FILE "${CMAKE_BINARY_DIR}/BundleConfig.cmake")

set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}")

add_custom_target(bundle
                  COMMAND ${CMAKE_CPACK_COMMAND} "--config" "${CMAKE_BINARY_DIR}/BundleConfig.cmake"
                  COMMENT "Running CPACK. Please wait..."
                  DEPENDS qfield)
set(CPACK_GENERATOR)

# Qt IFW packaging framework
if(BINARYCREATOR_EXECUTABLE)
    list(APPEND CPACK_GENERATOR IFW)
    message(STATUS "   + Qt Installer Framework               YES ")
else()
    message(STATUS "   + Qt Installer Framework                NO ")
endif()


if(LINUXDEPLOY_EXECUTABLE)
    message(STATUS "   + AppImage                             YES ")
    set(CPACK_GENERATOR "External;${CPACK_GENERATOR}")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CPackLinuxDeployQt.cmake.in "${CMAKE_BINARY_DIR}/CPackExternal.cmake")
    set(CPACK_EXTERNAL_PACKAGE_SCRIPT "${CMAKE_BINARY_DIR}/CPackExternal.cmake")
else()
    message(STATUS "   + AppImage                              NO ")
endif()

include(CPack)
