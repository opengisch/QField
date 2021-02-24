#!/usr/bin/env bash

SRC_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"/..

QFIELD_SDK_VERSION=$(awk -F "=" '/osgeo4a_version/{print $2}' sdk.conf)
export APK_VERSION_CODE=${APK_VERSION_CODE:-1}
export APP_VERSION_STR=${APP_VERSION_STR:-dev}

ARCH=${ARCH:-arm64_v8a}

docker run --rm \
  -v "$SRC_DIR":/usr/src/qfield:Z \
  $(if [ -n "$CACHE_DIR" ]; then echo "-v $CACHE_DIR:/io/data"; fi) \
  -e "BUILD_FOLDER=build-${ARCH}" \
  -e ARCH \
  -e STOREPASS \
  -e KEYNAME \
  -e KEYPASS \
  -e APP_PACKAGE_NAME \
  -e APP_NAME \
  -e APP_ICON \
  -e APP_VERSION \
  -e APP_VERSION_STR \
  -e APK_VERSION_CODE \
  opengisch/qfield-sdk:${QFIELD_SDK_VERSION} \
  /usr/src/qfield/scripts/docker-build-wrapper.sh
