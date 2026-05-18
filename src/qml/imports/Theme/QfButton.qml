import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import QtQuick.Effects
import QtQuick.Shapes
import org.qfield

/**
 * \ingroup qml
 */
Button {
  id: button

  property color bgcolor: Theme.buttonBackgroundColor
  property color color: button.enabled ? Theme.buttonColor : Theme.mainTextDisabledColor
  property alias radius: backgroundRectangle.radius
  property alias borderColor: backgroundRectangle.border.color
  property bool dropdown: false
  property real progressValue: 0.0
  property bool showProgress: false

  signal dropdownClicked

  verticalPadding: Material.buttonVerticalPadding
  leftPadding: Math.max((dropdown ? 40 : 0), Material.buttonLeftPadding(flat, hasIcon && (display !== AbstractButton.TextOnly)))
  rightPadding: Math.max((dropdown ? 40 : 0), Material.buttonRightPadding(flat, hasIcon && (display !== AbstractButton.TextOnly), (text !== "") && (display !== AbstractButton.IconOnly)))
  topInset: 2
  bottomInset: 2
  leftInset: 4
  rightInset: 4
  focusPolicy: Qt.NoFocus

  icon.color: button.color
  font: Theme.defaultFont

  background: Rectangle {
    id: backgroundRectangle
    anchors.fill: parent
    color: !button.enabled ? Theme.controlBackgroundDisabledColor : button.bgcolor
    radius: 12
    border.width: 1
    border.color: !parent.enabled ? Theme.controlBackgroundDisabledColor : button.bgcolor != "#00000000" ? button.bgcolor : button.color
    clip: true

    Ripple {
      clip: true
      width: parent.width
      height: parent.height
      pressed: button.down
      anchor: parent
      active: button.down
      color: Theme.darkTheme ? "#22000000" : button.bgcolor == "#ffffff" || button.bgcolor == "#00000000" ? "#10000000" : "#22ffffff"
    }

    Loader {
      active: showProgress
      sourceComponent: progressComponent
    }

    Component {
      id: progressComponent

      Shape {
        x: progressPath.strokeWidth / 2
        y: progressPath.strokeWidth / 2
        ShapePath {
          id: progressPath
          strokeColor: Theme.mainColor
          strokeWidth: 2
          strokeStyle: ShapePath.DashLine
          fillColor: "transparent"
          capStyle: ShapePath.RoundCap

          property real shapeWidth: button.width - progressPath.strokeWidth
          property real shapeHeight: button.height - progressPath.strokeWidth
          property real shapeRadius: button.radius - progressPath.strokeWidth
          property real shapeLength: {
            const circumferenceLength = 2 * Math.PI * progressPath.shapeRadius;
            const straightLinesLength = 2 * (progressPath.shapeWidth - (2 * progressPath.shapeRadius)) + 2 * (progressPath.shapeHeight - (2 * progressPath.shapeRadius));
            return (circumferenceLength + straightLinesLength) / progressPath.strokeWidth;
          }

          dashPattern: dashAnimation.running ? [progressPath.shapeLength * 0.2, progressPath.shapeLength - progressPath.shapeLength * 0.2] : [progressPath.shapeLength, progressPath.shapeLength * 2]
          dashOffset: dashAnimation.running ? progressPath.shapeLength * progressPath.dashOffsetMultiplier : progressPath.shapeLength * (1 - button.progressValue)
          property real dashOffsetMultiplier: 1
          NumberAnimation on dashOffsetMultiplier {
            id: dashAnimation
            running: button.progressValue == 0
            from: 1
            to: 0
            duration: 3000
            loops: Animation.Infinite
            easing.type: Easing.Linear
          }

          startX: progressPath.shapeRadius
          startY: 0

          PathLine {
            x: progressPath.shapeWidth - progressPath.shapeRadius
            y: 0
          }

          PathArc {
            x: progressPath.shapeWidth
            y: progressPath.shapeRadius
            radiusX: progressPath.shapeRadius
            radiusY: progressPath.shapeRadius
            useLargeArc: false
            direction: PathArc.Clockwise
          }

          PathLine {
            x: progressPath.shapeWidth
            y: progressPath.shapeHeight - progressPath.shapeRadius
          }

          PathArc {
            x: progressPath.shapeWidth - progressPath.shapeRadius
            y: progressPath.shapeHeight
            radiusX: progressPath.shapeRadius
            radiusY: progressPath.shapeRadius
            direction: PathArc.Clockwise
          }

          PathLine {
            x: progressPath.shapeRadius
            y: progressPath.shapeHeight
          }

          PathArc {
            x: 0
            y: progressPath.shapeHeight - progressPath.shapeRadius
            radiusX: progressPath.shapeRadius
            radiusY: progressPath.shapeRadius
            direction: PathArc.Clockwise
          }

          PathLine {
            x: 0
            y: progressPath.shapeRadius
          }

          PathArc {
            x: progressPath.shapeRadius
            y: 0
            radiusX: progressPath.shapeRadius
            radiusY: progressPath.shapeRadius
            direction: PathArc.Clockwise
          }
        }
      }
    }
  }

  contentItem: IconLabel {
    id: label
    spacing: parent.spacing
    mirrored: parent.mirrored
    display: parent.display

    icon: parent.icon
    color: button.color
    font {
      pointSize: button.font.pointSize
    }

    alignment: Qt.AlignCenter | Qt.AlignVCenter
    text: parent.text
  }

  Rectangle {
    id: dropdownArea
    anchors.right: parent.right
    anchors.verticalCenter: parent.verticalCenter
    visible: button.dropdown

    height: parent.height
    width: height

    color: "transparent"

    Shape {
      id: dropdownArrow
      anchors.centerIn: parent
      width: 20
      height: 20
      visible: true
      opacity: button.enabled ? 1 : 0.25

      ShapePath {
        strokeWidth: 2
        strokeColor: button.color
        fillColor: "transparent"

        startX: dropdownArrow.width * 0.25
        startY: dropdownArrow.height * 0.35

        PathLine {
          x: dropdownArrow.width * 0.5
          y: dropdownArrow.height * 0.55
        }
        PathLine {
          x: dropdownArrow.width * 0.75
          y: dropdownArrow.height * 0.35
        }
      }
    }

    property bool pressed: false
    states: [
      State {
        when: dropdownArea.pressed
        PropertyChanges {
          target: dropdownArea
          opacity: 0.25
        }
      },
      State {
        when: !dropdownArea.pressed
        PropertyChanges {
          target: dropdownArea
          opacity: 1
        }
      }
    ]
    transitions: [
      Transition {
        NumberAnimation {
          target: dropdownArea
          properties: "opacity"
          easing.type: Easing.InOutQuad
          duration: 250
        }
      }
    ]

    MouseArea {
      anchors.fill: parent

      onClicked: button.dropdownClicked()
      onPressed: dropdownArea.pressed = true
      onReleased: dropdownArea.pressed = false
      onCanceled: dropdownArea.pressed = false
    }
  }
}
