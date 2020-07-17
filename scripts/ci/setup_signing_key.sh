#!/bin/bash
set -e

if [[ ${CI_SECURE_ENV_VARS} = true ]]; then
  echo "With signing key"
  openssl aes-256-cbc -K $encrypted_c158cd588669_key -iv $encrypted_c158cd588669_iv -in keystore.p12.enc -out keystore.p12 -d
else
  echo "Without signing key"
fi
