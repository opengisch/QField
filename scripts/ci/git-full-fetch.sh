#!/usr/bin/env bash

set -e

# see https://stackoverflow.com/a/44036810/1548052

# fetch the tags
git config --replace-all remote.origin.fetch +refs/heads/*:refs/remotes/origin/*
git fetch --tags

# checkout the tagged commit
git checkout -qf ${CI_TAG}

