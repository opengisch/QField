#!/usr/bin/env bash

# In build environment set QGIS_INSTALL_PATH similarly as defined in config.pri
# Then, in project config, in run, add a custom deployment step
#   command: this script
#   arguments: "${QGIS_INSTALL_PATH}"

QGIS_INSTALL_PATH=$1

qgis_libs=( qgis_core qgis_analysis )
libs=( src/qgsquick/libqgsquick.dylib src/core/libcore.dylib src/app/qfield.app/Contents/MacOS/qfield output/bin/vertexmodeltest output/bin/qfield output/lib/libqfield_core.dylib )
for lib in "${libs[@]}"; do
  if [[ ! -f  ${lib} ]]; then
    echo "skipping missing ${lib}"
    continue
  else
    echo "running install_name_tool on ${lib}"
  fi
  for qgis_lib in "${qgis_libs[@]}"; do
    install_name_tool -change @executable_path/../Frameworks/${qgis_lib}.framework/${qgis_lib} ${QGIS_INSTALL_PATH}/QGIS.app/Contents/Frameworks/${qgis_lib}.framework/${qgis_lib} ${lib}
  done
done
