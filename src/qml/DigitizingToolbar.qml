import QtQuick 2.0
import org.qgis 1.0

Row {
  property alias geometry: feature.geometry
  property alias currentLayer: feature.layer

  signal geometryDigitized

  FeatureModel{
    id: feature
    geometry: Geometry {}
  }

  QFButton {
    id: addVertexButton

    icon: "/themes/holodark/accept.png"

    onClicked: {
      geometryDigitized()
      feature.applyGeometry()
      feature.create()
      feature.save()
    }
  }
}
