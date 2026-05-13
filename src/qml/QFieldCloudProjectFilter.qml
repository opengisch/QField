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
    },
    {
      id: "shared",
      label: qsTr("Projects shared with me"),
      owner: "",
      includePublic: false
    }
  ]

  readonly property string ownerTerm: ownerCombo.editText.trim()
  readonly property string searchTerm: searchTermField.text.trim()
  readonly property bool includePublic: publicSwitch.checked

  readonly property string queryString: {
    const parts = [];
    if (searchTerm) {
      parts.push(searchTerm);
    }
    if (ownerTerm) {
      parts.push("owner:" + ownerTerm);
    }
    if (typeCombo.currentValue) {
      parts.push("type:" + typeCombo.currentValue);
    }
    if (includePublic) {
      parts.push("public");
    }
    return parts.join(" ");
  }

  signal filterApplied

  function clear() {
    searchTermField.clear();
    ownerCombo.editText = "";
    typeCombo.currentIndex = 0;
    publicSwitch.checked = false;
    activePreset = "";
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

  ColumnLayout {
    anchors.fill: parent
    anchors.margins: 16
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
      placeholderText: qsTr("Title, description or related keywords...")
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

    Label {
      Layout.fillWidth: true
      text: qsTr("Type")
      font.pointSize: Theme.tipFont.pointSize
      color: Theme.mainTextColor
    }
    QfComboBox {
      id: typeCombo
      Layout.fillWidth: true
      textRole: "label"
      valueRole: "value"
      model: [
        {
          value: "",
          label: qsTr("any")
        },
        {
          value: "project",
          label: qsTr("project")
        },
        {
          value: "shared_dataset",
          label: qsTr("shared dataset")
        },
        {
          value: "template",
          label: qsTr("template")
        }
      ]
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

    Item {
      Layout.fillHeight: true
    }

    RowLayout {
      Layout.alignment: Qt.AlignRight
      Layout.topMargin: 8
      spacing: 10

      QfButton {
        text: qsTr("Reset")
        bgcolor: Theme.controlBackgroundDisabledColor
        color: Theme.mainTextColor
        onClicked: filterPanel.clear()
      }

      QfButton {
        text: qsTr("Search")
        bgcolor: Theme.mainColor
        color: Theme.mainBackgroundColor
        onClicked: filterPanel.filterApplied()
      }
    }
  }
}
