import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
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
  readonly property bool isDataCurrent: positionSource.positionInformation.isValid && positionSource.currentness

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
        value: positionSource.positionInformation.sourceName || "\u2014"
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
