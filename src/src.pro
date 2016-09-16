CONFIG += qt
CONFIG += ordered

TARGET = qfield

QT += widgets concurrent xml positioning printsupport svg sql opengl sensors quick
# MISSING in apk: printsupport svg sql opengl sensors

android {
    QT += androidextras
    HEADERS += androidplatformutilities.h \
               androidpicturesource.h
    SOURCES += androidplatformutilities.cpp \
               androidpicturesource.cpp
}
include( ../qfield.pri )
include( ../qgis.pri )
include( ../version.pri )


HEADERS += \
    appinterface.h \
    coordinatetransform.h \
    featurelistextentcontroller.h \
    featurelistmodel.h \
    featurelistmodelhighlight.h \
    featurelistmodelselection.h \
    featuremodel.h \
    mapsettings.h \
    maptransform.h \
    qgismobileapp.h \
    qgsquickmapcanvasmap.h \
    qgssggeometry.h \
    sgrubberband.h \
    settings.h \
    platformutilities.h \
    geometry.h \
    modelhelper.h \
    rubberbandmodel.h \
    rubberband.h \
    picturesource.h \
    messagelogmodel.h \
    attributeformmodel.h \
    layertreemapcanvasbridge.h \
    identifytool.h \
    submodel.h

SOURCES += \
    appinterface.cpp \
    coordinatetransform.cpp \
    featurelistextentcontroller.cpp \
    featurelistmodel.cpp \
    featurelistmodelhighlight.cpp \
    featurelistmodelselection.cpp \
    featuremodel.cpp \
    main.cpp \
    mapsettings.cpp \
    maptransform.cpp \
    qgismobileapp.cpp \
    qgsquickmapcanvasmap.cpp \
    qgssggeometry.cpp \
    sgrubberband.cpp \
    settings.cpp \
    platformutilities.cpp \
    geometry.cpp \
    modelhelper.cpp \
    rubberbandmodel.cpp \
    rubberband.cpp \
    picturesource.cpp \
    messagelogmodel.cpp \
    attributeformmodel.cpp \
    layertreemapcanvasbridge.cpp \
    identifytool.cpp \
    submodel.cpp

INCLUDEPATH += ../3rdparty/tessellate
LIBS += ../3rdparty/tessellate/libtessellate.a

# Make qml files translatable
lupdate_only{
  SOURCES = qml/*.qml
}

RESOURCES += \
    ../images/images.qrc \
    qml/qml.qrc \
    ../i18n/translations.qrc

android {
  message( "* Using settings for Android $$ANDROID_TARGET_ARCH$$" )
  message( "* Using OSGEO4A for Android at $${OSGEO4A_STAGE_DIR}" )

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
}

!android {
  message( "* Using QGIS from $${QGIS_INSTALL_PATH}" )
}

DISTFILES += \
    ../android/AndroidManifest.xml \
    ../android/gradle/wrapper/gradle-wrapper.jar \
    ../android/gradlew \
    ../android/res/values/libs.xml \
    ../android/build.gradle \
    ../android/gradle/wrapper/gradle-wrapper.properties \
    ../android/gradlew.bat \
    ../images/themes/qfield/hdpi/ic_camera_white_48dp.png \
    ../images/themes/qfield/mdpi/ic_camera_white_48dp.png \
    ../images/themes/qfield/xhdpi/ic_camera_white_48dp.png \
    ../images/themes/qfield/xxhdpi/ic_camera_white_48dp.png \
    ../images/themes/qfield/xxxhdpi/ic_camera_white_48dp.png
