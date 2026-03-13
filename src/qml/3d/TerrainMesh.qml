import QtQuick
import QtQuick3D
import org.qfield

Node {
  id: root

  property alias mapTerrainGeometry: mapTerrainGeometry
  property alias mapTexture: mapTexture
  property var mapTextureData: undefined

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

  Texture {
    id: mapTexture
    textureData: root.mapTextureData
    generateMipmaps: false
    mipFilter: Texture.None
    tilingModeHorizontal: Texture.ClampToEdge
    tilingModeVertical: Texture.ClampToEdge
    pivotU: 0.5
    pivotV: 0.5
  }

  Model {
    id: terrainModel
    position: Qt.vector3d(0, -(mapTerrainGeometry.boundsMax.y - mapTerrainGeometry.boundsMin.y) / 2, 0)

    geometry: Quick3DTerrainGeometry {
      id: mapTerrainGeometry
    }

    materials: [
      PrincipledMaterial {
        id: terrainMaterial
        baseColorMap: root.mapTextureData && root.mapTextureData.ready ? mapTexture : neutralTexture
        roughness: root.mapTextureData && root.mapTextureData.ready ? 0.9 : 0.85
        metalness: 0.0
        normalStrength: root.mapTextureData && root.mapTextureData.ready ? 0.0 : 0.3
      }
    ]
  }
}
