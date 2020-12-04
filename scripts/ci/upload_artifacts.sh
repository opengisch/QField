#!/bin/bash

set -e

# If we have secure env vars and are in either a pull request or a tag, we need to upload artifacts
if [[ "${S3CFG}" ]];
then
  echo -e "\e[31mAbout to upload build artifacts\e[0m"

  if [[ "${CI_TAG}" ]];
  then
    FILENAME="qfield-${CI_TAG}-${ARCH}.apk"
  else
    FILENAME="qfield-dev-${CI_UPLOAD_ARTIFACT_ID}-${CI_COMMIT}-${ARCH}.apk"
  fi

  sudo mv build-${ARCH}/android-build/build/outputs/apk/release/android-build-release-signed.apk /tmp/${FILENAME}
  echo "${S3CFG}" > ~/.s3cfg
  s3cmd put --acl-public /tmp/${FILENAME} s3://qfieldapks/ci-builds/${FILENAME}
  s3cmd modify --add-header=content-type:application/vnd.android.package-archive s3://qfieldapks/ci-builds/${FILENAME}
  echo -e "\e[31Uploaded to https://sos-ch-dk-2.exo.io/qfieldapks/ci-builds/${FILENAME} \e[0m"
else
  echo -e "Not uploading artifacts ..."
fi
