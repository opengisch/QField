#!/usr/bin/env bash


CURRENT_COMMIT=$(git rev-parse --short HEAD)

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh


if [[ -n ${CI_TAG} ]]; then
  echo "Building release from tag"
  export APP_NAME="QField"
  export APP_PACKAGE_NAME="qfield"
  export APP_ICON="qfield_logo"
  export APP_VERSION="${CI_TAG}" #  (v1.2.3 or v1.2.3-rc4)

  # adding the code name to the version string
  CURRENT_CODENAME=$(cat ${DIR}/../../RELEASE_NAME)
  APP_VERSION_STR="$( app_version_str ${CI_TAG} ) - ${CURRENT_CODENAME}"
  export APP_VERSION_STR

elif [[ ${CI_PULL_REQUEST} = false ]]; then
  ARCH_NUMBER=$(arch_to_build_number ${ARCH})
  # get numbers of masters commits
  NUMBER_OF_COMMITS=$(curl -I -k "https://api.github.com/repos/opengisch/QField/commits?per_page=1&sha=${CURRENT_COMMIT}" | sed -n '/^[Ll]ink:/ s/.*"next".*page=\([0-9]*\).*"last".*/\1/p')
  echo "Building dev (nightly)"
  export APP_NAME="QField Dev"
  export APP_PACKAGE_NAME="qfield_dev"
  export APP_ICON="qfield_logo_beta"
  export APP_VERSION=""
  # take 0 + (1930000 + number of masters commits) + arch
  # 01930000 has no meaning - it's just where we had to start
  # max = 2100000000
  export APP_VERSION_CODE=0$((1930000+NUMBER_OF_COMMITS))${ARCH_NUMBER}
  export APP_VERSION_STR="${LAST_TAG}-dev (commit ${CURRENT_COMMIT})"

else
  echo "Building pull request beta"
  export APP_NAME="QField Beta ${CI_PULL_REQUEST}"
  export APP_PACKAGE_NAME="qfield_beta"
  export APP_ICON="qfield_logo_pr"
  export APP_VERSION=""
  export APP_VERSION_CODE="1"
  export APP_VERSION_STR="PR${CI_PULL_REQUEST}"
fi

echo "Arch number: ${ARCH_NUMBER}"
echo "APP_ICON: ${APP_ICON}"
echo "APP_VERSION: ${APP_VERSION}"
echo "APP_VERSION_CODE: ${APP_VERSION_CODE}"
echo "APP_VERSION_STR: ${APP_VERSION_STR}"
echo "ANDROID_NDK_PLATFORM : ${ANDROID_NDK_PLATFORM}"

docker run -v $(pwd):/usr/src/qfield \
  -e "BUILD_FOLDER=build-${ARCH}" \
  -e ARCH -e STOREPASS -e KEYNAME -e KEYPASS -e APP_PACKAGE_NAME -e APP_NAME -e APP_ICON -e APP_VERSION -e APP_VERSION_CODE -e APP_VERSION_STR \
  -e ANDROID_NDK_PLATFORM \
  opengisch/qfield-sdk:${QFIELD_SDK_VERSION} /usr/src/qfield/scripts/docker-build.sh
