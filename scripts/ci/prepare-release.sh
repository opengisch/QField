#!/usr/bin/env bash

set -e

# This script will prepare branches and Transifex resources in case of a release
# see https://bit.ly/2SxeDNK
# if on master => create release branch
# if TX resource doesn't exist => create it from master

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
source ${DIR}/../version_number.sh


if [[ -z ${TRAVIS_TAG} ]]; then
  echo "we should not be here without a TRAVIS_TAG"
  exit 1
fi

# git-full-fetch has been called before, we should be on a complete clone
GIT_BRANCH=$(git rev-parse --abbrev-ref HEAD)

# define RELEASE_BRANCH
travis_to_release_branch

# Create release branch if needed (if we are on master)
if [[ ${TRAVIS_BRANCH} = master ]]; then
  if [[ ! ${RELEASE_BRANCH} =~ ^release ]]; then
    echo "Something wrong happened"
    exit 1
  fi
  # this will fail if the release branch already exists
  # i.e. you should not create the release from master but from the release branch
  git checkout -b ${RELEASE_BRANCH}
  git push -u origin ${RELEASE_BRANCH}
fi

# Pull/push TX to be sure the TX resource exists
RESOURCE="${RELEASE_BRANCH//_/-}--qfield"
TX_RESOURCE_EXISTS=$(python3 << EOF
from pytransifex import Transifex
t = Transifex("${TX_TOKEN}", 'opengisch', i18n_type='QT')
resources = [r['name'] for r in t.list_resources('qfield-for-qgis')]
if "${RESOURCE}" in resources:
  print("yes")
else:
  print("no")
EOF
)
echo "Resource '${RESOURCE}' exists: ${TX_RESOURCE_EXISTS}"
if [[ ${TX_RESOURCE_EXISTS} = no ]]; then
  tx pull --all --force
  tx push --source --translations --branch ${RELEASE_BRANCH//_/-} --skip
fi
