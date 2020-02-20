#!/usr/bin/env bash

# In build environment set QGIS_INSTALL_PATH similarly as defined in config.pri
# Then, in project config, in run, add a custom deployment step
#   command: this script
#   arguments: "${QGIS_INSTALL_PATH}"

QGIS_INSTALL_PATH=$1

qgis_libs=( qgis_core qgis_analysis )
for qgis_lib in "${qgis_libs[@]}"; do
  install_name_tool -change @executable_path/../Frameworks/${qgis_lib}.framework/${qgis_lib} ${QGIS_INSTALL_PATH}/QGIS.app/Contents/Frameworks/${qgis_lib}.framework/${qgis_lib} src/qgsquick/libqgsquick.dylib
  install_name_tool -change @executable_path/../Frameworks/${qgis_lib}.framework/${qgis_lib} ${QGIS_INSTALL_PATH}/QGIS.app/Contents/Frameworks/${qgis_lib}.framework/${qgis_lib} src/core/libcore.dylib
  install_name_tool -change @executable_path/../Frameworks/${qgis_lib}.framework/${qgis_lib} ${QGIS_INSTALL_PATH}/QGIS.app/Contents/Frameworks/${qgis_lib}.framework/${qgis_lib} src/app/qfield.app/Contents/MacOS/qfield
  # install_name_tool -change @executable_path/../Frameworks/${qgis_lib}.framework/${qgis_lib} ${QGIS_INSTALL_PATH}/QGIS.app/Contents/Frameworks/${qgis_lib}.framework/${qgis_lib} test/test.app/Contents/MacOS/test
done
