import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl

Button {
  id: button

  property color bgcolor: Theme.mainColor
  property color color: button.enabled ? Theme.buttonTextColor : Theme.mainTextDisabledColor
  property alias radius: backgroundRectangle.radius
  property alias borderColor: backgroundRectangle.border.color
  property bool dropdown: false

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

    Ripple {
      clip: true
      width: parent.width
      height: parent.height
      pressed: button.down
      anchor: parent
      active: button.down
      color: Theme.darkTheme ? "#22000000" : button.bgcolor == "#ffffff" || button.bgcolor == "#00000000" ? "#10000000" : "#22ffffff"
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

    width: 36
    height: parent.height

    color: "transparent"

    Rectangle {
      anchors.left: parent.left
      anchors.verticalCenter: parent.verticalCenter

      width: 1.5
      height: parent.height - 16

      color: button.color
    }

    Canvas {
      id: dropdownArrow
      anchors.centerIn: parent
      implicitWidth: 40
      implicitHeight: 40
      visible: true
      opacity: 1

      onPaint: {
        var ctx = getContext("2d");
        ctx.fillStyle = button.color;
        ctx.strokeStyle = button.color;
        ctx.lineWidth = 1;
        ctx.moveTo(14, 15);
        ctx.lineTo(width - 16, 15);
        ctx.lineTo((width / 2) - 1, height - 15);
        ctx.stroke();
        ctx.fill();
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
