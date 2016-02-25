import QtQuick 2.0
import org.qgis 1.0

Item {
  property point location
  property MapSettings mapSettings
  property CoordinateTransform coordinateTransform

  Rectangle {
    id: marker
    property point location
    width: 15
    height: 15

    x: location.x - width/2
    y: location.y - height/2

    radius: width/2

    gradient: Gradient  {
        GradientStop  {
            position: 0.0
            SequentialAnimation on color  {
                loops: Animation.Infinite
                ColorAnimation  { from: "Mistyrose"; to: "Indianred"; duration: 3000; easing.type: Easing.InOutQuad }
                ColorAnimation  { from: "Indianred"; to: "Mistyrose"; duration: 2000; easing.type: Easing.InOutQuad }
            }
        }
        GradientStop  {
            position: 1.0
            SequentialAnimation on color  {
                loops: Animation.Infinite
                ColorAnimation  { from: "Lightpink"; to: "Darkred"; duration: 3000; easing.type: Easing.InOutQuad }
                ColorAnimation  { from: "Darkred"; to: "Lightpink"; duration: 2000; easing.type: Easing.InOutQuad }
            }
        }
    }
    Connections {
      target: mapSettings
      onExtentChanged: {
        marker.location = mapSettings.coordinateToScreen( coordinateTransform.transform( location ) )
      }
    }
  }
}
