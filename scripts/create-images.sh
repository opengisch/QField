#!/bin/bash

SKIP_EXISTING=FALSE

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
FILES=${DIR}/../images/themes/qfield/nodpi/*.svg

for f in $FILES
do
  filename=$(basename "$f")
  filename="${filename%.*}"
  echo $filename
  for size in "mdpi" "hdpi" "xhdpi" "xxhdpi" "xxxhdpi"
  do
    case $size in
      mdpi)
        res=24
        ;;
      hdpi)
        res=36
        ;;
      xhdpi)
        res=48
        ;;
      xxhdpi)
        res=72
        ;;
      xxxhdpi)
        res=96
        ;;
    esac
    DEST_FILE=${DIR}/../images/themes/qfield/$size/$filename.png
    if ( [[ ! -f ${DEST_FILE} ]] || [[ ${SKIP_EXISTING} != TRUE ]] ); then
      inkscape -z -e ${DEST_FILE} -w $res -h $res $f
    else
      echo "  skipping"
    fi
  done
done
