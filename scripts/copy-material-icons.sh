#!/bin/bash

for size in "mdpi" "hdpi" "xhdpi" "xxhdpi" "xxxhdpi"
do
  cp ../../material-design-icons/$1/drawable-$size/ic_$2_$3_$4dp.png images/themes/qfield/$size
done
