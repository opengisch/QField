set(QT_MODULES Qt5 Qt5AndroidExtras Qt5Multimedia Qt5MultimediaQuick Qt5MultimediaWidgets Qt5WebChannel Qt5Gui Qt5Svg Qt5XmlPatterns Qt5PacketProtocol Qt5Qml Qt5QmlDebug Qt5QmlDevTools Qt5QmlImportScanner Qt5QmlModels Qt5QmlWorkerScript Qt5Quick Qt5QuickCompiler Qt5QuickParticles Qt5QuickShapes Qt5QuickTest Qt5QuickWidgets Qt5AttributionsScannerTools Qt5DocTools Qt5Help Qt5LinguistTools Qt5UiPlugin Qt5UiTools Qt5WebSockets Qt5Sensors Qt5SerialPort Qt5QuickControls2 Qt5QuickTemplates2 Qt5Location Qt5Positioning Qt5PositioningQuick Qt53DAnimation Qt53DCore Qt53DExtras Qt53DInput Qt53DLogic Qt53DQuick Qt53DQuickAnimation Qt53DQuickExtras Qt53DQuickInput Qt53DQuickRender Qt53DQuickScene2D Qt53DRender Qt5TextToSpeech Qt5Scxml Qt5RemoteObjects Qt5Core)

foreach(MOD ${QT_MODULES})
  set(${MOD}_DIR $ENV{Qt5_DIR}/lib/cmake/${MOD})
endforeach()
_find_package(${ARGS})
