

# File QfGeometryEditorBase.qml

[**File List**](files.md) **>** [**geometryeditors**](dir_bf8ee2ec6aa9a141ef86ab67bdbc4be6.md) **>** [**QfGeometryEditorBase.qml**](QfGeometryEditorBase_8qml.md)

[Go to the documentation of this file](QfGeometryEditorBase_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield.core
import org.qfield.gui

QfVisibilityFadingRow {
  property QfFeatureModel featureModel
  property QfGeometryRenderer editorRenderer
  property MapSettings mapSettings

  signal finished
  signal requestJumpToPoint(var center, real scale, bool handleMargins)
}
```


