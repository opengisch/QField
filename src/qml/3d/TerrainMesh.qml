import QtQuick
import QtQuick3D
import org.qfield

Node {
  id: root

  property alias resolution: terrainGeometry.resolution
  property alias terrainWidth: terrainGeometry.terrainWidth
  property alias terrainDepth: terrainGeometry.terrainDepth
  property alias heightData: terrainGeometry.heightData

  property var satelliteTexture: null
  property bool satelliteTextureReady: false

  Texture {
    id: grassTexture
    sourceItem: Canvas {
      id: grassCanvas
      width: 256
      height: 256

      onPaint: {
        var ctx = getContext("2d");
        var gradient = ctx.createLinearGradient(0, 0, 256, 256);
        gradient.addColorStop(0.0, "#3d6b3d");
        gradient.addColorStop(1.0, "#4a7c4e");
        ctx.fillStyle = gradient;
        ctx.fillRect(0, 0, 256, 256);
      }
    }
  }

  Model {
    id: terrainModel

    geometry: QgsQuick3DTerrainGeometry {
      id: terrainGeometry
    }

    materials: [
      PrincipledMaterial {
        id: terrainMaterial
        baseColorMap: root.satelliteTextureReady ? root.satelliteTexture : grassTexture
        roughness: root.satelliteTextureReady ? 0.9 : 0.85
        metalness: 0.0
        normalStrength: root.satelliteTextureReady ? 0.0 : 0.3
      }
    ]
  }
}
