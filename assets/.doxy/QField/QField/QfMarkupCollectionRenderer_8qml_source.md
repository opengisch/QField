

# File QfMarkupCollectionRenderer.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfMarkupCollectionRenderer.qml**](QfMarkupCollectionRenderer_8qml.md)

[Go to the documentation of this file](QfMarkupCollectionRenderer_8qml.md)


```C++
import QtQuick
import org.qgis
import org.qfield.core
import org.qfield.gui

Item {
  id: markupCollectionRenderer

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
```


