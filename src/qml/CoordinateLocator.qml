import QtQuick 2.0
import org.qgis 1.0

import QtPositioning 5.3

Item {
  property MapSettings mapSettings
  property color color: "black"
  property point coordinate //!< Read only!

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

    width: 1
    height: 40*dp
  }

  Rectangle {
    anchors.centerIn: crosshairCircle

    color: parent.color

    width: 40*dp
    height: 1
  }

  Connections {
    target: mapSettings

    onExtentChanged: __updateCoordinate()
  }

  function __updateCoordinate()
  {
    coordinate = mapSettings.screenToCoordinate( Qt.point( crosshairCircle.x + crosshairCircle.radius, crosshairCircle.y + crosshairCircle.radius ) )
  }
}

