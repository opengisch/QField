

# File QfMarkupItemRenderer.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfMarkupItemRenderer.qml**](QfMarkupItemRenderer_8qml.md)

[Go to the documentation of this file](QfMarkupItemRenderer_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qgis
import org.qfield.core
import org.qfield.gui

Item {
  id: markupItemRenderer

  property MapSettings mapSettings
  property QfMarkupItem markupItem

  QfGeometryRenderer {
    mapSettings: markupItemRenderer.mapSettings
    geometryWrapper.crs: QfCoordinateReferenceSystemUtils.wgs84Crs()
    geometryWrapper.qgsGeometry: markupItem.geometry
    color: markupItem.color
    lineWidth: 4.5
    borderSize: 4
    pointSize: 14
  }
}
```


