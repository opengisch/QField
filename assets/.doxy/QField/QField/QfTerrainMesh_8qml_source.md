

# File QfTerrainMesh.qml

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**qml**](dir_0f8ad8e65be0e5299208b1ff05c4c6ef.md) **>** [**QfTerrainMesh.qml**](QfTerrainMesh_8qml.md)

[Go to the documentation of this file](QfTerrainMesh_8qml.md)


```C++
import QtQuick
import QtQuick3D
import org.qfield._3d

Node {
  id: root

  property alias mapTerrainGeometry: mapTerrainGeometry
  property alias mapTexture: mapTexture
  property Qf3DMapTextureData mapTextureData

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
  }

  Model {
    id: terrainModel
    pickable: true

    geometry: Qf3DTerrainGeometry {
      id: mapTerrainGeometry
    }

    materials: [
      CustomMaterial {
        property TextureInput materialTexture: TextureInput {
          texture: root.mapTextureData.isReady ? mapTexture : neutralTexture
        }
        property real gridDensity: 40.0
        property real materialScale: 1 / mapTerrainGeometry.offsetScale
        property vector2d materialCenter: Qt.vector2d(0.5 + mapTerrainGeometry.offsetVector.x / 2000, 0.5 + mapTerrainGeometry.offsetVector.z / 2000)

        fragmentShader: "qrc:/3d/terrain_material.frag"
      }
    ]
  }
}
```


