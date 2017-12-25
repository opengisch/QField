#!/bin/bash

#
# ANDROID_NDK_PLATFORM and QT_VERSION are defined in docker-qt-crystax

mkdir -p /home/build
pushd /home/build
cp /usr/src/qfield/docker/config.pri /usr/src/qfield/config.pri
${QT_ANDROID}/bin/qmake /usr/src/qfield/QField.pro
make
make install INSTALL_ROOT=/home/install
androiddeployqt --input /home/build/src/android-libqfield.so-deployment-settings.json --output /home/install --deployment bundled --android-platform ${ANDROID_NDK_PLATFORM} --gradle
popd
