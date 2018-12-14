#!/bin/bash


source scripts/travis_env.sh

set -e

# If we have secure env vars and are in either a pull request or a tag, we need to upload artifacts
if [[ "${TRAVIS_SECURE_ENV_VARS}" = "true" ]];
then
  echo -e "\e[31mAbout to upload build artifacts\e[0m"
  sudo mv build-${ARCH}/out/build/outputs/apk/release/out-release-signed.apk /tmp/qfield-dev-${UPLOAD_ARTIFACT_ID}-${TRAVIS_COMMIT}-${ARCH}.apk
  openssl aes-256-cbc -K $encrypted_dev_upload_key -iv $encrypted_dev_upload_iv -in .ci/id_rsa.enc -out .ci/id_rsa -d
  chmod 400 .ci/id_rsa
  scp -i .ci/id_rsa -o "StrictHostKeyChecking no" /tmp/qfield-dev-${UPLOAD_ARTIFACT_ID}-${TRAVIS_COMMIT}-${ARCH}.apk ${DEV_UPLOAD_USER}@${DEV_UPLOAD_HOST}:~/download.opengis.ch/qfield/ci-builds
  echo -e "\e[31Uploaded to https://download.opengis.ch/qfield/ci-builds/qfield-dev-${UPLOAD_ARTIFACT_ID}-${TRAVIS_COMMIT}-${ARCH}.apk \e[0m"
else
  echo -e "Not uploading artifacts ..."
  if [ "${TRAVIS_SECURE_ENV_VARS}" != "true" ];
  then
    echo -e "  TRAVIS_SECURE_ENV_VARS is not true (${TRAVIS_SECURE_ENV_VARS})"
  fi
  if [ "${TRAVIS_SECURE_ENV_VARS}" = "false" ];
  then
    echo -e "  TRAVIS_PULL_REQUEST is false (${TRAVIS_PULL_REQUEST})"
  fi
  if [ "${TRAVIS_TAG}X" = "X" ];
  then
    echo -e "  TRAVIS_TAG is not set"
  fi
fi
