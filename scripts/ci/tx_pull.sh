#!/usr/bin/env bash

source $(git rev-parse --show-toplevel)/scripts/ci/envvars.sh

echo "travis_fold:start:tx-pull"
echo "${bold}Pulling translations${endbold}"
mkdir -p $(git rev-parse --show-toplevel)/i18n
tx pull -a
echo "travis_fold:end:tx-pull"
