

# File QfPositioningNtripSettings.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfPositioningNtripSettings.qml**](QfPositioningNtripSettings_8qml.md)

[Go to the documentation of this file](QfPositioningNtripSettings_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

QfPopup {
  id: popup
  parent: mainWindow.contentItem

  signal apply

  width: mainWindow.width - QfTheme.popupScreenEdgeHorizontalMargin * 2
  height: mainWindow.height - Math.max(QfTheme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)
  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2
  focus: visible
  closePolicy: Popup.CloseOnEscape

  property var presets: []
  property int presetIndex: 0

  onAboutToHide: {
    savePreset();
    popup.apply();
  }

  Page {
    id: page
    width: parent.width
    height: parent.height
    padding: 5
    header: QfPageHeader {
      id: pageHeader
      title: qsTr("NTRIP Settings")

      showBackButton: false
      showCancelButton: false
      showApplyButton: true

      onApply: {
        popup.close();
      }
    }

    ScrollView {
      topPadding: 0
      leftPadding: 5
      rightPadding: 5
      bottomPadding: 0
      ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
      ScrollBar.vertical: QfScrollBar {}
      contentWidth: settingsLayout.childrenRect.width
      contentHeight: settingsLayout.childrenRect.height
      width: parent.width
      height: parent.height
      clip: true

      ColumnLayout {
        id: settingsLayout
        spacing: 10
        width: page.width - 20

        RowLayout {
          Layout.fillWidth: true

          Label {
            id: ntripPresetLabel
            text: qsTr("Preset")
            font: QfTheme.defaultFont
            color: QfTheme.mainTextColor
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
          }

          QfToolButton {
            id: addNtripPresetButton
            Layout.topMargin: (ntripPresetLabel.implicitHeight - QfTheme.toolButtonSize) / 2
            Layout.bottomMargin: (ntripPresetLabel.implicitHeight - QfTheme.toolButtonSize) / 2
            iconSource: QfTheme.getThemeVectorIcon("ic_add_white_24dp")
            iconColor: QfTheme.mainTextColor
            bgcolor: "transparent"
            Layout.preferredWidth: QfTheme.toolButtonSize
            Layout.preferredHeight: QfTheme.toolButtonSize
            padding: 0

            onClicked: {
              ntripPresetNameDialog.isRenaming = false;
              ntripPresetNameDialog.open();
            }
          }
        }

        RowLayout {
          Layout.fillWidth: true

          QfComboBox {
            id: ntripPresetComboBox
            Layout.fillWidth: true
            font: QfTheme.defaultFont

            popup.font: QfTheme.defaultFont
            popup.topMargin: mainWindow.sceneTopMargin
            popup.bottomMargin: mainWindow.sceneTopMargin

            model: presets
            textRole: "name"

            onActivated: function (index) {
              switchToPreset(index);
            }
          }

          QfToolButton {
            id: renameNtripPresetButton
            iconSource: QfTheme.getThemeVectorIcon("ic_create_white_24dp")
            iconColor: QfTheme.mainTextColor
            bgcolor: "transparent"
            Layout.preferredWidth: QfTheme.toolButtonSize
            Layout.preferredHeight: QfTheme.toolButtonSize
            padding: 0

            onClicked: {
              ntripPresetNameDialog.isRenaming = true;
              ntripPresetNameDialog.open();
            }
          }

          QfToolButton {
            id: removeNtripPresetButton
            iconSource: QfTheme.getThemeVectorIcon("ic_delete_forever_white_24dp")
            iconColor: QfTheme.mainTextColor
            bgcolor: "transparent"
            Layout.preferredWidth: QfTheme.toolButtonSize
            Layout.preferredHeight: QfTheme.toolButtonSize
            padding: 0

            onClicked: {
              ntripPresetRemoveDialog.open();
            }
          }
        }

        Rectangle {
          Layout.fillWidth: true
          Layout.topMargin: 5
          Layout.bottomMargin: 5
          height: 1
          color: QfTheme.controlBorderColor
        }

        GridLayout {
          columns: 2
          columnSpacing: 5
          rowSpacing: 10

          Label {
            text: qsTr("Host")
            font: QfTheme.defaultFont
            color: QfTheme.mainTextColor
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
          }

          Label {
            text: qsTr("Port")
            font: QfTheme.defaultFont
            color: QfTheme.mainTextColor
            wrapMode: Text.WordWrap
            Layout.preferredWidth: 100
          }

          QfTextField {
            id: ntripHostTextField
            Layout.fillWidth: true
            font: QfTheme.defaultFont
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
          }

          QfTextField {
            id: ntripPortTextField
            Layout.preferredWidth: 100
            font: QfTheme.defaultFont
            inputMethodHints: Qt.ImhFormattedNumbersOnly | Qt.ImhNoPredictiveText
          }
        }

        Label {
          text: qsTr("Protocol")
          font: QfTheme.defaultFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        QfComboBox {
          id: ntripProtocolComboBox
          Layout.fillWidth: true
          font: QfTheme.defaultFont

          popup.font: QfTheme.defaultFont
          popup.topMargin: mainWindow.sceneTopMargin
          popup.bottomMargin: mainWindow.sceneTopMargin

          model: ListModel {
            ListElement {
              name: qsTr('NTRIP Revision 1')
              value: QfNtripSettings.NtripVersion1
            }
            ListElement {
              name: qsTr('NTRIP Revision 2')
              value: QfNtripSettings.NtripVersion2
            }
            ListElement {
              name: qsTr('NTRIP SSL/TLS')
              value: QfNtripSettings.NtripSsl
            }
          }

          textRole: "name"
          valueRole: "value"
        }

        Label {
          text: qsTr("Username")
          font: QfTheme.defaultFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        QfTextField {
          id: ntripUsernameTextField
          Layout.fillWidth: true
          font: QfTheme.defaultFont
          inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        }

        Label {
          text: qsTr("Password")
          font: QfTheme.defaultFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        QfTextField {
          id: ntripPasswordTextField
          Layout.fillWidth: true
          font: QfTheme.defaultFont
          echoMode: TextInput.Password
          passwordMaskDelay: Qt.platform.os === "ios" || Qt.platform.os === "android" ? 1000 : 0
          inputMethodHints: Qt.ImhHiddenText | Qt.ImhNoPredictiveText | Qt.ImhSensitiveData | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        }

        Label {
          text: qsTr("Mount point")
          font: QfTheme.defaultFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        RowLayout {
          Layout.fillWidth: true

          QfComboBox {
            id: ntripMountPointComboBox
            Layout.fillWidth: true
            font: QfTheme.defaultFont

            popup.font: QfTheme.defaultFont
            popup.topMargin: mainWindow.sceneTopMargin
            popup.bottomMargin: mainWindow.sceneTopMargin

            model: []
            textRole: "mountPoint"
            valueRole: "mountPoint"

            editable: true

            delegate: MenuItem {
              id: menuItem

              required property var model
              required property int index

              padding: 16
              verticalPadding: Material.menuItemVerticalPadding
              spacing: 16

              width: ListView.view.width
              Material.foreground: ntripMountPointComboBox.currentIndex === index ? ListView.view.contentItem.Material.accent : ListView.view.contentItem.Material.foreground
              highlighted: ntripMountPointComboBox.highlightedIndex === index
              hoverEnabled: ntripMountPointComboBox.hoverEnabled

              contentItem: RowLayout {
                anchors.left: parent.left
                anchors.leftMargin: parent.padding
                anchors.right: parent.right
                anchors.rightMargin: parent.padding
                height: menuItem.height

                Label {
                  Layout.fillWidth: true
                  Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                  font: ntripMountPointComboBox.font
                  color: QfTheme.mainTextColor
                  elide: Text.ElideRight
                  text: model.mountPoint
                }

                Label {
                  Layout.preferredWidth: distanceLabelMetrics.width
                  Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                  visible: text !== ''
                  font: ntripMountPointComboBox.font
                  color: QfTheme.secondaryTextColor
                  horizontalAlignment: Text.AlignRight
                  text: {
                    if (ntripMountPointComboBox.popup.visible && positionSource.positionInformation.latitudeValid) {
                      const pos = QfGeometryUtils.point(positionSource.positionInformation.longitude, positionSource.positionInformation.latitude);
                      const distance = QfGeometryUtils.distanceBetweenPoints(model.point, pos) * UnitTypes.fromUnitToUnitFactor(Qgis.DistanceUnit.Degrees, projectInfo.distanceUnits);
                      return UnitTypes.formatDistance(distance, 2, projectInfo.distanceUnits);
                    }
                    return '';
                  }
                }
              }
            }

            TextMetrics {
              id: distanceLabelMetrics
              font: ntripMountPointComboBox.font
              text: 'XXXXXXXXX'
            }
          }

          QfToolButton {
            id: refreshNtripMountPointsButton
            enabled: !ntripSourceTableFetcher.isFetching && ntripHostTextField.text !== "" && ntripPortTextField.text !== ""
            visible: !ntripSourceTableFetcher.isFetching
            iconSource: QfTheme.getThemeVectorIcon("refresh_24dp")
            iconColor: QfTheme.mainTextColor
            bgcolor: "transparent"
            Layout.preferredWidth: QfTheme.toolButtonSize
            Layout.preferredHeight: QfTheme.toolButtonSize
            padding: 0

            onClicked: {
              ntripSourceTableFetcher.fetch(QfPositioningUtils.createNtripSettings(createSettingsMap()));
            }
          }

          BusyIndicator {
            id: refreshNtripMountPointsIndicator
            Layout.margins: 6
            Layout.preferredWidth: 36
            Layout.preferredHeight: 36
            running: ntripSourceTableFetcher.isFetching
            visible: ntripSourceTableFetcher.isFetching
          }
        }

        Label {
          id: ntripMountPointsDetails
          visible: text !== ""
          font: QfTheme.defaultFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.leftMargin: 10
          text: {
            const mountPoint = ntripMountPointComboBox.editText;
            const idx = ntripMountPointComboBox.indexOfValue(mountPoint);
            let details = [];
            if (idx > -1) {
              if (ntripMountPointComboBox.model[idx].identifier !== '') {
                details.push(qsTr("Identifier:") + " " + ntripMountPointComboBox.model[idx].identifier);
              }
              if (ntripMountPointComboBox.model[idx].format !== '') {
                details.push(qsTr("Data format:") + " " + ntripMountPointComboBox.model[idx].format);
              }
              if (positionSource.positionInformation.latitudeValid) {
                const pos = QfGeometryUtils.point(positionSource.positionInformation.longitude, positionSource.positionInformation.latitude);
                const distance = QfGeometryUtils.distanceBetweenPoints(ntripMountPointComboBox.model[idx].point, pos) * UnitTypes.fromUnitToUnitFactor(Qgis.DistanceUnit.Degrees, projectInfo.distanceUnits);
                details.push(qsTr("Distance:") + " " + UnitTypes.formatDistance(distance, 2, projectInfo.distanceUnits));
              }
            }
            return details.join('\n');
          }
        }

        Label {
          id: refreshNtripMountPointsFeedback
          visible: text !== ""
          font: QfTheme.defaultFont
          color: QfTheme.errorColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.leftMargin: 10
        }

        RowLayout {
          Layout.fillWidth: true

          Label {
            id: ntripForwardNmeaSentencesLabel
            Layout.fillWidth: true
            text: qsTr('Forward position to NTRIP caster')
            font: QfTheme.defaultFont
            wrapMode: Text.WordWrap

            MouseArea {
              anchors.fill: parent
              onClicked: ntripForwardNmeaSentencesSwitch.toggle()
            }
          }

          QfSwitch {
            id: ntripForwardNmeaSentencesSwitch
            Layout.preferredWidth: QfTheme.toolButtonSize
          }
        }
      }
    }
  }

  QfNtripSourceTableFetcher {
    id: ntripSourceTableFetcher

    onMountPointsChanged: {
      // Preserve edited mountpoint text after model update
      const previousMountPoint = ntripMountPointComboBox.editText;
      let mps = [...mountPoints];
      if (positionSource.positionInformation.latitudeValid) {
        const pos = QfGeometryUtils.point(positionSource.positionInformation.longitude, positionSource.positionInformation.latitude);
        mps.sort((a, b) => {
          return QfGeometryUtils.distanceBetweenPoints(a.point, pos) > QfGeometryUtils.distanceBetweenPoints(b.point, pos) ? 1 : -1;
        });
      } else {
        mps.sort((a, b) => {
          return a.mountPoint > b.mountPoint ? 1 : -1;
        });
      }
      ntripMountPointComboBox.model = mps;
      if (previousMountPoint !== "") {
        const idx = ntripMountPointComboBox.indexOfValue(previousMountPoint);
        if (idx > -1) {
          ntripMountPointComboBox.currentIndex = idx;
        } else {
          ntripMountPointComboBox.currentIndex = -1;
          ntripMountPointComboBox.editText = previousMountPoint;
        }
      } else {
        ntripMountPointComboBox.currentIndex = 0;
      }
    }

    onFetchError: function (message) {
      refreshNtripMountPointsFeedback.text = message;
    }
  }

  QfDialog {
    id: ntripPresetNameDialog

    property bool isRenaming: false

    parent: mainWindow.contentItem
    title: isRenaming ? qsTr("Rename Preset") : qsTr("Add Preset")
    width: mainWindow.width - 40

    ColumnLayout {
      width: ntripPresetNameDialog.availableWidth
      spacing: 10

      Label {
        Layout.fillWidth: true
        wrapMode: Text.WordWrap
        text: qsTr("Enter the preset name")
      }

      QfTextField {
        id: ntripPresetNameTextField
        Layout.fillWidth: true
      }
    }

    onAboutToShow: {
      ntripPresetNameTextField.text = isRenaming ? presets[presetIndex].name : "";
      ntripPresetNameTextField.forceActiveFocus();
      standardButton(Dialog.Ok).enabled = Qt.binding(() => isPresetNameAvailable(ntripPresetNameTextField.text.trim(), isRenaming ? presetIndex : -1));
    }

    onAccepted: {
      const name = ntripPresetNameTextField.text.trim();
      if (isRenaming) {
        renamePreset(name);
      } else {
        addPreset(name);
      }
    }
  }

  QfDialog {
    id: ntripPresetRemoveDialog
    parent: mainWindow.contentItem

    title: qsTr("Remove Preset")

    Label {
      id: ntripPresetRemoveLabel
      width: mainWindow.width - 60
      wrapMode: Text.WordWrap
      font: QfTheme.defaultFont
      color: QfTheme.mainTextColor
    }

    onAboutToShow: {
      ntripPresetRemoveLabel.text = qsTr("Are you sure you want to remove `%1`?").arg(presets[presetIndex].name);
    }

    onAccepted: {
      removePreset();
    }
  }

  function updateFromNtripSettings(ntripSettings) {
    ntripHostTextField.text = ntripSettings.host;
    ntripPortTextField.text = ntripSettings.port;
    ntripProtocolComboBox.currentValue = ntripSettings.protocol;
    ntripUsernameTextField.text = ntripSettings.username;
    ntripPasswordTextField.text = ntripSettings.password;
    ntripMountPointComboBox.editText = ntripSettings.mountPoint;
    ntripForwardNmeaSentencesSwitch.checked = ntripSettings.forwardNmeaSentences;
  }

  function createSettingsMap() {
    return {
      "host": ntripHostTextField.text,
      "port": ntripPortTextField.text,
      "protocol": ntripProtocolComboBox.currentValue,
      "username": ntripUsernameTextField.text,
      "password": ntripPasswordTextField.text,
      "mountPoint": ntripMountPointComboBox.editText,
      "forwardNmeaSentences": ntripForwardNmeaSentencesSwitch.checked
    };
  }

  function updateFromNtripPresets(ntripPresets, ntripPresetIndex) {
    presets = [...ntripPresets];
    presetIndex = ntripPresetIndex;
    loadPreset();
  }

  function loadPreset() {
    ntripPresetComboBox.currentIndex = presetIndex;
    updateFromNtripSettings(QfPositioningUtils.createNtripSettings(presets[presetIndex].settings));
  }

  function savePreset() {
    let updatedPresets = [...presets];
    updatedPresets[presetIndex] = {
      "name": presets[presetIndex].name,
      "settings": createSettingsMap()
    };
    presets = updatedPresets;
  }

  function clearMountPoints() {
    ntripSourceTableFetcher.cancel();
    ntripMountPointComboBox.model = [];
    refreshNtripMountPointsFeedback.text = "";
  }

  function switchToPreset(index) {
    if (index === presetIndex) {
      return;
    }
    savePreset();
    presetIndex = index;
    clearMountPoints();
    loadPreset();
  }

  function addPreset(name) {
    savePreset();
    let updatedPresets = [...presets];
    updatedPresets.push({
      "name": name,
      "settings": {}
    });
    presets = updatedPresets;
    presetIndex = presets.length - 1;
    clearMountPoints();
    loadPreset();
  }

  function renamePreset(name) {
    let updatedPresets = [...presets];
    updatedPresets[presetIndex] = {
      "name": name,
      "settings": presets[presetIndex].settings
    };
    presets = updatedPresets;
    ntripPresetComboBox.currentIndex = presetIndex;
  }

  function removePreset() {
    let updatedPresets = [...presets];
    updatedPresets.splice(presetIndex, 1);
    if (updatedPresets.length === 0) {
      updatedPresets.push({
        "name": qsTr("Default server"),
        "settings": {}
      });
    }
    presets = updatedPresets;
    presetIndex = Math.min(presetIndex, presets.length - 1);
    clearMountPoints();
    loadPreset();
  }

  function isPresetNameAvailable(name, ignoredIndex) {
    return name !== "" && !presets.some((preset, index) => index !== ignoredIndex && preset.name === name);
  }
}
```


