import QtQuick 2.14

import org.qfield 1.0
import org.qgis 1.0
import Theme 1.0

Item {
  property alias mapSettings: measurement.mapSettings
  property double lineWidth: 2

  ScaleBarMeasurement {
      id: measurement
      project: qgisProject
      referenceScreenLength: 300
  }

  Rectangle {
    id: mainLineBackground
    width: measurement.screenLength + 2
    height: lineWidth + 2
    color: "#AAFFFFFF"
  }

  Rectangle {
    width: lineWidth + 2
    height: ( 3 * lineWidth ) + 1
    color: "#AAFFFFFF"
    anchors.left: mainLineBackground.left
    anchors.bottom: mainLineBackground.top
  }

  Rectangle {
    width: lineWidth + 2
    height: ( 3 * lineWidth ) + 1
    color: "#AAFFFFFF"
    anchors.right: mainLineBackground.right
    anchors.bottom: mainLineBackground.top
  }

  Rectangle {
    id: mainLine
    width: measurement.screenLength
    height: lineWidth
    color: Theme.darkGray
    anchors {
        horizontalCenter: mainLineBackground.horizontalCenter
        verticalCenter: mainLineBackground.verticalCenter
    }
  }

  Rectangle {
    width: lineWidth
    height: 3 * lineWidth
    color: Theme.darkGray
    anchors.left: mainLine.left
    anchors.bottom: mainLine.top
  }

  Rectangle {
    width: lineWidth
    height: 3 * lineWidth
    color: Theme.darkGray
    anchors.right: mainLine.right
    anchors.bottom: mainLine.top
  }

  Text {
    id: label
    anchors.bottomMargin: 7
    anchors.bottom: mainLine.top
    anchors.horizontalCenter: mainLine.horizontalCenter
    anchors.left: undefined // The value will be set to mainLine.left is the label is wider than the mainLine
    font: Theme.defaultFont
    color: Theme.darkGray

    style: Text.Outline
    styleColor: "#AAFFFFFF"

    states: State {
        name: "narrow"; when: label.width > mainLine.width
        AnchorChanges {
            target: label
            anchors.horizontalCenter: undefined
            anchors.left: mainLine.left
        }
    }

    text: measurement.label
  }
}
