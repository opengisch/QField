import QtQuick
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * A card grouping a primary action button with its title, description and
 * status texts. Extra rows (e.g. a toggle) can be added as children and are
 * placed below the action button.
 *
 * \ingroup qml
 */
Rectangle {
  id: actionCard

  property color accentColor: Theme.mainColor
  property alias iconSource: cardIcon.iconSource
  property alias title: titleLabel.text
  property alias description: descriptionLabel.text
  property alias badgeText: badgeLabel.text
  property alias metaText: metaLabel.text
  property alias indicatorVisible: indicator.visible
  property alias buttonText: actionButton.text
  property color buttonColor: Theme.buttonColor
  property alias buttonEnabled: actionButton.enabled
  property alias buttonShowProgress: actionButton.showProgress
  property alias buttonProgressValue: actionButton.progressValue

  // Draw the action button as an accent-colored outline instead of a filled background
  property bool outlined: false

  default property alias extraContent: extraContentColumn.data

  signal actionClicked

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
        iconColor: actionCard.accentColor
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
        color: actionCard.accentColor
        visible: badgeLabel.text !== ""

        Text {
          id: badgeLabel
          anchors.centerIn: parent
          font: Theme.tinyFont
          color: Theme.light
        }
      }

      Rectangle {
        id: indicator
        Layout.alignment: Qt.AlignVCenter
        width: 10
        height: 10
        radius: width / 2
        color: actionCard.accentColor
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
      id: metaLabel
      Layout.fillWidth: true
      font: Theme.tipFont
      color: Theme.secondaryTextColor
      opacity: 0.75
      wrapMode: Text.WordWrap
      visible: text !== ""
    }

    QfButton {
      id: actionButton
      Layout.fillWidth: true
      Layout.topMargin: 4
      bgcolor: actionCard.outlined ? "transparent" : actionCard.accentColor
      color: actionCard.outlined ? actionCard.accentColor : actionCard.buttonColor

      onClicked: actionCard.actionClicked()
    }

    ColumnLayout {
      id: extraContentColumn
      Layout.fillWidth: true
      spacing: 8
      visible: children.length > 0
    }
  }
}
