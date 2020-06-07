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

if [[ -z $APP_VERSION ]] && [[ -z $APP_VERSION_CODE ]]; then
  echo "*** ERROR MISSING APP_VERSION OR APP_VERSION_CODE ENVIRONMENT VARIABLE"
  exit 2
fi

apt update && apt install -y zip bc cmake ninja-build

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
QT_ANDROID=${QT_ANDROID_BASE}/android

echo "Package name ${PKG_NAME}"

if [[ -n ${APP_ICON} ]]; then
  # replace icon
  echo "Replacing icon with ${APP_ICON}"
  sed -i "s|<file alias=\"qfield_logo.svg\">icons/qfield_logo.svg</file>|<file alias=\"qfield_logo.svg\">icons/${APP_ICON}.svg</file>|" ${SOURCE_DIR}/images/images.qrc
  sed -i "s|@drawable/qfield_logo|@drawable/${APP_ICON}|g" ${SOURCE_DIR}/android/res/layout/unpacking_dialog.xml
  sed -i "s|@drawable/qfield_logo|@drawable/${APP_ICON}|g" ${SOURCE_DIR}/android/AndroidManifest.xml
  sed -i "s|@drawable/qfield_logo|@drawable/${APP_ICON}|g" ${SOURCE_DIR}/android/src/ch/opengis/qfield/QFieldProjectActivity.java
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

if [[ $( echo "${APP_VERSION_CODE} > 020000000" | bc ) == 1 ]]; then
  echo "*** ERROR TOO BIG VERSION CODE"
  echo "Remove this check if QField is now 2.x"
  exit 1
fi

mkdir -p ${BUILD_DIR}/.gradle
# androiddeployqt needs gradle and downloads it to /root/.gradle. By linking it to the build folder, this will be cached between builds.
ln -sfn ${BUILD_DIR}/.gradle /root/.gradle

pushd ${BUILD_DIR}

export ANDROIDNDK=/opt/android-ndk
export ANDROIDAPI=21
if [ "X${ARCH}" == "Xx86" ]; then
    export ANDROID_ARCH=x86
elif [ "X${ARCH}" == "Xarmv7" ]; then
    export ANDROID_ARCH=armeabi-v7a
elif [ "X${ARCH}" == "Xarm64" ]; then
    export ANDROID_ARCH=arm64-v8a # watch out when changing this, openssl depends on it
elif [ "X${ARCH}" == "Xx86_64" ]; then
    export ANDROID_ARCH=x86_64 # watch out when changing this, openssl depends on it
else
    echo "Error: Please report issue to enable support for arch (${ARCH})."
    exit 1
fi
export STAGE_PATH=/home/osgeo4a/${ANDROIC_ARCH}

export ANDROID_CMAKE_LINKER_FLAGS=""
if [ "X${ANDROID_ARCH}" == "Xarm64-v8a" ] || [ "X${ANDROID_ARCH}" == "Xx86_64" ]; then
  ANDROID_CMAKE_LINKER_FLAGS="$ANDROID_CMAKE_LINKER_FLAGS;-Wl,-rpath=$STAGE_PATH/lib"
  ANDROID_CMAKE_LINKER_FLAGS="$ANDROID_CMAKE_LINKER_FLAGS;-Wl,-rpath=$QT_ANDROID/lib"
  ANDROID_CMAKE_LINKER_FLAGS="$ANDROID_CMAKE_LINKER_FLAGS;-Wl,-rpath=$ANDROIDNDK/platforms/android-$ANDROIDAPI/arch-$ARCH/usr/lib"
#  ANDROID_CMAKE_LINKER_FLAGS="$ANDROID_CMAKE_LINKER_FLAGS;-Wl,-rpath=$ANDROIDNDK/sources/cxx-stl/llvm-libc++/libs/$ANDROID_ARCH"
  export LDFLAGS="-Wl,-rpath=$STAGE_PATH/lib $LDFLAGS"
fi
ANDROID_CMAKE_LINKER_FLAGS="$ANDROID_CMAKE_LINKER_FLAGS -landroid -llog"

cmake \
	-G Ninja \
	-DCMAKE_TOOLCHAIN_FILE=/opt/android-ndk/build/cmake/android.toolchain.cmake \
	-DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
	-DQt5_DIR:PATH=/opt/Qt/5.14.2/android/lib/cmake/Qt5 \
	-DANDROID_DEPLOY_QT=/opt/Qt/5.14.2/android/bin/androiddeployqt \
	-DCMAKE_FIND_ROOT_PATH:PATH=/opt/android-ndk\;/opt/Qt/5.14.2/android/\;/home/osgeo4a/${ANDROID_ARCH} \
	-DANDROID_LINKER_FLAGS="${ANDROID_CMAKE_LINKER_FLAGS}" \
	-DANDROID_ABI=${ANDROID_ARCH} \
	-DANDROID_BUILD_ABI_${ANDROID_ARCH}=ON \
	-DQGIS_CORE_LIBRARY=/home/osgeo4a/${ANDROID_ARCH}/lib/libqgis_core_${ANDROID_ARCH}.so \
	-DQGIS_ANALYSIS_LIBRARY=/home/osgeo4a/${ANDROID_ARCH}/lib/libqgis_analysis_${ANDROID_ARCH}.so \
	-DQGIS_INCLUDE_DIR=/home/osgeo4a/${ANDROID_ARCH}/include/qgis/ \
	-DSQLITE3_INCLUDE_DIR:PATH=/home/osgeo4a/${ANDROID_ARCH}/include/ \
	-DOSGEO4A_STAGE_DIR:PATH=/home/osgeo4a/ \
	-DANDROID_SDK=/opt/android-sdk/ \
	-DANDROID_NDK=/opt/android-ndk/ \
	-DANDROID_STL:STRING=c++_shared \
	ANDROID_NATIVE_API_LEVEL=21 \
	..

ninja

if [ -n "${KEYNAME}" ]; then
    ${QT_ANDROID}/bin/androiddeployqt \
      --sign ${SOURCE_DIR}/keystore.p12 "${KEYNAME}" \
      --storepass "${STOREPASS}" \
      --keypass "${KEYPASS}" \
      --input ${BUILD_DIR}/android_deployment_settings.json \
      --output ${INSTALL_DIR} \
      --deployment bundled \
      --android-platform ${ANDROID_NDK_PLATFORM} \
      --gradle
else
    ${QT_ANDROID}/bin/androiddeployqt \
      --input ${BUILD_DIR}/android_deployment_settings.json \
      --output ${BUILD_DIR}/android-build \
      --deployment bundled \
      --android-platform ${ANDROID_NDK_PLATFORM} \
      --gradle
fi
chown -R $(stat -c "%u" .):$(stat -c "%u" .) .
popd
