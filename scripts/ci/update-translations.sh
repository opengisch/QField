#!/usr/bin/env bash

lupdate QField.pro
if [[ ${TRAVIS_BRANCH} = master ]]; then
  tx push --source
else
  tx push --source --branch
fi
