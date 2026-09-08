vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO getsentry/sentry-cocoa
    REF 8.58.4
    SHA512 12898a3e23e9c7e60706b95180f534e1c4d1c33037ef932cec734945a9838779fa87cb8ab87378e620c080ffcabee4236f442d4e7ebf92ba7dc3d8f4cb7703cd
    HEAD_REF master
    PATCHES
      exception.patch
)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt DESTINATION ${SOURCE_PATH})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/SentryCocoaConfig.cmake.in DESTINATION ${SOURCE_PATH})

if(VCPKG_CROSSCOMPILING)
    list(APPEND SENTRY_COCOA_OPTIONS -DQT_HOST_PATH=${CURRENT_HOST_INSTALLED_DIR})
    list(APPEND SENTRY_COCOA_OPTIONS -DQT_HOST_PATH_CMAKE_DIR:PATH=${CURRENT_HOST_INSTALLED_DIR}/share)
endif()

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        ${SENTRY_COCOA_OPTIONS}
)

vcpkg_install_cmake()

vcpkg_cmake_config_fixup(
    CONFIG_PATH lib/cmake/SentryCocoa
    PACKAGE_NAME SentryCocoa
)

file(INSTALL "${SOURCE_PATH}/LICENSE.md" DESTINATION "${CURRENT_PACKAGES_DIR}/share/sentry-cocoa" RENAME copyright)
