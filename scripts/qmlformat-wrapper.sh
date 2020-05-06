#!/bin/bash

# !!!!! You probably want to call scripts/qmlformat.sh instead of this file !!!!!!

# Adds the possibility to run qmlformat on stdin if no arguments are given. Otherwise forwards parameters to qmlformat.
# Designed to run in a Docker container.

ARGS=""

for arg in "$@"
do
  if [[ $arg =~ .qml$ ]]; then
    echo "yes"
    ARGS="$ARGS /usr/src/$arg"
  else
    ARGS="$ARGS $arg"
  fi
done

if [ $# -eq 0 ];
then
  cp /dev/stdin /tmp/file.qml
  ARGS="/tmp/file.qml"
fi

/usr/local/Qt-5.15.0/bin/qmlformat $ARGS
