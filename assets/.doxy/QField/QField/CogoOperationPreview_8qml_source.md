

# File CogoOperationPreview.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**CogoOperationPreview.qml**](CogoOperationPreview_8qml.md)

[Go to the documentation of this file](CogoOperationPreview_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qgis
import org.qfield

Item {
  id: cogoOperationPreview

  property alias visualGuides: repeater.model

  Component {
    id: labelVisualGuide

    Text {
      color: visualGuideColor
      text: visualGuideDetails.text
      font.pointSize: visualGuideDetails.size
      font.bold: true

      style: Text.Outline
      styleColor: visualGuideOutlineColor

      x: visualGuideDetails.point.x - contentWidth / 2
      y: visualGuideDetails.point.y - contentHeight / 2
    }
  }

  Component {
    id: pointVisualGuide

    Rectangle {
      color: visualGuideColor
      width: visualGuideDetails.size
      height: width
      radius: width / 2

      border.color: visualGuideOutlineColor
      border.width: 2

      x: visualGuideDetails.point.x - width / 2
      y: visualGuideDetails.point.y - width / 2
    }
  }

  Component {
    id: lineVisualGuide

    Shape {
      ShapePath {
        strokeColor: visualGuideOutlineColor
        strokeWidth: visualGuideDetails.size + 2
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        joinStyle: ShapePath.RoundJoin
        capStyle: ShapePath.RoundCap

        PathPolyline {
          path: visualGuideDetails.polyline
        }
      }
      ShapePath {
        strokeColor: visualGuideColor
        strokeWidth: visualGuideDetails.size
        strokeStyle: ShapePath.DashLine
        fillColor: "transparent"
        joinStyle: ShapePath.RoundJoin
        capStyle: ShapePath.RoundCap

        PathPolyline {
          path: visualGuideDetails.polyline
        }
      }
    }
  }

  Component {
    id: circleVisualGuide

    Shape {
      ShapePath {
        strokeColor: visualGuideOutlineColor
        strokeWidth: visualGuideDetails.size + 2
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        joinStyle: ShapePath.RoundJoin
        capStyle: ShapePath.RoundCap

        PathAngleArc {
          centerX: visualGuideDetails.center.x
          centerY: visualGuideDetails.center.y
          radiusX: visualGuideDetails.radius
          radiusY: visualGuideDetails.radius
          startAngle: 0
          sweepAngle: 360
        }
      }
      ShapePath {
        strokeColor: visualGuideColor
        strokeWidth: visualGuideDetails.size
        strokeStyle: ShapePath.DashLine
        fillColor: "transparent"
        joinStyle: ShapePath.RoundJoin
        capStyle: ShapePath.RoundCap

        PathAngleArc {
          centerX: visualGuideDetails.center.x
          centerY: visualGuideDetails.center.y
          radiusX: visualGuideDetails.radius
          radiusY: visualGuideDetails.radius
          startAngle: 0
          sweepAngle: 360
        }
      }
    }
  }

  Repeater {
    id: repeater

    Loader {
      property var visualGuideColor: modelData.color
      property var visualGuideOutlineColor: modelData.outlineColor
      property var visualGuideDetails: modelData.details

      sourceComponent: {
        if (modelData.type === CogoVisualGuide.Circle) {
          return circleVisualGuide;
        } else if (modelData.type === CogoVisualGuide.Line) {
          return lineVisualGuide;
        } else if (modelData.type === CogoVisualGuide.Label) {
          return labelVisualGuide;
        } else {
          return pointVisualGuide;
        }
      }
    }
  }
}
```


