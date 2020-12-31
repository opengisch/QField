#!/usr/bin/env bash


CURRENT_COMMIT=$(git rev-parse --short HEAD)

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh


if [[ -n ${CI_TAG} ]]; then
  echo "Building release from tag"
  APP_VERSION_NAME=$(cat ${DIR}/../../RELEASE_NAME)
  # v1.2.3 Release Name -> 1.2.3 Release Name 
  # v1.2.3-rc4 Release Name -> 1.2.3 RC4 Release Name
  APP_VERSION_STR="$(app_version_str ${CI_TAG}) - ${APP_VERSION_NAME}"
  # v1.2.3-rc4 arm7 -> 0102030400
  APK_VERSION_CODE=$(apk_version_code "${CI_TAG}" "${ARCH}")
  # ^-- SC2155: Declare and assign separately to avoid masking return values.

  export APP_NAME="QField"
  export APP_PACKAGE_NAME="qfield"
  export APP_ICON="qfield_logo"
  export APP_VERSION="${CI_TAG}" # v1.2.3 or v1.2.3-rc4
  export APP_VERSION_STR
  export APK_VERSION_CODE
elif [[ ${CI_PULL_REQUEST} = false ]]; then
  echo "Building dev (nightly)"
  ARCH_NUMBER=$(arch_to_build_number ${ARCH})
  # get numbers of masters commits
  NUMBER_OF_COMMITS=$(curl -I -k "https://api.github.com/repos/${CI_REPO_SLUG}/commits?per_page=1&sha=${CURRENT_COMMIT}" | sed -n '/^[Ll]ink:/ s/.*"next".*page=\([0-9]*\).*"last".*/\1/p')
  CUSTOM_APP_PACKAGE_NAME=$(echo ${NIGHTLY_PACKAGE_NAME} | awk '{print $NF}' FS=.)

  export APP_NAME="${CUSTOM_APP_NAME:-QField Dev}"
  export APP_PACKAGE_NAME="${CUSTOM_APP_PACKAGE_NAME:-qfield_dev}"
  export APP_ICON="qfield_logo_beta"
  export APP_VERSION=""
  # take 0 + (1930000 + number of masters commits) + arch
  # 01930000 has no meaning - it's just where we had to start
  # max = 2100000000
  export APP_VERSION_STR="${CI_BRANCH}-dev"
  if [[ -n $CUSTOM_APP_PACKAGE_NAME ]]; then
    export APK_VERSION_CODE="${GITHUB_RUN_NUMBER}${ARCH_NUMBER}"
  else
    export APK_VERSION_CODE=0$((1930000+NUMBER_OF_COMMITS))${ARCH_NUMBER}
  fi

else
  echo "Building pull request beta"
  export APP_NAME="QField Beta ${CI_PULL_REQUEST_NUMBER}"
  export APP_PACKAGE_NAME="qfield_beta"
  export APP_ICON="qfield_logo_pr"
  export APP_VERSION=""
  export APP_VERSION_STR="PR${CI_PULL_REQUEST_NUMBER}"
  export APK_VERSION_CODE="1"
fi

echo "Arch number: ${ARCH_NUMBER}"
echo "APP_NAME: ${APP_NAME}"
echo "APP_PACKAGE_NAME: ${APP_PACKAGE_NAME}"
echo "APP_ICON: ${APP_ICON}"
echo "APP_VERSION: ${APP_VERSION}"
echo "APP_VERSION_STR: ${APP_VERSION_STR}"
echo "ANDROID_NDK_PLATFORM : ${ANDROID_NDK_PLATFORM}"
echo "APK_VERSION_CODE: ${APK_VERSION_CODE}"

docker run -v $(pwd):/usr/src/qfield \
  -e "BUILD_FOLDER=build-${ARCH}" \
  -e ARCH -e STOREPASS -e KEYNAME -e KEYPASS -e APP_PACKAGE_NAME -e APP_NAME -e APP_ICON -e APP_VERSION -e APP_VERSION_STR -e APK_VERSION_CODE  \
  -e ANDROID_NDK_PLATFORM \
  opengisch/qfield-sdk:${QFIELD_SDK_VERSION} /usr/src/qfield/scripts/docker-build.sh
