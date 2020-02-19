#!/bin/bash

# This is intended to be run within a opengisch/qfield-sdk docker container.
#
#     docker run -v $(pwd):/usr/src/qfield opengisch/qfield-sdk /usr/src/qfield/scripts/docker-build.sh
#
# The result will be put into `build-docker/out/build/outputs/apk/out-debug.apk`
#
#
# ANDROID_NDK_PLATFORM and QT_VERSION are defined in docker-qt-crystax
#
# You can either provide the version code directly with APP_VERSION_CODE (MMmmFFNNA: major,minor,fix,number,architecture_index)
# or you can provide the APP_VERSION (v1.2.3 or v1.2.3-rc4) and the APP_VERSION_CODE will be calculated
# The APP_VERSION_STR shall be provided in both case

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/version_number.sh

set -e

apt update && apt install -y zip bc

SOURCE_DIR=/usr/src/qfield
if [[ -z ${BUILD_FOLDER+x} ]]; then
    BUILD_DIR=${SOURCE_DIR}/build-docker
else
    BUILD_DIR=${SOURCE_DIR}/${BUILD_FOLDER}
fi

# Set default values if missing
[[ -z ${ARCH} ]] && ARCH=armv7
[[ -z ${APP_NAME} ]] && APP_NAME="QField"
[[ -z ${PKG_NAME} ]] && PKG_NAME="qfield"

INSTALL_DIR=${BUILD_DIR}/out
QT_ANDROID=${QT_ANDROID_BASE}/android_${ARCH}

echo "Package name ${PKG_NAME}"

if [[ -z ${APP_ICON} ]]; then
  # replace icon
  echo "Replacing icon with ${APP_ICON}"
  sed -i "s|<file alias=\"qfield_logo.svg\">icons/qfield_logo.svg</file>|<file alias=\"qfield_logo.svg\">icons/${APP_ICON}.svg</file>|" ${SOURCE_DIR}/images/images.qrc
  sed -i "s|@drawable/qfield_logo|@drawable/${APP_ICON}|g" ${SOURCE_DIR}/android/res/layout/unpacking_dialog.xml
  sed -i "s|@drawable/qfield_logo|@drawable/${APP_ICON}|g" ${SOURCE_DIR}/android/AndroidManifest.xml
fi
if [[ "X${PKG_NAME}" != "Xqfield" ]]; then
  grep "ch.opengis.qfield" -l -r ${SOURCE_DIR}/android/ | xargs sed -i "s/ch.opengis.qfield/ch.opengis.${PKG_NAME}/g"
  grep "ch.opengis.qfield" -l -r ${SOURCE_DIR}/src/ | xargs sed -i "s/ch.opengis.qfield/ch.opengis.${PKG_NAME}/g"
  mv ${SOURCE_DIR}/android/src/ch/opengis/qfield ${SOURCE_DIR}/android/src/ch/opengis/${PKG_NAME}
  sed -i "s|<string name=\"app_name\" translatable=\"false\">QField</string>|<string name=\"app_name\" translatable=\"false\">${APP_NAME}</string>|" ${SOURCE_DIR}/android/res/values/strings.xml
fi

# Replace the version number in version.pri with the one from the APP_VERSION which is being built (e.g. v1.2.3-rc4, v1.2.3)
if [[ -n ${APP_VERSION} ]];
then
  echo "Building release version APP_VERSION: ${APP_VERSION}"
  APP_VERSION_CODE=$(app_version_code "${APP_VERSION}" "${ARCH}")
  echo "Generated version code APP_VERSION_CODE: ${APP_VERSION_CODE}"
fi
sed -i "s/^VERSIONCODE\s*= .*/VERSIONCODE = ${APP_VERSION_CODE}/" ${SOURCE_DIR}/version.pri
sed -i "s/^VERSTR\s*= .*/VERSTR = '${APP_VERSION_STR:-${APP_VERSION_CODE}}'/" ${SOURCE_DIR}/version.pri
echo "Showing content of version.pri with APP_VERSION_CODE and APP_VERSION_STR:"
echo "$(cat ${SOURCE_DIR}/version.pri | grep -E '^VERS(IONCODE|TR)\s*=')"

if ( [[ $( echo "${APP_VERSION_CODE} > 020000000" | bc ) == 1 ]] ); then
  echo "*** ERROR TOO BIG VERSION CODE"
  echo "Remove this check if QField is now 2.x"
  exit 1
fi

mkdir -p ${BUILD_DIR}/.gradle
# androiddeployqt needs gradle and downloads it to /root/.gradle. By linking it to the build folder, this will be cached between builds.
ln -sfn ${BUILD_DIR}/.gradle /root/.gradle

pushd ${BUILD_DIR}
cp ${SOURCE_DIR}/scripts/ci/config.pri ${SOURCE_DIR}/config.pri
${QT_ANDROID}/bin/qmake ${SOURCE_DIR}/QField.pro "APP_NAME=${APP_NAME}"
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
