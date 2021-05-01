#!/bin/bash

# pre-commit hook
# Setup with:
#
#  ln -s ../../scripts/prepare-commit.sh .git/hooks/pre-commit
#

STYLE=$(git config --get hooks.clangformat.style)
if [ -n "${STYLE}" ] ; then
  STYLEARG="-style=${STYLE}"
else
  STYLEARG=""
fi

docker build -t clang-format-lint github.com/DoozyX/clang-format-lint-action

format_file() {
  file="${1}"
  if [ -f $file ]; then
    clang-format -i ${STYLEARG} ${1}
    docker run --rm --workdir /src -v $(pwd):/src \
    /clang-format/clang-format12 -i ${STYLEARG} ${1}
    git add ${1}
  fi
}

case "${1}" in
  --about )
    echo "Runs clang-format on source files"
    ;;
  * )
    for file in $(git diff-index --cached --name-only HEAD | grep -iE '\.(cpp|cc|h|hpp|java)$') ; do
      format_file "${file}"
    done
    ;;
esac
