#!/bin/bash

tx pull -a -f
for x in android/res/values-*_*;do mv $x $(echo $x | sed -e 's/_/-r/') ;done
lrelease QField.pro
