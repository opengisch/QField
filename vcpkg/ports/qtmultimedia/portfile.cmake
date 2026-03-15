set(SCRIPT_PATH "${CURRENT_INSTALLED_DIR}/share/qtbase")
include("${SCRIPT_PATH}/qt_install_submodule.cmake")

set(${PORT}_PATCHES
    static_find_modules.patch
    remove-static-ssl-stub.patch
    ffmpeg-compile-def-and-devendor-signalsmith-stretch.patch
    ffmpeg.patch
)

vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
FEATURES
    "qml"           CMAKE_REQUIRE_FIND_PACKAGE_Qt6Quick
    "widgets"       CMAKE_REQUIRE_FIND_PACKAGE_Qt6Widgets
INVERTED_FEATURES
    "qml"           CMAKE_DISABLE_FIND_PACKAGE_Qt6Quick
    "widgets"       CMAKE_DISABLE_FIND_PACKAGE_Qt6Widgets
    "gstreamer"     CMAKE_DISABLE_FIND_PACKAGE_GStreamer
    "ffmpeg"        CMAKE_DISABLE_FIND_PACKAGE_FFmpeg
    # Features not yet added in the manifest:
    "vaapi"         CMAKE_DISABLE_FIND_PACKAGE_VAAPI # not in vpckg
)

list(APPEND FEATURE_OPTIONS "-DCMAKE_DISABLE_FIND_PACKAGE_ALSA=ON")

# Force all gstreamer extra features to off to not poison the cache
# since enabling them is done depening on how gstreamer was built
list(APPEND FEATURE_OPTIONS "-DFEATURE_gstreamer_gl=OFF")
list(APPEND FEATURE_OPTIONS "-DFEATURE_gstreamer_gl_wayland=OFF")
list(APPEND FEATURE_OPTIONS "-DFEATURE_gstreamer_gl_egl=OFF")
list(APPEND FEATURE_OPTIONS "-DFEATURE_gstreamer_gl_x11=OFF")
list(APPEND FEATURE_OPTIONS "-DFEATURE_gstreamer_photography=OFF")

set(unused "")
if("gstreamer" IN_LIST FEATURES)
    list(APPEND FEATURE_OPTIONS "-DINPUT_gstreamer='yes'")
else()
    list(APPEND FEATURE_OPTIONS "-DINPUT_gstreamer='no'")
endif()

if(VCPKG_TARGET_IS_WINDOWS)
    list(APPEND FEATURE_OPTIONS "-DFEATURE_wmf=ON")
else()
    list(APPEND FEATURE_OPTIONS "-DFEATURE_wmf=OFF")
endif()

if("ffmpeg" IN_LIST FEATURES)
    # Note: Requires pulsadio on linux and wmfsdk on windows
    list(APPEND FEATURE_OPTIONS "-DINPUT_ffmpeg='yes'")
    if(VCPKG_TARGET_IS_LINUX)
        list(APPEND FEATURE_OPTIONS "-DINPUT_pulseaudio='yes'")
    else()
        list(APPEND FEATURE_OPTIONS "-DINPUT_pulseaudio='no'")
    endif()
else()
    list(APPEND FEATURE_OPTIONS "-DINPUT_ffmpeg='no'")
    list(APPEND FEATURE_OPTIONS "-DINPUT_pulseaudio='no'")
endif()

list(APPEND FEATURE_OPTIONS "-DINPUT_pipewire='no'")

qt_install_submodule(PATCHES    ${${PORT}_PATCHES}
                     CONFIGURE_OPTIONS
                        ${FEATURE_OPTIONS}
                        -DCMAKE_FIND_PACKAGE_TARGETS_GLOBAL=ON
                     CONFIGURE_OPTIONS_RELEASE
                     CONFIGURE_OPTIONS_DEBUG
                     CONFIGURE_OPTIONS_MAYBE_UNUSED ${unused}
                    )

if("gstreamer" IN_LIST FEATURES AND VCPKG_LIBRARY_LINKAGE STREQUAL "static")
    vcpkg_replace_string("${CURRENT_PACKAGES_DIR}/share/Qt6Multimedia/Qt6QGstreamerMediaPluginDependencies.cmake" "GStreamer\;FALSE\;\;\;;GStreamer\;FALSE\;\;App\;;GStreamer\;FALSE\;\;\;Gl" "GStreamer\;FALSE\;\;\;;GStreamer\;FALSE\;\;App\;;GStreamer\;FALSE\;\;\;Gl;EGL\;FALSE\;\;\;" IGNORE_UNCHANGED)
endif()
