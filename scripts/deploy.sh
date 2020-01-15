#!/bin/bash
set -e

source scripts/travis_env.sh

ARCHS=( armv7 x86 arm64_v8a x86_64 )

COMMENT="Uploaded test apks for\n"
for ARCH in "${ARCHS[@]}"
do
  COMMENT="${COMMENT} - [**${ARCH}**](https://download.opengis.ch/qfield/ci-builds/qfield-dev-${UPLOAD_ARTIFACT_ID}-${TRAVIS_COMMIT}-${ARCH}.apk)\n"
done


if [[ ${TRAVIS_SECURE_ENV_VARS} = true ]]; then
  if [ ${TRAVIS_PULL_REQUEST} != false ]; then
    echo -e "\e[31mDeploying app to pull request\e[0m"
    curl -u m-kuhn:${GITHUB_API_TOKEN} -X POST --data '{"body": "'${COMMENT}'"}' https://api.github.com/repos/opengisch/QField/issues/${TRAVIS_PULL_REQUEST}/comments

  elif [[ -n ${TRAVIS_TAG} ]]; then
    echo -e "\e[93;1mStarting to deploy a new release\e[0m"
    openssl aes-256-cbc -K $encrypted_play_upload_key -iv $encrypted_play_upload_iv -in .ci/play_developer.p12.enc -out .ci/play_developer.p12 -d

    ASSETS=""
    for ARCH in "${ARCHS[@]}"
    do
      echo -e "\e[93m * Collecting apks to upload...\e[0m"
      curl -L -s -S -o /tmp/qfield-${TRAVIS_TAG}-${ARCH}.apk https://download.opengis.ch/qfield/ci-builds/qfield-dev-${UPLOAD_ARTIFACT_ID}-${TRAVIS_COMMIT}-${ARCH}.apk

      echo -e "\e[93m * Deploying app to github release...\e[0m"
      ./scripts/upload_release_asset.py /tmp/qfield-${TRAVIS_TAG}-${ARCH}.apk ${TRAVIS_TAG}

      ASSETS="${ASSETS} /tmp/qfield-${TRAVIS_TAG}-${ARCH}.apk"
    done

    echo -e "\e[93m * Deploying app to google play (release version)...\e[0m"
    ./scripts/basic_upload_apks_service_account.py ch.opengis.qfield ${ASSETS}

  elif [[ ${TRAVIS_BRANCH} = master ]]; then
    # we are on a standard commit on master branch
    curl -u m-kuhn:${GITHUB_API_TOKEN} -X POST --data '{"body": "'${COMMENT}'"}' https://api.github.com/repos/opengisch/QField/commits/${TRAVIS_COMMIT}/comments

    ASSETS=""
    for ARCH in "${ARCHS[@]}"
    do
      echo -e "\e[93m * Collecting apks to upload...\e[0m"
      curl -L -s -S -o /tmp/qfield-dev-${ARCH}.apk https://download.opengis.ch/qfield/ci-builds/qfield-dev-${UPLOAD_ARTIFACT_ID}-${TRAVIS_COMMIT}-${ARCH}.apk
      ASSETS="${ASSETS} /tmp/qfield-dev-${ARCH}.apk"
    done

    echo -e "\e[93m * Deploying app to google play (release version)...\e[0m"
    ./scripts/basic_upload_apks_service_account.py ch.opengis.qfield ${ASSETS}
  fi
fi
