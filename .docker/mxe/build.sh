#!/bin/bash
# This script should be launched from the top level directory
# of QField repository in order to create a windows build
# in a zip file.
# The zip file will be stored in the same top level directory.

# Location of current script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

docker  run  \
    -v ${PWD}:${PWD} \
    -w $(pwd) --rm  \
    --user $(id -u):$(id -g) \
    -it elpaso/mxe-qfield-builder:latest \
    ${DIR}/build-mxe.sh
