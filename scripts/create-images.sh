#!/bin/bash

SKIP_EXISTING=FALSE

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
if [[ -z $1 ]]; then
  FILES=${DIR}/../images/themes/qfield/nodpi/$1*.svg
else
  FILES=$1
fi
if [[ -z $2 ]]; then
  DESTINATION_PATH=${DIR}/../images/themes/qfield
else
  DESTINATION_PATH=${DIR}/../android/res
  PREFIX="drawable-"
fi

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
    DEST_FILE=${DESTINATION_PATH}/${PREFIX}${size}/$filename.png
    if ( [[ ! -f ${DEST_FILE} ]] || [[ ${SKIP_EXISTING} != TRUE ]] ); then
      inkscape -z -o ${DEST_FILE} -w $res -h $res $f
    else
      echo "  skipping"
    fi
  done
done
