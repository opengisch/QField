import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import Theme

/**
 * \ingroup qml
 */
T.ScrollBar {
  id: control

  property color color: Theme.mainColor
  property color backgroundColor: Theme.scrollBarBackgroundColor
  property real _maxSize: 8
  property real _minSize: 4

  width: horizontal ? parent.width : _maxSize
  height: horizontal ? _maxSize : parent.height
  visible: control.policy !== T.ScrollBar.AlwaysOff
  anchors.right: parent.right
  background: Rectangle {
    id: background
    radius: _minSize
    color: control.backgroundColor
    opacity: {
      if (vertical) {
        return handle.width === _maxSize;
      }
      return handle.height === _maxSize;
    }
    Behavior on opacity {
      NumberAnimation {
        duration: 20
      }
    }
  }

  contentItem: Item {
    property bool collapsed: (control.policy === T.ScrollBar.AlwaysOn || (control.active && control.size < 1.0))
    implicitWidth: control.interactive ? _maxSize : _minSize
    implicitHeight: Math.max(10, control.interactive ? _maxSize : _minSize)

    Rectangle {
      id: handle
      width: vertical ? _minSize : parent.width
      height: horizontal ? _minSize : Math.max(10, parent.height)
      color: control.color
      anchors {
        right: vertical ? parent.right : undefined
        bottom: horizontal ? parent.bottom : undefined
      }
      radius: width / 2
      visible: control.size < 1.0
    }

    states: [
      State {
        name: "fullyVisible"
        when: contentItem.collapsed
        PropertyChanges {
          target: handle
          width: vertical ? _maxSize : parent.width
          height: horizontal ? _maxSize : Math.max(10, parent.height)
        }
      },
      State {
        name: "semiVisible"
        when: !contentItem.collapsed
        PropertyChanges {
          target: handle
          width: vertical ? _minSize : parent.width
          height: horizontal ? _minSize : Math.max(10, parent.height)
        }
      }
    ]

    transitions: [
      Transition {
        to: "semiVisible"
        NumberAnimation {
          target: handle
          properties: vertical ? "width" : "height"
          duration: 170
          easing.type: Easing.OutCubic
        }
      },
      Transition {
        to: "fullyVisible"
        NumberAnimation {
          target: handle
          properties: vertical ? "width" : "height"
          duration: 170
          easing.type: Easing.OutCubic
        }
      }
    ]
  }
}
