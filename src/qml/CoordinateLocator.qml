import QtQuick 2.5
import org.qgis 1.0

import QtPositioning 5.3

Item {
  id: locator
  property MapSettings mapSettings
  property color color: "black"
  property point coordinate

  property bool __coordinateChangedByMapSettings: false


  Rectangle {
    anchors.centerIn: crosshairCircle

    width: crosshairCircle.width - 3
    height: crosshairCircle.height - 3

    border.color: "#77aaaaaa"
    border.width: 5
    radius: width / 2
    color: "transparent"
  }

  Rectangle {
    id: crosshairCircle

    x: parent.width / 2 - radius
    y: parent.height / 2 - radius

    border.color: parent.color
    border.width: 2
    color: "transparent"
    antialiasing: true

    width: 48*dp
    height: width
    radius: width / 2
  }

  Rectangle {
    anchors.centerIn: crosshairCircle

    color: parent.color

    width: 1.2
    height: 40*dp
  }

  Rectangle {
    anchors.centerIn: crosshairCircle

    color: parent.color

    width: 40*dp
    height: 1.2
  }

  Connections {
    target: mapSettings

    onExtentChanged: __updateCoordinate()
  }

  SequentialAnimation {
    id: flashAnimation

    ScaleAnimator {
      target: crosshairCircle
      from: 1
      to: 0.7
      duration: 150

      easing.type: Easing.InOutQuad
    }
    ScaleAnimator {
      target: crosshairCircle
      from: 0.7
      to: 1
      duration: 150

      easing.type: Easing.InOutCubic
    }
  }

  function flash()
  {
    flashAnimation.start()
  }

  function __updateCoordinate()
  {
    __coordinateChangedByMapSettings = true
    coordinate = mapSettings.screenToCoordinate( Qt.point( crosshairCircle.x + crosshairCircle.radius, crosshairCircle.y + crosshairCircle.radius ) )
    __coordinateChangedByMapSettings = false
  }

  onCoordinateChanged: {
    if ( !__coordinateChangedByMapSettings )
      mapSettings.setCenter( coordinate )
  }
}
