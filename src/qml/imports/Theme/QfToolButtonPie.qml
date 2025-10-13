import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Menu {
  id: container
  scale: 0
  opacity: 0

  background: Item {
  }

  property int bandWidth: 48
  property real currentAngle: 0
  property alias strokeColor: shapePath.strokeColor

  QtObject {
    id: internal
    property int animationDuration: 200
    property real outerRadius: container.width / 2
    property real innerRadius: outerRadius - bandWidth
    property real pathRadius: (innerRadius + outerRadius + bandWidth) / 2
  }

  contentItem: Item {
    Shape {
      id: shape
      anchors.fill: parent

      ShapePath {
        id: shapePath
        strokeWidth: container.bandWidth
        strokeColor: Qt.hsla(Theme.toolButtonBackgroundColor.hslHue, Theme.toolButtonBackgroundColor.hslSaturation, Theme.toolButtonBackgroundColor.hslLightness, 0.3)
        fillColor: "transparent"
        capStyle: ShapePath.RoundCap

        PathAngleArc {
          centerX: container.width / 2
          centerY: container.height / 2
          radiusX: container.width / 2
          radiusY: container.height / 2
          startAngle: 270
          sweepAngle: container.currentAngle
        }
      }
    }

    PathView {
      id: menuItemsView
      anchors.fill: parent
      interactive: false
      model: container.contentModel

      delegate: Item {
      }

      path: Path {
        startX: container.width / 2
        startY: container.height / 2 - internal.pathRadius

        PathArc {
          x: container.width / 2
          y: container.height / 2 + internal.pathRadius
          radiusX: internal.pathRadius
          radiusY: internal.pathRadius
          useLargeArc: true
        }

        PathArc {
          x: container.width / 2
          y: container.height / 2 - internal.pathRadius
          radiusX: internal.pathRadius
          radiusY: internal.pathRadius
          useLargeArc: true
        }
      }

      preferredHighlightBegin: 0
      preferredHighlightEnd: 0
      highlightRangeMode: PathView.NoHighlightRange
      pathItemCount: container.contentModel.count
      snapMode: PathView.SnapOneItem
    }

    MouseArea {
      anchors.fill: parent
      anchors.margins: internal.outerRadius - internal.innerRadius
      propagateComposedEvents: true
      onClicked: function (event) {
        if (container.visible)
          container.close();
        else
          container.open();
        event.accepted = false;
      }
    }
  }

  NumberAnimation {
    id: progressAnimation
    target: container
    property: "currentAngle"
    from: 0
    to: 360
    duration: internal.animationDuration * 2
    easing.type: Easing.InOutQuad
  }

  onAboutToShow: {
    container.opacity = 1;
    container.scale = 1;
    container.currentAngle = 0;
    progressAnimation.start();
  }

  onAboutToHide: {
    container.opacity = 0;
    container.scale = 0;
    container.visible = false;
  }
}
