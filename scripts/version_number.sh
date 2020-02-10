#!/usr/bin/env bash

# GNU prefix command for mac os support (gsed, gsplit)
GP=
if [[ "$OSTYPE" =~ darwin* ]]; then
  GP=g
fi

# give the ARCH name and it returns an integer as build number increment
# arg1: ARCH
arch_to_build_number() {
  ARCH=$1
  if ([[ $ARCH == armv7 ]]); then
    echo 1
  elif ([[ $ARCH == arm64_v8a ]]); then
    echo 2
  elif ([[ $ARCH == x86 ]]); then
    echo 3
  elif ([[ $ARCH == x86_64 ]]); then
    echo 4
  else
    echo 0
  fi
}

# version to version code
# arg1: APP_VERSION (e.g. v1.2.3 or v1.2.3-rc4)
# arg2: ARCH
app_version_code() {
  APP_VERSION=$1
  ARCH=$2
  ARCH_BUILD_NUMBER=$(arch_to_build_number ${ARCH})

  VERSION_MAJOR=$(echo "${APP_VERSION}" | cut -f 2 -dv | cut -f1 -d.)
  VERSION_MINOR=$(echo "${APP_VERSION}" | cut -f 2 -d.)
  VERSION_FIX=$(echo "${APP_VERSION}" | cut -f 3 -d. | cut -f1 -d-)
  VERSION_NUMBER=$(echo "${APP_VERSION}" | ${GP}sed -r -e 's/^.*-rc([0-9]+)/\1/;t;d') # v.1.2.3-rc4 => 4, v1.2.3 => NULL

  APP_VERSION_CODE=$(printf "%02d%02d%02d%02d%01d" ${VERSION_MAJOR} ${VERSION_MINOR} ${VERSION_FIX} ${VERSION_NUMBER:-99} ${ARCH_BUILD_NUMBER})
  echo ${APP_VERSION_CODE}
}

# version to version str
# arg1: APP_VERSION (e.g. v1.2.3 or v1.2.3-rc4)
# returns: 1.2.3 or 1.2.3 RC4
app_version_str() {
  APP_VERSION=$1
  echo ${APP_VERSION} | ${GP}sed -r 's/^v//; s/-(rc[0-9]+)/ \U\1/'
}
