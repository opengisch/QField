import QtQuick
import QtQuick3D
import org.qfield

/**
 * Wraps Quick3DRubberbandGeometry in a ready-to-use Node with a matte PBR material.
 * \ingroup qml
 */
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
