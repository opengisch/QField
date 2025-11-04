import QtQuick
import QtQuick.Controls
import Theme
import org.qgis
import org.qfield
import ".."

QfVisibilityFadingRow {
  property FeatureModel featureModel
  property GeometryRenderer editorRenderer
  property MapSettings mapSettings

  signal finished
  signal requestJumpToPoint(var center, real scale, bool handleMargins)
}
