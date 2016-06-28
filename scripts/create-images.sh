#!/bin/bash

FILES=images/themes/qfield/nodpi/*.svg
for f in $FILES
do
  filename=$(basename "$f")
  filename="${filename%.*}"

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
    inkscape -z -e images/themes/qfield/$size/$filename.png -w $res -h $res $f
  done
done
