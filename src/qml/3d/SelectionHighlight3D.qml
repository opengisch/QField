import QtQuick
import QtQuick3D
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Node {
  id: selectionHighlight3D

  property var selectionModel
  property var terrainProvider

  property color color: "yellow"
  property color focusedColor: "red"
  property color selectedColor: Theme.mainColor

  property real lineWidth: 3.0
  property real heightOffset: 15.0

  Repeater3D {
    model: selectionHighlight3D.selectionModel ? selectionHighlight3D.selectionModel.model : null

    delegate: Node {
      required property var model
      required property int index

      visible: selectionHighlight3D.visible

      Model {
        geometry: Quick3DGeometry {
          qgsGeometry: model.geometry
          crs: model.crs
          terrainProvider: selectionHighlight3D.terrainProvider
          lineWidth: selectionHighlight3D.lineWidth
          heightOffset: selectionHighlight3D.heightOffset
          color: model.featureSelected ? selectionHighlight3D.selectedColor : selectionHighlight3D.selectionModel.model.selectedCount === 0 && index === selectionHighlight3D.selectionModel.focusedItem ? selectionHighlight3D.focusedColor : selectionHighlight3D.color
        }

        materials: [
          PrincipledMaterial {
            baseColor: "white"
            metalness: 0.0
            roughness: 1.0
            vertexColorsEnabled: true
            alphaMode: PrincipledMaterial.Blend
            cullMode: PrincipledMaterial.NoCulling
          }
        ]
      }
    }
  }
}
