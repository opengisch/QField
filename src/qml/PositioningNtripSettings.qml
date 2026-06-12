import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
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

          QfTextField {
            id: ntripHostTextField
            Layout.fillWidth: true
            font: Theme.defaultFont
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
          }

          QfTextField {
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
            ListElement {
              name: qsTr('NTRIP SSL/TLS')
              value: NtripSettings.NtripSsl
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

        QfTextField {
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

        QfTextField {
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
                  color: Theme.mainTextColor
                  elide: Text.ElideRight
                  text: model.mountPoint
                }

                Label {
                  Layout.preferredWidth: distanceLabelMetrics.width
                  Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                  visible: text !== ''
                  font: ntripMountPointComboBox.font
                  color: Theme.secondaryTextColor
                  horizontalAlignment: Text.AlignRight
                  text: {
                    if (ntripMountPointComboBox.popup.visible && positionSource.positionInformation.latitudeValid) {
                      const pos = GeometryUtils.point(positionSource.positionInformation.longitude, positionSource.positionInformation.latitude);
                      const distance = GeometryUtils.distanceBetweenPoints(model.point, pos) * UnitTypes.fromUnitToUnitFactor(Qgis.DistanceUnit.Degrees, projectInfo.distanceUnits);
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
          font: Theme.defaultFont
          color: Theme.secondaryTextColor
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
                const pos = GeometryUtils.point(positionSource.positionInformation.longitude, positionSource.positionInformation.latitude);
                const distance = GeometryUtils.distanceBetweenPoints(ntripMountPointComboBox.model[idx].point, pos) * UnitTypes.fromUnitToUnitFactor(Qgis.DistanceUnit.Degrees, projectInfo.distanceUnits);
                details.push(qsTr("Distance:") + " " + UnitTypes.formatDistance(distance, 2, projectInfo.distanceUnits));
              }
            }
            return details.join('\n');
          }
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
            text: qsTr('Forward position to NTRIP caster')
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
      let mps = [...mountPoints];
      if (positionSource.positionInformation.latitudeValid) {
        const pos = GeometryUtils.point(positionSource.positionInformation.longitude, positionSource.positionInformation.latitude);
        mps.sort((a, b) => {
          return GeometryUtils.distanceBetweenPoints(a.point, pos) > GeometryUtils.distanceBetweenPoints(b.point, pos) ? 1 : -1;
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
