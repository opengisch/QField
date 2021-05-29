#!/bin/bash

set -e

/usr/src/vcpkg/base/bootstrap-vcpkg.sh
/usr/src/vcpkg/base/vcpkg install --overlay-ports=/usr/src/vcpkg/overlay spix

mkdir /usr/src/build
cd /usr/src/build

echo ::group::cmake
cmake -GNinja -DSpix_DIR:PATH=/usr/src/vcpkg/base/installed/x64-linux/share/Spix/cmake -DAnyRPC_DIR:PATH=/usr/src/vcpkg/base/installed/x64-linux/share/anyrpc -DWITH_SPIX=TRUE /usr/src
echo ::endgroup::

echo ::group::make
ninja -j2
echo ::endgroup::

xvfb-run ctest --output-on-failure

