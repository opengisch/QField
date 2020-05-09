#!/bin/bash

UPLOAD_ARTIFACT_ID=$( [[ ${TRAVIS_PULL_REQUEST} =~ false ]] && echo ${TRAVIS_TAG} || echo ${TRAVIS_PULL_REQUEST} )
export UPLOAD_ARTIFACT_ID
