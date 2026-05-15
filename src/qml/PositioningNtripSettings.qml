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
          }

          TextField {
            id: ntripPortTextField
            Layout.preferredWidth: 100
            font: Theme.defaultFont
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

            editable: true
          }

          QfToolButton {
            id: refreshNtripMountPointsButton
            enabled: !ntripSourceTableFetcher.fetching && ntripHostTextField.text !== "" && ntripPortTextField.text !== ""
            visible: !ntripSourceTableFetcher.fetching
            iconSource: Theme.getThemeVectorIcon("refresh_24dp")
            iconColor: Theme.mainTextColor
            bgcolor: "transparent"
            Layout.preferredWidth: 36
            Layout.preferredHeight: 36
            padding: 0

            onClicked: {
              ntripSourceTableFetcher.fetch(PositioningUtils.createNtripSettings(createSettingsMap()));
            }
          }

          BusyIndicator {
            id: refreshNtripMountPointsIndicator
            Layout.preferredWidth: refreshNtripMountPointsButton.height
            Layout.preferredHeight: refreshNtripMountPointsButton.height
            running: ntripSourceTableFetcher.fetching
            visible: ntripSourceTableFetcher.fetching
          }
        }

        Label {
          id: refreshNtripMountPointsFeedback
          visible: text !== ""
          font: Theme.defaultFont
          color: Theme.errorColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        CheckBox {
          id: ntripForwardNmeaSentencesCheckBox
          Layout.fillWidth: true
          text: qsTr('Forward NMEA sentences to NTRIP caster')
          font: Theme.defaultFont
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
    ntripForwardNmeaSentencesCheckBox.checked = ntripSettings.forwardNmeaSentences;
  }

  function createSettingsMap() {
    return {
      "host": ntripHostTextField.text,
      "port": ntripPortTextField.text,
      "protocol": ntripProtocolComboBox.currentValue,
      "username": ntripUsernameTextField.text,
      "password": ntripPasswordTextField.text,
      "mountPoint": ntripMountPointComboBox.editText,
      "forwardNmeaSentences": ntripForwardNmeaSentencesCheckBox.checked
    };
  }
}
