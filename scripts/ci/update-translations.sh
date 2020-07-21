#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh

lupdate QField.pro
tx status
if [[ ${CI_BRANCH} = master ]]; then
  # tx push --source
else
  ci_to_release_branch
  # tx push --source --branch ${RELEASE_BRANCH//_/-}
fi
