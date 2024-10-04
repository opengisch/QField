import QtQuick
import QtQuick.Shapes
import org.qfield
import Theme

Item {
  id: gridRenderer

  property alias mapSettings: gridModel.mapSettings

  property alias xInterval: gridModel.xInterval
  property alias yInterval: gridModel.yInterval
  property alias xOffset: gridModel.xOffset
  property alias yOffset: gridModel.yOffset

  property color lineColor: "#000000"
  property color markerColor: "#000000"
  property color annotationColor: "#000000"

  GridModel {
    id: gridModel

    onMarkersChanged: {
      let svgPath = "";
      if (gridModel.markers.length > 0 && gridModel.markers.length < 2000) {
        for (const marker of gridModel.markers) {
          svgPath += "M " + (marker.x) + " " + (marker.y - 5) + " L " + (marker.x) + " " + (marker.y + 5) + " " + "M " + (marker.x - 5) + " " + (marker.y) + " L " + (marker.x + 5) + " " + (marker.y) + " ";
        }
      }
      markerSvgPath.path = svgPath;
    }
  }

  Instantiator {
    id: lineInstantiator
    asynchronous: true
    model: gridModel.lines
    onModelChanged: linesPath.pathElements = []
    onObjectAdded: (index, object) => linesPath.pathElements.push(object)

    PathPolyline {
      path: modelData
    }
  }

  Shape {
    id: lines
    visible: gridModel.lines.length > 0
    anchors.fill: parent

    ShapePath {
      id: linesPath
      strokeColor: lineColor
      strokeWidth: 1
      strokeStyle: ShapePath.SolidLine
      fillColor: "transparent"
      joinStyle: ShapePath.RoundJoin
      capStyle: ShapePath.RoundCap
    }
  }

  Shape {
    id: markers
    visible: gridModel.markers.length > 0
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
    id: annotations
    model: gridModel.annotations

    Rectangle {
      x: modelData.coordinate.x - width / 2
      y: modelData.coordinate.y - height / 2

      width: 1
      height: 1

      color: lineColor

      Text {
        anchors.top: modelData.position === GridAnnotation.Top ? parent.bottom : undefined
        anchors.bottom: modelData.position === GridAnnotation.Bottom ? parent.top : undefined
        anchors.left: modelData.position === GridAnnotation.Left ? parent.right : undefined
        anchors.right: modelData.position === GridAnnotation.Right ? parent.left : undefined
        anchors.horizontalCenter: modelData.position === GridAnnotation.Top || modelData.position === GridAnnotation.Bottom ? parent.horizontalCenter : undefined
        anchors.verticalCenter: modelData.position === GridAnnotation.Left || modelData.position === GridAnnotation.Right ? parent.verticalCenter : undefined

        font: Theme.tinyFont
        color: annotationColor
        style: Text.Outline
        styleColor: "#ffffff"

        text: modelData.label
      }
    }
  }
}
