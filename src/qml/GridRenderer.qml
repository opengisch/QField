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
  property color annotationColor: "#000000"

  GridModel {
    id: gridModel
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

  Repeater {
    id: annotations
    model: gridModel.annotations

    Rectangle {
      x: modelData.coordinate.x - width / 2
      y: modelData.coordinate.y - height / 2

      width: 4
      height: 4
      radius: width / 2

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
