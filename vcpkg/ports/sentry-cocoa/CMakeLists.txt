cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0025 NEW)
project(SentryCocoa)

file(GLOB_RECURSE SRCS "Sources/*.m" "Sources/*.c" "Sources/*.cpp" "Sources/*.mm")

file(GLOB_RECURSE PUBLIC_HDRS Sources/Sentry/Public/*.h)

set (CMAKE_CXX_STANDARD 14)
add_library(sentry_cocoa
    ${SRCS}
)

target_include_directories(
  sentry_cocoa
  PRIVATE
    ${CMAKE_SOURCE_DIR}/Sources/Sentry/include
    ${CMAKE_SOURCE_DIR}/Sources/SentryCrash/Recording
    ${CMAKE_SOURCE_DIR}/Sources/SentryCrash/Recording/Tools
    ${CMAKE_SOURCE_DIR}/Sources/SentryCrash/Reporting/Filters
    ${CMAKE_SOURCE_DIR}/Sources/SentryCrash/Installations
    ${CMAKE_SOURCE_DIR}/Sources/SentryCrash/Reporting/Filters/Tools
    ${CMAKE_SOURCE_DIR}/Sources/SentryCrash/Reporting/Tools
    ${CMAKE_SOURCE_DIR}/Sources/SentryCrash/Recording/Monitors)

target_include_directories(
  sentry_cocoa
  PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Sources/Sentry/Public>"
  INTERFACE "$<INSTALL_INTERFACE:include/Sentry>")

set_target_properties(sentry_cocoa PROPERTIES PUBLIC_HEADER "${PUBLIC_HDRS}")

target_compile_options(sentry_cocoa PRIVATE "-fobjc-arc")

# initializes ${CMAKE_INSTALL_*DIR} to sane defaults
include(GNUInstallDirs)
install(
    TARGETS sentry_cocoa
    EXPORT SentryCocoaTargets
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Sentry")

set(CMAKECONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/SentryCocoa)

install (
    EXPORT SentryCocoaTargets
    DESTINATION ${CMAKECONFIG_INSTALL_DIR} NAMESPACE SentryCocoa::
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  SentryCocoaConfig.cmake.in
  SentryCocoaConfig.cmake
  INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR})

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/SentryCocoaConfigVersion.cmake
  VERSION 1.2.3
  COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SentryCocoaConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/SentryCocoaConfigVersion.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})
