import QtQuick
import QtQuick.Shapes
import org.qfield
import org.qgis

/**
 * \ingroup qml
 */
RubberbandShape {
  id: rubberbandShape

  property bool showVertices: false
  property var activePolyline: rubberbandShape.polylines.length > 0 ? rubberbandShape.polylines[0] : []

  Shape {
    anchors.fill: parent
    ShapePath {
      strokeColor: rubberbandShape.outlineColor
      strokeWidth: rubberbandShape.lineWidth / rubberbandShape.scale + 2
      strokeStyle: ShapePath.SolidLine
      fillColor: "transparent"
      joinStyle: ShapePath.RoundJoin
      capStyle: ShapePath.RoundCap

      PathPolyline {
        path: activePolyline
      }
    }
    ShapePath {
      strokeColor: rubberbandShape.color
      strokeWidth: rubberbandShape.lineWidth / rubberbandShape.scale
      strokeStyle: ShapePath.SolidLine
      fillColor: rubberbandShape.polylinesType === Qgis.GeometryType.Polygon ? Qt.hsla(strokeColor.hslHue, strokeColor.hslSaturation, strokeColor.hslLightness, 0.25) : "transparent"
      joinStyle: ShapePath.RoundJoin
      capStyle: ShapePath.RoundCap

      PathPolyline {
        path: activePolyline
      }
    }

    Repeater {
      id: rubberbandVertices
      model: showVertices && activePolyline.length > 1 ? activePolyline : []

      Rectangle {
        width: rubberbandShape.lineWidth / rubberbandShape.scale * 2
        height: width

        x: modelData.x - width / 2
        y: modelData.y - width / 2

        color: rubberbandShape.color
        border.width: 1
        border.color: rubberbandShape.outlineColor
      }
    }
  }
}
