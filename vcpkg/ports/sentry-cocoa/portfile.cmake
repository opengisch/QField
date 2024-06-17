vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO getsentry/sentry-cocoa
    REF ${VERSION}
    SHA512 177959e55e7ef70467df331ab940b28170063d389bf2ec8722fc81cdb09fe3e8a73f539b1182b4865e7e619f69dc623e4b0fce52f01d682e28536eb99c15d2b4
    HEAD_REF master
    PATCHES
      ucontext64.patch
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
