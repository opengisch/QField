import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 *
 * Full-screen GNSS Status page showing core positioning metrics
 * with scrollable label:value sections.
 */
Page {
  id: gnssStatusPage

  signal finished

  visible: false
  focus: visible

  leftPadding: mainWindow.sceneLeftMargin
  rightPadding: mainWindow.sceneRightMargin

  header: QfPageHeader {
    title: qsTr("GNSS Status")

    showBackButton: true
    showApplyButton: false
    showCancelButton: false
    backAsCancel: true

    topMargin: mainWindow.sceneTopMargin

    onFinished: {
      parent.finished();
    }
  }

  // Data freshness check for AC4 (stale >30s → show "—")
  readonly property bool isDataCurrent: positionSource.positionInformation && positionSource.positionInformation.isValid && positionSource.currentness

  function formatBytes(bytes) {
    if (bytes < 1024)
      return bytes + " B";
    if (bytes < 1048576)
      return (bytes / 1024).toFixed(1) + " KB";
    return (bytes / 1048576).toFixed(1) + " MB";
  }

  // Inline component for section headers
  component SectionHeader: ColumnLayout {
    required property string title

    Layout.fillWidth: true
    Layout.topMargin: 16
    Layout.leftMargin: 12
    Layout.rightMargin: 12
    spacing: 4

    Text {
      text: title
      font: Theme.strongTipFont
      color: Theme.mainTextColor
    }

    Rectangle {
      Layout.fillWidth: true
      height: 1
      color: Theme.controlBorderColor
    }
  }

  // Inline component for label:value data rows
  component DataRow: RowLayout {
    required property string label
    property string value: "\u2014"
    property color valueColor: value === "\u2014" ? Theme.secondaryTextColor : Theme.mainTextColor

    Layout.fillWidth: true
    Layout.leftMargin: 16
    Layout.rightMargin: 12
    Layout.topMargin: 6
    spacing: 8

    Text {
      text: label
      font: Theme.tipFont
      color: Theme.positionColor
      Layout.minimumWidth: 80
      Layout.maximumWidth: 140
    }

    Text {
      text: value
      font: Theme.defaultFont
      color: valueColor
      Layout.fillWidth: true
      elide: Text.ElideRight
    }
  }

  Flickable {
    anchors.fill: parent
    contentHeight: contentColumn.height + 20
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.vertical: QfScrollBar {}

    ColumnLayout {
      id: contentColumn
      width: parent.width
      spacing: 0

      // === Source / Time ===
      SectionHeader {
        title: qsTr("Source / Time")
        Layout.topMargin: 8
      }

      DataRow {
        label: qsTr("Source")
        value: positionSource.positionInformation && positionSource.positionInformation.sourceName ? positionSource.positionInformation.sourceName : "\u2014"
      }

      DataRow {
        label: qsTr("UTC")
        value: gnssStatusPage.isDataCurrent ? positionSource.positionInformation.utcDateTime.toLocaleTimeString(Qt.locale(), "HH:mm:ss") : "\u2014"
      }

      // === Position ===
      SectionHeader {
        title: qsTr("Position")
      }

      DataRow {
        label: qsTr("Latitude")
        value: gnssStatusPage.isDataCurrent && positionSource.positionInformation.latitudeValid ? positionSource.positionInformation.latitude.toFixed(6) + "°" : "\u2014"
      }

      DataRow {
        label: qsTr("Longitude")
        value: gnssStatusPage.isDataCurrent && positionSource.positionInformation.longitudeValid ? positionSource.positionInformation.longitude.toFixed(6) + "°" : "\u2014"
      }

      // === Altitude ===
      SectionHeader {
        title: qsTr("Altitude")
      }

      DataRow {
        label: qsTr("Elevation")
        value: gnssStatusPage.isDataCurrent && positionSource.positionInformation.elevationValid ? positionSource.positionInformation.elevation.toFixed(2) + " m" : "\u2014"
      }

      // === Fix ===
      SectionHeader {
        title: qsTr("Fix")
      }

      DataRow {
        label: qsTr("Quality")
        value: gnssStatusPage.isDataCurrent ? positionSource.positionInformation.qualityDescription || "\u2014" : "\u2014"
        valueColor: gnssStatusPage.isDataCurrent ? Theme.fixTypeColor(positionSource.positionInformation.quality) : Theme.secondaryTextColor
      }

      DataRow {
        label: qsTr("Status")
        value: gnssStatusPage.isDataCurrent ? positionSource.positionInformation.fixStatusDescription : "\u2014"
      }

      // === Speed ===
      SectionHeader {
        title: qsTr("Speed")
      }

      DataRow {
        label: qsTr("Ground speed")
        value: gnssStatusPage.isDataCurrent && positionSource.positionInformation.speedValid ? positionSource.positionInformation.speed.toFixed(1) + " km/h" : "\u2014"
      }

      DataRow {
        label: qsTr("Heading")
        value: gnssStatusPage.isDataCurrent && positionSource.positionInformation.directionValid ? positionSource.positionInformation.direction.toFixed(1) + "°" : "\u2014"
      }

      // === DOP ===
      SectionHeader {
        title: qsTr("DOP")
      }

      DataRow {
        label: qsTr("PDOP")
        value: gnssStatusPage.isDataCurrent && positionSource.positionInformation.pdop > 0 ? positionSource.positionInformation.pdop.toFixed(1) : "\u2014"
      }

      DataRow {
        label: qsTr("HDOP")
        value: gnssStatusPage.isDataCurrent && positionSource.positionInformation.hdop > 0 ? positionSource.positionInformation.hdop.toFixed(1) : "\u2014"
      }

      DataRow {
        label: qsTr("VDOP")
        value: gnssStatusPage.isDataCurrent && positionSource.positionInformation.vdop > 0 ? positionSource.positionInformation.vdop.toFixed(1) : "\u2014"
      }

      // === Satellites ===
      SectionHeader {
        title: qsTr("Satellites")
      }

      DataRow {
        label: qsTr("Used")
        value: gnssStatusPage.isDataCurrent && positionSource.positionInformation.satellitesUsed > 0 ? positionSource.positionInformation.satellitesUsed.toString() : "\u2014"
      }

      DataRow {
        label: qsTr("In view")
        value: gnssStatusPage.isDataCurrent && positionSource.satelliteModel.count > 0 ? positionSource.satelliteModel.count.toString() : "\u2014"
      }

      // === Precision ===
      SectionHeader {
        title: qsTr("Precision")
      }

      DataRow {
        label: qsTr("H. Accuracy")
        value: gnssStatusPage.isDataCurrent && positionSource.positionInformation.haccValid ? (positionSource.positionInformation.hacc * 100).toFixed(1) + " cm" : "\u2014"
        valueColor: gnssStatusPage.isDataCurrent && positionSource.positionInformation.haccValid ? Theme.accuracyColor(positionSource.positionInformation.hacc) : Theme.secondaryTextColor
      }

      DataRow {
        label: qsTr("V. Accuracy")
        value: gnssStatusPage.isDataCurrent && positionSource.positionInformation.vaccValid ? (positionSource.positionInformation.vacc * 100).toFixed(1) + " cm" : "\u2014"
        valueColor: gnssStatusPage.isDataCurrent && positionSource.positionInformation.vaccValid ? Theme.accuracyColor(positionSource.positionInformation.vacc) : Theme.secondaryTextColor
      }

      // === NTRIP ===
      SectionHeader {
        visible: positioningSettings.enableNtripClient
        title: qsTr("NTRIP")
      }

      DataRow {
        visible: positioningSettings.enableNtripClient
        label: qsTr("State")
        value: {
          switch (positionSource.ntripState) {
          case 1:
            return qsTr("Connecting...");
          case 2:
            return qsTr("Connected");
          case 3:
            return qsTr("Reconnecting...");
          case 4:
            return qsTr("Error");
          default:
            return qsTr("Disconnected");
          }
        }
        valueColor: Theme.ntripStateColor(positionSource.ntripState)
      }

      DataRow {
        visible: positioningSettings.enableNtripClient
        label: qsTr("Details")
        value: positionSource.ntripStatus || "\u2014"
      }

      DataRow {
        visible: positioningSettings.enableNtripClient
        label: qsTr("Data")
        value: positionSource.ntripState > 0 ? "\u2193 " + gnssStatusPage.formatBytes(positionSource.ntripBytesReceived) + " / \u2191 " + gnssStatusPage.formatBytes(positionSource.ntripBytesSent) : "\u2014"
      }

      // === Differential ===
      SectionHeader {
        title: qsTr("Differential")
      }

      DataRow {
        label: qsTr("Differential age")
      }

      DataRow {
        label: qsTr("Station ID")
      }

      // === Baseline ===
      SectionHeader {
        title: qsTr("Baseline")
      }

      DataRow {
        label: qsTr("Length")
      }

      // === Battery ===
      SectionHeader {
        title: qsTr("Battery")
      }

      DataRow {
        label: qsTr("Level")
        Layout.bottomMargin: 16
      }
    }
  }

  Keys.onReleased: event => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true;
      finished();
    }
  }
}
