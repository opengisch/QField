#!/usr/bin/env bash

set -e

# see https://stackoverflow.com/a/44036810/1548052

# Keep track of where Travis put us.
# We are on a detached head, and we need to be able to go back to it.
build_head=$(git rev-parse HEAD)

# fetch the tags
git config --replace-all remote.origin.fetch +refs/heads/*:refs/remotes/origin/*
git fetch --tags

# checkout the tagged commit
git checkout -qf ${TRAVIS_TAG}

