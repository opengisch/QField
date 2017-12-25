#!/bin/bash

# This is intended to be run within q field-sdk docker container.
#
#     docker run -v $(pwd):/usr/src/qfield opengisch/qfield-sdk /usr/src/qfield/scripts/docker-build.sh
#
# The result will be put into `build-docker/out/build/outputs/apk/out-debug.apk`
#
#
# ANDROID_NDK_PLATFORM and QT_VERSION are defined in docker-qt-crystax

SOURCE_DIR=/usr/src/qfield
BUILD_DIR=${SOURCE_DIR}/build-docker
INSTALL_DIR=${SOURCE_DIR}/build-docker/out

mkdir ${BUILD_DIR}
pushd ${BUILD_DIR}
cp ${SOURCE_DIR}/scripts/ci/config.pri ${SOURCE_DIR}/config.pri
${QT_ANDROID}/bin/qmake ${SOURCE_DIR}/QField.pro
make
make install INSTALL_ROOT=${INSTALL_DIR}
androiddeployqt --input ${BUILD_DIR}/src/android-libqfield.so-deployment-settings.json --output ${INSTALL_DIR} --deployment bundled --android-platform ${ANDROID_NDK_PLATFORM} --gradle
popd
