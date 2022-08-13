vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO getsentry/sentry-cocoa
    REF 7.23.0
    SHA512 2cc9d1dc39221b31dbbfb650ebbed30356f44d9adac7a8a9c13fdaabc58aa3a7f5d4192d521a4296efe45f17ca0a86c53f9171697340a25c50e75be602033b68
    HEAD_REF master
    PATCHES
      stdint.patch
      ucontext64.patch
)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt DESTINATION ${SOURCE_PATH})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/SentryCocoaConfig.cmake.in DESTINATION ${SOURCE_PATH})

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
)

vcpkg_install_cmake()

vcpkg_cmake_config_fixup(
    CONFIG_PATH lib/cmake/SentryCocoa
    PACKAGE_NAME SentryCocoa
)

file(INSTALL "${SOURCE_PATH}/LICENSE.md" DESTINATION "${CURRENT_PACKAGES_DIR}/share/sentry-cocoa" RENAME copyright)
