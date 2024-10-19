import QtQuick
import QtQuick.Shapes
import org.qfield
import org.qgis
import Theme

/**
 * \ingroup qml
 */
Item {
  id: scaleBar

  property alias mapSettings: measurement.mapSettings
  property double lineWidth: 2

  height: childrenRect.height

  ScaleBarMeasurement {
    id: measurement
    project: qgisProject
    referenceScreenLength: 300
  }

  Text {
    id: label
    anchors.horizontalCenter: bar.horizontalCenter
    anchors.left: undefined
    font: Theme.defaultFont
    color: Theme.darkGray
    style: Text.Outline
    styleColor: "#CCFFFFFF"

    states: State {
      name: "narrow"
      when: label.width > bar.width
      AnchorChanges {
        target: label
        anchors.horizontalCenter: undefined
        anchors.left: bar.left
      }
    }

    text: measurement.label
  }

  Shape {
    id: bar
    anchors.top: label.bottom
    anchors.topMargin: 2
    width: measurement.screenLength
    height: 8

    ShapePath {
      strokeWidth: barLine.strokeWidth + 1.5
      strokeColor: "#CCFFFFFF"
      fillColor: "transparent"
      startX: 0
      startY: 0

      PathLine {
        x: 0
        y: bar.height
      }
      PathLine {
        x: measurement.screenLength
        y: bar.height
      }
      PathLine {
        x: measurement.screenLength
        y: 0
      }
    }

    ShapePath {
      id: barLine
      strokeWidth: scaleBar.lineWidth
      strokeColor: "#000000"
      fillColor: "transparent"
      startX: 0
      startY: 0

      PathLine {
        x: 0
        y: bar.height
      }
      PathLine {
        x: measurement.screenLength
        y: bar.height
      }
      PathLine {
        x: measurement.screenLength
        y: 0
      }
    }
  }
}
