import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield.core
import org.qfield.gui

QfVisibilityFadingRow {
  property FeatureModel featureModel
  property QfGeometryRenderer editorRenderer
  /// type:QgsQuickMapSettings
  property MapSettings mapSettings

  signal finished
  signal requestJumpToPoint(var center, real scale, bool handleMargins)
}
