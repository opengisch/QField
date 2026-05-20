

# File Color.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_7621275d8427768f32eeef555af1c8d1.md) **>** [**Color.qml**](Color_8qml.md)

[Go to the documentation of this file](Color_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

EditorWidgetBase {
  id: colorControl

  property double desiredHeight: 48

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
      height: 48
      spacing: 5

      Rectangle {
        id: colorArea
        Layout.fillWidth: true
        Layout.preferredHeight: 48
        height: 48
        radius: height / 2

        color: value == null ? "transparent" : value

        QfToolButton {
          anchors.right: parent.right
          width: 48
          height: 48
          visible: isEnabled
          enabled: false
          iconSource: Theme.getThemeVectorIcon("ic_ellipsis_black_24dp")
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
      height: 48
      spacing: 5

      ListView {
        Layout.fillWidth: true
        Layout.preferredHeight: 48
        orientation: ListView.Horizontal
        spacing: 10
        model: ["#e41a1c", "#377eb8", "#4daf4a", "#984ea3", "#ff7f00"]

        clip: true

        delegate: QfToolButton {
          Layout.preferredWidth: 48
          Layout.preferredHeight: 48
          bgcolor: modelData
          round: true

          iconSource: modelData === value ? Theme.getThemeVectorIcon("ic_check_white_24dp") : ""
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


