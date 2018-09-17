#!/usr/bin/env bash

source $(git rev-parse --show-toplevel)/scripts/ci/env_vars.sh

echo "travis_fold:start:tx-pull"
echo "${bold}Pulling translations${endbold}"
mkdir -p $(git rev-parse --show-toplevel)/i18n
tx pull -a

echo '<RCC>' > i18n/translations.qrc
echo '    <qresource prefix="/">' >> i18n/translations.qrc
for filename in i18n/*.ts; do
  filename=$(basename -- "$filename")
  filename="${filename%.*}"
  echo "        <file>${filename}.qm</file>" >> i18n/translations.qrc
done
echo '' >> i18n/translations.qrc
echo '    </qresource>' >> i18n/translations.qrc
echo '</RCC>' >> i18n/translations.qrc

echo "travis_fold:end:tx-pull"
