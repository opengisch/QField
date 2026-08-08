

# File QfNavigationHighlight.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfNavigationHighlight.qml**](QfNavigationHighlight_8qml.md)

[Go to the documentation of this file](QfNavigationHighlight_8qml.md)


```C++
import QtQuick
import org.qgis
import org.qfield.core
import org.qfield.gui

Item {
  id: navigationHighlight
  property Navigation navigation

  QfLinePolygon {
    visible: positionSource.active
    mapSettings: navigation.mapSettings
    geometry: QgsGeometryWrapper {
      qgsGeometry: navigation.path
      crs: navigation.mapSettings.crs ? navigation.mapSettings.crs : CoordinateReferenceSystemUtils.invalidCrs()
    }
    color: Theme.navigationColorSemiOpaque
    lineWidth: positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid ? 5 : 1
  }

  Repeater {
    model: navigation.model

    delegate: QfNavigationRenderer {
      mapSettings: navigation.mapSettings
      geometryWrapper.qgsGeometry: model.Point
      geometryWrapper.crs: navigation.mapSettings.destinationCrs
      pointIndex: model.index
      pointType: model.PointType
    }
  }
}
```


