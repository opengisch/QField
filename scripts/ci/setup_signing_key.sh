#!/bin/bash
set -e

if [[ ${TRAVIS_SECURE_ENV_VARS} = true ]]; then
  openssl aes-256-cbc -K $encrypted_c158cd588669_key -iv $encrypted_c158cd588669_iv -in keystore.p12.enc -out keystore.p12 -d
fi
