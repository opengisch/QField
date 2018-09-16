#!/usr/bin/env bash

source $(git rev-parse --show-toplevel)/scripts/ci/envvars.sh
echo "travis_fold:start:docker-pull"
echo "${bold}Docker Pull${endbold}"
docker pull opengisch/qfield-sdk:${QFIELD_SDK_VERSION}
echo "travis_fold:end:docker-pull"
