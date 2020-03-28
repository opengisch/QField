#!/bin/bash

docker run --rm -v $(pwd):/usr/src forderud/qtwasm:qt-5.15 /usr/src/scripts/ci/qmlformat.sh
