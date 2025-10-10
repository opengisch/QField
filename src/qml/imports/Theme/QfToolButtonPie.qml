import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Container {
  id: pieMenuCircle

  scale: 0
  opacity: 0
  visible: opacity > 0

  property int shapeWidth: 48
  property int animationDuration: 200
  property real innerRadius: outerRadius - shapeWidth
  property real outerRadius: width / 2
  property real pathRadius: (innerRadius + outerRadius + shapeWidth) / 2
  property real currentAngle: 0

  contentItem: Item {
    Shape {
      id: shape
      anchors.fill: parent

      ShapePath {
        strokeWidth: pieMenuCircle.shapeWidth
        strokeColor: Qt.hsla(Theme.toolButtonBackgroundColor.hslHue, Theme.toolButtonBackgroundColor.hslSaturation, Theme.toolButtonBackgroundColor.hslLightness, 0.3)
        fillColor: "transparent"
        capStyle: ShapePath.RoundCap

        PathAngleArc {
          centerX: pieMenuCircle.width / 2
          centerY: pieMenuCircle.height / 2
          radiusX: pieMenuCircle.width / 2
          radiusY: pieMenuCircle.height / 2
          startAngle: 270
          sweepAngle: pieMenuCircle.currentAngle
        }
      }
    }

    PathView {
      id: menuItemsView
      anchors.fill: parent
      interactive: false
      model: pieMenuCircle.contentModel

      delegate: Item {
        width: pieMenuCircle.shapeWidth - 8
        height: pieMenuCircle.shapeWidth - 8
      }

      path: Path {
        startX: pieMenuCircle.width / 2
        startY: pieMenuCircle.height / 2 - pieMenuCircle.pathRadius

        PathArc {
          x: pieMenuCircle.width / 2
          y: pieMenuCircle.height / 2 + pieMenuCircle.pathRadius
          radiusX: pieMenuCircle.pathRadius
          radiusY: pieMenuCircle.pathRadius
          useLargeArc: true
        }

        PathArc {
          x: pieMenuCircle.width / 2
          y: pieMenuCircle.height / 2 - pieMenuCircle.pathRadius
          radiusX: pieMenuCircle.pathRadius
          radiusY: pieMenuCircle.pathRadius
          useLargeArc: true
        }
      }

      preferredHighlightBegin: 0
      preferredHighlightEnd: 0
      highlightRangeMode: PathView.NoHighlightRange
      pathItemCount: pieMenuCircle.contentModel.count
      snapMode: PathView.SnapOneItem
    }

    MouseArea {
      anchors.fill: parent
      anchors.margins: pieMenuCircle.outerRadius - pieMenuCircle.innerRadius
      propagateComposedEvents: true
      onClicked: event => {
        pieMenuCircle.toggleMenu();
        event.accepted = false;
      }
    }
  }

  NumberAnimation {
    id: progressAnimation
    target: pieMenuCircle
    property: "currentAngle"
    from: 0
    to: 360
    duration: pieMenuCircle.animationDuration * 2
    easing.type: Easing.InOutQuad
  }

  Behavior on scale  {
    NumberAnimation {
      duration: pieMenuCircle.animationDuration * 2
      easing.type: Easing.OutBack
    }
  }

  Behavior on opacity  {
    NumberAnimation {
      duration: pieMenuCircle.animationDuration * 2
    }
  }

  function toggleMenu() {
    if (!pieMenuCircle.visible) {
      pieMenuCircle.opacity = 1;
      pieMenuCircle.scale = 1;
      pieMenuCircle.currentAngle = 0;
      progressAnimation.start();
    } else {
      pieMenuCircle.opacity = 0;
      pieMenuCircle.scale = 0;
    }
  }
}
