// used: https://github.com/rschiang/material/blob/master/qml/material/PaperRipple.qml

import QtQuick 2.14
import Qt5Compat.GraphicalEffects

Item {
  id: control
  anchors.fill: parent

  property alias radius: mask.radius
  property color color: "#de000000"
  property alias mouseArea: connections.target

  Rectangle {
    id: mask
    anchors.fill: parent
    color: "black"
    visible: false
  }

  Item {
    id: container
    anchors.fill: parent
    visible: false
  }

  OpacityMask {
    anchors.fill: parent
    source: container
    maskSource: mask
  }

  Component {
    id: ripple

    Rectangle {
      id: ink
      radius: 0
      opacity: 0.25
      color: control.color
      property int startX
      property int startY
      property int maxRadius: 150

      x: startX - radius
      y: startY - radius
      width: radius * 2
      height: radius * 2

      NumberAnimation {
        id: growAnimation
        target: ink
        property: "radius"
        from: 0
        to: maxRadius
        duration: 550
        easing.type: Easing.OutCubic
      }

      SequentialAnimation {
        id: fadeAnimation
        NumberAnimation {
          target: ink
          property: "opacity"
          from: 0.25
          to: 0
          duration: 312
        }
        ScriptAction {
          script: ink.destroy()
        }
      }

      Connections {
        target: mouseArea
        function onReleased() {
          fadeIfApplicable()
        }
        function onExited() {
          fadeIfApplicable()
        }
        function onCanceled() {
          fadeIfApplicable()
        }
      }

      Component.onCompleted: {
        growAnimation.start()
        if (!(mouseArea.pressed || fadeAnimation.running))
          fadeAnimation.start()
      }

      function fadeIfApplicable() {
        if (!fadeAnimation.running)
          fadeAnimation.start()
      }
    }
  }

  Connections {
    id: connections
    function onPressed() {
      var wave = ripple.createObject(container, {
                                       "startX": target.mouseX,
                                       "startY": target.mouseY,
                                       "maxRadius": furthestDistance(target.mouseX, target.mouseY)
                                     })
    }
  }

  function distance(x1, y1, x2, y2) {
    return Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2))
  }

  function furthestDistance(x, y) {
    return Math.max(distance(x, y, 0, 0), distance(x, y, width, height), distance(x, y, 0, height), distance(x, y, width, 0))
  }
}
