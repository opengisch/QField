

# File QfGridRenderer.qml

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qml**](dir_5e9804d2322dd99f421cb2c64b2c04c3.md) **>** [**QfGridRenderer.qml**](QfGridRenderer_8qml.md)

[Go to the documentation of this file](QfGridRenderer_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qfield.core

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
  property font annotationFont

  property alias prepareLines: gridModel.prepareLines
  property alias prepareMarkers: gridModel.prepareMarkers
  property alias prepareAnnotations: gridModel.prepareAnnotations

  QfGridModel {
    id: gridModel
  }

  Shape {
    id: minorLinesContainer
    visible: gridModel.prepareLines && gridModel.minorLinesPath != ""
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
        path: gridModel.minorLinesPath
      }
    }
  }

  Shape {
    id: majorLinesContainer
    visible: gridModel.prepareLines && gridModel.majorLinesPath != ""
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
        path: gridModel.majorLinesPath
      }
    }
  }

  Shape {
    id: markersContainer
    visible: gridModel.prepareMarkers && gridModel.markersPath != ""
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
        path: gridModel.markersPath
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
        anchors.top: modelData.position === QfGridAnnotation.Top ? parent.bottom : undefined
        anchors.bottom: modelData.position === QfGridAnnotation.Bottom ? parent.top : undefined
        anchors.left: modelData.position === QfGridAnnotation.Left ? parent.right : undefined
        anchors.right: modelData.position === QfGridAnnotation.Right ? parent.left : undefined
        anchors.horizontalCenter: modelData.position === QfGridAnnotation.Top || modelData.position === QfGridAnnotation.Bottom ? parent.horizontalCenter : undefined
        anchors.verticalCenter: modelData.position === QfGridAnnotation.Left || modelData.position === QfGridAnnotation.Right ? parent.verticalCenter : undefined

        font: gridRenderer.annotationFont
        color: annotationColor
        style: annotationHasOutline ? Text.Outline : Text.Normal
        styleColor: annotationOutlineColor

        text: Number(modelData.value).toLocaleString(Qt.locale(), 'f', annotationPrecision)
      }
    }
  }
}
```


