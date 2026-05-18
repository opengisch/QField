import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
QfPopup {
  id: popup
  parent: mainWindow.contentItem

  signal apply

  width: mainWindow.width - Theme.popupScreenEdgeHorizontalMargin * 2
  height: mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)
  x: (mainWindow.width - width) / 2
  y: (mainWindow.height - height) / 2
  focus: visible
  closePolicy: Popup.CloseOnEscape

  onAboutToHide: {
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

      showBackButton: true
      showCancelButton: false
      showApplyButton: false

      onBack: {
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

        GridLayout {
          columns: 2
          columnSpacing: 5
          rowSpacing: 10

          Label {
            text: qsTr("Host")
            font: Theme.defaultFont
            color: Theme.mainTextColor
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
          }

          Label {
            text: qsTr("Port")
            font: Theme.defaultFont
            color: Theme.mainTextColor
            wrapMode: Text.WordWrap
            Layout.preferredWidth: 100
          }

          TextField {
            id: ntripHostTextField
            Layout.fillWidth: true
            font: Theme.defaultFont
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
          }

          TextField {
            id: ntripPortTextField
            Layout.preferredWidth: 100
            font: Theme.defaultFont
            inputMethodHints: Qt.ImhFormattedNumbersOnly | Qt.ImhNoPredictiveText
          }
        }

        Label {
          text: qsTr("Protocol")
          font: Theme.defaultFont
          color: Theme.mainTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        QfComboBox {
          id: ntripProtocolComboBox
          Layout.fillWidth: true
          font: Theme.defaultFont

          popup.font: Theme.defaultFont
          popup.topMargin: mainWindow.sceneTopMargin
          popup.bottomMargin: mainWindow.sceneTopMargin

          model: ListModel {
            ListElement {
              name: qsTr('NTRIP Revision 1')
              value: NtripSettings.NtripVersion1
            }
            ListElement {
              name: qsTr('NTRIP Revision 2')
              value: NtripSettings.NtripVersion2
            }
          }

          textRole: "name"
          valueRole: "value"
        }

        Label {
          text: qsTr("Username")
          font: Theme.defaultFont
          color: Theme.mainTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        TextField {
          id: ntripUsernameTextField
          Layout.fillWidth: true
          font: Theme.defaultFont
          inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        }

        Label {
          text: qsTr("Password")
          font: Theme.defaultFont
          color: Theme.mainTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        TextField {
          id: ntripPasswordTextField
          Layout.fillWidth: true
          font: Theme.defaultFont
          echoMode: TextInput.Password
          passwordMaskDelay: Qt.platform.os === "ios" || Qt.platform.os === "android" ? 1000 : 0
          inputMethodHints: Qt.ImhHiddenText | Qt.ImhNoPredictiveText | Qt.ImhSensitiveData | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
        }

        Label {
          text: qsTr("Mount point")
          font: Theme.defaultFont
          color: Theme.mainTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        RowLayout {
          Layout.fillWidth: true

          QfComboBox {
            id: ntripMountPointComboBox
            Layout.fillWidth: true
            font: Theme.defaultFont

            popup.font: Theme.defaultFont
            popup.topMargin: mainWindow.sceneTopMargin
            popup.bottomMargin: mainWindow.sceneTopMargin

            model: []
            textRole: "mountPoint"
            valueRole: "mountPoint"

            editable: true

            onEditTextChanged: {
              const idx = ntripMountPointComboBox.indexOfValue(editText);
              let details = [];
              if (idx > -1) {
                if (model[currentIndex].identifier !== '') {
                  details.push(qsTr("Source identifier: ") + model[currentIndex].identifier);
                }
                if (model[currentIndex].format !== '') {
                  details.push(qsTr("Data format: ") + model[currentIndex].format);
                }
              }
              ntripMountPointsDetails.text = details.join('\n');
            }
          }

          QfToolButton {
            id: refreshNtripMountPointsButton
            enabled: !ntripSourceTableFetcher.isFetching && ntripHostTextField.text !== "" && ntripPortTextField.text !== ""
            visible: !ntripSourceTableFetcher.isFetching
            iconSource: Theme.getThemeVectorIcon("refresh_24dp")
            iconColor: Theme.mainTextColor
            bgcolor: "transparent"
            Layout.preferredWidth: 48
            Layout.preferredHeight: 48
            padding: 0

            onClicked: {
              ntripSourceTableFetcher.fetch(PositioningUtils.createNtripSettings(createSettingsMap()));
            }
          }

          BusyIndicator {
            id: refreshNtripMountPointsIndicator
            Layout.preferredWidth: refreshNtripMountPointsButton.height
            Layout.preferredHeight: refreshNtripMountPointsButton.height
            running: ntripSourceTableFetcher.isFetching
            visible: ntripSourceTableFetcher.isFetching
          }
        }

        Label {
          id: ntripMountPointsDetails
          visible: text !== ""
          font: Theme.defaultFont
          color: Theme.secondaryTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.leftMargin: 10
        }

        Label {
          id: refreshNtripMountPointsFeedback
          visible: text !== ""
          font: Theme.defaultFont
          color: Theme.errorColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.leftMargin: 10
        }

        RowLayout {
          Layout.fillWidth: true

          Label {
            id: ntripForwardNmeaSentencesLabel
            Layout.fillWidth: true
            text: qsTr('Forward NMEA sentences to the NTRIP caster')
            font: Theme.defaultFont
            wrapMode: Text.WordWrap

            MouseArea {
              anchors.fill: parent
              onClicked: ntripForwardNmeaSentencesSwitch.toggle()
            }
          }

          QfSwitch {
            id: ntripForwardNmeaSentencesSwitch
            Layout.preferredWidth: 48
          }
        }
      }
    }
  }

  NtripSourceTableFetcher {
    id: ntripSourceTableFetcher

    onMountPointsChanged: {
      // Preserve edited mountpoint text after model update
      const previousMountPoint = ntripMountPointComboBox.editText;
      ntripMountPointComboBox.model = mountPoints;
      if (previousMountPoint !== "") {
        ntripMountPointComboBox.editText = previousMountPoint;
      }
    }

    onFetchError: function (message) {
      refreshNtripMountPointsFeedback.text = message;
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
}
