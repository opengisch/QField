import QtQuick
import QtQuick.Effects

MultiEffect {
  id: effect

  // dummy properties to support Qt5 and Qt6
  property int samples
  property int radius
  property bool transparentBorder

  // property aliases
  property alias color: effect.shadowColor
  property alias horizontalOffset: effect.shadowHorizontalOffset
  property alias verticalOffset: effect.shadowVerticalOffset

  shadowEnabled: true
}
