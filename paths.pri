################################################
# DON'T EDIT THIS FILE, USE config.pri instead #
################################################

# this is a convenience file where subpaths can be created to be used in other 
# pri files. Include this file in other files not config.pri 
include( config.pri )
android {
  OSGEO4A_STAGE_DIR = $${OSGEO4A_DIR}/stage

  # This is where the QT libQt5*****.so reside
  # we need this so we can inject QT dependencies of external bundled libs
  # these need to be added to in libs.xml and to qfield.pri
  QT_LIBS_DIR = $$dirname(QMAKE_QMAKE)/../lib

}

