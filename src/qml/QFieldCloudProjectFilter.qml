import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.qfield
import Theme

Pane {
  id: filterPanel

  signal applyFilter

  property string currentUsername: ""
  property var organizations: []

  property string activePreset: ""
  readonly property var presets: {
    const list = [
      {
        id: "mine",
        label: qsTr("My Own Projects"),
        query: "owner:" + filterPanel.currentUsername
      }
    ];
    for (const org of filterPanel.organizations) {
      list.push({
        id: "org_" + org,
        label: qsTr("%1's projects").arg(org),
        query: "owner:" + org
      });
    }
    return list;
  }

  property string queryString: ""

  property bool blockQueryUpdate: false

  Connections {
    target: cloudConnection
    function onUserOrganizationsReceived(organizations) {
      filterPanel.organizations = organizations;
    }
    function onUsernameChanged() {
      filterPanel.organizations = [];
    }
  }

  function updateQuery() {
    if (filterPanel.blockQueryUpdate) {
      return;
    }

    let parts = [];
    if (ownerComboBox.editText !== "") {
      parts.push("owner:" + ownerComboBox.editText);
    }
    if (includePublicSwitch.checked) {
      parts.push("include:public");
    }
    if (searchTermTextField.text !== "") {
      parts.push(searchTermTextField.text);
    }

    queryString = parts.join(" ").trim();

    const preset = presets.find(preset => preset.query === queryString);
    const presetName = preset ? preset.id : "";
    if (activePreset !== presetName) {
      activePreset = presetName;
    }
  }

  function updateQueryFromString(query) {
    blockQueryUpdate = true;

    let searchTerm = [];
    let owner = "";
    let includePublic = false;

    let parts = query.trim().split(/\s+/);
    for (const part of parts) {
      if (part.indexOf("owner:") === 0) {
        owner = part.substring(6);
      } else if (part.indexOf("include:public") === 0) {
        includePublic = true;
      } else {
        searchTerm.push(part);
      }
    }

    searchTermTextField.text = searchTerm.join(' ');
    ownerComboBox.editText = owner;
    includePublicSwitch.checked = includePublic;

    queryString = query;

    blockQueryUpdate = false;

    const preset = presets.find(preset => preset.query === queryString);
    const presetName = preset ? preset.id : "";
    if (activePreset !== presetName) {
      activePreset = presetName;
    }
  }

  function clear() {
    updateQueryFromString("");
    activePreset = "";
  }

  onVisibleChanged: {
    if (visible) {
      blockQueryUpdate = true;
      const previousOwner = ownerComboBox.editText;
      ownerComboBox.model = [""].concat(cloudProjectsModel.uniqueOwners());
      ownerComboBox.editText = previousOwner;
      blockQueryUpdate = false;
    }
  }

  padding: 0
  background: Rectangle {
    color: Theme.mainBackgroundColor
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
        font: Theme.secondaryTitleFont
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
          onClicked: {
            filterPanel.activePreset = modelData.id;
            if (filterPanel.queryString !== modelData.query) {
              updateQueryFromString(modelData.query);
            }
          }
        }
      }

      Label {
        Layout.fillWidth: true
        text: qsTr("Criteria")
        font: Theme.secondaryTitleFont
        color: Theme.mainTextColor
      }

      Label {
        Layout.fillWidth: true
        text: qsTr("Search term")
        font: Theme.tipFont
        color: Theme.mainTextColor
      }

      QfTextField {
        id: searchTermTextField
        Layout.fillWidth: true
        font: Theme.defaultFont

        onTextEdited: {
          updateQuery();
        }
      }

      Label {
        Layout.fillWidth: true
        text: qsTr("Owner")
        font: Theme.tipFont
        color: Theme.mainTextColor
      }

      QfComboBox {
        id: ownerComboBox
        Layout.fillWidth: true
        editable: true
        Material.accent: Theme.mainColor
        font: Theme.defaultFont

        onEditTextChanged: {
          updateQuery();
        }
      }

      RowLayout {
        Layout.fillWidth: true
        Layout.topMargin: 8

        Label {
          Layout.fillWidth: true
          text: qsTr("Include public projects")
          font: Theme.tipFont
          color: Theme.mainTextColor
        }

        Switch {
          id: includePublicSwitch
          Layout.alignment: Qt.AlignRight

          onClicked: {
            updateQuery();
          }
        }
      }
    }
  }

  Rectangle {
    id: searchButtonRow
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    height: searchButton.height + 10
    color: "transparent"

    QfButton {
      id: searchButton
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.topMargin: 10
      text: qsTr("Search")
      bgcolor: Theme.mainColor
      color: Theme.mainBackgroundColor

      onClicked: {
        filterPanel.applyFilter();
      }
    }
  }
}
