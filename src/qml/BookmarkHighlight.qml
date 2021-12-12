import QtQuick 2.12

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Repeater {
  id: bookmarkHighlight
  property MapSettings mapSettings

  model: bookmarkModel

  delegate: BookmarkRenderer {
    mapSettings: bookmarkHighlight.mapSettings
    geometryWrapper.qgsGeometry: model.BookmarkPoint
    geometryWrapper.crs: model.BookmarkCrs
    bookmarkName: model.BookmarkName
    bookmarkId: model.BookmarkId
  }
}
