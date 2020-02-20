#!/bin/bash

echo "travis_fold:start:docker-pull"
docker pull opengisch/qfield-sdk:${QFIELD_SDK_VERSION}
echo "travis_fold:end:docker-pull"

