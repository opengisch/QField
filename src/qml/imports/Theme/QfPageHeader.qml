import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Theme

ToolBar {
  property alias title: titleLabel.text

  property bool backgroundFill: true
  property bool backAsCancel: false

  property alias showBackButton: backButton.visible
  property alias showApplyButton: applyButton.visible
  property alias showCancelButton: cancelButton.visible
  property alias showMenuButton: menuButton.visible

  property alias busyIndicatorState: busyIndicator.state
  property alias busyIndicatorValue: busyIndicator.value

  property double topMargin: 0.0

  height: topMargin + 48

  signal cancel
  signal apply
  signal back
  signal finished
  signal openMenu

  anchors {
    top: parent.top
    left: parent.left
    right: parent.right
  }

  background: Rectangle {
    id: backgroundRect
    color: backgroundFill ? Theme.mainColor : "transparent"

    ProgressBar {
      id: busyIndicator
      anchors.top: parent.bottom
      anchors.left: parent.left
      width: parent.width
      height: 6
      value: 50
      indeterminate: value == 0 ? true : false

      state: "off"

      visible: opacity > 0

      states: [
        State {
          name: 'on'
          PropertyChanges {
            target: busyIndicator
            opacity: 1.0
          }
        },
        State {
          name: 'off'
          PropertyChanges {
            target: busyIndicator
            opacity: 0.0
          }
        }
      ]
      transitions: [
        Transition {
          from: "off"
          to: "on"
          SequentialAnimation {
            NumberAnimation {
              target: busyIndicator
              property: 'opacity'
              duration: 100
            }
          }
        },
        Transition {
          from: "on"
          to: "off"
          SequentialAnimation {
            PauseAnimation {
              duration: 100
            }
            NumberAnimation {
              target: busyIndicator
              property: 'opacity'
              duration: 200
            }
          }
        }
      ]
    }
  }
  Material.foreground: undefined

  RowLayout {
    anchors.fill: parent
    anchors.topMargin: topMargin
    Layout.margins: 0

    QfToolButton {
      id: backButton

      Layout.alignment: Qt.AlignTop | Qt.AlignLeft
      clip: true
      iconSource: backAsCancel ? Theme.getThemeVectorIcon('ic_close_white_24dp') : Theme.getThemeVectorIcon('ic_arrow_left_white_24dp')
      iconColor: backgroundFill ? Theme.mainOverlayColor : Theme.mainTextColor

      onClicked: {
        if (backAsCancel) {
          cancel();
        } else {
          back();
        }
        finished();
      }
    }

    QfToolButton {
      id: applyButton

      Layout.alignment: Qt.AlignTop | Qt.AlignLeft
      clip: true
      iconSource: Theme.getThemeVectorIcon('ic_check_white_24dp')
      iconColor: backgroundFill ? Theme.mainOverlayColor : Theme.mainTextColor

      onClicked: {
        apply();
        finished();
      }
    }

    Label {
      id: titleLabel
      leftPadding: !showApplyButton && showCancelButton ? 48 : 0
      rightPadding: (showApplyButton || showBackButton) && !showCancelButton && !showMenuButton ? 48 : 0
      font: Theme.strongFont
      color: backgroundFill ? Theme.mainOverlayColor : Theme.mainColor
      elide: Label.ElideRight
      horizontalAlignment: Qt.AlignHCenter
      verticalAlignment: Qt.AlignVCenter
      Layout.fillWidth: true
    }

    QfToolButton {
      id: cancelButton

      Layout.alignment: Qt.AlignTop | Qt.AlignRight
      clip: true
      iconSource: Theme.getThemeVectorIcon('ic_close_white_24dp')
      iconColor: backgroundFill ? Theme.mainOverlayColor : Theme.mainTextColor

      onClicked: {
        cancel();
        finished();
      }
    }

    QfToolButton {
      id: menuButton

      Layout.alignment: Qt.AlignTop | Qt.AlignRight
      clip: true
      visible: false

      iconSource: Theme.getThemeVectorIcon("ic_dot_menu_black_24dp")
      iconColor: Theme.mainOverlayColor

      onClicked: {
        openMenu();
      }
    }
  }
}
