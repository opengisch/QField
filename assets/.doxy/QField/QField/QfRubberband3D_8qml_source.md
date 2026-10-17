

# File QfRubberband3D.qml

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**qml**](dir_0f8ad8e65be0e5299208b1ff05c4c6ef.md) **>** [**QfRubberband3D.qml**](QfRubberband3D_8qml.md)

[Go to the documentation of this file](QfRubberband3D_8qml.md)


```C++
import QtQuick
import QtQuick3D
import org.qfield._3d

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

    geometry: Qf3DRubberbandGeometry {
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


