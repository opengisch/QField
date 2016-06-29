import QtQuick 2.0
import org.qgis 1.0

Item {
  id: item

  property point location
  property MapSettings mapSettings
  property CoordinateTransform coordinateTransform

  Rectangle {
    id: marker
    property point location
    width: 10*dp
    height: 10*dp

    x: location.x - width/2
    y: location.y - height/2

    radius: width/2

    gradient: Gradient  {
      GradientStop  {
        position: 0.0
        color: "#ffffff"
        SequentialAnimation on color  {
          loops: Animation.Infinite
          ColorAnimation  { from: "#AD1457"; to: "#FCE4EC"; duration: 3000; easing.type: Easing.InOutQuad }
          ColorAnimation  { from: "#FCE4EC"; to: "#AD1457"; duration: 2000; easing.type: Easing.InOutQuad }
        }
      }
      GradientStop  {
        position: 1.0
        SequentialAnimation on color  {
          loops: Animation.Infinite
          ColorAnimation  { from: "#FCE4EC"; to: "#AD1457"; duration: 3000; easing.type: Easing.InOutQuad }
          ColorAnimation  { from: "#AD1457"; to: "#FCE4EC"; duration: 2000; easing.type: Easing.InOutQuad }
        }
      }
    }
    border.color: "#880E4F"
    border.width: 0.7 * dp

    Connections {
      target: mapSettings
      onExtentChanged: {
        marker.location = mapSettings.coordinateToScreen( coordinateTransform.transform( location ) )
      }
    }
  }

  onLocationChanged: {
    marker.location = mapSettings.coordinateToScreen( coordinateTransform.transform( location ) )
  }
}
