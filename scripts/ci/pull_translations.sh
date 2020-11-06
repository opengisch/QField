#!/bin/bash

set -e

echo "::group::tx-pull"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh

tx pull --all --force

for x in android/res/values-*_*;do mv $x $(echo $x | sed -e 's/_/-r/') ;done

find ${DIR}/../../i18n -type f -name "*.ts" -exec lrelease "{}" \;

echo "::endgroup::"
