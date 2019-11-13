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
    padding: 20 * dp

    header: PageHeader {
      title: qsTr("Positioning Settings")

      showCancelButton: false

      onFinished: popup.visible = false
    }

    GridLayout {
        anchors.left: parent.left
        anchors.right: parent.right

        columns: 2
        columnSpacing: 2 * dp
        rowSpacing: 10 * dp

        Label {
            text: qsTr("Activate Antenna Height Compensation")
            font: Theme.defaultFont
        }

        QfSwitch {
            id: antennaHeightActivated
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

            Layout.fillWidth: true
            Layout.minimumWidth: 60 * dp
            Layout.preferredHeight: font.height + 20 * dp
            font: Theme.defaultFont

            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: DoubleValidator {}
        }

        Label {
            leftPadding: 30 * dp
            rightPadding: 30 * dp
            enabled: antennaHeightActivated.checked
            text: qsTr( "Z values which are recorded from a positioning receiver will be corrected by this value. If a value of 1.6 is entered, this will result in a correction of -1.6\u00A0m for each recorded value. The value shown in the position information view will be corrected by this value." )
            font: Theme.tipFont

            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        Item {
            // empty cell in grid layout
            width: 1
        }

        Label {
            padding: 8 * dp
            wrapMode: Text.WordWrap

            text: qsTr( "Skip altitude correction" )
            font: Theme.defaultFont

            MouseArea {
                anchors.fill: parent
                onClicked: skipAltitudeCorrectionSwitch.toggle()
            }
        }

        QfSwitch {
            id: skipAltitudeCorrectionSwitch
        }

        Label {
            padding: 8 * dp
            topPadding: 0
            leftPadding: 22 * dp
            text: qsTr( "Use the altitude as reported by the positioning interface. Skip any altitude correction that may be implied by the coordinate system transformation." )
            font: Theme.tipFont

            wrapMode: Text.WordWrap
        }
    }
  }
}
