#!/usr/bin/env bash
set -e


echo "Spell check"
cd $(git rev-parse --show-toplevel)

if [[ -n  $CI_PULL_REQUEST_BRANCH  ]]; then
  # if on a PR, just analyze the changed files
  echo "CI PR BRANCH: $CI_PULL_REQUEST_BRANCH"
  FILES=$(git diff --diff-filter=AM --name-only $(git merge-base HEAD ${CI_BRANCH}) | tr '\n' ' ' )
  export PATH=${HOME}/osgeo4travis/bin:${PATH}
elif [[ -n  $CI_COMMIT_RANGE  ]]; then
  echo "CI COMMIT RANGE: $CI_COMMIT_RANGE"
  FILES=$(git diff --diff-filter=AM --name-only ${CI_COMMIT_RANGE/.../..} | tr '\n' ' ' )
  export PATH=${HOME}/osgeo4travis/bin:${PATH}
else
  echo "Running spell test on all files (might take a while)"
  FILES=""
fi

if [[ -n $FILES ]]; then
  DIR=$(git rev-parse --show-toplevel)/scripts/spell_check
  ${DIR}/check_spelling.sh -r $FILES
fi
