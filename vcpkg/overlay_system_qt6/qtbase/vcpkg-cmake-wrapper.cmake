set(QT_MODULES
  Qt6
  Qt63DAnimation
  Qt63DCore
  Qt63DExtras
  Qt63DInput
  Qt63DLogic
  Qt63DQuick
  Qt63DQuickAnimation
  Qt63DQuickExtras
  Qt63DQuickInput
  Qt63DQuickRender
  Qt63DQuickScene2D
  Qt63DRender
  Qt6AccessibilitySupport
  Qt6AttributionsScannerTools
  Qt6Bluetooth
  Qt6Bootstrap
  Qt6BundledPcre2Config
  Qt6Charts
  Qt6ClipboardSupport
  Qt6Concurrent
  Qt6Core
  Qt6Core5Compat
  Qt6DeviceDiscoverySupport
  Qt6DocTools
  Qt6EdidSupport
  Qt6EglSupport
  Qt6EntryPointPrivate
  Qt6EventDispatcherSupport
  Qt6FbSupport
  Qt6FontDatabaseSupport
  Qt6Gamepad
  Qt6Gui
  Qt6GraphicsSupport
  Qt6Help
  Qt6HostInfo
  Qt6InputSupport
  Qt6Location
  Qt6MacExtras
  Qt6Multimedia
  Qt6MultimediaQuick
  Qt6MultimediaWidgets
  Qt6Network
  Qt6Nfc
  Qt6OpenGL
  Qt6OpenGLExtensions
  Qt6PacketProtocol
  Qt6PlatformCompositorSupport
  Qt6Positioning
  Qt6PositioningQuick
  Qt6PrintSupport
  Qt6Qml
  Qt6QmlCompilerPrivate
  Qt6QmlCore
  Qt6QmlDebugPrivate
  Qt6QmlDomPrivate
  Qt6QmlIntegration
  Qt6QmlLocalStorage
  Qt6QmlModels
  Qt6QmlWorkerScript
  Qt6QmlXmlListModel
  Qt6Quick
  Qt6QuickControls2
  Qt6QuickDialogs2
  Qt6QuickLayouts
  Qt6QuickParticlesPrivate
  Qt6QuickShapesPrivate
  Qt6QuickTemplates2
  Qt6QuickTest
  Qt6QuickWidgets
  Qt6RemoteObjects
  Qt6RepParser
  Qt6Scxml
  Qt6Sensors
  Qt6SerialPort
  Qt6ServiceSupport
  Qt6Sql
  Qt6Svg
  Qt6Test
  Qt6TextToSpeech
  Qt6ThemeSupport
  Qt6UiPlugin
  Qt6UiTools
  Qt6VulkanSupport
  Qt6WebChannel
  Qt6WebSockets
  Qt6WebView
  Qt6Widgets
  Qt6Xml
  Qt6XmlPatterns
  Qt6Zlib
  Qt6BundledPcre2
)

foreach(MOD ${QT_MODULES})
  set(${MOD}_DIR $ENV{Qt6_DIR}/lib/cmake/${MOD})
endforeach()

if("${CMAKE_HOST_SYSTEM}" MATCHES "Darwin")
    set(_QT_HOST_INSTALL_DIR "macos")
else() # if("${CMAKE_HOST_SYSTEM}" MATCHES "Linux")
    set(_QT_HOST_INSTALL_DIR "gcc_64")
endif()

set(Qt6CoreTools_DIR $ENV{Qt6_DIR}/../${_QT_HOST_INSTALL_DIR}/lib/cmake/Qt6CoreTools)
set(Qt6GuiTools_DIR $ENV{Qt6_DIR}/../${_QT_HOST_INSTALL_DIR}/lib/cmake/Qt6GuiTools)
set(Qt6LinguistTools_DIR $ENV{Qt6_DIR}/../${_QT_HOST_INSTALL_DIR}/lib/cmake/Qt6LinguistTools)
set(Qt6QmlTools_DIR $ENV{Qt6_DIR}/../${_QT_HOST_INSTALL_DIR}/lib/cmake/Qt6QmlTools)
set(Qt6QmlImportScanner_DIR $ENV{Qt6_DIR}/../${_QT_HOST_INSTALL_DIR}/lib/cmake/Qt6QmlImportScanner)
set(Qt6WidgetsTools_DIR $ENV{Qt6_DIR}/../${_QT_HOST_INSTALL_DIR}/lib/cmake/Qt6WidgetsTools)

_find_package(${ARGS})
