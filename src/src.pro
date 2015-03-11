CONFIG += qt
CONFIG += ordered

TARGET = qfield

QT += widgets quick concurrent xml positioning

include( ../qfield.pri )
include( ../qgis.pri )

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
  ANDROID_PACKAGE_SOURCE_DIR=$$PWD/../android

  DISTFILES += \
      android/gradle/wrapper/gradle-wrapper.jar \
      android/AndroidManifest.xml \
      android/gradlew.bat \
      android/res/values/libs.xml \
      android/build.gradle \
      android/gradle/wrapper/gradle-wrapper.properties \
      android/gradlew
}
