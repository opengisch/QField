

# File QfBookmarkHighlight.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfBookmarkHighlight.qml**](QfBookmarkHighlight_8qml.md)

[Go to the documentation of this file](QfBookmarkHighlight_8qml.md)


```C++
import QtQuick
import org.qgis
import org.qfield.core

Repeater {
  id: bookmarkHighlight

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
```


