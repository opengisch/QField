find_program(LINUXDEPLOY_EXECUTABLE linuxdeploy linuxdeploy-x86_64.AppImage)

function(windeployqt target)
    # Bundle Library Files
    if(CMAKE_BUILD_TYPE_UPPER STREQUAL "DEBUG")
        set(WINDEPLOYQT_ARGS --debug)
    else()
        set(WINDEPLOYQT_ARGS --release)
    endif()

    add_custom_command(TARGET ${target} POST_BUILD
                       COMMAND "${CMAKE_COMMAND}" -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/winqt/"
                       COMMAND "${CMAKE_COMMAND}" -E
                               env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                               ${WINDEPLOYQT_ARGS}
                               --verbose 0
                               --no-compiler-runtime
                               --no-angle
                               --no-opengl-sw
                               --dir "${CMAKE_CURRENT_BINARY_DIR}/winqt/"
                               $<TARGET_FILE:${target}>
                       COMMENT "Deploying Qt..."
    )
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/winqt/" DESTINATION bin)
    set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
    include(InstallRequiredSystemLibraries)
endfunction()

if(WIN32)
    # NSIS windows installer
    find_program(NSIS_PATH nsis PATH_SUFFIXES nsis)
    if(NSIS_PATH)
        list(APPEND CPACK_GENERATOR NSIS)
        message(STATUS "   + NSIS                                 YES ")

        set(CPACK_NSIS_DISPLAY_NAME ${CPACK_PACKAGE_NAME})
        # Icon of the installer
        file(TO_NATIVE_PATH "${PACKAGE_ICON}" CPACK_NSIS_MUI_ICON)
        file(TO_NATIVE_PATH "${PACKAGE_ICON}" CPACK_NSIS_MUI_HEADERIMAGE_BITMAP)
        set(CPACK_NSIS_CONTACT "${CPACK_PACKAGE_CONTACT}")
        set(CPACK_NSIS_MODIFY_PATH ON)
    else()
        message(STATUS "   + NSIS                                 NO ")
    endif()

    windeployqt(QField)
endif()

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
