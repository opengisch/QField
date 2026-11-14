

# File QfWelcomeCard.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfWelcomeCard.qml**](QfWelcomeCard_8qml.md)

[Go to the documentation of this file](QfWelcomeCard_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

SwipeView {
  id: root

  clip: true
  interactive: false
  property bool collapsed: false
  property bool uniformHeight: false
  readonly property real tallestHeight: {
    let tallest = 0;
    for (let i = 0; i < count; ++i) {
      const panel = itemAt(i);
      if (panel) {
        tallest = Math.max(tallest, panel.implicitHeight);
      }
    }
    return tallest;
  }
  Layout.preferredHeight: collapsed ? 0 : (uniformHeight ? tallestHeight : (currentItem ? currentItem.implicitHeight : implicitHeight))

  Behavior on implicitHeight {
    NumberAnimation {
      duration: 100
      easing.type: Easing.InQuad
    }
  }

  function collapse() {
    root.collapsed = true;
  }

  background: Rectangle {
    color: Qt.rgba(QfTheme.mainColor.r, QfTheme.mainColor.g, QfTheme.mainColor.b, 0.06)
    border.color: Qt.rgba(QfTheme.mainColor.r, QfTheme.mainColor.g, QfTheme.mainColor.b, 0.3)
    border.width: 1
    radius: 12
  }

  component Panel: Item {
    id: panel
    property alias title: titleText.text
    property alias message: messageText.text
    property alias messageMaximumLineCount: messageText.maximumLineCount
    property bool closable: false
    property bool collapseOnClose: true
    property url imageSource
    default property alias actions: actionRow.children

    signal closeClicked

    implicitHeight: panelLayout.implicitHeight

    Rectangle {
      id: imageFadeMask
      anchors.fill: backgroundImage
      radius: 11
      visible: false
      layer.enabled: true
      gradient: Gradient {
        orientation: Gradient.Horizontal
        GradientStop {
          position: 0.2
          color: "transparent"
        }
        GradientStop {
          position: 0.9
          color: "white"
        }
      }
    }

    Image {
      id: backgroundImage
      anchors.fill: parent
      anchors.margins: 1
      visible: status === Image.Ready
      source: panel.imageSource
      fillMode: Image.PreserveAspectCrop
      asynchronous: true
      cache: true
      opacity: 0.32

      layer.enabled: true
      layer.effect: QfOpacityMask {
        maskSource: imageFadeMask
        maskThresholdMin: 0.5
        maskSpreadAtMin: 1.0
        colorization: 1.0
        colorizationColor: QfTheme.mainColor
      }
    }

    ColumnLayout {
      id: panelLayout
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.top: parent.top
      anchors.bottom: panel.SwipeView.view && panel.SwipeView.view.uniformHeight ? parent.bottom : undefined
      spacing: 0

      ColumnLayout {
        id: textColumn
        Layout.fillWidth: true
        Layout.leftMargin: 16
        Layout.rightMargin: panel.closable ? closeButton.width + 8 : 16
        Layout.topMargin: 11
        Layout.bottomMargin: actionRow.children.length > 0 ? 0 : 16
        spacing: 6

        Text {
          id: titleText
          Layout.fillWidth: true
          visible: text !== ""
          font: QfTheme.strongFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
        }

        Text {
          id: messageText
          Layout.fillWidth: true
          font: titleText.visible ? QfTheme.tipFont : QfTheme.defaultFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
          elide: Text.ElideRight
        }
      }

      Item {
        Layout.fillHeight: true
      }

      RowLayout {
        id: actionRow

        Layout.fillWidth: true
        Layout.leftMargin: 16
        Layout.rightMargin: 16
        Layout.topMargin: children.length > 0 ? 12 : 0
        Layout.bottomMargin: children.length > 0 ? 16 : 0
        spacing: 8
      }
    }

    QfToolButton {
      id: closeButton
      anchors.top: parent.top
      anchors.right: parent.right
      anchors.rightMargin: 4
      width: 44
      height: 44
      visible: panel.closable
      iconSource: QfTheme.getThemeVectorIcon("ic_close_white_24dp")
      iconColor: QfTheme.mainTextColor

      onClicked: {
        panel.closeClicked();
        if (panel.collapseOnClose) {
          panel.SwipeView.view.collapse();
        }
      }
    }
  }
}
```


