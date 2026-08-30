import QtQuick
import org.qgis
import org.qfield.core

/**
 * \ingroup qml_gui
 */
Repeater {
  id: bookmarkHighlight

  /// type:QgsQuickMapSettings
  property MapSettings mapSettings

  model: qfieldSettings.showBookmarks ? bookmarkModel : undefined

  delegate: QfBookmarkRenderer {
    mapSettings: bookmarkHighlight.mapSettings
    geometryWrapper.crs: model.BookmarkCrs
    geometryWrapper.qgsGeometry: model.BookmarkPoint

    bookmarkIndex: model.index
    bookmarkId: model.BookmarkId
    bookmarkName: model.BookmarkName
    bookmarkGroup: model.BookmarkGroup
    bookmarkUser: model.BookmarkUser
  }
}
