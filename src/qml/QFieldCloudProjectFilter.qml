import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.qfield
import Theme

Pane {
  id: filterPanel

  property string currentUsername: ""

  property string activePreset: ""
  readonly property var presets: [
    {
      id: "mine",
      label: qsTr("My Own Projects"),
      owner: filterPanel.currentUsername,
      includePublic: false
    }
  ]

  readonly property string ownerTerm: ownerCombo.editText.trim()
  readonly property string searchTerm: searchTermField.text.trim()
  readonly property bool includePublic: publicSwitch.checked

  readonly property string queryString: {
    const parts = [];
    if (ownerTerm) {
      parts.push("owner:" + ownerTerm);
    }
    if (includePublic) {
      parts.push("include:public");
    }
    if (searchTerm) {
      parts.push(searchTerm);
    }
    return parts.join(" ");
  }

  signal filterApplied

  function clear() {
    searchTermField.clear();
    ownerCombo.editText = "";
    publicSwitch.checked = false;
    activePreset = "";
  }

  onVisibleChanged: {
    if (visible) {
      ownerCombo.model = cloudProjectsModel.uniqueOwners();
    }
  }

  onActivePresetChanged: {
    if (!activePreset) {
      return;
    }
    const p = presets.find(x => x.id === activePreset);
    if (!p) {
      return;
    }
    ownerCombo.editText = p.owner;
    publicSwitch.checked = p.includePublic;
  }

  padding: 0
  background: Rectangle {
    color: Theme.mainBackgroundColor
    opacity: 0.95
  }

  ScrollView {
    anchors.fill: parent
    anchors.bottomMargin: searchButtonRow.height
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical: QfScrollBar {}
    clip: true

    ColumnLayout {
      width: filterPanel.width - 32
      x: 16
      y: 16
      spacing: 14

      Label {
        Layout.fillWidth: true
        text: qsTr("Predefined Filters")
        font.pointSize: Theme.secondaryTitleFont.pointSize
        color: Theme.mainTextColor
      }

      ListView {
        Layout.fillWidth: true
        Layout.preferredHeight: 40
        orientation: ListView.Horizontal
        spacing: 10
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        model: filterPanel.presets

        delegate: QfButton {
          readonly property bool isActive: filterPanel.activePreset === modelData.id
          height: ListView.view.height
          text: modelData.label
          font.pointSize: Theme.tipFont.pointSize
          radius: 4
          bgcolor: isActive ? Theme.mainColor : "transparent"
          color: isActive ? Theme.mainBackgroundColor : Theme.mainColor
          onClicked: filterPanel.activePreset = isActive ? "" : modelData.id
        }
      }

      Label {
        Layout.fillWidth: true
        text: qsTr("Criteria")
        font.pointSize: Theme.secondaryTitleFont.pointSize
        color: Theme.mainTextColor
      }

      Label {
        Layout.fillWidth: true
        text: qsTr("Search term")
        font.pointSize: Theme.tipFont.pointSize
        color: Theme.mainTextColor
      }
      QfTextField {
        id: searchTermField
        Layout.fillWidth: true
      }

      Label {
        Layout.fillWidth: true
        text: qsTr("Owner")
        font.pointSize: Theme.tipFont.pointSize
        color: Theme.mainTextColor
      }
      QfComboBox {
        id: ownerCombo
        Layout.fillWidth: true
        editable: true
      }

      RowLayout {
        Layout.fillWidth: true
        Layout.topMargin: 8

        Label {
          Layout.fillWidth: true
          text: qsTr("Include public projects")
          font.pointSize: Theme.tipFont.pointSize
          color: Theme.mainTextColor
        }

        Switch {
          id: publicSwitch
          Layout.alignment: Qt.AlignRight
        }
      }
    }
  }

  Rectangle {
    id: searchButtonRow
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    height: searchButton.height + 20
    color: Theme.darkTheme ? Theme.mainBackgroundColorSemiOpaque : Theme.lightestGraySemiOpaque

    QfButton {
      id: searchButton
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.bottom: parent.bottom
      anchors.leftMargin: 10
      anchors.rightMargin: 10
      anchors.bottomMargin: 10
      text: qsTr("Search")
      bgcolor: Theme.mainColor
      color: Theme.mainBackgroundColor
      onClicked: filterPanel.filterApplied()
    }
  }
}
