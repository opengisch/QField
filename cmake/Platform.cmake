if(APPLE AND NOT IOS)
  set(USE_MAC_BUNDLING ON) # Will be overwritten by the VcpkgToolchain for vcpkg builds
endif()

if(VCPKG_TARGET_TRIPLET STREQUAL "arm64-android")
  set(ANDROID_ABI "arm64-v8a")
elseif(VCPKG_TARGET_TRIPLET STREQUAL "arm-android")
  set(ANDROID_ABI "armeabi-v7a")
elseif(VCPKG_TARGET_TRIPLET STREQUAL "x64-android")
  set(ANDROID_ABI "x86_64")
elseif(VCPKG_TARGET_TRIPLET STREQUAL "x86-android")
  set(ANDROID_ABI "x86")
endif()

if(ANDROID_ABI)
  set(ANDROID_TARGET_PLATFORM 30 CACHE INT "Target Android platform SDK version")
  set(ANDROID_PLATFORM 21 CACHE INT "Minimum Android platform SDK version")
endif()
