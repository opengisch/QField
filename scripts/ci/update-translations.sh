#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh

sudo apt install qt5-default
lupdate -recursive ${CI_BRANCH} -ts ${CI_BRANCH}/i18n/qfield_en.ts

# release only if the branch is master
if [[ ${CI_BRANCH} = master ]]; then
  tx push --source
fi
