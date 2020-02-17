#!/usr/bin/env bash

set -e

# This script will prepare branches and Transifex resources in case of a release
# see https://bit.ly/2SxeDNK
# if on master => create release branch
# if TX resource doesn't exist => create it from master

# GNU prefix command for mac os support (gsed, gsplit)
GP=
if [[ "$OSTYPE" =~ darwin* ]]; then
  GP=g
fi

if [[ -n ${TRAVIS_TAG} ]] && [[ ! ${TRAVIS_BRANCH} =~ ^(master|release-[0-9_]+)$ ]]; then
  echo "we should not be here without a TRAVIS_TAG"
  exit 1
fi

# Create release branch if needed
if [[ ${TRAVIS_BRANCH} = master ]]; then
  NEW_BRANCH=$(${GP}sed -r 's/v([0-9])\.([0-9])(\.[0-9])?/release-\1_\2/' <<< ${TRAVIS_TAG})
  if [[ ! ${NEW_BRANCH} =~ ^release ]]; then
    echo "Something wrong happened"
    exit 1
  fi
  # this will fail if the release branch already exists
  # i.e. you should not create the release from master but from the release branch
  git checkout -b ${NEW_BRANCH}
  git push -u origin ${NEW_BANCH}
fi

# Pull/push TX to be sure the TX resource exists
GIT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
TX_RESOURCE_EXISTS=$([[ -n $(tx status | grep -E "^qfield-for-qgis -> ${NEW_BRANCH}--qfield") ]] && echo "true" || echo "false")
if [[ ${TX_RESOURCE_EXISTS} = false ]]; then
  tx pull --all --force
  tx push --source --translations --branch --skip
fi


