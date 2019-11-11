#!/bin/bash

if [ -n "${TRAVIS_TAG}" ]; then
  echo "Building release"
  APP_NAME="QField"
  PKG_NAME="qfield"
  APP_ICON="qfield-logo.svg"
elif [ "${TRAVIS_PULL_REQUEST}" = "false" ]; then
  echo "Building nightly"
  APP_NAME="QField Nightly"
  PKG_NAME="qfield_nightly"
  APP_ICON="qfield-testlogo.svg"
else
  echo "Building pull request beta"
  APP_NAME="QField Beta ${TRAVIS_PULL_REQUEST}"
  PKG_NAME="qfield_beta"
  APP_ICON="qfield-testlogo.svg"
fi
docker run -v $(pwd):/usr/src/qfield -e "BUILD_FOLDER=build-${ARCH}" -e ARCH -e STOREPASS -e KEYNAME -e KEYPASS -e "VERSION=${TRAVIS_TAG}" -e PKG_NAME -e APP_NAME -e APP_ICON opengisch/qfield-sdk:${QFIELD_SDK_VERSION} /usr/src/qfield/scripts/docker-build.sh
