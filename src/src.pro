CONFIG += qt
CONFIG += ordered

TARGET = qfield

QT += widgets quick concurrent xml positioning

include( ../qfield.pri )
include( ../qgis.pri )
include( ../version.pri )


HEADERS += appinterface.h \
            coordinatetransform.h \
            crs.h \
            feature.h \
            featurelistextentcontroller.h \
            featurelistmodel.h \
            featurelistmodelhighlight.h \
            featurelistmodelselection.h \
            featuremodel.h \
            mapsettings.h \
            maptransform.h \
            qgismobileapp.h \
            qgsmapcanvasproxy.h \
            qgsquickmapcanvasmap.h \
            qgssggeometry.h \
            settings.h

SOURCES += appinterface.cpp \
            coordinatetransform.cpp \
            crs.cpp \
            feature.cpp \
            featurelistextentcontroller.cpp \
            featurelistmodel.cpp \
            featurelistmodelhighlight.cpp \
            featurelistmodelselection.cpp \
            featuremodel.cpp \
            main.cpp \
            mapsettings.cpp \
            maptransform.cpp \
            qgismobileapp.cpp \
            qgsmapcanvasproxy.cpp \
            qgsquickmapcanvasmap.cpp \
            qgssggeometry.cpp \
            settings.cpp

INCLUDEPATH += ../3rdparty/tessellate
LIBS += ../3rdparty/tessellate/libtessellate.a

RESOURCES += \
    ../images/images.qrc \
    qml/qml.qrc

android {
  message("* Using settings for Android $$ANDROID_TARGET_ARCH$$")
  message("* Using OSGEO4A for Android at $${OSGEO4A_STAGE_DIR}")

  #see http://doc.qt.io/qt-5/deployment-android.html#androiddeployqt for docs
  ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../android

  # That's already done in qgis.pri but it doesn't hurt doing it again for safety
  system( mkdir -p $$shadowed($$PWD)/../tmp )

  system( cp $$PWD/../templates/generated.xml $$PWD/../android/res/values/generated.xml )
  system( cp $$PWD/../templates/version.properties $$shadowed($$PWD)/../tmp/version.properties )
  GIT_REV = $$system( git --work-tree $$PWD rev-parse HEAD )
  system( sed -i \"s|%%GIT_REV%%|$$GIT_REV|g\" $$PWD/../android/res/values/generated.xml )
  system( sed -i \"s|%%APP_VERSION%%|$$VERSTR|g\" $$shadowed($$PWD)/../tmp/version.properties )
  system( sed -i \"s|%%APP_VERSIONCODE%%|$${VERSIONCODE}|g\" $$shadowed($$PWD)/../tmp/version.properties )

  gen_android_strings.path = /res/values
  gen_android_strings.files = "$$shadowed($$PWD)/../tmp/generated.xml"
  INSTALLS += gen_android_strings
  gen_android_version_props.path = /
  gen_android_version_props.files = "$$shadowed($$PWD)/../tmp/version.properties"
  INSTALLS += gen_android_version_props
  message(($$INSTALLS)$$shadowed($$PWD))
}

!android {
  message("* Using QGIS from $${QGIS_INSTALL_PATH}")
}
