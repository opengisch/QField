#!/bin/bash

# This is intended to be run within a opengisch/qfield-sdk docker container.
#
#     docker run -v $(pwd):/usr/src/qfield opengisch/qfield-sdk /usr/src/qfield/scripts/docker-build.sh
#
# The result will be put into `build-docker/out/build/outputs/apk/out-debug.apk`
#
#
# ANDROID_NDK_PLATFORM and QT_VERSION are defined in docker-qt-crystax

SOURCE_DIR=/usr/src/qfield
if [[ -z ${BUILD_FOLDER+x} ]]; then
    BUILD_DIR=${SOURCE_DIR}/build-docker
else
    BUILD_DIR=${SOURCE_DIR}/${BUILD_FOLDER}
fi
if [[ -z ${ARCH+x} ]]; then
    ARCH=armv7
fi
INSTALL_DIR=${BUILD_DIR}/out
QT_ANDROID=${QT_ANDROID_BASE}/android_${ARCH}
export QMAKESPEC=android-clang

set -e

# Replace the version number in version.pri with the one from the VERSION which is being built
if [[ -n ${VERSION} ]];
then
  echo "Building release version ${VERSION}"
  sed -i "s/^VERSION_MAJOR\s*= .*/VERSION_MAJOR = $(echo "${VERSION}" | cut -f 2 -d 'v' | cut -f 1 -d '.')/g" ${SOURCE_DIR}/version.pri
  sed -i "s/^VERSION_MINOR\s*= .*/VERSION_MINOR = $(echo "${VERSION}" | cut -f 2 -d '.')/g" ${SOURCE_DIR}/version.pri
  sed -i "s/^VERSION_FIX\s*= .*/VERSION_FIX = $(echo "${VERSION}" | cut -f 3 -d '.' | cut -f 1 -d '-')/g" ${SOURCE_DIR}/version.pri

  export RC_SUFFIX=$(echo "${VERSION}" | cut -f 2 -d 'c' -s)
  sed -i "s/^VERSION_RC\s*= .*/VERSION_RC = ${RC_SUFFIX:-99}/g" ${SOURCE_DIR}/version.pri
  sed -i "s/^VERSION_SUFFIX\s*= .*/VERSION_SUFFIX = '${RC_SUFFIX:+-rc}$RC_SUFFIX'/g" ${SOURCE_DIR}/version.pri
fi

mkdir -p ${BUILD_DIR}/.gradle
# androiddeployqt needs gradle and downloads it to /root/.gradle. By linking it to the build folder, this will be cached between builds.
ln -s ${BUILD_DIR}/.gradle /root/.gradle

pushd ${BUILD_DIR}
cp ${SOURCE_DIR}/scripts/ci/config.pri ${SOURCE_DIR}/config.pri
${QT_ANDROID}/bin/qmake ${SOURCE_DIR}/QField.pro
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
