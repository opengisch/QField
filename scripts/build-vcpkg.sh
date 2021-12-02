#!/bin/bash

echo "building for ${triplet}"

cd /usr/src/qfield || exit

CMAKE_BUILD_DIR=/usr/src/qfield/build-${triplet}

export ANDROID_NDK_HOME=/home/devel/android/ndk/21.4.7075529/
export Qt5_DIR=/home/devel/5.14.2/android/
export ANDROID_PLATFORM=21
export ANDROID_TARGET_PLATFORM=30

[[ -z ${APP_NAME} ]] && APP_NAME="QField"
[[ -z ${APP_PACKAGE_NAME} ]] && APP_PACKAGE_NAME="qfield"
[[ -z ${APP_ICON} ]] && APP_ICON="qfield_logo"

echo "Package name ${APP_PACKAGE_NAME}"

# This should be factored out, we also need it in .github/workflows/android.yaml
case ${triplet} in
  arm64-android)
    export ARCH=arm64_v8a
    export ABI=arm64-v8a
    ;;
  arm-android)
    export ARCH=armv7
    export ABI=armeabi-v7a
    ;;
  x64-android)
    export ARCH=x86_64
    export ABI=x86_64
    ;;
  x86-android)
    export ARCH=x86
    export ABI=x86
    ;;
esac

# Configure and install deps
cmake -S "${SOURCE_DIR}" \
      -B "${CMAKE_BUILD_DIR}" \
      -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=/usr/src/qfield/vcpkg/base/scripts/buildsystems/vcpkg.cmake \
      -DVCPKG_OVERLAY_PORTS=/usr/src/qfield/vcpkg/overlay_system_qt\;/usr/src/qfield/vcpkg/overlay \
      -DVCPKG_TARGET_TRIPLET="${triplet}" \
      -DWITH_VCPKG=ON \
      -DWITH_SPIX=OFF \
      -DAPP_VERSION="v1.0.0" \
      -DAPK_VERSION_CODE="${APK_VERSION_CODE}" \
      -DAPP_VERSION_STR="${APP_VERSION_STR}" \
      -DAPP_PACKAGE_NAME="${APP_PACKAGE_NAME}" \
      -DAPP_ICON="${APP_ICON}" \
      -DAPP_NAME="${APP_NAME}" \
      -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake \
      -DANDROID_ABI=${ABI} \
      -DANDROID_PLATFORM=${ANDROID_PLATFORM} \
      -DANDROID_TARGET_PLATFORM=${ANDROID_TARGET_PLATFORM} \
      -DNUGET_TOKEN=${NUGET_TOKEN} \
      -DNUGET_TOKEN=${NUGET_USERNAME}

# Build app
cmake --build "${CMAKE_BUILD_DIR}"

# Package app
mv ${CMAKE_BUILD_DIR}/android_deployment_settings.json ${CMAKE_BUILD_DIR}/android_deployment_settings.tmp
cat <<< "$(jq ". += { \"sdkBuildToolsRevision\" : \"29.0.2\" }" < ${CMAKE_BUILD_DIR}/android_deployment_settings.tmp)" > ${CMAKE_BUILD_DIR}/android_deployment_settings.json
cmake --build  "${CMAKE_BUILD_DIR}" --target bundle --config Release
${ANDROID_SDK_ROOT}/build-tools/29.0.2/apksigner sign --v2-signing-enabled true --ks ./keystore.p12 --ks-pass pass:"${STOREPASS}" --ks-key-alias "qfield" --key-pass pass:"${KEYPASS}" ${CMAKE_BUILD_DIR}/android-build/build/outputs/apk/release/android-build-release-signed.apk
