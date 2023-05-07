import QtQuick 2.14
import QtQuick.Effects

MultiEffect {
  property int samples // ?
  property bool transparentBorder // ?
  property int radius // ?
  property color color
  property int horizontalOffset
  property int verticalOffset

  shadowEnabled: true
  shadowColor: color
  shadowHorizontalOffset: horizontalOffset
  shadowVerticalOffset: verticalOffset
}
