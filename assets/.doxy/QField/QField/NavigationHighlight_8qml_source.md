

# File NavigationHighlight.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**NavigationHighlight.qml**](NavigationHighlight_8qml.md)

[Go to the documentation of this file](NavigationHighlight_8qml.md)


```C++
import QtQuick
import org.qgis
import org.qfield
import Theme

Item {
  id: navigationHighlight
  property Navigation navigation

  LinePolygon {
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

    delegate: NavigationRenderer {
      mapSettings: navigation.mapSettings
      geometryWrapper.qgsGeometry: model.Point
      geometryWrapper.crs: navigation.mapSettings.destinationCrs
      pointIndex: model.index
      pointType: model.PointType
    }
  }
}
```


