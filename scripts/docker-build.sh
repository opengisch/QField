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

SOURCE_DIR=/usr/src/qfield
if [[ -z ${BUILD_FOLDER+x} ]]; then
    BUILD_DIR=${SOURCE_DIR}/build-docker
else
    BUILD_DIR=${SOURCE_DIR}/${BUILD_FOLDER}
fi
if [[ -z ${ARCH+x} ]]; then
    ARCH=armv7
fi
if [[ -z ${TRAVIS_PULL_REQUEST} ]]; then
  TRAVIS_PULL_REQUEST=false
fi
INSTALL_DIR=${BUILD_DIR}/out
QT_ANDROID=${QT_ANDROID_BASE}/android_${ARCH}


# Replace app name if we are on a pull request
if [[ ${TRAVIS_PULL_REQUEST} != false ]]; then
  APP_NAME_PKG=qfield_beta_${TRAVIS_PULL_REQUEST}
  APP_NAME_STR="QField Beta ${TRAVIS_PULL_REQUEST}"
  sed -i 's@package=\"ch.opengis.qfield\"@package=\"ch.opengis.${APP_NAME_PKG}@\"' ${SOURCE_DIR}/android/AndroidManifest.xml
  sed -i 's@<string name=\"app_name\" translatable=\"false\">QField</string>@<string name=\"app_name\" translatable=\"false\">${APP_NAME_STR}</string>@' android/res/values/strings.xml
  sed -i 's@<string name=\"lib_name\" translatable=\"false\">qfield</string>@<string name=\"app_name\" translatable=\"false\">${APP_NAME_PKG}</string>@' android/res/values/strings.xml
fi


# Replace the version number in version.pri with the one from the VERSION which is being built
if [[ -n ${VERSION} ]];
then
  echo "Building release version ${VERSION}"
  sed -i "s/VERSION_MAJOR\s*= .*/VERSION_MAJOR = $(echo "${VERSION}" | cut -f 2 -d '-' | cut -f 1 -d '.')/g" ${SOURCE_DIR}/version.pri
  sed -i "s/VERSION_MINOR\s*= .*/VERSION_MINOR = $(echo "${VERSION}" | cut -f 2 -d '.')/g" ${SOURCE_DIR}/version.pri
  sed -i "s/VERSION_FIX\s*= .*/VERSION_FIX = $(echo "${VERSION}" | cut -f 3 -d '.')/g" ${SOURCE_DIR}/version.pri
  grep 'VERSION_MAJOR' ${SOURCE_DIR}/version.pri
  grep 'VERSION_MINOR' ${SOURCE_DIR}/version.pri
  grep 'VERSION_FIX' ${SOURCE_DIR}/version.pri
fi

mkdir -p ${BUILD_DIR}/.gradle
# androiddeployqt needs gradle and downloads it to /root/.gradle. By linking it to the build folder, this will be cached between builds.
ln -s ${BUILD_DIR}/.gradle /root/.gradle

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
