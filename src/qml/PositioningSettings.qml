import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.settings 1.0

Popup {
  id: popup

  property double antennaHeight: parseFloat(antennaHeightInput.text)
  property alias antennaHeightActivated: antennaHeightActivated.checked

  padding: 0

  Settings {
    property alias antennaHeight: antennaHeightInput.text
    property alias antennaHeightActivated: antennaHeightActivated.checked
  }

  Page {
    anchors.fill: parent

    header: PageHeader {
      title: qsTr("Positioning Settings")

      showCancelButton: false

      onFinished: popup.visible = false
    }

    GroupBox {
        anchors.left: parent.left
        anchors.right: parent.right

        label: CheckBox {
          id: antennaHeightActivated
          text: qsTr("Activate Antenna Height Compensation")

          indicator.height: 16 * dp
          indicator.width: 16 * dp
          icon.height: 16 * dp
          icon.width: 16 * dp
        }

        GridLayout {
          columns: 2
          flow: GridLayout.LeftToRight
          anchors.fill: parent
          enabled: antennaHeightActivated.checked

          Text {
            text: qsTr("Antenna Height")
            enabled: antennaHeightActivated.checked
          }

          TextField {
            id: antennaHeightInput
            enabled: antennaHeightActivated.checked
            text: "0"

            Layout.fillWidth: true
            Layout.preferredHeight: font.height + 20 * dp
            font: Theme.defaultFont

            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: DoubleValidator {}
          }

          Label {
            leftPadding: 30 * dp
            rightPadding: ( 30 + 54 ) * dp
            bottomPadding: 55 * dp
            text: qsTr( "Z values which are recorded from a positioning receiver will be corrected by this value. If a value of 1.6 is entered, this will result in a correction of -1.6\u00A0m for each recorded value." )
            font: Theme.tipFont

            wrapMode: Text.WordWrap
            Layout.fillWidth: true
          }
        }
    }
  }
}
