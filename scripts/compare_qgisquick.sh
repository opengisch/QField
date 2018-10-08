#!/usr/bin/env bash

set -e 

FILE=$1

QGIS_SRC=~/opt/qgis/QGIS
OUT_PATH=~/Documents/temp/quick
CURDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

mkdir -p ${OUT_PATH}
cp -R ${QGIS_SRC}/src/quickgui/* ${OUT_PATH}


for f in $(find ${OUT_PATH} -type f); do
    perl -i -pe 's/^( *\*\n)? *\* \\since QGIS 3.*//igs' $f
    gsed -i -r '/^ *\* \\ingroup quick/d;' $f
	gsed -i -r '/#include "qgis_quick.h"/d;' $f
	gsed -i -r 's/^#include "(qgs[^"]+)"/#include <\1>/; s/QUICK_EXPORT //;' $f
	gsed -i -r 's/QUICK_EXPORT //;' $f
	gsed -i -r 's/^#include <(qgsquick[^>]+)>/#include "\1"/;' $f
	#tmpfile=$(mktemp /tmp/abc-script.XXXXXX)
	
done
echo "running: meld ${CURDIR}/../src/qgsquick/${FILE} ${OUT_PATH}/${FILE} &"
if [[ -n ${FILE} ]]; then
  meld ${CURDIR}/../src/qgsquick/${FILE} ${OUT_PATH}/${FILE} &
fi
