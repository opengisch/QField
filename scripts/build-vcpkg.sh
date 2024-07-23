#!/bin/bash

echo "building for ${triplet} on Qt ${install_qt_version} (${install_qt_arch})"

export SOURCE_DIR=/usr/src/qfield

CMAKE_BUILD_DIR=/usr/src/qfield/build-${triplet}
ANDROID_NDK_VERSION=$(sed -En -e 's/^Pkg.Revision\s*=\s*([0-9a-f]+)/\1/p' ${ANDROID_NDK_HOME}/source.properties)

[[ -z ${APP_NAME} ]] && APP_NAME="QField Home"
[[ -z ${APP_PACKAGE_NAME} ]] && APP_PACKAGE_NAME="qfield_home"
[[ -z ${APP_ICON} ]] && APP_ICON="qfield_logo_beta"

echo "Package name ${APP_PACKAGE_NAME}"

# Configure and install deps
cmake -S "${SOURCE_DIR}" \
	-B "${CMAKE_BUILD_DIR}" \
	-G Ninja \
	-D VCPKG_TARGET_TRIPLET="${triplet}" \
	-D ANDROID_NDK_VERSION="${ANDROID_NDK_VERSION}" \
	-D ANDROID_SDK_ROOT="${ANDROID_SDK_ROOT}" \
	-D ANDROID_BUILD_TOOLS_VERSION="${ANDROID_BUILD_TOOLS_VERSION}" \
	-D WITH_SPIX=OFF \
	-D APK_VERSION_CODE="1" \
	-D APP_VERSION="" \
	-D APP_VERSION_STR="Home" \
	-D APP_PACKAGE_NAME="${APP_PACKAGE_NAME}" \
	-D APP_ICON="${APP_ICON}" \
	-D APP_NAME="${APP_NAME}" \
	-D VCPKG_INSTALL_OPTIONS="--allow-unsupported" \
	${NUGET_TOKEN:+-D NUGET_TOKEN=${NUGET_TOKEN}} \
	${NUGET_USERNAME:+-D NUGET_USERNAME=${NUGET_USERNAME}}

# Build app
cmake --build "${CMAKE_BUILD_DIR}"

# Package app
cmake --build "${CMAKE_BUILD_DIR}" --target bundle --config Release
