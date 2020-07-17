#!/bin/bash

echo "::group::docker-pull"
docker pull opengisch/qfield-sdk:${QFIELD_SDK_VERSION}
echo "::endgroup::"

