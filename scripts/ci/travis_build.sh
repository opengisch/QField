#!/usr/bin/env bash


CURRENT_COMMIT=$(git rev-parse --short HEAD)
LAST_TAG=$(git describe --abbrev=0 --tags)
COMMIT_COUNT_SINCE_LAST_TAG=$(git log --oneline ${LAST_TAG}...HEAD | wc -l | bc)
COMMIT_COUNT_SINCE_v140=$(git log --oneline v1.4.0...HEAD | wc -l | bc)


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh


if [ -n "${TRAVIS_TAG}" ]; then
  echo "Building release"
  export APP_NAME="QField"
  export PKG_NAME="qfield"
  export APP_ICON="qfield-logo.svg"
  export APP_VERSION="${TRAVIS_TAG}" #  (v1.2.3 or v1.2.3-rc4)

  # adding the code name to the version string
  CURRENT_CODENAME=$(cat ${DIR}/../../RELEASE_NAME)
  APP_VERSION_STR="$( app_version_str ${TRAVIS_TAG} ) - ${CURRENT_CODENAME}"

elif [ "${TRAVIS_PULL_REQUEST}" = "false" ]; then
  ARCH_NUMBER=$(arch_to_build_number ${ARCH})
  echo "Building dev (nightly)"
  export APP_NAME="QField Dev"
  export PKG_NAME="qfield_dev"
  export APP_ICON="qfield-testlogo.svg"
  export APP_VERSION=""
  export APP_VERSION_CODE=${COMMIT_COUNT_SINCE_v140}${ARCH_NUMBER}
  export APP_VERSION_STR="${LAST_TAG}-dev${COMMIT_COUNT_SINCE_LAST_TAG} (${CURRENT_COMMIT})"

  echo "Commit number: ${COMMIT_COUNT_SINCE_LAST_TAG}"
  echo "Arch number: ${ARCH_NUMBER}"

else
  echo "Building pull request beta"
  export APP_NAME="QField Beta ${TRAVIS_PULL_REQUEST}"
  export PKG_NAME="qfield_beta"
  export APP_ICON="qfield-testlogo.svg"
  export APP_VERSION=""
  export APP_VERSION_CODE="1"
  export APP_VERSION_STR="PR${TRAVIS_PULL_REQUEST}"
fi

echo "APP_VERSION: ${APP_VERSION}"
echo "APP_VERSION_CODE: ${APP_VERSION_CODE}"
echo "APP_VERSION_STR: ${APP_VERSION_STR}"


docker run -v $(pwd):/usr/src/qfield \
  -e "BUILD_FOLDER=build-${ARCH}" \
  -e ARCH -e STOREPASS -e KEYNAME -e KEYPASS -e PKG_NAME -e APP_NAME -e APP_ICON -e APP_VERSION -e APP_VERSION_CODE -e APP_VERSION_STR \
  opengisch/qfield-sdk:${QFIELD_SDK_VERSION} /usr/src/qfield/scripts/docker-build.sh
