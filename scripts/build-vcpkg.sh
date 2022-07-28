#!/bin/bash

echo "building for ${triplet}"

cd /usr/src/qfield || exit

CMAKE_BUILD_DIR=/usr/src/qfield/build-${triplet}

export ANDROID_NDK_HOME=${ANDROID_NDK_ROOT}
export Qt5_DIR=/home/devel/5.14.2/android/
export ANDROID_PLATFORM=21
export ANDROID_TARGET_PLATFORM=30
export ANDROID_BUILD_TOOLS_VERSION=29.0.2
export ANDROID_NDK_VERSION=21.4.7075529

[[ -z ${APP_NAME} ]] && APP_NAME="QField"
[[ -z ${APP_PACKAGE_NAME} ]] && APP_PACKAGE_NAME="qfield"
[[ -z ${APP_ICON} ]] && APP_ICON="qfield_logo"

echo "Package name ${APP_PACKAGE_NAME}"

# Configure and install deps
cmake -S "${SOURCE_DIR}" \
      -B "${CMAKE_BUILD_DIR}" \
      -G Ninja \
      -D CMAKE_PREFIX_PATH=${Qt5_DIR} \
      -D CMAKE_TOOLCHAIN_FILE=/usr/src/qfield/vcpkg/base/scripts/buildsystems/vcpkg.cmake \
      -D VCPKG_OVERLAY_PORTS=/usr/src/qfield/vcpkg/overlay_system_qt\;/usr/src/qfield/vcpkg/overlay \
      -D VCPKG_TARGET_TRIPLET="${triplet}" \
      -D SYSTEM_QT=ON \
      -D ANDROID_SDK=/home/devel/android/ \
      -D ANDROID_NDK_VERSION="${ANDROID_NDK_VERSION}" \
      -D ANDROID_BUILD_TOOLS_VERSION="${ANDROID_BUILD_TOOLS_VERSION}" \
      -D WITH_SPIX=OFF \
      -D APP_VERSION="v1.0.0" \
      -D APK_VERSION_CODE="${APK_VERSION_CODE}" \
      -D APP_VERSION_STR="${APP_VERSION_STR}" \
      -D APP_PACKAGE_NAME="${APP_PACKAGE_NAME}" \
      -D APP_ICON="${APP_ICON}" \
      -D APP_NAME="${APP_NAME}" \
      ${NUGET_TOKEN:+-D NUGET_TOKEN=${NUGET_TOKEN}} \
      ${NUGET_USERNAME:+-D NUGET_USERNAME=${NUGET_USERNAME}}

# Build app
cmake --build "${CMAKE_BUILD_DIR}"

# Package app
cmake --build  "${CMAKE_BUILD_DIR}" --target bundle --config Release
