#!/bin/bash

GITDIR=$(git rev-parse --show-toplevel)

for LANG in "ca" "de" "en" "es" "fi" "fr" "gl" "hu" "it" "lt" "mn" "nl" "pl" "pt" "ro" "uk" "zh"
do
  langfiles="$QTDIR/android_armv7/translations/*_${LANG}.qm"

  echo "Translating $LANG ..."
  echo "    $langfiles"

  if [ -n "$langfiles" ]
  then
    lconvert-qt5 -o $GITDIR/i18n/qt_$LANG.qm $langfiles
  else
    echo "No Qt translations available for $LANG"
  fi
done
