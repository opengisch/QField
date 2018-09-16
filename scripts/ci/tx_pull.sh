#!/usr/bin/env bash

source $(git rev-parse --show-toplevel)/scripts/ci/envvars.sh

echo "travis_fold:start:tx-pull"
echo "${bold}Pulling translations${endbold}"
tx pull -a
echo "travis_fold:end:tx-pull"
