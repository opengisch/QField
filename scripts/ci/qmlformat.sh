#!/bin/bash

FILES=$(find /usr/src -name '*.qml')
RESULT=0

for f in $FILES
do
  /usr/local/Qt-5.15.0/bin/qmlformat -i $f
  RESULT=$(($RESULT || $?))
done

exit $RESULT
