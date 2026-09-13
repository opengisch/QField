

# File QfContainerCard.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfContainerCard.qml**](QfContainerCard_8qml.md)

[Go to the documentation of this file](QfContainerCard_8qml.md)


```C++
import QtQuick
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

Rectangle {
  id: containerCard

  property alias title: titleLabel.text
  property alias description: descriptionLabel.text
  property alias footnote: footnoteLabel.text

  property color accentColor: QfTheme.mainColor
  property alias iconSource: cardIcon.iconSource

  property bool indicatorVisible: false
  property int indicatorCount: 0

  default property alias extraContent: extraContentColumn.data

  implicitHeight: cardColumn.implicitHeight + 32
  radius: 12
  color: QfTheme.groupBoxBackgroundColor
  border.width: 1
  border.color: QfTheme.controlBorderColor

  ColumnLayout {
    id: cardColumn
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.verticalCenter: parent.verticalCenter
    anchors.leftMargin: 16
    anchors.rightMargin: 16
    spacing: 6

    RowLayout {
      Layout.fillWidth: true
      spacing: 8

      QfToolButton {
        id: cardIcon
        Layout.preferredWidth: 24
        Layout.preferredHeight: 24
        Layout.alignment: Qt.AlignVCenter
        padding: 0
        icon.width: 22
        icon.height: 22
        visible: iconSource !== ''
        enabled: false
        bgcolor: "transparent"
        iconColor: containerCard.accentColor
      }

      Text {
        id: titleLabel
        Layout.fillWidth: true
        font: QfTheme.strongFont
        color: QfTheme.mainTextColor
        wrapMode: Text.WordWrap
      }

      Rectangle {
        id: indicator
        Layout.alignment: Qt.AlignVCenter
        width: badgeLabel.implicitHeight + 4
        height: width
        radius: width / 2
        color: containerCard.accentColor
        visible: containerCard.indicatorVisible

        Text {
          id: badgeLabel
          anchors.centerIn: parent
          font: QfTheme.tinyFont
          color: QfTheme.light
          text: containerCard.indicatorCount
          visible: containerCard.indicatorCount > 0
        }
      }
    }

    Text {
      id: descriptionLabel
      Layout.fillWidth: true
      font: QfTheme.tipFont
      color: QfTheme.secondaryTextColor
      linkColor: QfTheme.mainColor
      wrapMode: Text.WordWrap
      visible: text !== ""
      textFormat: Text.StyledText

      onLinkActivated: link => Qt.openUrlExternally(link)
    }

    Text {
      id: footnoteLabel
      Layout.fillWidth: true
      font: QfTheme.tipFont
      color: QfTheme.secondaryTextColor
      opacity: 0.75
      wrapMode: Text.WordWrap
      visible: text !== ""
    }

    ColumnLayout {
      id: extraContentColumn
      Layout.fillWidth: true
      spacing: 6
      visible: children.length > 0
    }
  }
}
```


