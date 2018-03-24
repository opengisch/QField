#!/bin/bash
set -e

source scripts/travis_env.sh

if [ "${TRAVIS_SECURE_ENV_VARS}" = "true" ];
then
  if [ "${TRAVIS_PULL_REQUEST}" != "false" ];
  then
    echo -e "\e[31mDeploying app to pull request\e[0m"
    curl -u m-kuhn:${GITHUB_API_TOKEN} -X POST --data '{"body": "Uploaded test apks for [armv7](https://opengis.ch/download/qfield/ci-builds/qfield-dev-'${UPLOAD_ARTIFACT_ID}'-'${TRAVIS_COMMIT}'-armv7.apk) and [x86](https://opengis.ch/download/qfield/ci-builds/qfield-dev-'${UPLOAD_ARTIFACT_ID}'-'${TRAVIS_COMMIT}'-x86.apk)"}' https://api.github.com/repos/opengisch/QField/issues/${TRAVIS_PULL_REQUEST}/comments
  fi 

  if [ "${TRAVIS_TAG}X" != "X" ];
  then
    echo -e "\e[93;1mStarting to deploy a new release\e[0m"
    openssl aes-256-cbc -K $encrypted_play_upload_key -iv $encrypted_play_upload_iv -in .ci/play_developer.p12.enc -out .ci/play_developer.p12 -d
    echo -e "\e[93m * Collecting apks to upload...\e[0m"
    curl -s -S -o /tmp/qfield-${TRAVIS_TAG}-armv7.apk https://opengis.ch/download/qfield/ci-builds/qfield-dev-${UPLOAD_ARTIFACT_ID}-${TRAVIS_COMMIT}-armv7.apk
    curl -s -S -o /tmp/qfield-${TRAVIS_TAG}-x86.apk https://opengis.ch/download/qfield/ci-builds/qfield-dev-${UPLOAD_ARTIFACT_ID}-${TRAVIS_COMMIT}-x86.apk

#    echo -e "\e[93m * Deploying app to google play...\e[0m"
#    ./scripts/basic_upload_apks_service_account.py ch.opengis.qfield /tmp/qfield-${TRAVIS_TAG}-armv7.apk
#    ./scripts/basic_upload_apks_service_account.py ch.opengis.qfield /tmp/qfield-${TRAVIS_TAG}-x86.apk

    echo -e "\e[93m * Deploying app to github release...\e[0m"
    ./scripts/release-upload.py --release=${TRAVIS_TAG} --oauth-token=${GITHUB_API_TOKEN} /tmp/qfield-${TRAVIS_TAG}-armv7.apk /tmp/qfield-${TRAVIS_TAG}-x86.apk
  fi
fi
