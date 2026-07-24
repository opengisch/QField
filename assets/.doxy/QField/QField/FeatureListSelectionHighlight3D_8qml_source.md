

# File FeatureListSelectionHighlight3D.qml

[**File List**](files.md) **>** [**3d**](dir_47cbdfc3401e26ec6b0f990982ff02ef.md) **>** [**FeatureListSelectionHighlight3D.qml**](FeatureListSelectionHighlight3D_8qml.md)

[Go to the documentation of this file](FeatureListSelectionHighlight3D_8qml.md)


```C++
import QtQuick
import QtQuick3D
import org.qfield
import Theme

Node {
  id: featureListSelectionHighlight3D

  property FeatureListModelSelection selectionModel
  property Quick3DTerrainProvider terrainProvider

  property color color: "yellow"
  property color focusedColor: "red"
  property color selectedColor: Theme.mainColor

  property real lineWidth: 3.0
  property real heightOffset: 15.0
  property int altitudeClamping: Quick3DGeometry.Ignore

  Repeater3D {
    model: featureListSelectionHighlight3D.selectionModel ? featureListSelectionHighlight3D.selectionModel.model : null

    delegate: Node {
      required property var model
      required property int index

      visible: featureListSelectionHighlight3D.visible

      Model {
        geometry: Quick3DGeometry {
          qgsGeometry: model.geometry
          crs: model.crs
          terrainProvider: featureListSelectionHighlight3D.terrainProvider
          lineWidth: featureListSelectionHighlight3D.lineWidth
          heightOffset: featureListSelectionHighlight3D.heightOffset
          altitudeClamping: featureListSelectionHighlight3D.altitudeClamping
          extrusion: model.extrusion
          color: model.featureSelected ? featureListSelectionHighlight3D.selectedColor : featureListSelectionHighlight3D.selectionModel.model.selectedCount === 0 && index === featureListSelectionHighlight3D.selectionModel.focusedItem ? featureListSelectionHighlight3D.focusedColor : featureListSelectionHighlight3D.color
        }

        materials: [
          PrincipledMaterial {
            baseColor: "white"
            metalness: 0.0
            roughness: 1.0
            vertexColorsEnabled: true
            alphaMode: model.extrusion > 0 ? PrincipledMaterial.Opaque : PrincipledMaterial.Blend
            depthDrawMode: Material.AlwaysDepthDraw
            cullMode: PrincipledMaterial.NoCulling
          }
        ]
      }
    }
  }
}
```


