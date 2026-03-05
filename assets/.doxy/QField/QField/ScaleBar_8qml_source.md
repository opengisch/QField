

# File ScaleBar.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**ScaleBar.qml**](ScaleBar_8qml.md)

[Go to the documentation of this file](ScaleBar_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qfield
import org.qgis

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
    color: Theme.toolButtonBackgroundColor
    style: Text.Outline
    styleColor: Qt.hsla(Theme.toolButtonColor.hslHue, Theme.toolButtonColor.hslSaturation, Theme.toolButtonColor.hslLightness, 0.8)

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
      strokeColor: Qt.hsla(Theme.toolButtonColor.hslHue, Theme.toolButtonColor.hslSaturation, Theme.toolButtonColor.hslLightness, 0.8)
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
      strokeColor: Theme.toolButtonBackgroundColor
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
```


