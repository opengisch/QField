#!/usr/bin/env bash


CURRENT_COMMIT=$(git rev-parse --short HEAD)

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh


if [[ -n ${TRAVIS_TAG} ]]; then
  echo "Building release from tag"
  export APP_NAME="QField"
  export PKG_NAME="qfield"
  export APP_ICON="qfield_logo"
  export APP_VERSION="${TRAVIS_TAG}" #  (v1.2.3 or v1.2.3-rc4)

  # adding the code name to the version string
  CURRENT_CODENAME=$(cat ${DIR}/../../RELEASE_NAME)
  export APP_VERSION_STR="$( app_version_str ${TRAVIS_TAG} ) - ${CURRENT_CODENAME}"

elif [[ ${TRAVIS_PULL_REQUEST} = false ]]; then
  ARCH_NUMBER=$(arch_to_build_number ${ARCH})
  # DATE as YYYY DDD HH MM (without spaces)
  CUR_DATE=$(date +%Y%j%H%M)
  echo "Building dev (nightly)"
  export APP_NAME="QField Dev"
  export PKG_NAME="qfield_dev"
  export APP_ICON="qfield_logo_beta"
  export APP_VERSION=""
  # remove first 3 digits of the year (so YDDDHHMM) + arch
  #        YDDDHHMMA
  # max = 2100000000
  export APP_VERSION_CODE=${CUR_DATE:3:8}${ARCH_NUMBER}
  export APP_VERSION_STR="${LAST_TAG}-dev (commit ${CURRENT_COMMIT})"

else
  echo "Building pull request beta"
  export APP_NAME="QField Beta ${TRAVIS_PULL_REQUEST}"
  export PKG_NAME="qfield_beta"
  export APP_ICON="qfield_logo_pr"
  export APP_VERSION=""
  export APP_VERSION_CODE="1"
  export APP_VERSION_STR="PR${TRAVIS_PULL_REQUEST}"
fi

echo "Arch number: ${ARCH_NUMBER}"
echo "APP_VERSION: ${APP_VERSION}"
echo "APP_VERSION_CODE: ${APP_VERSION_CODE}"
echo "APP_VERSION_STR: ${APP_VERSION_STR}"


docker run -v $(pwd):/usr/src/qfield \
  -e "BUILD_FOLDER=build-${ARCH}" \
  -e ARCH -e STOREPASS -e KEYNAME -e KEYPASS -e PKG_NAME -e APP_NAME -e APP_ICON -e APP_VERSION -e APP_VERSION_CODE -e APP_VERSION_STR \
  opengisch/qfield-sdk:${QFIELD_SDK_VERSION} /usr/src/qfield/scripts/docker-build.sh
