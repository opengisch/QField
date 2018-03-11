#!/bin/bash

set -e

if [ "${TRAVIS_SECURE_ENV_VARS}" = "true" -a "${TRAVIS_PULL_REQUEST}" != "false" ];
then
  echo -e "\e[31mDeploying app\e[0m"
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
