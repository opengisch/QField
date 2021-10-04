#!/bin/bash

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh


echo "::group::tx-pull"
tx pull --all --force
echo "::endgroup::"

echo "::group::android specific translations"
for x in android/res/values-*_*;do mv $x $(echo $x | sed -e 's/_/-r/') ;done
echo "::endgroup::"

echo "::group::lrelease"
find ${DIR}/../../i18n -type f -name "*.ts" | xargs lrelease
echo "::endgroup::"
