#!/usr/bin/env bash

set -e


CURRENT_COMMIT=$(git rev-parse --short HEAD)
echo "CURRENT_COMMIT: ${CURRENT_COMMIT}"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

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
  CUSTOM_APP_PACKAGE_NAME=$(echo ${NIGHTLY_PACKAGE_NAME} | awk '{print $NF}' FS=.)

  export APP_NAME="${CUSTOM_APP_NAME:-QField Dev}"
  export APP_PACKAGE_NAME="${CUSTOM_APP_PACKAGE_NAME:-qfield_dev}"
  export APP_ICON="qfield_logo_beta"
  export APP_VERSION=""
  # take 0 + (1930000 + number of CI runs) + arch
  # 01930000 has no meaning - it's just where we had to start
  # max = 2100000000
  export APP_VERSION_STR="${CI_BRANCH}-dev"
  if [[ -n ${CUSTOM_APP_PACKAGE_NAME} ]]; then
    export APK_VERSION_CODE="${CI_RUN_NUMBER}${ARCH_NUMBER}"
  else
    export APK_VERSION_CODE=0$((1934000+CI_RUN_NUMBER))${ARCH_NUMBER}
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


# safe guard to avoid to big number
if [[ ( "${APK_VERSION_CODE}" -gt 2000000000 )  ]] ; then
  echo "APK_VERSION_CODE is getting too big!"
  exit 1
fi
