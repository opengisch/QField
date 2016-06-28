import QtQuick 2.5
import org.qgis 1.0

import QtPositioning 5.3

Item {
  id: locator
  property MapSettings mapSettings
  property color color: "#263238"
  property color highlightColor: "#CFD8DC"

  property point coordinate

  property bool __coordinateChangedByMapSettings: false


  Rectangle {
    id: crosshairCircleInnerBuffer
    anchors.centerIn: crosshairCircle

    width: crosshairCircle.width - dp
    height: crosshairCircle.height - dp

    border.color: highlightColor
    border.width: 2 * dp
    color: "transparent"
    radius: width / 2
  }

  Rectangle {
    id: crosshairCircle

    x: parent.width / 2 - radius
    y: parent.height / 2 - radius

    border.color: parent.color
    border.width: 1.2 * dp
    color: "transparent"
    antialiasing: true

    width: 48 * dp
    height: width
    radius: width / 2
  }

  Rectangle {
    anchors.centerIn: crosshairCircle

    color: parent.color

    width: 1.2 * dp
    height: crosshairCircle.height * 4 / 6
  }

  Rectangle {
    anchors.centerIn: crosshairCircle

    color: parent.color

    width: crosshairCircle.width * 4 / 6
    height: 1.2 * dp
  }

  Connections {
    target: mapSettings

    onExtentChanged: __updateCoordinate()
  }

  ParallelAnimation {
    id: flashAnimation

    SequentialAnimation {
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

    SequentialAnimation {
      ScaleAnimator {
        target: crosshairCircleInnerBuffer
        from: 1
        to: 0.7
        duration: 150

        easing.type: Easing.InOutQuad
      }
      ScaleAnimator {
        target: crosshairCircleInnerBuffer
        from: 0.7
        to: 1
        duration: 150

        easing.type: Easing.InOutCubic
      }
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
