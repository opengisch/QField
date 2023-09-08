#!/bin/bash

set -e

echo "::group::tx-pull"
# `-a` Of all the languages that exist on the remote,
# `--minimum-perc=50 --force` force-pull only those with at least 50% translation coverage
./tx pull -a --minimum-perc=50 --force
echo "::endgroup::"

echo "::group::android specific translations"
for x in platform/android/res/values-*_*; do
	# $x is evaluated to the glob string literal when no dir is found
	if [[ -d $x ]]; then
		rm -rf $(echo $x | sed -e 's/_/-r/')
		mv $x $(echo $x | sed -e 's/_/-r/')
	fi
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
