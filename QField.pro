TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += 3rdparty
SUBDIRS += src

ANDROID_PACKAGE_SOURCE_DIR=$$PWD/android

DISTFILES += \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/AndroidManifest.xml \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew
