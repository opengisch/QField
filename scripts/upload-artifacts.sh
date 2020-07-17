#!/bin/bash

set -e

# If we have secure env vars and are in either a pull request or a tag, we need to upload artifacts
if [[ "${CI_SECURE_ENV_VARS}" = "true" ]];
then
  echo -e "\e[31mAbout to upload build artifacts\e[0m"
  sudo mv build-${ARCH}/android-build/build/outputs/apk/release/android-build-release-signed.apk /tmp/qfield-dev-${CI_UPLOAD_ARTIFACT_ID}-${CI_COMMIT}-${ARCH}.apk
  openssl aes-256-cbc -K $encrypted_dev_upload_key -iv $encrypted_dev_upload_iv -in .ci/id_rsa.enc -out .ci/id_rsa -d
  chmod 400 .ci/id_rsa
  scp -i .ci/id_rsa -o "StrictHostKeyChecking no" /tmp/qfield-dev-${CI_UPLOAD_ARTIFACT_ID}-${CI_COMMIT}-${ARCH}.apk ${DEV_UPLOAD_USER}@${DEV_UPLOAD_HOST}:~/download.opengis.ch/qfield/ci-builds
  echo -e "\e[31Uploaded to https://download.opengis.ch/qfield/ci-builds/qfield-dev-${CI_UPLOAD_ARTIFACT_ID}-${CI_COMMIT}-${ARCH}.apk \e[0m"
else
  echo -e "Not uploading artifacts ..."
  if [ "${CI_SECURE_ENV_VARS}" != "true" ];
  then
    echo -e "  CI_SECURE_ENV_VARS is not true (${CI_SECURE_ENV_VARS})"
  fi
  if [ "${CI_SECURE_ENV_VARS}" = "false" ];
  then
    echo -e "  CI_PULL_REQUEST is false (${CI_PULL_REQUEST})"
  fi
  if [ "${CI_TAG}X" = "X" ];
  then
    echo -e "  CI_TAG is not set"
  fi
fi
