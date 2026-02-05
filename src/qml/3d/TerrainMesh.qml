import QtQuick
import QtQuick3D
import org.qfield

Node {
  id: root

  property alias gridSize: mapTerrainGeometry.gridSize
  property alias size: mapTerrainGeometry.size
  property alias heightData: mapTerrainGeometry.heightData

  property var texture: null
  property bool textureReady: false

  Texture {
    id: neutralTexture
    sourceItem: Canvas {
      width: 256
      height: 256

      onPaint: {
        var ctx = getContext("2d");
        var gradient = ctx.createLinearGradient(0, 0, 256, 256);
        gradient.addColorStop(0.0, "#999999");
        gradient.addColorStop(1.0, "#bbbbbb");
        ctx.fillStyle = gradient;
        ctx.fillRect(0, 0, 256, 256);
      }
    }
  }

  Model {
    id: terrainModel

    geometry: Quick3DTerrainGeometry {
      id: mapTerrainGeometry
    }

    materials: [
      PrincipledMaterial {
        id: terrainMaterial
        baseColorMap: root.textureReady ? root.texture : neutralTexture
        roughness: root.textureReady ? 0.9 : 0.85
        metalness: 0.0
        normalStrength: root.textureReady ? 0.0 : 0.3
      }
    ]
  }
}
