import QtQuick 2.0

Row {
  QFButton {
    id: addVertexButton

    icon: "/themes/holodark/location_off.png"

    onClicked: {
      iface.openFeatureForm()

      layerSelector.model.
      displayToast( qsTr( "Point digitized. " + coordinateLocator.coordinate.x + " / " + coordinateLocator.coordinate.y ) )
    }
  }
}
