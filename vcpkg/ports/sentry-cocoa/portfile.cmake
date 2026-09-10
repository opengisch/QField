vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO getsentry/sentry-cocoa
    REF 7.31.5
    SHA512 328190f6794e1174c6c95bc1cf10ff7d39e8a5f1e3da88781e083c5e8fe3b5dc656fa7ca96de4dd77a2e5f04a0e43b83d0b40b7697a2d5c96024a567362cf511
    HEAD_REF master
    PATCHES
      stdint.patch
      ucontext64.patch
      exception.patch
      const-vector.patch
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
