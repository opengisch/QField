#!/bin/bash

cmake -S . -B $(git rev-parse --show-toplevel)/build-x64-linux -GNinja -DWITH_VCPKG=ON -DWITH_SPIX=ON -DENABLE_TESTS=ON
cmake --build $(git rev-parse --show-toplevel)/build-x64-linux
