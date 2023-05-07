#!/bin/bash

# Executes a bash in a android build container for debug reasons

docker run -it --rm -v$(git rev-parse --show-toplevel):/usr/src/qfield:Z qfield_and_dev bash
