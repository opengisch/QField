

# File GeometryEditorBase.qml

[**File List**](files.md) **>** [**geometryeditors**](dir_a044c600d9d985eeadf51620d76e3ed0.md) **>** [**GeometryEditorBase.qml**](GeometryEditorBase_8qml.md)

[Go to the documentation of this file](GeometryEditorBase_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield
import Theme
import ".."

QfVisibilityFadingRow {
  property FeatureModel featureModel
  property GeometryRenderer editorRenderer
  property MapSettings mapSettings

  signal finished
  signal requestJumpToPoint(var center, real scale, bool handleMargins)
}
```


