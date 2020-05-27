import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.settings 1.0

import Theme 1.0

Popup {
  id: popup

  padding: 0

  Page {
    anchors.fill: parent

    header: PageHeader {
        title: qsTr("Positioning Settings")

        showApplyButton: true
        showCancelButton: false

        onFinished: popup.visible = false
      }


    ScrollView {
      padding: 20
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
        columnSpacing: 2
        rowSpacing: 10

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
          checked: positioningSettings.antennaHeightActivated
          onCheckedChanged: {
              positioningSettings.antennaHeightActivated = checked
          }
        }

        Label {
          text: qsTr("Antenna Height")
          leftPadding: 22
          enabled: antennaHeightActivated.checked
          font: Theme.defaultFont
        }

        TextField {
          id: antennaHeightInput
          enabled: antennaHeightActivated.checked
          width: 60
          font: Theme.defaultFont
          Layout.preferredWidth: 60
          Layout.preferredHeight: font.height + 20

          inputMethodHints: Qt.ImhFormattedNumbersOnly
          validator: DoubleValidator {}

          Component.onCompleted: {
              text = isNaN( positioningSettings.antennaHeight ) ? '' : positioningSettings.antennaHeight
          }

          onTextChanged: {
            if( text.length === 0 || isNaN(text) ) {
              positioningSettings.antennaHeight = NaN
            } else {
              positioningSettings.antennaHeight = parseFloat( text )
            }
          }
        }

        Label {
          leftPadding: 22
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
          checked: positioningSettings.skipAltitudeCorrection
          onCheckedChanged: {
              positioningSettings.skipAltitudeCorrection = checked
          }
        }

        Label {
          padding: 8
          topPadding: 0
          leftPadding: 22
          text: qsTr( "Use the altitude as reported by the positioning interface. Skip any altitude correction that may be implied by the coordinate system transformation." )
          font: Theme.tipFont

          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }
      }
    }
  }
}
