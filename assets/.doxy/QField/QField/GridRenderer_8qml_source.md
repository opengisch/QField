

# File GridRenderer.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**GridRenderer.qml**](GridRenderer_8qml.md)

[Go to the documentation of this file](GridRenderer_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qfield

Item {
  id: gridRenderer

  property alias enabled: gridModel.enabled
  property alias mapSettings: gridModel.mapSettings
  property alias indeterminate: gridModel.indeterminate

  property alias xInterval: gridModel.xInterval
  property alias yInterval: gridModel.yInterval
  property alias xOffset: gridModel.xOffset
  property alias yOffset: gridModel.yOffset

  property alias autoColor: gridModel.autoColor
  property alias majorLineColor: gridModel.majorLineColor
  property alias minorLineColor: gridModel.minorLineColor
  property alias markerColor: gridModel.markerColor

  property alias annotationColor: gridModel.annotationColor
  property alias annotationOutlineColor: gridModel.annotationOutlineColor
  property alias annotationHasOutline: gridModel.annotationHasOutline
  property alias annotationPrecision: gridModel.annotationPrecision

  property alias prepareLines: gridModel.prepareLines
  property alias prepareMarkers: gridModel.prepareMarkers
  property alias prepareAnnotations: gridModel.prepareAnnotations

  GridModel {
    id: gridModel

    onMajorLinesChanged: {
      let svgPath = "";
      for (const line of majorLines) {
        svgPath += "M " + (line[0].x) + " " + (line[0].y) + " L " + (line[1].x) + " " + (line[1].y) + " ";
      }
      majorLineSvgPath.path = svgPath;
    }

    onMinorLinesChanged: {
      let svgPath = "";
      for (const line of minorLines) {
        svgPath += "M " + (line[0].x) + " " + (line[0].y) + " L " + (line[1].x) + " " + (line[1].y) + " ";
      }
      minorLineSvgPath.path = svgPath;
    }

    onMarkersChanged: {
      let svgPath = "";
      for (const marker of markers) {
        svgPath += "M " + (marker.x) + " " + (marker.y - 5) + " L " + (marker.x) + " " + (marker.y + 5) + " " + "M " + (marker.x - 5) + " " + (marker.y) + " L " + (marker.x + 5) + " " + (marker.y) + " ";
      }
      markerSvgPath.path = svgPath;
    }
  }

  Shape {
    id: minorLinesContainer
    visible: gridModel.prepareLines && gridModel.minorLines.length > 0
    anchors.fill: parent

    ShapePath {
      id: minorLinesPath
      strokeColor: minorLineColor
      strokeWidth: 1
      strokeStyle: ShapePath.SolidLine
      fillColor: "transparent"
      joinStyle: ShapePath.RoundJoin
      capStyle: ShapePath.RoundCap

      PathSvg {
        id: minorLineSvgPath
        path: ""
      }
    }
  }

  Shape {
    id: majorLinesContainer
    visible: gridModel.prepareLines && gridModel.majorLines.length > 0
    anchors.fill: parent

    ShapePath {
      id: majorLinesPath
      strokeColor: majorLineColor
      strokeWidth: 1
      strokeStyle: ShapePath.SolidLine
      fillColor: "transparent"
      joinStyle: ShapePath.RoundJoin
      capStyle: ShapePath.RoundCap

      PathSvg {
        id: majorLineSvgPath
        path: ""
      }
    }
  }

  Shape {
    id: markersContainer
    visible: gridModel.prepareMarkers && gridModel.markers.length > 0
    anchors.fill: parent

    ShapePath {
      id: markerPath
      strokeColor: markerColor
      strokeWidth: 2
      strokeStyle: ShapePath.SolidLine
      fillColor: "transparent"
      joinStyle: ShapePath.RoundJoin
      capStyle: ShapePath.RoundCap

      PathSvg {
        id: markerSvgPath
        path: ""
      }
    }
  }

  Repeater {
    id: annotationsContainer
    model: gridModel.annotations

    Rectangle {
      x: modelData.coordinate.x - width / 2
      y: modelData.coordinate.y - height / 2

      width: 1
      height: 1

      color: majorLineColor

      Text {
        id: annotation
        anchors.top: modelData.position === GridAnnotation.Top ? parent.bottom : undefined
        anchors.bottom: modelData.position === GridAnnotation.Bottom ? parent.top : undefined
        anchors.left: modelData.position === GridAnnotation.Left ? parent.right : undefined
        anchors.right: modelData.position === GridAnnotation.Right ? parent.left : undefined
        anchors.horizontalCenter: modelData.position === GridAnnotation.Top || modelData.position === GridAnnotation.Bottom ? parent.horizontalCenter : undefined
        anchors.verticalCenter: modelData.position === GridAnnotation.Left || modelData.position === GridAnnotation.Right ? parent.verticalCenter : undefined

        font: Theme.tinyFont
        color: annotationColor
        style: annotationHasOutline ? Text.Outline : Text.Normal
        styleColor: annotationOutlineColor

        text: Number(modelData.value).toLocaleString(Qt.locale(), 'f', annotationPrecision)
      }
    }
  }
}
```


