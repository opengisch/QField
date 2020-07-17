#!/bin/bash
set -e

ARCHS=( armv7 x86 arm64_v8a x86_64 )

COMMENT="Uploaded test apks for\n"
for ARCH in "${ARCHS[@]}"
do
  COMMENT=${COMMENT}" - [**${ARCH}**](https://download.opengis.ch/qfield/ci-builds/qfield-dev-${CI_UPLOAD_ARTIFACT_ID}-${CI_COMMIT}-${ARCH}.apk)\n"
done
BODY="{\"body\": \"${COMMENT}\"}"


fetch_asset() {
  ARCH=$1
  ASSET_PATH=$2
  curl -L -s -S -o ${ASSET_PATH} https://download.opengis.ch/qfield/ci-builds/qfield-dev-${CI_UPLOAD_ARTIFACT_ID}-${CI_COMMIT}-${ARCH}.apk
}

if [[ ${CI_SECURE_ENV_VARS} = true ]]; then
  if [ ${CI_PULL_REQUEST} != false ]; then
    echo -e "\e[31mDeploying app to pull request\e[0m"
    curl -H "Authorization: token ${GITHUB_API_TOKEN}" -X POST --data "${BODY}" https://api.github.com/repos/opengisch/QField/issues/${CI_PULL_REQUEST}/comments

  elif [[ -n ${CI_TAG} ]]; then
    # we are on a tag and on a release branch (if released from master, the release branched should have been checked out)
    # This cannot be checked on Travis since we are on a detached state
    #GIT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
    #[[ ! $(git rev-parse --abbrev-ref HEAD) =~ ^release-[0-9_]+$ ]] && echo "we must be on a release branch" && exit 1

    echo -e "\e[93;1mStarting to deploy a new release\e[0m"
    openssl aes-256-cbc -K $encrypted_play_upload_key -iv $encrypted_play_upload_iv -in .ci/play_developer.p12.enc -out .ci/play_developer.p12 -d

    ASSETS=""
    for ARCH in "${ARCHS[@]}"
    do
      ASSET_PATH=/tmp/qfield-${CI_TAG}-${ARCH}.apk
      echo -e "\e[93m * Collecting apks to upload...\e[0m"
      fetch_asset ${ARCH} ${ASSET_PATH}
      echo -e "\e[93m * Deploying app to github release...\e[0m"
      ./scripts/upload_release_asset.py ${ASSET_PATH} ${CI_TAG}
      ASSETS="${ASSETS} ${ASSET_PATH}"
    done

    RELEASE_URL="https://github.com/opengisch/QField/releases/tag/${CI_TAG}"
    echo -e "\e[93m * Deploying app to google play (release version)...\e[0m"
    ./scripts/basic_upload_apks_service_account.py ch.opengis.qfield internal ${RELEASE_URL} ${ASSETS}

  else
    # we are on a standard commit (i.e. no tag) on master or release-* branch or any branch
    # write comment
    echo "writing comment in https://api.github.com/repos/opengisch/QField/commits/${CI_COMMIT}/comments"
    curl -H "Authorization: token ${GITHUB_API_TOKEN}" -X POST --data "${BODY}" https://api.github.com/repos/opengisch/QField/commits/${CI_COMMIT}/comments

    # only master builds are pushed to play store
    if [[ ${CI_BRANCH} = master ]] && [[ ${CI_PULL_REQUEST} = false ]]; then
      ASSETS=""
      for ARCH in "${ARCHS[@]}"
      do
        echo -e "\e[93m * Collecting apks to upload...\e[0m"
        ASSET_PATH=/tmp/qfield-dev-${ARCH}.apk
        fetch_asset ${ARCH} ${ASSET_PATH}
        ASSETS="${ASSETS} ${ASSET_PATH}"
      done
      openssl aes-256-cbc -K $encrypted_play_upload_key -iv $encrypted_play_upload_iv -in .ci/play_developer.p12.enc -out .ci/play_developer.p12 -d
      echo -e "\e[93m * Deploying app to google play (unstable version)...\e[0m"
      COMMIT_URL="https://github.com/opengisch/QField/commit/${CI_COMMIT}"
      ./scripts/basic_upload_apks_service_account.py ch.opengis.qfield_dev beta ${COMMIT_URL} ${ASSETS}
    fi
  fi
fi
