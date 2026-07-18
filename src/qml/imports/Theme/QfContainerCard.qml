import QtQuick
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * A card grouping a title, description and status texts. Content (e.g. an
 * action button or a toggle) can be added as children and is placed below
 * the texts.
 *
 * \ingroup qml
 */
Rectangle {
  id: containerCard

  property color accentColor: Theme.mainColor
  property alias iconSource: cardIcon.iconSource
  property alias title: titleLabel.text
  property alias description: descriptionLabel.text
  property int count: 0
  property alias footnote: footnoteLabel.text
  property alias indicatorVisible: indicator.visible

  default property alias extraContent: extraContentColumn.data

  implicitHeight: cardColumn.implicitHeight + 32
  radius: 12
  color: Theme.groupBoxBackgroundColor
  border.width: 1
  border.color: Theme.controlBorderColor

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
        font: Theme.strongFont
        color: Theme.mainTextColor
        wrapMode: Text.WordWrap
      }

      Rectangle {
        Layout.alignment: Qt.AlignVCenter
        implicitWidth: Math.max(implicitHeight, badgeLabel.implicitWidth + 10)
        implicitHeight: badgeLabel.implicitHeight + 4
        radius: height / 2
        color: containerCard.accentColor
        visible: containerCard.count > 0

        Text {
          id: badgeLabel
          anchors.centerIn: parent
          font: Theme.tinyFont
          color: Theme.light
          text: containerCard.count
        }
      }

      Rectangle {
        id: indicator
        Layout.alignment: Qt.AlignVCenter
        width: 10
        height: 10
        radius: width / 2
        color: containerCard.accentColor
        visible: false
      }

      Item {
        Layout.fillWidth: true
      }
    }

    Text {
      id: descriptionLabel
      Layout.fillWidth: true
      font: Theme.tipFont
      color: Theme.secondaryTextColor
      linkColor: Theme.mainColor
      wrapMode: Text.WordWrap
      visible: text !== ""
      textFormat: Text.StyledText

      onLinkActivated: link => Qt.openUrlExternally(link)
    }

    Text {
      id: footnoteLabel
      Layout.fillWidth: true
      font: Theme.tipFont
      color: Theme.secondaryTextColor
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
