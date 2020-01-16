#!/usr/bin/env bash


CURRENT_CODENAME=$(cat /usr/src/qfield/RELEASE_NAME)

CURRENT_COMMIT=$(git rev-parse --short HEAD)
LAST_TAG=$(git describe --abbrev=0 --tags)
COMMIT_COUNT_SINCE_LAST_TAG=$(git log --oneline ${LAST_TAG}...HEAD | wc -l | bc)
COMMIT_COUNT_SINCE_v140=$(git log --oneline v1.4.0...HEAD | wc -l | bc)


if [ -n "${TRAVIS_TAG}" ]; then
  echo "Building release"
  export APP_NAME="QField"
  export PKG_NAME="qfield"
  export APP_ICON="qfield-logo.svg"
  export APP_VERSION="${TRAVIS_TAG}" #  (v1.2.3 or v1.2.3-rc4)

elif [ "${TRAVIS_PULL_REQUEST}" = "false" ]; then
  echo "Building dev (nightly)"
  export APP_NAME="QField Dev"
  export PKG_NAME="qfield_dev"
  export APP_ICON="qfield-testlogo.svg"
  export APP_VERSION=""
  export APP_VERSION_CODE=${COMMIT_COUNT_SINCE_v140}
  export APP_VERSION_STR="${LAST_TAG}-dev${COMMIT_COUNT} (${CURRENT_COMMIT})"

else
  echo "Building pull request beta"
  export APP_NAME="QField Beta ${TRAVIS_PULL_REQUEST}"
  export PKG_NAME="qfield_beta"
  export APP_ICON="qfield-testlogo.svg"
  export APP_VERSION=""
  export APP_VERSION_CODE="1"
  export APP_VERSION_STR="PR${TRAVIS_PULL_REQUEST}"
fi
docker run -v $(pwd):/usr/src/qfield \
  -e "BUILD_FOLDER=build-${ARCH}" \
  -e ARCH -e STOREPASS -e KEYNAME -e KEYPASS -e PKG_NAME -e APP_NAME -e APP_ICON -e APP_VERSION -e APP_VERSION_CODE -e APP_VERSION_STR \
  opengisch/qfield-sdk:${QFIELD_SDK_VERSION} /usr/src/qfield/scripts/docker-build.sh
