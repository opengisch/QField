import QtQuick 2.0
import org.qgis 1.0

Row {
  property Geometry geometry
  property VectorLayer currentLayer

  signal geometryDigitized


  QFButton {
    id: addVertexButton

    icon: "/themes/holodark/accept.png"

    onClicked: {
      geometryDigitized()
    }
  }
}
