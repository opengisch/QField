vcpkg_download_distfile(ARCHIVE
    URLS "https://github.com/getsentry/sentry-cocoa/releases/download/${VERSION}/Sentry.xcframework.zip"
    FILENAME "sentry-cocoa-${VERSION}-Sentry.xcframework.zip"
    SHA512 ed2b739b4a3ce4ee60c736ee0b63699a8ce21ac7e34c3ff8a8432440b8c00d609546b4d45914a5c91f11f26a5d45ee6c46ac75a49cfc58db7624ed503c79b9e8
)

vcpkg_download_distfile(LICENSE_FILE
    URLS "https://raw.githubusercontent.com/getsentry/sentry-cocoa/${VERSION}/LICENSE.md"
    FILENAME "sentry-cocoa-${VERSION}-LICENSE.md"
    SHA512 1d717ad4fa2eef387a57b7044fd5753f4bd5020bdf15e3d7920ccc6c522dcda337b47a527aa43dd6325ce4f1866dd1828e23bfe2abfd4f61905c37a4e1c52a0f
)

vcpkg_extract_source_archive(
    SOURCE_PATH
    ARCHIVE "${ARCHIVE}"
    NO_REMOVE_ONE_LEVEL
)

if(VCPKG_TARGET_IS_IOS)
    set(SENTRY_SLICE "ios-arm64_arm64e")
elseif(VCPKG_TARGET_IS_OSX)
    set(SENTRY_SLICE "macos-arm64_arm64e_x86_64")
else()
    message(FATAL_ERROR "sentry-cocoa only supports iOS and macOS targets.")
endif()

set(SENTRY_FRAMEWORK "${SOURCE_PATH}/Sentry.xcframework/${SENTRY_SLICE}/Sentry.framework")
if(NOT EXISTS "${SENTRY_FRAMEWORK}")
    message(FATAL_ERROR "Slice '${SENTRY_SLICE}' not found in Sentry.xcframework.")
endif()

file(COPY "${SENTRY_FRAMEWORK}" DESTINATION "${CURRENT_PACKAGES_DIR}/lib")
file(COPY "${SENTRY_FRAMEWORK}" DESTINATION "${CURRENT_PACKAGES_DIR}/debug/lib")

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/SentryCocoaConfig.cmake.in"
    "${CURRENT_PACKAGES_DIR}/lib/cmake/SentryCocoa/SentryCocoaConfig.cmake"
    @ONLY)

vcpkg_install_copyright(FILE_LIST "${LICENSE_FILE}")
