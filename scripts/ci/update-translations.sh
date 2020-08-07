#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh

lupdate QField.pro

# release only if the branch is master
if [[ ${CI_BRANCH} = master ]]; then
  tx push --source
fi
