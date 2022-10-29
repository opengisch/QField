import QtQuick 2.14
import QtQuick.Controls 2.14

import Theme 1.0

Item {
  id: screenLocker

  property bool enabled: false
  property double threshold: 200

  anchors.fill: parent
  visible: enabled

  onEnabledChanged: {
    if (enabled) {
      unlockNode.opacity = 1
      unlockOpacityTimer.restart()
    } else {
      unlockNode.opacity = 0
    }
  }

  // Grab stylus
  TapHandler {
    enabled: screenLocker.enabled
    target: null
    grabPermissions: PointerHandler.CanTakeOverFromAnything | PointerHandler.TakeOverForbidden
    acceptedDevices: PointerDevice.AllDevices
    acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton
  }

  // Grab drag
  DragHandler {
    enabled: screenLocker.enabled
    target: null
    grabPermissions: PointerHandler.CanTakeOverFromAnything | PointerHandler.TakeOverForbidden
    acceptedDevices: PointerDevice.AllDevices
    acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton
  }

  // Grab pinch
  PinchHandler {
    enabled: screenLocker.enabled
    target: null
    grabPermissions: PointerHandler.CanTakeOverFromAnything | PointerHandler.TakeOverForbidden
    acceptedDevices: PointerDevice.AllDevices
    acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton
  }

  // Grab wheel
  WheelHandler {
    enabled: screenLocker.enabled
    target: null
    grabPermissions: PointerHandler.CanTakeOverFromAnything | PointerHandler.TakeOverForbidden
    acceptedDevices: PointerDevice.AllDevices
    acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton
  }

  // Grab mouse (et al)
  MouseArea {
    enabled: screenLocker.enabled
    anchors.fill: parent

    onPressed: {
      unlockNode.opacity = 1
      unlockOpacityTimer.restart()
    }
  }

  Rectangle {
    id: unlockThreshold

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    width: screenLocker.threshold * 2
    height: screenLocker.threshold * 2
    color: "transparent"
    border.color: unlockHandler.unlocked ? Theme.mainColor : Theme.darkGray
    border.width: 2
    radius: width / 2
    opacity: unlockHandler.entered ? 1 : 0

    Behavior on opacity {
      NumberAnimation { duration: 250 }
    }
  }

  Rectangle {
    id: unlockNode

    property double offsetX: 0
    property double offsetY: 0

    x: (parent.width - width) / 2 + offsetX
    y: (parent.height - height) / 2 + offsetY

    width: 60
    height: 60
    color: Theme.darkGraySemiOpaque
    radius: 30
    opacity: 0

    Image {
      anchors.centerIn: parent
      width: 36
      height: 36
      fillMode: Image.PreserveAspectFit
      smooth: true
      source: unlockHandler.unlocked
              ? Theme.getThemeVectorIcon('ic_lock_open_green_24dp')
              : unlockHandler.entered
                ? Theme.getThemeVectorIcon('ic_lock_open_white_24dp')
                : Theme.getThemeVectorIcon('ic_lock_white_24dp')
      sourceSize.width: 96
      sourceSize.height: 96
    }

    Behavior on opacity {
      NumberAnimation { duration: 250 }
    }
  }

  DragHandler {
    id: unlockHandler
    enabled: screenLocker.enabled
    target: null
    grabPermissions: PointerHandler.CanTakeOverFromAnything | PointerHandler.TakeOverForbidden
    acceptedDevices: PointerDevice.AllDevices
    acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton

    property double centerX: parent.width / 2
    property double centerY: parent.height / 2
    property bool entered: false
    property bool unlocked: false

    onActiveChanged: {
      if (active) {
        var x = centroid.position.x - centerX
        var y = centroid.position.y - centerY
        var distance = Math.sqrt(Math.pow(x, 2) + Math.pow(y, 2))
        if (distance < screenLocker.threshold / 3) {
          unlockOpacityTimer.stop()
          entered = true
        }
      } else {
        if (unlocked) {
          screenLocker.enabled = false
        } else {
          unlockOpacityTimer.restart()
        }
        entered = false
        unlocked = false
        unlockNode.offsetX = 0
        unlockNode.offsetY = 0
      }
    }

    onCentroidChanged: {
      if (entered) {
        var x = centroid.position.x - centerX
        var y = centroid.position.y - centerY
        var distance = Math.sqrt(Math.pow(x, 2) + Math.pow(y, 2))
        if (distance < screenLocker.threshold) {
          unlockNode.offsetX = x
          unlockNode.offsetY = y
          unlocked = false
        } else {
          var angle = Math.atan2(x, y)
          unlockNode.offsetX = screenLocker.threshold * Math.sin(angle)
          unlockNode.offsetY = screenLocker.threshold * Math.cos(angle)
          unlocked = true
        }
      }
    }
  }

  Timer {
    id: unlockOpacityTimer
    interval: 2500
    repeat: false
    running: false

    onTriggered: {
      unlockNode.opacity = 0
    }
  }
}

