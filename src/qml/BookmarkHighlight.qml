import QtQuick
import org.qgis
import org.qfield
import Theme

Repeater {
  id: bookmarkHighlight
  property MapSettings mapSettings

  model: qfieldSettings.showBookmarks ? bookmarkModel : undefined

  delegate: BookmarkRenderer {
    mapSettings: bookmarkHighlight.mapSettings
    geometryWrapper.qgsGeometry: model.BookmarkPoint
    geometryWrapper.crs: model.BookmarkCrs

    bookmarkIndex: model.index
    bookmarkId: model.BookmarkId
    bookmarkName: model.BookmarkName
    bookmarkGroup: model.BookmarkGroup
    bookmarkUser: model.BookmarkUser
  }
}
