#!/bin/bash

echo "::group::tx-pull"

if [[ ${CI_SECURE_ENV_VARS} = true ]]; then
  DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
  source ${DIR}/../version_number.sh
  
  tx pull --all --force
  
  for x in android/res/values-*_*;do mv $x $(echo $x | sed -e 's/_/-r/') ;done
  
  find ${DIR}/i18n -type f -name "*.ts" -exec lrelease "{}" \;
fi

echo "::endgroup::"
