#!/bin/bash

set -e

if [ "${TRAVIS_SECURE_ENV_VARS}" = "true" -a "${TRAVIS_PULL_REQUEST}" != "false" ];
then
  echo -e "\e[31mDo the release\e[0m"
  sudo mv build-armv7/out/build/outputs/apk/out-release-signed.apk build-docker/out/build/outputs/apk/qfield-dev-${TRAVIS_PULL_REQUEST}-${TRAVIS_COMMIT}-armv7.apk
  sudo mv build-x86/out/build/outputs/apk/out-release-signed.apk build-docker/out/build/outputs/apk/qfield-dev-${TRAVIS_PULL_REQUEST}-${TRAVIS_COMMIT}-x86.apk
  openssl aes-256-cbc -K $encrypted_dev_upload_key -iv $encrypted_dev_upload_iv -in .ci/id_rsa.enc -out .ci/id_rsa -d
  chmod 400 .ci/id_rsa
  scp -i .ci/id_rsa -o "StrictHostKeyChecking no" build-armv7/out/build/outputs/apk/qfield-dev-${TRAVIS_PULL_REQUEST}-${TRAVIS_COMMIT}.apk ${DEV_UPLOAD_USER}@${DEV_UPLOAD_HOST}:~/ci-builds/
  scp -i .ci/id_rsa -o "StrictHostKeyChecking no" build-x86/out/build/outputs/apk/qfield-dev-${TRAVIS_PULL_REQUEST}-${TRAVIS_COMMIT}.apk ${DEV_UPLOAD_USER}@${DEV_UPLOAD_HOST}:~/ci-builds/
  echo -e "\e[31Uploaded to https://opengis.ch/download/qfield/ci-builds/qfield-dev-${TRAVIS_PULL_REQUEST}-${TRAVIS_COMMIT}-armv7.apk \e[0m"
  curl -u m-kuhn:${GITHUB_API_TOKEN} -X POST --data '{"body": "Uploaded test apks for [armv7](https://opengis.ch/download/qfield/ci-builds/qfield-dev-'${TRAVIS_PULL_REQUEST}'-'${TRAVIS_COMMIT}'-armv7.apk) and [x86](https://opengis.ch/download/qfield/ci-builds/qfield-dev-'${TRAVIS_PULL_REQUEST}'-'${TRAVIS_COMMIT}'-x86.apk)"}' https://api.github.com/repos/opengisch/QField/issues/${TRAVIS_PULL_REQUEST}/comments
else
  echo -e "Skipping dev build upload because"
  if [ "${TRAVIS_SECURE_ENV_VARS}" != "true" ];
  then
    echo -e "  TRAVIS_SECURE_ENV_VARS is not true (${TRAVIS_SECURE_ENV_VARS})"
  fi
  if [ "${TRAVIS_SECURE_ENV_VARS}" = "false" ];
  then
    echo -e "  TRAVIS_PULL_REQUEST is false (${TRAVIS_PULL_REQUEST})"
  fi
fi
