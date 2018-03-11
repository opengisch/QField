################################################
# DON'T EDIT THIS FILE, USE config.pri instead #
################################################

include( paths.pri )

!android {
  macx {
    QMAKE_LFLAGS += -F$${QGIS_INSTALL_PATH}/QGIS.app/Contents/Frameworks/
    LIBS += -F$${QGIS_INSTALL_PATH}/QGIS.app/Contents/Frameworks/
    LIBS += -framework qgis_core

    INCLUDEPATH += $${QGIS_INSTALL_PATH}/QGIS.app/Contents/Frameworks/qgis_core.framework/Versions/Current/Headers
    INCLUDEPATH += $${GEOS_INCLUDE_PATH}
  }
  !macx {
    INCLUDEPATH += $${QGIS_INSTALL_PATH}/include/qgis
    INCLUDEPATH += $${GEOS_INCLUDE_PATH}
    LIBS += $${QGIS_INSTALL_PATH}/lib/libqgis_core.so
  }
}
android {
  QGIS_INSTALL_PATH = $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/files
  INCLUDEPATH += $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/include/qgis
  LIBS += $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libqgis_core.so

  system( mkdir -p $$shadowed($$PWD)/tmp )
  CURRENT_PATH = $$shadowed($$PWD)
  system( "cd $$QGIS_INSTALL_PATH$$; rm $$shadowed($$PWD)/tmp/assets.zip; zip -r $$shadowed($$PWD)/tmp/assets.zip share/resources/ share/svg/; cd $$CURRENT_PATH$$" )
  qgis_assets.path = /assets
  qgis_assets.files = "$$shadowed($$PWD)/tmp/assets.zip"
  INSTALLS += qgis_assets
  qgis_providers.path = /libs/$$ANDROID_TARGET_ARCH$$
  qgis_providers.files = $$files( $$OSGEO4A_DIR/$$ANDROID_TARGET_ARCH$$/lib*provider.so )
  INSTALLS += qgis_providers
}

macx {
  DEFINES += "QGIS_PLUGIN_DIR=\\\"$$QGIS_INSTALL_PATH$$/QGIS.app/Contents/PlugIns/qgis\\\"" \
             "QGIS_INSTALL_DIR=\\\"$$QGIS_INSTALL_PATH$$\\\""
  QMAKE_RPATHDIR += $${QGIS_INSTALL_PATH}//Contents/MacOS/lib/
} else {
  DEFINES += "QGIS_PLUGIN_DIR=\\\"$$QGIS_INSTALL_PATH$$/lib/qgis/plugins/\\\"" \
             "QGIS_INSTALL_DIR=\\\"$$QGIS_INSTALL_PATH$$\\\""
  QMAKE_RPATHDIR += $${QGIS_INSTALL_PATH}/lib/
}

# Add QWT and QScintilla custom build paths if your distro only ships Qt4 versions
# and you had to build them manually for Qt5
QMAKE_RPATHDIR += $${QWT_DIR}
QMAKE_RPATHDIR += $${QSCINTILLA_DIR}
