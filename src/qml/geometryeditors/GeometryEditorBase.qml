import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield
import Theme
import ".."

QfVisibilityFadingRow {
  property FeatureModel featureModel
  property GeometryRenderer editorRenderer
  /// type:QgsQuickMapSettings
  property MapSettings mapSettings

  signal finished
  signal requestJumpToPoint(var center, real scale, bool handleMargins)
}
