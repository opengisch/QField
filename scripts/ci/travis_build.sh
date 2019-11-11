#!/bin/bash

if [ -n "${TRAVIS_TAG}" ]; then
  echo "Building release"
  export APP_NAME="QField"
  export PKG_NAME="qfield"
  export APP_ICON="qfield-logo.svg"
elif [ "${TRAVIS_PULL_REQUEST}" = "false" ]; then
  echo "Building nightly"
  export APP_NAME="QField Nightly"
  export PKG_NAME="qfield_nightly"
  export APP_ICON="qfield-testlogo.svg"
else
  echo "Building pull request beta"
  export APP_NAME="QField Beta ${TRAVIS_PULL_REQUEST}"
  export PKG_NAME="qfield_beta"
  export APP_ICON="qfield-testlogo.svg"
fi
docker run -v $(pwd):/usr/src/qfield -e "BUILD_FOLDER=build-${ARCH}" -e ARCH -e STOREPASS -e KEYNAME -e KEYPASS -e "VERSION=${TRAVIS_TAG}" -e PKG_NAME -e APP_NAME -e APP_ICON opengisch/qfield-sdk:${QFIELD_SDK_VERSION} /usr/src/qfield/scripts/docker-build.sh
