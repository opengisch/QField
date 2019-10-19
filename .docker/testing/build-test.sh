#!/bin/bash

set -e

mkdir /usr/src/build
cd /usr/src/build
cmake /usr/src
make -j2
echo "travis_fold:end:build"
cd test
xvfb-run ctest --output-on-failure

