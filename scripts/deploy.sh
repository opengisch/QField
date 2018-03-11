#!/bin/bash

set -e

if [ "${TRAVIS_SECURE_ENV_VARS}" = "true" ];
then
  if [ "${TRAVIS_PULL_REQUEST}" != "false" ];
  then
    echo -e "\e[31mDeploying app to pull request\e[0m"
    curl -u m-kuhn:${GITHUB_API_TOKEN} -X POST --data '{"body": "Uploaded test apks for [armv7](https://opengis.ch/download/qfield/ci-builds/qfield-dev-'${TRAVIS_PULL_REQUEST}'-'${TRAVIS_COMMIT}'-armv7.apk) and [x86](https://opengis.ch/download/qfield/ci-builds/qfield-dev-'${TRAVIS_PULL_REQUEST}'-'${TRAVIS_COMMIT}'-x86.apk)"}' https://api.github.com/repos/opengisch/QField/issues/${TRAVIS_PULL_REQUEST}/comments
  fi 

  if [ "${TRAVIS_TAG}X" != "X" ];
  then
    echo -e "\e[31mDeploying app to releases\e[0m"
    wget -o /tmp/qfield-${TRAVIS_TAG}-armv7.apk https://opengis.ch/download/qfield/ci-builds/qfield-dev-'${TRAVIS_PULL_REQUEST}'-'${TRAVIS_COMMIT}'-armv7.apk
    wget -o /tmp/qfield-${TRAVIS_TAG}-x86.apk https://opengis.ch/download/qfield/ci-builds/qfield-dev-'${TRAVIS_PULL_REQUEST}'-'${TRAVIS_COMMIT}'-x86.apk
    ./release-upload.py --release=${TRAVIS_TAG} --oauth-token=${GITHUB_API_TOKEN} /tmp/qfield-${TRAVIS_TAG}-armv7.apk /tmp/qfield-${TRAVIS_TAG}-x86.apk
    echo -e "Skipping dev build upload because"
  fi
fi
