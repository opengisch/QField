#!/usr/bin/env bash
#sudo apt install qt5-default

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh

lupdate-qt5 -recursive ${CI_BRANCH} -ts ${CI_BRANCH}/i18n/qfield_en.ts

# release only if the branch is master
if [[ ${CI_BRANCH} = master ]]; then
  tx push --source
fi

# release only if the branch is master
if [[ ${CI_BRANCH} = trans-test ]]; then
  tx push --source
fi

echo ==================1
Grep -Ri Trans-test-ta ${CI_BUILD_DIR}/i18n/qfield_en.ts
echo ===========1
