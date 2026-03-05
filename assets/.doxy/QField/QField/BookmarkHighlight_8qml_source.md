

# File BookmarkHighlight.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**BookmarkHighlight.qml**](BookmarkHighlight_8qml.md)

[Go to the documentation of this file](BookmarkHighlight_8qml.md)


```C++
import QtQuick
import org.qgis
import org.qfield

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
```


