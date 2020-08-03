#!/bin/bash

set -e

mkdir /usr/src/build
cd /usr/src/build

echo ::group::cmake
cmake -GNinja /usr/src
echo ::endgroup::

echo ::group::make
ninja -j2
echo ::endgroup::

xvfb-run ctest --output-on-failure

