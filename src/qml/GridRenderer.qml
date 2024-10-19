import QtQuick
import QtQuick.Shapes
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Item {
  id: gridRenderer

  property alias enabled: gridModel.enabled
  property alias mapSettings: gridModel.mapSettings

  property alias xInterval: gridModel.xInterval
  property alias yInterval: gridModel.yInterval
  property alias xOffset: gridModel.xOffset
  property alias yOffset: gridModel.yOffset

  property alias prepareLines: gridModel.prepareLines
  property color lineColor: "#000000"

  property alias prepareMarkers: gridModel.prepareMarkers
  property color markerColor: "#000000"

  property alias prepareAnnotations: gridModel.prepareAnnotations
  property int annotationPrecision: 0
  property color annotationColor: "#000000"
  property bool annotationHasOutline: false
  property color annotationOutlineColor: "#ffffff"

  GridModel {
    id: gridModel

    onLinesChanged: {
      let svgPath = "";
      for (const line of lines) {
        svgPath += "M " + (line[0].x) + " " + (line[0].y) + " L " + (line[1].x) + " " + (line[1].y) + " ";
      }
      lineSvgPath.path = svgPath;
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
    id: linesContainer
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

      PathSvg {
        id: lineSvgPath
        path: ""
      }
    }
  }

  Shape {
    id: markersContainer
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
    id: annotationsContainer
    model: gridModel.annotations

    Rectangle {
      x: modelData.coordinate.x - width / 2
      y: modelData.coordinate.y - height / 2

      width: 1
      height: 1

      color: lineColor

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
