import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes
import org.qfield
import Theme

/**
 * \ingroup qml
 *
 * Full-screen Skyplot page showing satellite positions on a polar view
 * with SNR bars and positioning info.
 */
Page {
  id: skyplotPage

  signal finished

  visible: false
  focus: visible

  leftPadding: mainWindow.sceneLeftMargin
  rightPadding: mainWindow.sceneRightMargin

  header: QfPageHeader {
    title: qsTr("Skyplot")

    showBackButton: true
    showApplyButton: false
    showCancelButton: false
    backAsCancel: true

    topMargin: mainWindow.sceneTopMargin

    onFinished: {
      parent.finished();
    }
  }

  function constellationPrefix(constellation) {
    switch (constellation) {
    case "GPS":
      return "G";
    case "GLONASS":
      return "R";
    case "Galileo":
      return "E";
    case "BeiDou":
      return "B";
    case "SBAS":
      return "S";
    case "QZSS":
      return "J";
    default:
      return "";
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
      spacing: 12

      // === Info Block (AC7) ===
      GridLayout {
        Layout.fillWidth: true
        Layout.leftMargin: 12
        Layout.rightMargin: 12
        Layout.topMargin: 8
        columns: 4
        rowSpacing: 4
        columnSpacing: 8

        // Row 1: UTC time + Fix type
        Text {
          font: Theme.tipFont
          color: Theme.secondaryTextColor
          text: qsTr("UTC")
        }
        Text {
          font: Theme.tipFont
          color: Theme.mainTextColor
          text: positionSource.positionInformation && positionSource.positionInformation.utcDateTime ? positionSource.positionInformation.utcDateTime.toLocaleTimeString(Qt.locale(), "HH:mm:ss") : qsTr("N/A")
          Layout.fillWidth: true
        }
        Text {
          font: Theme.tipFont
          color: Theme.secondaryTextColor
          text: qsTr("Fix")
        }
        Text {
          font: Theme.strongTipFont
          color: positionSource.positionInformation ? Theme.fixTypeColor(positionSource.positionInformation.quality) : Theme.secondaryTextColor
          text: positionSource.positionInformation ? positionSource.positionInformation.qualityDescription : qsTr("N/A")
          Layout.fillWidth: true
        }

        // Row 2: Lat + HDOP
        Text {
          font: Theme.tipFont
          color: Theme.secondaryTextColor
          text: qsTr("Lat")
        }
        Text {
          font: Theme.tipFont
          color: Theme.mainTextColor
          text: positionSource.positionInformation && positionSource.positionInformation.latitudeValid ? positionSource.positionInformation.latitude.toFixed(6) + "°" : qsTr("N/A")
          Layout.fillWidth: true
        }
        Text {
          font: Theme.tipFont
          color: Theme.secondaryTextColor
          text: qsTr("HDOP")
        }
        Text {
          font: Theme.tipFont
          color: Theme.mainTextColor
          text: positionSource.positionInformation && positionSource.positionInformation.hdop > 0 ? positionSource.positionInformation.hdop.toFixed(1) : qsTr("N/A")
          Layout.fillWidth: true
        }

        // Row 3: Lon + Satellites count
        Text {
          font: Theme.tipFont
          color: Theme.secondaryTextColor
          text: qsTr("Lon")
        }
        Text {
          font: Theme.tipFont
          color: Theme.mainTextColor
          text: positionSource.positionInformation && positionSource.positionInformation.longitudeValid ? positionSource.positionInformation.longitude.toFixed(6) + "°" : qsTr("N/A")
          Layout.fillWidth: true
        }
        Text {
          font: Theme.tipFont
          color: Theme.secondaryTextColor
          text: qsTr("Sats")
        }
        Text {
          font: Theme.tipFont
          color: Theme.mainTextColor
          text: (positionSource.positionInformation ? positionSource.positionInformation.satellitesUsed : 0) + " / " + positionSource.satelliteModel.count
          Layout.fillWidth: true
        }
      }

      // === Polar View (AC2, AC3, AC4, AC9) ===
      Item {
        id: polarContainer

        property real polarSize: Math.min(Math.max(250, contentColumn.width * 0.85), contentColumn.width - 20)
        property real labelPadding: 20
        property real maxRadius: (polarSize / 2) - labelPadding
        property real centerX: polarSize / 2
        property real centerY: polarSize / 2

        Layout.preferredWidth: polarSize
        Layout.preferredHeight: polarSize
        Layout.alignment: Qt.AlignHCenter

        // Polar grid
        Shape {
          anchors.fill: parent

          // Outermost circle (0° elevation)
          ShapePath {
            strokeWidth: 2
            strokeColor: Theme.mainTextColor
            strokeStyle: ShapePath.SolidLine
            fillColor: "transparent"
            startX: 0
            startY: 0
            PathAngleArc {
              centerX: polarContainer.centerX
              centerY: polarContainer.centerY
              radiusX: polarContainer.maxRadius
              radiusY: polarContainer.maxRadius
              startAngle: 0
              sweepAngle: 360
            }
          }

          // 30° elevation circle (radius = 2/3 of maxRadius)
          ShapePath {
            strokeWidth: 1
            strokeColor: Theme.secondaryTextColor
            strokeStyle: ShapePath.SolidLine
            fillColor: "transparent"
            startX: 0
            startY: 0
            PathAngleArc {
              centerX: polarContainer.centerX
              centerY: polarContainer.centerY
              radiusX: polarContainer.maxRadius * 2 / 3
              radiusY: polarContainer.maxRadius * 2 / 3
              startAngle: 0
              sweepAngle: 360
            }
          }

          // 60° elevation circle (radius = 1/3 of maxRadius)
          ShapePath {
            strokeWidth: 1
            strokeColor: Theme.secondaryTextColor
            strokeStyle: ShapePath.SolidLine
            fillColor: "transparent"
            startX: 0
            startY: 0
            PathAngleArc {
              centerX: polarContainer.centerX
              centerY: polarContainer.centerY
              radiusX: polarContainer.maxRadius / 3
              radiusY: polarContainer.maxRadius / 3
              startAngle: 0
              sweepAngle: 360
            }
          }

          // N-S axis
          ShapePath {
            strokeWidth: 1
            strokeColor: Theme.secondaryTextColor
            strokeStyle: ShapePath.SolidLine
            fillColor: "transparent"
            startX: polarContainer.centerX
            startY: polarContainer.centerY - polarContainer.maxRadius
            PathLine {
              x: polarContainer.centerX
              y: polarContainer.centerY + polarContainer.maxRadius
            }
          }

          // E-W axis
          ShapePath {
            strokeWidth: 1
            strokeColor: Theme.secondaryTextColor
            strokeStyle: ShapePath.SolidLine
            fillColor: "transparent"
            startX: polarContainer.centerX - polarContainer.maxRadius
            startY: polarContainer.centerY
            PathLine {
              x: polarContainer.centerX + polarContainer.maxRadius
              y: polarContainer.centerY
            }
          }
        }

        // Cardinal labels
        Text {
          text: "N"
          font: Theme.strongFont
          color: Theme.mainTextColor
          anchors.horizontalCenter: parent.horizontalCenter
          anchors.top: parent.top
          anchors.topMargin: polarContainer.labelPadding - height - 2
        }
        Text {
          text: "S"
          font: Theme.strongFont
          color: Theme.mainTextColor
          anchors.horizontalCenter: parent.horizontalCenter
          anchors.bottom: parent.bottom
          anchors.bottomMargin: polarContainer.labelPadding - height - 2
        }
        Text {
          text: "E"
          font: Theme.strongFont
          color: Theme.mainTextColor
          anchors.verticalCenter: parent.verticalCenter
          anchors.right: parent.right
          anchors.rightMargin: polarContainer.labelPadding - width - 2
        }
        Text {
          text: "W"
          font: Theme.strongFont
          color: Theme.mainTextColor
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          anchors.leftMargin: polarContainer.labelPadding - width - 2
        }

        // Elevation labels on north axis
        Text {
          text: "30°"
          font: Theme.tinyFont
          color: Theme.secondaryTextColor
          x: polarContainer.centerX + 3
          y: polarContainer.centerY - polarContainer.maxRadius * 2 / 3 - height / 2
        }
        Text {
          text: "60°"
          font: Theme.tinyFont
          color: Theme.secondaryTextColor
          x: polarContainer.centerX + 3
          y: polarContainer.centerY - polarContainer.maxRadius / 3 - height / 2
        }

        // Satellite markers (AC3, AC4)
        Repeater {
          model: positionSource.satelliteModel

          delegate: Item {
            id: satDelegate

            property real satRadius: (90 - Math.max(0, Math.min(90, model.elevation))) / 90 * polarContainer.maxRadius
            property real satX: polarContainer.centerX + satRadius * Math.sin(model.azimuth * Math.PI / 180)
            property real satY: polarContainer.centerY - satRadius * Math.cos(model.azimuth * Math.PI / 180)
            property color satColor: Theme.constellationColor(model.constellation)

            x: satX - 8
            y: satY - 8
            width: 16
            height: 16

            // Filled circle for in-use satellites
            Rectangle {
              anchors.centerIn: parent
              width: 12
              height: 12
              radius: 6
              color: satDelegate.satColor
              visible: model.inUse
            }

            // Triangle for tracked-only satellites
            Shape {
              anchors.centerIn: parent
              width: 14
              height: 12
              visible: !model.inUse
              ShapePath {
                strokeWidth: 0
                strokeColor: "transparent"
                fillColor: satDelegate.satColor
                startX: 7
                startY: 0
                PathLine {
                  x: 14
                  y: 12
                }
                PathLine {
                  x: 0
                  y: 12
                }
                PathLine {
                  x: 7
                  y: 0
                }
              }
            }

            // Satellite ID label
            Text {
              anchors.left: parent.right
              anchors.verticalCenter: parent.verticalCenter
              text: model.satId
              font: Theme.tinyFont
              color: Theme.mainTextColor
            }
          }
        }

        // Empty state (AC9)
        Text {
          anchors.centerIn: parent
          visible: positionSource.satelliteModel.count === 0
          text: qsTr("No satellites")
          font: Theme.strongFont
          color: Theme.secondaryTextColor
        }
      }

      // === SNR Bars (AC5) ===
      Item {
        Layout.fillWidth: true
        Layout.preferredHeight: 120
        Layout.leftMargin: 8
        Layout.rightMargin: 8

        Flickable {
          anchors.fill: parent
          contentWidth: snrRow.width
          clip: true
          boundsBehavior: Flickable.StopAtBounds
          flickableDirection: Flickable.HorizontalFlick

          Row {
            id: snrRow
            height: parent.height
            spacing: 2

            Repeater {
              model: positionSource.satelliteModel

              delegate: Item {
                width: 28
                height: snrRow.height
                visible: model.snr > 0

                property real maxBarHeight: height - snrValueText.height - satIdText.height - 8
                property real barHeight: model.snr > 0 ? Math.max(2, (model.snr / 50) * maxBarHeight) : 0

                Text {
                  id: snrValueText
                  anchors.horizontalCenter: parent.horizontalCenter
                  y: snrBar.y - height - 1
                  text: model.snr > 0 ? model.snr.toString() : ""
                  font: Theme.tinyFont
                  color: Theme.secondaryTextColor
                }

                Rectangle {
                  id: snrBar
                  anchors.horizontalCenter: parent.horizontalCenter
                  width: 18
                  height: barHeight
                  y: parent.height - satIdText.height - 4 - height
                  color: Theme.constellationColor(model.constellation)
                  radius: 2
                }

                Text {
                  id: satIdText
                  anchors.horizontalCenter: parent.horizontalCenter
                  anchors.bottom: parent.bottom
                  text: skyplotPage.constellationPrefix(model.constellation) + model.satId
                  font: Theme.tinyFont
                  color: Theme.secondaryTextColor
                }
              }
            }
          }
        }
      }

      // === Constellation Legend (AC8) ===
      Flow {
        Layout.fillWidth: true
        Layout.leftMargin: 12
        Layout.rightMargin: 12
        Layout.bottomMargin: 12
        spacing: 16

        // Constellation colors
        Row {
          spacing: 4
          Rectangle {
            width: 10
            height: 10
            radius: 5
            color: Theme.constellationGPS
            anchors.verticalCenter: parent.verticalCenter
          }
          Text {
            text: "GPS"
            font: Theme.tinyFont
            color: Theme.mainTextColor
            anchors.verticalCenter: parent.verticalCenter
          }
        }
        Row {
          spacing: 4
          Rectangle {
            width: 10
            height: 10
            radius: 5
            color: Theme.constellationGLONASS
            anchors.verticalCenter: parent.verticalCenter
          }
          Text {
            text: "GLONASS"
            font: Theme.tinyFont
            color: Theme.mainTextColor
            anchors.verticalCenter: parent.verticalCenter
          }
        }
        Row {
          spacing: 4
          Rectangle {
            width: 10
            height: 10
            radius: 5
            color: Theme.constellationGalileo
            anchors.verticalCenter: parent.verticalCenter
          }
          Text {
            text: "Galileo"
            font: Theme.tinyFont
            color: Theme.mainTextColor
            anchors.verticalCenter: parent.verticalCenter
          }
        }
        Row {
          spacing: 4
          Rectangle {
            width: 10
            height: 10
            radius: 5
            color: Theme.constellationBeiDou
            anchors.verticalCenter: parent.verticalCenter
          }
          Text {
            text: "BeiDou"
            font: Theme.tinyFont
            color: Theme.mainTextColor
            anchors.verticalCenter: parent.verticalCenter
          }
        }

        // Shape distinction
        Row {
          spacing: 4
          Text {
            text: "\u25CF"
            font: Theme.tipFont
            color: Theme.mainTextColor
            anchors.verticalCenter: parent.verticalCenter
          }
          Text {
            text: qsTr("Used")
            font: Theme.tinyFont
            color: Theme.mainTextColor
            anchors.verticalCenter: parent.verticalCenter
          }
        }
        Row {
          spacing: 4
          Text {
            text: "\u25B2"
            font: Theme.tipFont
            color: Theme.mainTextColor
            anchors.verticalCenter: parent.verticalCenter
          }
          Text {
            text: qsTr("In view")
            font: Theme.tinyFont
            color: Theme.mainTextColor
            anchors.verticalCenter: parent.verticalCenter
          }
        }
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
