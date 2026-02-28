

# File QfToolButtonPie.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfToolButtonPie.qml**](QfToolButtonPie_8qml.md)

[Go to the documentation of this file](QfToolButtonPie_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import org.qfield

Menu {
  id: pieMenu
  scale: 0
  opacity: 0

  readonly property int numberOfButtons: menuItemsView.count

  property int bandWidth: 48
  property real openingAngle: 0

  property alias strokeColor: shapePath.strokeColor
  property color linkColor: Theme.positionColor
  property var targetPoint: null
  property bool showConnectionLine: false

  property Component centralActionComponent: null
  property bool centralActionVisible: false

  QtObject {
    id: internal
    property int animationDuration: 75
    property real outerRadius: pieMenu.width / 2
    property real innerRadius: outerRadius - bandWidth
    property real pathRadius: (innerRadius + outerRadius + bandWidth) / 2
    property real centralButtonSize: Math.min(innerRadius * 1.4, bandWidth)
  }

  background: Shape {
    id: connectionShape
    visible: pieMenu.showConnectionLine && pieMenu.targetPoint !== null
    opacity: visible ? 1 : 0

    Behavior on opacity {
      NumberAnimation {
        duration: internal.animationDuration
      }
    }

    readonly property real markerCircleRadius: 3
    readonly property real menuCircleRadius: 6

    ShapePath {
      strokeWidth: 2
      strokeColor: pieMenu.linkColor
      fillColor: "transparent"

      startX: width / 2
      startY: height / 2 + 4

      PathLine {
        x: pieMenu.targetPoint ? pieMenu.targetPoint.x - pieMenu.x : 0
        y: pieMenu.targetPoint ? pieMenu.targetPoint.y - pieMenu.y : 0
      }
    }

    ShapePath {
      strokeWidth: 0
      strokeColor: "transparent"
      fillColor: pieMenu.linkColor

      PathAngleArc {
        centerX: width / 2
        centerY: height / 2 + 4
        radiusX: connectionShape.menuCircleRadius
        radiusY: connectionShape.menuCircleRadius
        startAngle: 0
        sweepAngle: 360
      }
    }

    ShapePath {
      strokeWidth: 0
      strokeColor: "transparent"
      fillColor: pieMenu.linkColor

      PathAngleArc {
        centerX: pieMenu.targetPoint ? pieMenu.targetPoint.x - pieMenu.x : 0
        centerY: pieMenu.targetPoint ? pieMenu.targetPoint.y - pieMenu.y : 0
        radiusX: connectionShape.markerCircleRadius
        radiusY: connectionShape.markerCircleRadius
        startAngle: 0
        sweepAngle: 360
      }
    }
  }

  contentItem: Item {
    Shape {
      id: shape
      x: 0
      y: -4

      ShapePath {
        id: shapePath
        strokeWidth: pieMenu.bandWidth
        strokeColor: Qt.hsla(Theme.toolButtonBackgroundColor.hslHue, Theme.toolButtonBackgroundColor.hslSaturation, Theme.toolButtonBackgroundColor.hslLightness, 0.3)
        fillColor: "transparent"
        capStyle: ShapePath.RoundCap

        PathAngleArc {
          centerX: pieMenu.width / 2
          centerY: pieMenu.height / 2
          radiusX: pieMenu.width / 2
          radiusY: pieMenu.height / 2
          startAngle: 270
          sweepAngle: pieMenu.openingAngle
        }
      }
    }

    PathView {
      id: menuItemsView
      x: 0
      y: -4
      interactive: false
      model: pieMenu.contentModel

      delegate: Item {}

      path: Path {
        startX: pieMenu.width / 2
        startY: pieMenu.height / 2 - internal.pathRadius

        PathArc {
          x: pieMenu.width / 2
          y: pieMenu.height / 2 + internal.pathRadius
          radiusX: internal.pathRadius
          radiusY: internal.pathRadius
          useLargeArc: true
        }

        PathArc {
          x: pieMenu.width / 2
          y: pieMenu.height / 2 - internal.pathRadius
          radiusX: internal.pathRadius
          radiusY: internal.pathRadius
          useLargeArc: true
        }
      }

      preferredHighlightBegin: 0
      preferredHighlightEnd: 0
      highlightRangeMode: PathView.NoHighlightRange
      pathItemCount: pieMenu.contentModel.count
      snapMode: PathView.SnapOneItem
    }

    MouseArea {
      anchors.fill: parent
      anchors.margins: internal.outerRadius - internal.innerRadius
      propagateComposedEvents: true
      onClicked: function (event) {
        if (pieMenu.visible)
          pieMenu.close();
        else
          pieMenu.open();
        event.accepted = false;
      }
    }

    Item {
      id: centralActionContainer
      anchors.centerIn: parent
      width: internal.centralButtonSize + 12
      height: internal.centralButtonSize + 12
      visible: pieMenu.centralActionVisible
      opacity: visible ? 1 : 0

      Behavior on opacity {
        NumberAnimation {
          duration: internal.animationDuration
        }
      }

      Rectangle {
        id: centralBg
        anchors.fill: parent
        color: Qt.hsla(Theme.toolButtonBackgroundColor.hslHue, Theme.toolButtonBackgroundColor.hslSaturation, Theme.toolButtonBackgroundColor.hslLightness, 0.3)
        radius: width / 2
      }

      Loader {
        id: centralActionLoader
        anchors.centerIn: parent
        sourceComponent: pieMenu.centralActionComponent
        active: pieMenu.centralActionComponent !== null
      }
    }
  }

  NumberAnimation {
    id: progressAnimation
    target: pieMenu
    property: "openingAngle"
    from: 0
    to: 360
    duration: internal.animationDuration * 2
    easing.type: Easing.InOutQuad
  }

  onAboutToShow: {
    pieMenu.opacity = 1;
    pieMenu.scale = 1;
    pieMenu.openingAngle = 0;
    progressAnimation.start();
  }

  onAboutToHide: {
    pieMenu.opacity = 0;
    pieMenu.scale = 0;
    pieMenu.visible = false;
  }
}
```


