#!/usr/bin/env bash

echo "logcat"
adb logcat -c
adb shell am start -n ch.opengis.qfield/ch.opengis.qfield.QFieldActivity
adb logcat | grep "QField Project"
