#!/bin/bash

set -e

echo "::group::tx-pull"
tx pull --resource qfield-for-qgis.qfield --all --source --minimum-perc=50 --force
tx pull --resource qfield-for-qgis.qfield_android --all --minimum-perc=50 --force
echo "::endgroup::"

echo "::group::android specific translations"
for x in platform/android/res/values-*_*; do
    rm -rf $(echo $x | sed -e 's/_/-r/')
    mv $x $(echo $x | sed -e 's/_/-r/')
done
find platform/android/res/values-* -name strings.xml -type f -print0 | while read -d $'\0' file; do
    # .bak is a workaround GNU & BSD/macOS compatibility
    sed -i.bak 's/<!\[CDATA \[/<!\[CDATA\[/g' $file
    rm $file.bak
done
echo "::endgroup::"

echo "::group::remove line numbers"
    lupdate -locations none -recursive src -ts i18n/qfield_en.ts
echo "::endgroup::"
