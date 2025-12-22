import QtQuick
import QtQuick.Controls
import Theme
import org.qgis
import org.qfield
import ".."

QfOverlayContainer {
  /// type:QgsQuickMapSettings
  property MapSettings mapSettings

  signal finished
  signal requestJumpToPoint(var center, real scale, bool handleMargins)

  width: parent.width
  height: childrenRect.height
}
