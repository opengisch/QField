import QtQuick 2.0
import org.qgis 1.0

Item {
  property point location
  property string icon
  property MapSettings mapSettings
  property CoordinateTransform coordinateTransform
  property int iconWidth
  property int iconHeitht

  Rectangle {
    id: marker
    property point location
    width: 15
    height: 15

    x: location.x
    y: location.y

    radius: width/2

    color: "red"


    Connections {
      target: mapSettings
      onExtentChanged: {
        marker.location = mapSettings.coordinateToScreen( coordinateTransform.transform( location ) )
        console.info( "Marker location: " + marker.location )
      }
    }
  }
}
