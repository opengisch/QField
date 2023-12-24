if(APPLE AND NOT IOS)
  set(USE_MAC_BUNDLING ON) # Will be overwritten by the VcpkgToolchain for vcpkg builds
endif()

if(VCPKG_TARGET_TRIPLET STREQUAL "arm64-android")
  set(ANDROID_ABI "arm64-v8a")
  set(ANDROID_STL c++_shared)
elseif(VCPKG_TARGET_TRIPLET STREQUAL "arm-neon-android")
  set(ANDROID_ABI "armeabi-v7a")
  set(ANDROID_STL c++_static)
elseif(VCPKG_TARGET_TRIPLET STREQUAL "arm-android")
  set(ANDROID_ABI "armeabi-v7a")
  set(ANDROID_STL c++_static)
elseif(VCPKG_TARGET_TRIPLET STREQUAL "x64-android")
  set(ANDROID_ABI "x86_64")
  set(ANDROID_STL c++_shared)
elseif(VCPKG_TARGET_TRIPLET STREQUAL "x86-android")
  set(ANDROID_ABI "x86")
  set(ANDROID_STL c++_shared)
endif()

if(ANDROID_ABI)
  # Qt by default assumes that cmake files live in `lib/cmake`. Point to `share` instead.
  # See https://github.com/qt/qtbase/blob/39290c508e2c7fa816c6f79b3eb284adc0cad3c0/src/corelib/Qt6AndroidMacros.cmake#L1016C16-L1026
  get_filename_component(QT_ANDROID_PATH_CMAKE_DIR_${ANDROID_ABI} "${Qt6_DIR}/.." ABSOLUTE)

  set(ANDROID_BUILD_TOOLS_VERSION "33.0.0" CACHE STRING "Android build-tools version")
  set(ANDROID_TARGET_PLATFORM 33 CACHE INT "Target Android platform SDK version")
  set(ANDROID_PLATFORM 21 CACHE INT "Minimum Android platform SDK version")
  set(ANDROID_PLATFORM_INT 21 CACHE INT "Minimum Android platform SDK version") # Used in build.gradle.in
  set(ANDROID_CPP_FEATURES "rtti exceptions")
endif()
