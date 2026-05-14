import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.qfield
import Theme

Pane {
  id: filterPanel

  signal applyFilter

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

  property string queryString: ""

  function updateQueryString() {
    let parts = [];
    if (ownerComboBox.editText !== "") {
      parts.push("owner:" + ownerComboBox.editText);
    }
    if (includePublicSwitch.checked) {
      parts.push("include:public");
    }
    if (searchTermTextField !== "") {
      parts.push(searchTermTextField.text);
    }

    queryString = parts.join(" ");
  }

  function clear() {
    searchTermTextField.clear();
    ownerComboBox.editText = "";
    includePublicSwitch.checked = false;
    activePreset = "";
  }

  onVisibleChanged: {
    if (visible) {
      const previousOwner = ownerComboBox.currentText;
      ownerComboBox.model = [""].concat(cloudProjectsModel.uniqueOwners());
      ownerComboBox.editText = ownerComboBox.currentText;
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

    ownerComboBox.editText = p.owner;
    includePublicSwitch.checked = p.includePublic;

    updateQueryString();
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
          updateQueryString();
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
          updateQueryString();
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
            updateQueryString();
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
      onClicked: filterPanel.applyFilter()
    }
  }
}
