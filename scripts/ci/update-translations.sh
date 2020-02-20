#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh

lupdate QField.pro
if [[ ${TRAVIS_BRANCH} = master ]]; then
  tx push --source
else
  travis_to_release_branch
  tx push --source --branch ${RELEASE_BRANCH//_/-}
fi
