################################################
# DON'T EDIT THIS FILE, USE config.pri instead #
################################################

include( paths.pri )

QMAKE_CXXFLAGS += -std=c++11

android {
  LIBS += -L$${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/lib                      
  INCLUDEPATH += $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/include      

  ANDROID_DEPLOYMENT_DEPENDENCIES += \
    jar/QtAndroid-bundled.jar \
    jar/QtAndroidAccessibility-bundled.jar \
    jar/QtMultimedia-bundled.jar \
    lib/libQt5Core.so \
    lib/libQt5Gui.so \
    lib/libQt5Network.so \
    lib/libQt5Qml.so \
    lib/libQt5Quick.so \
    lib/libQt5PrintSupport.so \
    lib/libQt5Widgets.so \
    lib/libQt5Svg.so \
    lib/libQt5Sql.so \
    lib/libQt5Xml.so \
    lib/libQt5OpenGL.so \
    lib/libQt5Sensors.so \
    lib/libQt5Concurrent.so \
    lib/libQt5Positioning.so \
    plugins/platforms/android/libqtforandroid.so \
    qml/QtQuick.2/qmldir \
    qml/QtQuick.2/libqtquick2plugin.so \
    qml/QtQuick/Dialogs/qmldir \
    qml/QtQuick/Dialogs/libdialogplugin.so \
    qml/QtQuick/Dialogs/Private/qmldir \
    qml/QtQuick/Dialogs/Private/libdialogsprivateplugin.so \
    qml/QtQuick/Controls/qmldir \
    qml/QtQuick/Controls/libqtquickcontrolsplugin.so \
    qml/QtQuick/Layouts/qmldir \
    qml/QtQuick/Layouts/libqquicklayoutsplugin.so \
    qml/QtQuick/Window.2/qmldir \
    qml/QtQuick/Window.2/libwindowplugin.so \
    qml/Qt/labs/folderlistmodel/qmldir \
    qml/Qt/labs/folderlistmodel/libqmlfolderlistmodelplugin.so \
    qml/Qt/labs/settings/qmldir \
    qml/Qt/labs/settings/libqmlsettingsplugin.so \
    qml/QtQuick/Controls/Styles/Android/qmldir \
    qml/QtQuick/Controls/Styles/Android/ButtonStyle.qml \
    qml/QtQuick/Controls/Styles/Android/BusyIndicatorStyle.qml \
    qml/QtQuick/Controls/Styles/Android/ComboBoxStyle.qml \
    qml/QtQuick/Controls/Styles/Android/TextFieldStyle.qml \
    qml/QtQuick/Controls/Styles/Android/ToolBarStyle.qml \
    qml/QtQuick/Controls/Styles/Android/FocusFrameStyle.qml \
    qml/QtQuick/Controls/Styles/Android/TableViewStyle.qml \
    qml/QtQuick/Controls/Styles/Android/ScrollViewStyle.qml \
    qml/QtQuick/Controls/Styles/Android/drawables/AnimationDrawable.qml \
    qml/QtQuick/Controls/Styles/Android/libqtquickcontrolsandroidstyleplugin.so \
    qml/QtQml/StateMachine/qmldir \
    qml/QtQml/StateMachine/libqtqmlstatemachine.so \
    qml/QtQml/Models.2/qmldir \
    qml/QtQml/Models.2/libmodelsplugin.so

  ANDROID_EXTRA_LIBS = \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libexpat.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libgeos.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libgeos_c.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libgslcblas.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libsqlite3.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libcharset.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libiconv.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libfreexl.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libtiff.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libgdal.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libproj.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libspatialindex.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libqwt.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libpq.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libqscintilla2.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libspatialite.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libqgis_core.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libqgis_gui.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libspatialiteprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libdelimitedtextprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libgdalprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libgpxprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libmemoryprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libmssqlprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libogrprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libowsprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libpostgresprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libspatialiteprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libwcsprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libwfsprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libwmsprovider.so
}

