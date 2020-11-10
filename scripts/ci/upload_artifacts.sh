#!/bin/bash

set -e

# If we have secure env vars and are in either a pull request or a tag, we need to upload artifacts
if [[ "${ID_RSA}" ]];
then
  echo -e "\e[31mAbout to upload build artifacts\e[0m"

  if [[ "${CI_TAG}" ]];
  then
    FILENAME="qfield-${CI_TAG}-${ARCH}.apk"
  else
    FILENAME="qfield-dev-${CI_UPLOAD_ARTIFACT_ID}-${CI_COMMIT}-${ARCH}.apk"
  fi

  sudo mv build-${ARCH}/android-build/build/outputs/apk/release/android-build-release-signed.apk /tmp/${FILENAME}
  echo "${ID_RSA}" > /tmp/id_rsa
  head -n1 /tmp/id_rsa
  chmod 400 /tmp/id_rsa
  scp -i /tmp/id_rsa -o "StrictHostKeyChecking no" /tmp/${FILENAME} ${DEV_UPLOAD_USER}@${DEV_UPLOAD_HOST}:~/download.opengis.ch/qfield/ci-builds
  echo -e "\e[31Uploaded to https://download.opengis.ch/qfield/ci-builds/${FILENAME} \e[0m"
else
  echo -e "Not uploading artifacts ..."
fi
