#!/bin/bash

# This is intended to be run within a opengisch/qfield-sdk docker container.
#
#     docker run -v $(pwd):/usr/src/qfield opengisch/qfield-sdk /usr/src/qfield/scripts/docker-build.sh
#
# The result will be put into `build-docker/out/build/outputs/apk/out-debug.apk`
#
#
# ANDROID_NDK_PLATFORM and QT_VERSION are defined in docker-qt-crystax

RUN useradd -r -u 2000 -g docker docker
USER docker

echo 'dockout stat -c %u: '$(stat -c "%u" .)
echo 'dockout stat -c %g: '$(stat -c "%g" .)
echo 'dockout stat -c %U: '$(stat -c "%U" .)
echo 'dockout stat -c %G: '$(stat -c "%G" .)
echo 'dockout ls -la: '$(ls -la)

SOURCE_DIR=/usr/src/qfield
BUILD_DIR=${SOURCE_DIR}/build-docker
INSTALL_DIR=${SOURCE_DIR}/build-docker/out

set -e

mkdir -p ${BUILD_DIR}/.gradle
# androiddeployqt needs gradle and downloads it to /root/.gradle. By linking it to the build folder, this will be cached between builds.
ln -s ${BUILD_DIR}/.gradle /root/.gradle

pushd ${BUILD_DIR}
cp ${SOURCE_DIR}/scripts/ci/config.pri ${SOURCE_DIR}/config.pri
${QT_ANDROID}/bin/qmake ${SOURCE_DIR}/QField.pro
make
make install INSTALL_ROOT=${INSTALL_DIR}
if [ -n "${KEYNAME}" ]; then
    androiddeployqt \
	    --sign ${SOURCE_DIR}/keystore.p12 "${KEYNAME}" \
	    --storepass "${STOREPASS}" \
	    --keypass "${KEYPASS}" \
	    --input ${BUILD_DIR}/src/android-libqfield.so-deployment-settings.json \
	    --output ${INSTALL_DIR} \
	    --deployment bundled \
	    --android-platform ${ANDROID_NDK_PLATFORM} \
	    --gradle
else
    androiddeployqt \
	    --input ${BUILD_DIR}/src/android-libqfield.so-deployment-settings.json \
	    --output ${INSTALL_DIR} \
	    --deployment bundled \
	    --android-platform ${ANDROID_NDK_PLATFORM} \
	    --gradle
fi
chown -R $(stat -c "%u" .):$(stat -c "%u" .) .
popd
