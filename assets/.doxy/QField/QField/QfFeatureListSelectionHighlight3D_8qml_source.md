

# File QfFeatureListSelectionHighlight3D.qml

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**qml**](dir_0f8ad8e65be0e5299208b1ff05c4c6ef.md) **>** [**QfFeatureListSelectionHighlight3D.qml**](QfFeatureListSelectionHighlight3D_8qml.md)

[Go to the documentation of this file](QfFeatureListSelectionHighlight3D_8qml.md)


```C++
import QtQuick
import QtQuick3D
import org.qfield._3d
import org.qfield.core

Node {
  id: featureListSelectionHighlight3D

  property QfFeatureListModelSelection selectionModel
  property Qf3DTerrainProvider terrainProvider

  property color color: "yellow"
  property color focusedColor: "red"
  property color selectedColor: "#80cc28"

  property real lineWidth: 3.0
  property real heightOffset: 15.0
  property int altitudeClamping: Qf3DGeometry.Ignore

  Repeater3D {
    model: featureListSelectionHighlight3D.selectionModel ? featureListSelectionHighlight3D.selectionModel.model : null

    delegate: Node {
      required property var model
      required property int index

      visible: featureListSelectionHighlight3D.visible

      Model {
        geometry: Qf3DGeometry {
          crs: model.crs
          qgsGeometry: model.geometry
          terrainProvider: featureListSelectionHighlight3D.terrainProvider
          lineWidth: featureListSelectionHighlight3D.lineWidth
          heightOffset: featureListSelectionHighlight3D.heightOffset
          altitudeClamping: featureListSelectionHighlight3D.altitudeClamping
          extrusion: model.extrusion
          color: model.featureSelected ? featureListSelectionHighlight3D.selectedColor : featureListSelectionHighlight3D.selectionModel.model.selectedCount === 0 && index === featureListSelectionHighlight3D.selectionModel.focusedItem ? featureListSelectionHighlight3D.focusedColor : featureListSelectionHighlight3D.color
        }

        materials: [
          PrincipledMaterial {
            vertexColorsEnabled: true
            alphaMode: model.extrusion > 0 ? PrincipledMaterial.Opaque : PrincipledMaterial.Blend
            depthDrawMode: Material.OpaqueOnlyDepthDraw
            cullMode: PrincipledMaterial.NoCulling
            lighting: PrincipledMaterial.NoLighting
          }
        ]
      }
    }
  }
}
```


