#!/bin/bash

echo "building for ${triplet}"

export SOURCE_DIR=/usr/src/qfield

CMAKE_BUILD_DIR=/usr/src/qfield/build-${triplet}

export Qt6_DIR=/home/devel/6.5.1/android_arm64_v8a

[[ -z ${APP_NAME} ]] && APP_NAME="QField"
[[ -z ${APP_PACKAGE_NAME} ]] && APP_PACKAGE_NAME="qfield"
[[ -z ${APP_ICON} ]] && APP_ICON="qfield_logo"

echo "Package name ${APP_PACKAGE_NAME}"

# Configure and install deps
cmake -S "${SOURCE_DIR}" \
      -B "${CMAKE_BUILD_DIR}" \
      -G Ninja \
      -D BUILD_WITH_QT6=ON \
      -D CMAKE_PREFIX_PATH=${Qt6_DIR} \
      -D QT_HOST_PATH=/home/devel/6.5.1/gcc_64 \
      -D QT_HOST_PATH_CMAKE_DIR=/home/devel/6.5.1/gcc_64/lib/cmake \
      -D VCPKG_TARGET_TRIPLET="${triplet}" \
      -D SYSTEM_QT=ON \
      -D ANDROID_NDK_VERSION="${ANDROID_NDK_VERSION}" \
      -D ANDROID_SDK=/opt/android-sdk-linux \
      -D ANDROID_SDK_ROOT=/opt/android-sdk-linux \
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
