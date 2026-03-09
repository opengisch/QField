import QtQuick
import QtQuick3D
import org.qfield

Node {
  id: root

  property alias mapTerrainGeometry: mapTerrainGeometry
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
    // Texture is 3x3 metagrid; scale 1/3 maps UVs to center block only
    scaleU: (1.0 * terrainGeometry.offsetScale / 3.0)
    scaleV: (1.0 * terrainGeometry.offsetScale / 3.0)
    positionU: mapTerrainProvider.size.width > 0 ? -(terrainGeometry.offsetX / mapTerrainProvider.size.width) * (1.0 / 3.0) : 0
    positionV: mapTerrainProvider.size.height > 0 ? -(terrainGeometry.offsetZ / mapTerrainProvider.size.height) * (1.0 / 3.0) : 0
  }

  Model {
    id: terrainModel

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
