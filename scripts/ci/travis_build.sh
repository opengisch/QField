#!/usr/bin/env bash

CURRENT_COMMIT=$(git rev-parse --short HEAD)
LAST_TAG=$(git describe --abbrev=0 --tags)
COMMIT_COUNT=$(git log --oneline ${LAST_TAG}...HEAD | wc -l | bc)

if [ -n "${TRAVIS_TAG}" ]; then
  echo "Building release"
  export APP_NAME="QField"
  export PKG_NAME="qfield"
  export APP_ICON="qfield-logo.svg"
  export APP_VERSION="${TRAVIS_TAG}"
  export VERSION_SUFFIX_STR=$(echo "${APP_VERSION}" | sed -r -e 's/.*?(-.*)/\1/;t;d') # v.1.2.3-dev456 => -dev456, v.1.2.3 => EMPTY
elif [ "${TRAVIS_PULL_REQUEST}" = "false" ]; then
  echo "Building nightly"
  export APP_NAME="QField Dev"
  export PKG_NAME="qfield_dev"
  export APP_ICON="qfield-testlogo.svg"
  export APP_VERSION="${LAST_TAG}-dev${COMMIT_COUNT}"
  export VERSION_SUFFIX_STR="-dev${COMMIT_COUNT} ${CURRENT_COMMIT}"
else
  echo "Building pull request beta"
  export APP_NAME="QField Beta ${TRAVIS_PULL_REQUEST}"
  export PKG_NAME="qfield_beta"
  export APP_ICON="qfield-testlogo.svg"
  export APP_VERSION=""
  export VERSION_SUFFIX_STR=" PR${TRAVIS_PULL_REQUEST}"
fi
docker run -v $(pwd):/usr/src/qfield \
  -e "BUILD_FOLDER=build-${ARCH}" \
  -e ARCH -e STOREPASS -e KEYNAME -e KEYPASS -e APP_VERSION -e PKG_NAME -e APP_NAME -e APP_ICON -e VERSION_SUFFIX_STR \
  opengisch/qfield-sdk:${QFIELD_SDK_VERSION} /usr/src/qfield/scripts/docker-build.sh
