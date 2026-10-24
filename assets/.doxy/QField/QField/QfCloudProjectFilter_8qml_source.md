

# File QfCloudProjectFilter.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfCloudProjectFilter.qml**](QfCloudProjectFilter_8qml.md)

[Go to the documentation of this file](QfCloudProjectFilter_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.qfield.core
import org.qfield.gui

Pane {
  id: filterPanel

  signal applyFilter

  property string currentUsername: ""
  property bool isTemplateSearch: false

  property var organizations: []
  property bool organizationsFetched: false
  property string activePreset: ""
  readonly property var presets: {
    const list = [
      {
        id: "mine",
        label: qsTr("My own"),
        query: "owner:" + filterPanel.currentUsername
      }
    ];
    for (const org of filterPanel.organizations) {
      list.push({
        id: "org_" + org,
        label: qsTr("Owned by %1").arg(org),
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
      filterPanel.organizationsFetched = false;
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

    const parameters = getQueryParametersFromString(query);
    searchTermTextField.text = parameters["searchTerm"];
    ownerComboBox.editText = parameters["owner"];
    includePublicSwitch.checked = parameters["includePublic"];

    queryString = query;
    blockQueryUpdate = false;

    const preset = presets.find(preset => preset.query === queryString);
    const presetName = preset ? preset.id : "";
    if (activePreset !== presetName) {
      activePreset = presetName;
    }
  }

  function getQueryParametersFromString(query) {
    let parameters = {
      "searchTerm": "",
      "owner": "",
      "includePublic": false
    };

    let searchTerm = [];
    let parts = query.trim().split(/\s+/);
    for (const part of parts) {
      if (part.indexOf("owner:") === 0) {
        parameters["owner"] = part.substring(6);
      } else if (part.indexOf("include:public") === 0) {
        parameters["includePublic"] = true;
      } else {
        searchTerm.push(part);
      }
    }
    parameters["searchTerm"] = searchTerm.join(" ");

    return parameters;
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

      if (!filterPanel.organizationsFetched && cloudConnection.status === QfCloudConnection.LoggedIn) {
        cloudConnection.getUserOrganizations(filterPanel.currentUsername);
        filterPanel.organizationsFetched = true;
      }
    }
  }

  padding: 0
  background: Rectangle {
    color: QfTheme.mainBackgroundColor
  }

  ScrollView {
    anchors.fill: parent
    anchors.bottomMargin: searchButtonRow.height
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical: QfScrollBar {}
    clip: true

    ColumnLayout {
      width: filterPanel.width
      spacing: 10

      Label {
        Layout.fillWidth: true
        text: qsTr("Predefined Filters")
        font: QfTheme.strongTipFont
        color: QfTheme.mainTextColor
      }

      ListView {
        Layout.fillWidth: true
        Layout.preferredHeight: 38
        orientation: ListView.Horizontal
        spacing: 6
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        model: filterPanel.presets

        delegate: QfButton {
          readonly property bool isActive: filterPanel.activePreset === modelData.id
          height: ListView.view.height
          text: modelData.label
          font.pointSize: QfTheme.tipFont.pointSize
          radius: 4
          bgcolor: isActive ? QfTheme.mainColor : "transparent"
          color: isActive ? QfTheme.mainBackgroundColor : QfTheme.mainColor
          onClicked: {
            filterPanel.activePreset = modelData.id;
            if (filterPanel.queryString !== modelData.query) {
              updateQueryFromString(modelData.query);
            } else {
              filterPanel.applyFilter();
            }
          }
        }
      }

      Label {
        Layout.fillWidth: true
        text: qsTr("Criteria")
        font: QfTheme.strongTipFont
        color: QfTheme.mainTextColor
      }

      ColumnLayout {
        Layout.fillWidth: true
        spacing: 6

        Label {
          Layout.fillWidth: true
          text: qsTr("Title or description contains")
          font: QfTheme.tipFont
          color: QfTheme.mainTextColor
        }

        QfTextField {
          id: searchTermTextField
          Layout.fillWidth: true
          font: QfTheme.defaultFont

          onTextEdited: {
            updateQuery();
          }
        }

        Label {
          Layout.fillWidth: true
          text: qsTr("Owner is")
          font: QfTheme.tipFont
          color: QfTheme.mainTextColor
        }

        QfComboBox {
          id: ownerComboBox
          Layout.fillWidth: true
          editable: true
          Material.accent: QfTheme.mainColor
          font: QfTheme.defaultFont
          inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase

          onEditTextChanged: {
            updateQuery();
          }
        }

        RowLayout {
          Layout.fillWidth: true

          Label {
            Layout.fillWidth: true
            text: filterPanel.isTemplateSearch ? qsTr("Include public templates") : qsTr("Include public projects")
            font: QfTheme.tipFont
            color: QfTheme.mainTextColor
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
  }

  Rectangle {
    id: searchButtonRow
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    height: searchButton.height + 12
    color: "transparent"

    QfButton {
      id: searchButton
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.topMargin: 10
      text: qsTr("Search")
      bgcolor: QfTheme.mainColor
      color: QfTheme.mainBackgroundColor

      onClicked: {
        filterPanel.applyFilter();
      }
    }
  }
}
```


