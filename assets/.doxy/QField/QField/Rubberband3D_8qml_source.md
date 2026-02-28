

# File Rubberband3D.qml

[**File List**](files.md) **>** [**3d**](dir_47cbdfc3401e26ec6b0f990982ff02ef.md) **>** [**Rubberband3D.qml**](Rubberband3D_8qml.md)

[Go to the documentation of this file](Rubberband3D_8qml.md)


```C++
import QtQuick
import QtQuick3D
import org.qfield

Node {
  id: root

  property alias rubberbandModel: rubberbandGeometry.rubberbandModel
  property alias terrainProvider: rubberbandGeometry.terrainProvider
  property alias color: rubberbandGeometry.color
  property alias radius: rubberbandGeometry.radius
  property alias segments: rubberbandGeometry.segments
  property alias heightOffset: rubberbandGeometry.heightOffset

  Model {
    id: rubberbandModel3D

    geometry: Quick3DRubberbandGeometry {
      id: rubberbandGeometry
    }

    materials: [
      PrincipledMaterial {
        baseColor: "white"
        metalness: 0.0
        roughness: 1.0
        vertexColorsEnabled: true
      }
    ]
  }
}
```


