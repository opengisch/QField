import QtQuick
import org.qgis
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup qml_gui
 */
Item {
  id: markupCollectionRenderer

  /// type:QgsQuickMapSettings
  property MapSettings mapSettings
  property QfMarkupCollection markupCollection

  Repeater {
    model: markupCollection ? markupCollection.items : []

    QfMarkupItemRenderer {
      markupItem: modelData
      mapSettings: markupCollectionRenderer.mapSettings
    }
  }
}
