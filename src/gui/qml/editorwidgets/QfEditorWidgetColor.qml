import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

QfEditorWidgetBase {
  id: colorControl

  height: childrenRect.height

  anchors {
    right: parent.right
    left: parent.left
  }

  SwipeView {
    id: colorContainer

    anchors {
      top: parent.top
      left: parent.left
      right: parent.right
    }

    height: Math.max(Material.textFieldHeight + 2, QfTheme.toolButtonSize)
    clip: true
    interactive: false
    currentIndex: 0

    enabled: isEnabled
    onEnabledChanged: {
      if (colorContainer.currentIndex > 0) {
        colorContainer.currentIndex = 0;
      }
    }

    RowLayout {
      id: currentColorView
      width: colorControl.width
      spacing: 5

      Rectangle {
        id: colorArea
        Layout.fillWidth: true
        Layout.preferredHeight: Material.textFieldHeight
        Layout.alignment: Qt.AlignVCenter

        color: value == null ? "transparent" : value
        radius: 4

        TextField {
          anchors.fill: parent
          enabled: isEnabled
          visible: isEnabled
          Material.containerStyle: Material.Outlined

          onActiveFocusChanged: {
            if (activeFocus) {
              colorPickerButton.clicked();
            }
          }
        }

        MouseArea {
          anchors.fill: parent
          enabled: isEnabled

          onClicked: {
            colorPickerButton.clicked();
          }
        }
      }

      QfToolButton {
        id: colorPickerButton
        Layout.preferredWidth: QfTheme.toolButtonSize
        Layout.preferredHeight: QfTheme.toolButtonSize
        Layout.alignment: Qt.AlignVCenter
        visible: isEnabled
        enabled: isEnabled
        iconSource: QfTheme.getThemeVectorIcon("ic_ellipsis_black_24dp")
        iconColor: QfTheme.mainTextColor
        bgcolor: "transparent"

        onClicked: {
          colorContainer.currentIndex = 1;
        }
      }
    }

    RowLayout {
      id: selectColorView
      width: colorControl.width
      spacing: 5

      ListView {
        Layout.fillWidth: true
        Layout.preferredHeight: QfTheme.toolButtonSize
        orientation: ListView.Horizontal
        spacing: 10
        model: QfProjectUtils.colorPresets(qgisProject)

        clip: true

        delegate: QfToolButton {
          Layout.preferredWidth: QfTheme.toolButtonSize
          Layout.preferredHeight: QfTheme.toolButtonSize
          bgcolor: modelData
          round: true

          iconSource: modelData === value ? QfTheme.getThemeVectorIcon("ic_check_white_24dp") : ""
          iconColor: "#ffffff"

          onClicked: {
            valueChangeRequested(modelData, false);
            colorContainer.currentIndex = 0;
          }
        }
      }
    }
  }
}
