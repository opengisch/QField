import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.settings 1.0
import Theme 1.0

Popup {
  id: popup

  property double antennaHeight: parseFloat(antennaHeightInput.text)
  property alias antennaHeightActivated: antennaHeightActivated.checked
  property alias skipAltitudeCorrection: skipAltitudeCorrectionSwitch.checked

  padding: 0

  Settings {
    property alias antennaHeight: antennaHeightInput.text
    property alias antennaHeightActivated: antennaHeightActivated.checked
    property alias skipAltitudeCorrection: skipAltitudeCorrectionSwitch.checked
  }

  Page {
    anchors.fill: parent

    header: PageHeader {
        title: qsTr("Positioning Settings")

        showApplyButton: true
        showCancelButton: false

        onFinished: popup.visible = false
      }


    ScrollView {
      padding: 20 * dp
      ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
      ScrollBar.vertical.policy: ScrollBar.AsNeeded
      contentWidth: positioningGrid.width
      contentHeight: positioningGrid.height
      anchors.fill: parent
      clip: true

      GridLayout {
        id: positioningGrid
        width: parent.parent.width

        columns: 2
        columnSpacing: 2 * dp
        rowSpacing: 10 * dp

        Label {
          text: qsTr("Activate antenna height compensation")
          font: Theme.defaultFont
          wrapMode: Text.WordWrap
          Layout.fillWidth: true

          MouseArea {
            anchors.fill: parent
            onClicked: antennaHeightActivated.toggle()
            }
        }

        QfSwitch {
          id: antennaHeightActivated
          Layout.alignment: Qt.AlignTop
        }

        Label {
          text: qsTr("Antenna Height")
          leftPadding: 22 * dp
          enabled: antennaHeightActivated.checked
          font: Theme.defaultFont
        }

        TextField {
          id: antennaHeightInput
          enabled: antennaHeightActivated.checked
          text: "0"
          width: 60 * dp
          font: Theme.defaultFont
          Layout.preferredWidth: 60 * dp
          Layout.preferredHeight: font.height + 20 * dp

          inputMethodHints: Qt.ImhFormattedNumbersOnly
          validator: DoubleValidator {}
        }

        Label {
          leftPadding: 22 * dp
          enabled: antennaHeightActivated.checked
          text: qsTr( "Z values which are recorded from a positioning receiver will be corrected by this value. If a value of 1.6 is entered, this will result in a correction of -1.6\u00A0m for each recorded value. The value shown in the position information view is already corrected by this value." )
          font: Theme.tipFont

          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        Item {
          // empty cell in grid layout
          width: 1
        }

        Label {
          text: qsTr( "Skip altitude correction" )
          font: Theme.defaultFont
          wrapMode: Text.WordWrap
          Layout.fillWidth: true

          MouseArea {
            anchors.fill: parent
            onClicked: skipAltitudeCorrectionSwitch.toggle()
          }
        }

        QfSwitch {
          id: skipAltitudeCorrectionSwitch
          Layout.alignment: Qt.AlignTop
        }

        Label {
          padding: 8 * dp
          topPadding: 0
          leftPadding: 22 * dp
          text: qsTr( "Use the altitude as reported by the positioning interface. Skip any altitude correction that may be implied by the coordinate system transformation." )
          font: Theme.tipFont

          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }
      }
    }
  }
}
