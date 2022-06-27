import QtQuick 2.14

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

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
