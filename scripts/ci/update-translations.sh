#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh

lupdate QField.pro

if [[ ${CI_BRANCH} = master ]]; then
  tx status
  # tx push --source
else
  tx status
  ci_to_release_branch
  # tx push --source --branch ${RELEASE_BRANCH//_/-}
fi
