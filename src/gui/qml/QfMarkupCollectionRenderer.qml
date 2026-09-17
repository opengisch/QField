import QtQuick
import QtQuick.Shapes
import org.qgis
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml_gui
 */
Item {
  id: markupCollectionRenderer

  /// type:QfMarkupCollection
  property var markupCollection: undefined

  /// type:QgsQuickMapSettings
  property MapSettings mapSettings

  Repeater {
    model: markupCollection ? markupCollection.items : []

    QfMarkupItemRenderer {
      markupItem: modelData
      mapSettings: markupCollectionRenderer.mapSettings
    }
  }
}
