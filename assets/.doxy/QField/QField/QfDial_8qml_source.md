

# File QfDial.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfDial.qml**](QfDial_8qml.md)

[Go to the documentation of this file](QfDial_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import QtQuick.Shapes
import org.qfield.gui

Dial {
  id: dial

  property bool hasValue: true
  property string placeholderText: ""
  property int precision: 0
  property string suffixText: ""
  property bool showValueLabel: true
  property color textColor: QfTheme.mainTextColor

  readonly property real trackWidth: 4
  readonly property real trackRadius: Math.min(width, height) / 2 - 11
  readonly property real innerRadius: trackRadius - trackWidth / 2

  implicitWidth: QfTheme.toolButtonSize * 2.75
  implicitHeight: implicitWidth

  wrap: true
  startAngle: 0
  endAngle: 360
  wheelEnabled: true

  background: Shape {
    width: dial.width
    height: dial.height
    opacity: dial.enabled ? 1 : 0.5
    preferredRendererType: Shape.CurveRenderer

    ShapePath {
      strokeWidth: dial.trackWidth
      strokeColor: dial.enabled ? Qt.alpha(dial.Material.accentColor, 0.33) : dial.Material.sliderDisabledColor
      fillColor: "transparent"

      PathAngleArc {
        centerX: dial.width / 2
        centerY: dial.height / 2
        radiusX: dial.trackRadius
        radiusY: dial.trackRadius
        startAngle: -90
        sweepAngle: 360
      }
    }

    ShapePath {
      strokeWidth: dial.trackWidth
      strokeColor: !dial.hasValue ? "transparent" : dial.enabled ? dial.Material.accentColor : dial.Material.sliderDisabledColor
      fillColor: "transparent"
      capStyle: ShapePath.FlatCap

      PathAngleArc {
        centerX: dial.width / 2
        centerY: dial.height / 2
        radiusX: dial.trackRadius
        radiusY: dial.trackRadius
        startAngle: dial.startAngle - 90
        sweepAngle: dial.angle - dial.startAngle
      }
    }
  }

  handle: SliderHandle {
    x: dial.width / 2 + dial.trackRadius * Math.sin(dial.angle * Math.PI / 180) - width / 2
    y: dial.height / 2 - dial.trackRadius * Math.cos(dial.angle * Math.PI / 180) - height / 2
    visible: dial.hasValue
    opacity: dial.enabled ? 1 : 0.5
    value: dial.value
    handleHasFocus: dial.visualFocus
    handlePressed: dial.pressed
    handleHovered: dial.hovered
  }

  Item {
    id: labels
    x: (dial.width - width) / 2
    y: (dial.height - valueLabel.height - (rangeLabel.visible ? Math.min(rangeMetrics.height, rangeLabel.height) : 0)) / 2
    width: dial.innerRadius * 1.6
    height: valueLabel.height + rangeLabel.height
    visible: dial.showValueLabel

    Text {
      id: valueLabel
      width: labels.width
      height: Math.min(valueMetrics.height, dial.innerRadius * 0.7)
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignBottom
      fontSizeMode: Text.Fit
      minimumPointSize: QfTheme.tinyFont.pointSize / 2
      elide: Text.ElideRight
      text: dial.hasValue ? Number(dial.value).toFixed(dial.precision) + (dial.suffixText !== "" ? " " + dial.suffixText : "") : dial.placeholderText
      font: QfTheme.defaultFont
      color: dial.hasValue ? dial.textColor : QfTheme.mainTextDisabledColor
    }

    Text {
      id: rangeLabel
      anchors.top: valueLabel.bottom
      anchors.horizontalCenter: parent.horizontalCenter
      width: labels.width * 0.85
      // Leaves room for a second line, so wide ranges wrap before they get elided
      height: visible ? Math.min(rangeMetrics.height * 2, dial.innerRadius * 0.5) : 0
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignTop
      fontSizeMode: Text.Fit
      minimumPointSize: QfTheme.tinyFont.pointSize / 2
      wrapMode: Text.Wrap
      elide: Text.ElideRight
      visible: Number.isFinite(dial.to - dial.from)
      text: Number(dial.from).toFixed(dial.precision) + " – " + Number(dial.to).toFixed(dial.precision)
      font: QfTheme.tinyFont
      color: QfTheme.secondaryTextColor
      opacity: 0.5
    }

    FontMetrics {
      id: valueMetrics
      font: valueLabel.font
    }

    FontMetrics {
      id: rangeMetrics
      font: rangeLabel.font
    }
  }

  TapHandler {
    id: ringTapHandler

    property real tappedValue: 0
    property bool applyingTap: false

    enabled: dial.enabled
    acceptedDevices: PointerDevice.TouchScreen

    onTapped: (eventPoint, button) => {
      const dx = eventPoint.position.x - dial.width / 2;
      const dy = eventPoint.position.y - dial.height / 2;
      if (Math.abs(Math.hypot(dx, dy) - dial.trackRadius) > QfTheme.toolButtonSize / 2) {
        return;
      }
      const tapAngle = (Math.atan2(dx, -dy) * 180 / Math.PI + 360) % 360;
      const tapPosition = Math.min(1, Math.max(0, (tapAngle - dial.startAngle) / (dial.endAngle - dial.startAngle)));
      let tapValue = dial.from + tapPosition * (dial.to - dial.from);
      if (dial.snapMode !== Dial.NoSnap && dial.stepSize > 0) {
        tapValue = dial.from + Math.round((tapValue - dial.from) / dial.stepSize) * dial.stepSize;
      }
      tappedValue = Math.min(Math.max(tapValue, Math.min(dial.from, dial.to)), Math.max(dial.from, dial.to));
      applyingTap = true;
      dial.moved();
      applyingTap = false;
    }
  }

  Binding {
    target: dial
    property: "value"
    value: ringTapHandler.tappedValue
    when: ringTapHandler.applyingTap
    restoreMode: Binding.RestoreBinding
  }
}
```


