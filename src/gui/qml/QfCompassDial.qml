import QtQuick
import QtQuick.Shapes
import org.qfield.gui

/**
 * A compass dial face made of a two-toned needle surrounded by evenly spaced
 * tick marks.
 * \ingroup qml_gui
 */
Item {
  id: compassDial

  //! Map canvas rotation in degrees represented by the needle
  property real mapRotation: 0

  //! Color of the needle half pointing north
  property color northColor: QfTheme.mainColor

  //! Color of the needle half pointing south and the rim circling the dial disc
  property color southColor: QfTheme.gray

  //! Color of the tick marks
  property color tickColor: QfTheme.lightGray

  //! Color of the dial disc and the needle hub ring
  property color dialColor: QfTheme.darkGray

  readonly property real centerX: width / 2
  readonly property real centerY: height / 2
  readonly property real needleHalfLength: height * 0.26
  readonly property real needleHalfWidth: width * 0.09
  readonly property real hubRadius: height * 0.085
  readonly property real tickRadius: height * 0.37

  Rectangle {
    anchors.fill: parent
    radius: width / 2
    color: Qt.hsla(compassDial.dialColor.hslHue, compassDial.dialColor.hslSaturation, compassDial.dialColor.hslLightness, 0.8)
    border.color: compassDial.southColor
    border.width: Math.max(1.5, width * 0.028)
    antialiasing: true

    layer.enabled: true
    layer.effect: QfDropShadow {
      transparentBorder: true
      samples: 16
      color: QfTheme.shadowColor
      horizontalOffset: 0
      verticalOffset: 0
    }
  }

  Repeater {
    model: 8

    delegate: Rectangle {
      readonly property real angleDegrees: index * 45
      readonly property real angleRadians: angleDegrees * Math.PI / 180

      x: compassDial.centerX + compassDial.tickRadius * Math.sin(angleRadians) - width / 2
      y: compassDial.centerY - compassDial.tickRadius * Math.cos(angleRadians) - height / 2
      width: Math.max(1.5, compassDial.width * 0.032)
      height: compassDial.height * 0.055
      radius: width / 2
      rotation: angleDegrees
      color: compassDial.tickColor
      opacity: 0.45
      antialiasing: true
    }
  }

  Shape {
    anchors.fill: parent
    rotation: compassDial.mapRotation
    preferredRendererType: Shape.CurveRenderer

    ShapePath {
      strokeColor: "transparent"
      fillColor: compassDial.northColor
      startX: compassDial.centerX
      startY: compassDial.centerY - compassDial.needleHalfLength

      PathLine {
        x: compassDial.centerX + compassDial.needleHalfWidth
        y: compassDial.centerY
      }
      PathLine {
        x: compassDial.centerX - compassDial.needleHalfWidth
        y: compassDial.centerY
      }
    }

    ShapePath {
      strokeColor: "transparent"
      fillColor: compassDial.southColor
      startX: compassDial.centerX
      startY: compassDial.centerY + compassDial.needleHalfLength

      PathLine {
        x: compassDial.centerX + compassDial.needleHalfWidth
        y: compassDial.centerY
      }
      PathLine {
        x: compassDial.centerX - compassDial.needleHalfWidth
        y: compassDial.centerY
      }
    }
  }

  Rectangle {
    x: compassDial.centerX - compassDial.hubRadius
    y: compassDial.centerY - compassDial.hubRadius
    width: compassDial.hubRadius * 2
    height: compassDial.hubRadius * 2
    radius: compassDial.hubRadius
    color: QfTheme.light
    border.color: compassDial.dialColor
    border.width: Math.max(1, compassDial.hubRadius * 0.45)
    antialiasing: true
  }
}
