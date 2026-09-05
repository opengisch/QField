

# File QfEditorWidgetColor.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_aa4aab3cdee284f0e217d9df55b13787.md) **>** [**QfEditorWidgetColor.qml**](QfEditorWidgetColor_8qml.md)

[Go to the documentation of this file](QfEditorWidgetColor_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

QfEditorWidgetBase {
  id: colorControl

  property double desiredHeight: QfTheme.toolButtonSize

  height: desiredHeight

  anchors {
    right: parent.right
    left: parent.left
  }

  onEnabledChanged: {
    if (!isEnabled && colorContainer.currentIndex > 0) {
      colorContainer.currentIndex = 0;
    }
  }

  SwipeView {
    id: colorContainer

    anchors {
      top: parent.top
      left: parent.left
      right: parent.right
    }

    clip: true
    interactive: false
    currentIndex: 0

    RowLayout {
      id: currentColorView
      width: colorControl.width
      height: QfTheme.toolButtonSize
      spacing: 5

      Rectangle {
        id: colorArea
        Layout.fillWidth: true
        Layout.preferredHeight: QfTheme.toolButtonSize
        height: QfTheme.toolButtonSize
        radius: height / 2

        color: value == null ? "transparent" : value

        QfToolButton {
          anchors.right: parent.right
          width: QfTheme.toolButtonSize
          height: QfTheme.toolButtonSize
          visible: isEnabled
          enabled: false
          iconSource: QfTheme.getThemeVectorIcon("ic_ellipsis_black_24dp")
          iconColor: "white"
          bgcolor: "transparent"
        }

        MouseArea {
          anchors.fill: parent
          enabled: isEnabled

          onClicked: {
            colorContainer.currentIndex = 1;
          }
        }
      }
    }

    RowLayout {
      id: selectColorView
      width: colorControl.width
      height: QfTheme.toolButtonSize
      spacing: 5

      ListView {
        Layout.fillWidth: true
        Layout.preferredHeight: QfTheme.toolButtonSize
        orientation: ListView.Horizontal
        spacing: 10
        model: ["#e41a1c", "#377eb8", "#4daf4a", "#984ea3", "#ff7f00"]

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
```


