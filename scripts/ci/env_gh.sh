#!/bin/bash -e

if [[ ${GITHUB_REF} == *"refs/heads"* ]]; then
    TMP_CI_BRANCH=${GITHUB_REF#refs/heads/};
elif [[ ${GITHUB_REF} == *"refs/tags"* ]]; then
    TMP_CI_TAG=${GITHUB_REF#refs/tags/}
    TMP_CI_BRANCH=${TMP_CI_TAG}
else
    TMP_CI_BRANCH=${TMP_CI_BRANCH:=""}
    TMP_CI_TAG=${TMP_CI_TAG:=""}
fi

TMP_CI_COMMIT_BEFORE=$(jq --raw-output .before "${GITHUB_EVENT_PATH}")
TMP_CI_COMMIT_AFTER=$(jq --raw-output .after "${GITHUB_EVENT_PATH}")
TMP_CI_PULL_REQUEST_NUMBER=${TMP_CI_PULL_REQUEST_NUMBER:=$(jq --raw-output ".pull_request.number" "${GITHUB_EVENT_PATH}")}


# -- SC2004: $/${} is unnecessary on arithmetic variables.
if (( TMP_CI_PULL_REQUEST_NUMBER > 0 )); then
    TMP_CI_PULL_REQUEST=true
    TMP_CI_UPLOAD_ARTIFACT_ID=${TMP_CI_PULL_REQUEST_NUMBER}
else
    TMP_CI_PULL_REQUEST=false
    TMP_CI_UPLOAD_ARTIFACT_ID=${TMP_CI_BRANCH}
fi

export CI_BUILD_DIR=${CI_BUILD_DIR:=${GITHUB_WORKSPACE}}
export CI_COMMIT=${CI_COMMIT:=${GITHUB_SHA}}
export CI_BRANCH=${CI_BRANCH:=${TMP_CI_BRANCH}}
export CI_TAG=${CI_TAG:=${TMP_CI_TAG}}
export CI_PULL_REQUEST=${TMP_CI_PULL_REQUEST:=false}
export CI_PULL_REQUEST_NUMBER=${CI_PULL_REQUEST_NUMBER:=${TMP_CI_PULL_REQUEST_NUMBER}}
export CI_PULL_REQUEST_BRANCH=${CI_BRANCH:=${TMP_CI_BRANCH}}
export CI_COMMIT_RANGE=${CI_COMMIT_RANGE:="${TMP_CI_COMMIT_BEFORE}...${TMP_CI_COMMIT_AFTER}"}
export CI_REPO_SLUG=${CI_REPO_SLUG:=${GITHUB_REPOSITORY}}
export CI_UPLOAD_ARTIFACT_ID=${CI_UPLOAD_ARTIFACT_ID:=${TMP_CI_UPLOAD_ARTIFACT_ID}}
export CI_RUN_NUMBER=${GITHUB_RUN_NUMBER}

if [[ "${CI_TAG}" ]];
then
  export CI_PACKAGE_FILE_BASENAME="qfield-${CI_TAG}"

  if [[ ${ALL_FILES_ACCESS} == "ON" ]]; then
    export APP_PACKAGE_NAME="qfield_plus"
  else
    export APP_PACKAGE_NAME="qfield"
  fi
else
  export CI_PACKAGE_FILE_BASENAME="qfield-dev-${CI_UPLOAD_ARTIFACT_ID}-${CI_COMMIT}"
  if [[ ${ALL_FILES_ACCESS} == "ON" ]]; then
    export APP_PACKAGE_NAME="qfield_plus_dev"
  else
    export APP_PACKAGE_NAME="qfield_dev"
  fi
fi
export CI_PACKAGE_FILENAME="${CI_PACKAGE_FILE_BASENAME}-${ARCH}.apk"

{
    echo "CI_BUILD_DIR=${CI_BUILD_DIR}"
    echo "CI_COMMIT=${CI_COMMIT}"
    echo "CI_BRANCH=${CI_BRANCH}"
    echo "CI_TAG=${CI_TAG}"
    echo "CI_SECURE_ENV_VARS=${CI_SECURE_ENV_VARS}"
    echo "CI_PULL_REQUEST=${CI_PULL_REQUEST}"
    echo "CI_PULL_REQUEST_NUMBER=${CI_PULL_REQUEST_NUMBER}"
    echo "CI_PULL_REQUEST_BRANCH=${CI_PULL_REQUEST_BRANCH}"
    echo "CI_COMMIT_RANGE=${CI_COMMIT_RANGE}"
    echo "CI_REPO_SLUG=${CI_REPO_SLUG}"
    echo "CI_UPLOAD_ARTIFACT_ID=${CI_UPLOAD_ARTIFACT_ID}"
    echo "CI_PACKAGE_FILE_BASENAME=${CI_PACKAGE_FILE_BASENAME}"
    echo "CI_PACKAGE_FILENAME=${CI_PACKAGE_FILENAME}"
    echo "CI_RUN_NUMBER=${CI_RUN_NUMBER}"
} >> $GITHUB_ENV

# make sure ::set-output is on newlines
echo ""
echo "CI_BUILD_DIR: ${CI_BUILD_DIR}"
echo "CI_COMMIT: ${CI_COMMIT}"
echo "CI_BRANCH: ${CI_BRANCH}"
echo "CI_TAG: ${CI_TAG}"
echo "CI_SECURE_ENV_VARS: ${CI_SECURE_ENV_VARS}"
echo "CI_PULL_REQUEST: ${CI_PULL_REQUEST}"
echo "CI_PULL_REQUEST_NUMBER: ${CI_PULL_REQUEST_NUMBER}"
echo "CI_PULL_REQUEST_BRANCH: ${CI_PULL_REQUEST_BRANCH}"
echo "CI_COMMIT_RANGE: ${CI_COMMIT_RANGE}"
echo "CI_REPO_SLUG: ${CI_REPO_SLUG}"
echo "CI_UPLOAD_ARTIFACT_ID: ${CI_UPLOAD_ARTIFACT_ID}"
echo "CI_RUN_NUMBER: ${CI_RUN_NUMBER}"
echo ""
echo ""
echo "::set-output name=CI_BUILD_DIR::${CI_BUILD_DIR}"
echo "::set-output name=CI_COMMIT::${CI_COMMIT}"
echo "::set-output name=CI_BRANCH::${CI_BRANCH}"
echo "::set-output name=CI_TAG::${CI_TAG}"
echo "::set-output name=CI_SECURE_ENV_VARS::${CI_SECURE_ENV_VARS}"
echo "::set-output name=CI_PULL_REQUEST::${CI_PULL_REQUEST}"
echo "::set-output name=CI_PULL_REQUEST_NUMBER::${CI_PULL_REQUEST_NUMBER}"
echo "::set-output name=CI_PULL_REQUEST_BRANCH::${CI_PULL_REQUEST_BRANCH}"
echo "::set-output name=CI_COMMIT_RANGE::${CI_COMMIT_RANGE}"
echo "::set-output name=CI_REPO_SLUG::${CI_REPO_SLUG}"
echo "::set-output name=CI_UPLOAD_ARTIFACT_ID::${CI_UPLOAD_ARTIFACT_ID}"
echo "::set-output name=CI_PACKAGE_FILE_BASENAME::${CI_PACKAGE_FILE_BASENAME}"
echo "::set-output name=APP_PACKAGE_NAME::${APP_PACKAGE_NAME}"
echo ""
