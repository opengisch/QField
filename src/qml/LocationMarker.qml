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

    x: location.x - width/2
    y: location.y - height/2

    radius: width/2

    gradient: Gradient  {
        GradientStop  {
            position: 0.0
            SequentialAnimation on color  {
                loops: Animation.Infinite
                ColorAnimation  { from: "Lightpink"; to: "Mistyrose"; duration: 3000 }
                ColorAnimation  { from: "Mistyrose"; to: "Lightpink"; duration: 3000 }
            }
        }
        GradientStop  {
            position: 1.0
            SequentialAnimation on color  {
                loops: Animation.Infinite
                ColorAnimation  { from: "Indianred"; to: "Darkred"; duration: 5000 }
                ColorAnimation  { from: "Darkred"; to: "Indianred"; duration: 5000 }
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
