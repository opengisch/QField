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

# define RELEASE_BRANCH
ci_to_release_branch

# Get git sha for master branch
git fetch origin master:master --depth 1
GIT_MASTER_SHA=$(git rev-list -n1 master)

# If the current commit is the latest on master -> we need to create a new release branch
if [[ "${CI_COMMIT}" = "${GIT_MASTER_SHA}" ]]; then
  if [[ ! "${RELEASE_BRANCH}" =~ ^release ]]; then
    echo "Something wrong happened"
    exit 1
  fi
  echo "Sorry, something has gone wrong. The latest release commit is identical to the latest master commit."
  echo "This is not supported"
  echo ""
  echo " 0. Delete the tag you just created 'git push origin :${CI_TAG}'. No worries, the changelog will not be deleted."
  echo " 1. Create a new release branch 'git checkout -b ${RELEASE_BRANCH}'"
  echo " 2. Edit the file RELEASE_NAME"
  echo " 3. Push this new branch 'git push -u origin ${RELEASE_BRANCH}'"
  echo " 4. Recreate the release"

  exit 1
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
