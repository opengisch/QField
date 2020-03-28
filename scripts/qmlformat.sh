#!/bin/bash

apt install colordiff

FILES=$(find /usr/src -name '*.qml')
RESULT=0

for f in $FILES
do
  colordiff $f <(/usr/local/Qt-5.15.0/bin/qmlformat $f)
  RESULT=$(($RESULT || $?))
done

exit $RESULT
