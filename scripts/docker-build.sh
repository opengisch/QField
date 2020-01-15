#!/bin/bash

# This is intended to be run within a opengisch/qfield-sdk docker container.
#
#     docker run -v $(pwd):/usr/src/qfield opengisch/qfield-sdk /usr/src/qfield/scripts/docker-build.sh
#
# The result will be put into `build-docker/out/build/outputs/apk/out-debug.apk`
#
#
# ANDROID_NDK_PLATFORM and QT_VERSION are defined in docker-qt-crystax

set -e

apt update && apt install zip

SOURCE_DIR=/usr/src/qfield
if [[ -z ${BUILD_FOLDER+x} ]]; then
    BUILD_DIR=${SOURCE_DIR}/build-docker
else
    BUILD_DIR=${SOURCE_DIR}/${BUILD_FOLDER}
fi
if [[ -z ${ARCH+x} ]]; then
    ARCH=armv7
fi
if [[ -z ${APP_NAME+x} ]]; then
  APP_NAME="QField"
fi
if [[ -z ${PKG_NAME+x} ]]; then
  PKG_NAME="qfield"
fi

INSTALL_DIR=${BUILD_DIR}/out
QT_ANDROID=${QT_ANDROID_BASE}/android_${ARCH}

echo "Package name ${PKG_NAME}"

if [[ -z ${APP_ICON+x} ]]; then
  sed -i "s|<file alias=\"qfield-logo.svg\">icons/qfield-logo.svg</file>|<file alias=\"qfield-logo.svg\">icons/${APP_ICON}</file>|" ${SOURCE_DIR}/images/images.qrc
fi
if [[ "X${PKG_NAME}" != "Xqfield" ]]; then
  grep "ch.opengis.qfield" -l -r ${SOURCE_DIR}/android/ | xargs sed -i "s/ch.opengis.qfield/ch.opengis.${PKG_NAME}/g"
  grep "ch.opengis.qfield" -l -r ${SOURCE_DIR}/src/ | xargs sed -i "s/ch.opengis.qfield/ch.opengis.${PKG_NAME}/g"
  mv ${SOURCE_DIR}/android/src/ch/opengis/qfield ${SOURCE_DIR}/android/src/ch/opengis/${PKG_NAME}
  sed -i "s|<string name=\"app_name\" translatable=\"false\">QField</string>|<string name=\"app_name\" translatable=\"false\">${APP_NAME}</string>|" ${SOURCE_DIR}/android/res/values/strings.xml
fi

# Replace the version number in version.pri with the one from the APP_VERSION which is being built
if [[ -n ${APP_VERSION} ]];
then
  echo "Building release version ${APP_VERSION}"
  sed -i "s/^VERSION_MAJOR\s*= .*/VERSION_MAJOR = $(echo "${APP_VERSION}" | cut -f 2 -d 'v' | cut -f 1 -d '.')/g" ${SOURCE_DIR}/version.pri
  sed -i "s/^VERSION_MINOR\s*= .*/VERSION_MINOR = $(echo "${APP_VERSION}" | cut -f 2 -d '.')/g" ${SOURCE_DIR}/version.pri
  sed -i "s/^VERSION_FIX\s*= .*/VERSION_FIX = $(echo "${APP_VERSION}" | cut -f 3 -d '.' | cut -f 1 -d '-')/g" ${SOURCE_DIR}/version.pri

  # for RC or dev releases, append the version number (up to 3 digits)
  export VERSION_NUMBER=$(echo "${APP_VERSION}" | sed -r -e 's/.*?-(\w+)(\d+)/\2/;t;d') # v.1.2.3-dev456 => 456, v.1.2.3 => EMPTY
  sed -i "s/^VERSION_NUMBER\s*= .*/VERSION_NUMBER = ${VERSION_NUMBER:999}/" ${SOURCE_DIR}/version.pri

  # coming fron env var, might be empty
  sed -i "s/^VERSION_SUFFIX_STR\s*= .*/VERSION_SUFFIX_STR = ${VERSION_SUFFIX_STR}/" ${SOURCE_DIR}/version.pri
fi

mkdir -p ${BUILD_DIR}/.gradle
# androiddeployqt needs gradle and downloads it to /root/.gradle. By linking it to the build folder, this will be cached between builds.
ln -sfn ${BUILD_DIR}/.gradle /root/.gradle

pushd ${BUILD_DIR}
cp ${SOURCE_DIR}/scripts/ci/config.pri ${SOURCE_DIR}/config.pri
${QT_ANDROID}/bin/qmake ${SOURCE_DIR}/QField.pro "APP_NAME=${APP_NAME_PKG}"
make
make install INSTALL_ROOT=${INSTALL_DIR}
if [ -n "${KEYNAME}" ]; then
    ${QT_ANDROID}/bin/androiddeployqt \
	    --sign ${SOURCE_DIR}/keystore.p12 "${KEYNAME}" \
	    --storepass "${STOREPASS}" \
	    --keypass "${KEYPASS}" \
      --input ${BUILD_DIR}/src/app/android-libqfield.so-deployment-settings.json \
	    --output ${INSTALL_DIR} \
	    --deployment bundled \
	    --android-platform ${ANDROID_NDK_PLATFORM} \
	    --gradle
else
    ${QT_ANDROID}/bin/androiddeployqt \
      --input ${BUILD_DIR}/src/app/android-libqfield.so-deployment-settings.json \
	    --output ${INSTALL_DIR} \
	    --deployment bundled \
	    --android-platform ${ANDROID_NDK_PLATFORM} \
	    --gradle
fi
chown -R $(stat -c "%u" .):$(stat -c "%u" .) .
popd
