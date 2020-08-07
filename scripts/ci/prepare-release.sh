#!/usr/bin/env bash

set -e

# This script will prepare branches and Transifex resources in case of a release
# see https://bit.ly/2SxeDNK
# if on master => create release branch
# if TX resource doesn't exist => create it from master

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh


if [[ -z ${CI_TAG} ]]; then
  echo "we should not be here without a CI_TAG"
  exit 1
fi
