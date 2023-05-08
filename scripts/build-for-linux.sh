#!/bin/bash
QT_PREFIX=${HOME}/.qt
QT_VERSION=6.5.0

# pip install aqt
# aqt install-qt linux desktop ${QT_VERSION} ${QT_PREFIX} -m [...] <- check android.yaml
# make sure flex and bison are installed (use `apt` or similar)
# sudo apt install mesa-common-dev libgl1-mesa-dev (makes QtGui/OpenGL happy)

export Qt6_DIR=${QT_PREFIX}/${QT_VERSION}/gcc_64
cmake -S . -B $(git rev-parse --show-toplevel)/build-x64-linux -DSYSTEM_QT=ON -DBUILD_WITH_QT6=ON -GNinja -DWITH_VCPKG=ON -DQt6_ROOT=${Qt6_DIR}
cmake --build $(git rev-parse --show-toplevel)/build-x64-linux
