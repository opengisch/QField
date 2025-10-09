import QtQuick
import QtQuick.Shapes

Shape {
  id: pieMenuCircle

  scale: 0
  opacity: 0
  visible: opacity > 0

  property alias model: menuItemsView.model
  property int shapeWidth: 48
  property int animationDuration: 200
  property real innerRadius: width / 2 - shapeWidth
  property real outerRadius: width / 2
  property real pathRadius: (innerRadius + outerRadius + shapeWidth) / 2
  property real currentAngle: 0

  ShapePath {
    strokeWidth: shapeWidth
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

  PathView {
    id: menuItemsView
    anchors.centerIn: parent
    width: parent.width
    height: width
    interactive: false

    delegate: QfToolButton {
      id: menuButton
      width: 40
      height: width
      radius: width / 2

      property real buttonAngle: (index / menuItemsModel.count) * 360

      visible: buttonAngle <= pieMenuCircle.currentAngle
      opacity: pieMenuCircle.scale === 0 ? 0 : (visible ? 1 : 0)
      scale: opacity

      Behavior on opacity  {
        NumberAnimation {
          duration: pieMenuCircle.animationDuration
          easing.type: Easing.OutQuad
        }
      }

      Behavior on scale  {
        NumberAnimation {
          duration: pieMenuCircle.animationDuration * 2
          easing.type: Easing.OutBack
        }
      }

      iconSource: {
        if (itemCheckable) {
          return checked ? Theme.getThemeVectorIcon(buttonStates()["On"]["icon"]) : Theme.getThemeVectorIcon(buttonStates()["Off"]["icon"]);
        }
        return Theme.getThemeVectorIcon(source);
      }
      iconColor: {
        if (Object.keys(buttonStates()).length > 0) {
          return checked ? buttonStates()["On"]["iconColor"] : buttonStates()["Off"]["iconColor"];
        }
        return Theme.light;
      }
      bgcolor: {
        if (Object.keys(buttonStates()).length > 0) {
          return checked ? buttonStates()["On"]["bgColor"] : buttonStates()["Off"]["bgColor"];
        }
        return Theme.toolButtonBackgroundColor;
      }
      round: true
      checkable: itemCheckable
      checked: checkable ? itemChecked() : false
      enabled: itemEnabled

      onClicked: {
        clickEvent(checked);
      }
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
    pathItemCount: menuItemsModel.count
    snapMode: PathView.SnapOneItem
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
