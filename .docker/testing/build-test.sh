#!/bin/bash

set -e

mkdir /usr/src/build
cd /usr/src/build

cmake -GNinja /usr/src
ninja -j2

xvfb-run ctest --output-on-failure

