#!/bin/bash
QT_PREFIX=${HOME}/.qt
QT_VERSION=6.5.1

# pip install aqtinstall
# aqt install-qt linux desktop 6.5.1 gcc_64 -O .qt -m qt5compat qtcharts qtpositioning qtserialport qtconnectivity qtmultimedia qtwebview qtsensors # Modules come from .github/workflows/android.yaml
# make sure flex and bison are installed (use `apt` or similar)
# sudo apt install mesa-common-dev libgl1-mesa-dev (makes QtGui/OpenGL happy)

export Qt6_DIR=${QT_PREFIX}/${QT_VERSION}/gcc_64
cmake -S . -B $(git rev-parse --show-toplevel)/build-x64-linux -DSYSTEM_QT=ON -GNinja -DWITH_VCPKG=ON -DQt6_ROOT=${Qt6_DIR} -DENABLE_TESTS=ON
cmake --build $(git rev-parse --show-toplevel)/build-x64-linux
