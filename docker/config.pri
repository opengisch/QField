# set the following paths according to your system
#
# Don't include this file in other files, include paths.pri

android {
  # To build for android you need OSGEO4A
  OSGEO4A_DIR = /home/osgeo4a
}
!android {
  # To build for a PC, you need Qt5 builds of QGIS, QWT and QScintilla
  QGIS_INSTALL_PATH = /home/mkuhn/dev/qgis/install
}

