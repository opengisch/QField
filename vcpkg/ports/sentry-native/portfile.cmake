vcpkg_download_distfile(ARCHIVE
    URLS "https://github.com/getsentry/sentry-native/releases/download/${VERSION}/sentry-native.zip"
    FILENAME "sentry-native-${VERSION}.zip"
    SHA512 6317e30f15f87d92dcbec72cea0f61ded012c768c23a0ed2eaab4355e0264aefab4a16cec15628e0a1527a605e4edf5b168b701593d782fb6e515a05ac7215d1
)

vcpkg_extract_source_archive(
    SOURCE_PATH
    ARCHIVE "${ARCHIVE}"
    NO_REMOVE_ONE_LEVEL
    PATCHES
        fix-crashpad-wer.patch
        fix-usage-runtime.patch
)
file(REMOVE_RECURSE "${SOURCE_PATH}/external/crashpad/third_party/zlib/zlib")

vcpkg_list(SET options)

if(NOT "backend" IN_LIST FEATURES)
    vcpkg_list(APPEND options "-DSENTRY_BACKEND=none")
elseif("wer" IN_LIST FEATURES)
    vcpkg_list(APPEND options "-DSENTRY_BACKEND=crashpad")
elseif("breakpad" IN_LIST FEATURES)
    vcpkg_list(APPEND options "-DSENTRY_BACKEND=breakpad")
elseif(DEFINED SENTRY_BACKEND)
    # Legacy, possible override from triplet, but cannot handle dependencies
    vcpkg_list(APPEND options "-DSENTRY_BACKEND=${SENTRY_BACKEND}")
endif()

if(NOT "transport" IN_LIST FEATURES)
    vcpkg_list(APPEND options "-DSENTRY_TRANSPORT=none")
endif()

if("wer" IN_LIST FEATURES)
    vcpkg_list(APPEND options "-DSENTRY_TRANSPORT_CRASHPAD_USE_WER=ON")
endif()

if("compression" IN_LIST FEATURES)
    vcpkg_list(APPEND options "-DSENTRY_TRANSPORT_COMPRESSION=ON")
endif()

if(VCPKG_TARGET_IS_WINDOWS AND NOT VCPKG_TARGET_IS_MINGW)
    set(VCPKG_CXX_FLAGS "/D_CRT_DECLARE_NONSTDC_NAMES ${VCPKG_CXX_FLAGS}")
    set(VCPKG_C_FLAGS "/D_CRT_DECLARE_NONSTDC_NAMES ${VCPKG_C_FLAGS}")
endif()

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        ${options}
        -DSENTRY_BUILD_TESTS=OFF
        -DSENTRY_BUILD_EXAMPLES=OFF
        -DCRASHPAD_ZLIB_SYSTEM=ON
    MAYBE_UNUSED_VARIABLES
        CRASHPAD_ZLIB_SYSTEM
)

vcpkg_cmake_install()

vcpkg_copy_pdbs()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

vcpkg_cmake_config_fixup(PACKAGE_NAME sentry CONFIG_PATH lib/cmake/sentry)

if(EXISTS "${CURRENT_PACKAGES_DIR}/bin/crashpad_handler${VCPKG_TARGET_EXECUTABLE_SUFFIX}")
    vcpkg_copy_tools(TOOL_NAMES crashpad_handler AUTO_CLEAN)
endif()

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
