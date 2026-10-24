

# File QfScaleBar.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfScaleBar.qml**](QfScaleBar_8qml.md)

[Go to the documentation of this file](QfScaleBar_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qfield.core
import org.qfield.gui
import org.qgis

Item {
  id: scaleBar

  property alias mapSettings: measurement.mapSettings
  property double lineWidth: 2

  height: childrenRect.height

  QfScaleBarMeasurement {
    id: measurement
    project: qgisProject
    referenceScreenLength: 300
  }

  Text {
    id: label
    anchors.horizontalCenter: bar.horizontalCenter
    anchors.left: undefined
    font: QfTheme.defaultFont
    color: QfTheme.toolButtonBackgroundColor
    style: Text.Outline
    styleColor: Qt.hsla(QfTheme.toolButtonColor.hslHue, QfTheme.toolButtonColor.hslSaturation, QfTheme.toolButtonColor.hslLightness, 0.8)

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
      strokeColor: Qt.hsla(QfTheme.toolButtonColor.hslHue, QfTheme.toolButtonColor.hslSaturation, QfTheme.toolButtonColor.hslLightness, 0.8)
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
      strokeColor: QfTheme.toolButtonBackgroundColor
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


