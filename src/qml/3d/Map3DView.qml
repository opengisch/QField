import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import org.qfield
import Theme

Item {
  id: root
  focus: true

  property var qgisProject
  property var bookmarkModel: null
  property bool wireframeMode: false
  property var initialExtent

  QtObject {
    id: internal
    property real terrainWidth: terrainProvider.terrainBaseSize
    property real terrainDepth: terrainProvider.terrainBaseSize
    property real minHeight: 0
    property real maxHeight: 0
    property real initialCameraY: 0

    // height scale for bookmark positioning
    property real heightScale: 1.0
  }

  QgsQuick3DTerrainProvider {
    id: terrainProvider
    project: root.qgisProject
    extent: root.initialExtent
    resolution: 64 * 10

    onTerrainDataReady: {
      const stats = terrainProvider.terrainStats();
      internal.minHeight = stats.minHeight || 0;
      internal.maxHeight = stats.maxHeight || 0;

      textureGenerator.extent = root.initialExtent;

      loadTerrain();
    }
  }

  QgsQuick3DMapTextureGenerator {
    id: textureGenerator
    project: root.qgisProject

    property bool isReady: false

    onReady: {
      satelliteTexture.source = "file://" + textureFilePath;
      textureGenerator.isReady = true;
    }
  }

  Texture {
    id: satelliteTexture
  }

  // Load normalized terrain data from C++
  function loadTerrain() {
    const heights = terrainProvider.normalizedData;
    if (heights.length === 0) {
      return;
    }

    const stats = terrainProvider.terrainStats();
    const extentWidth = terrainProvider.extent.width;
    const extentHeight = terrainProvider.extent.height;
    const extentSize = Math.max(extentWidth, extentHeight);

    // Calculate terrain dimensions based on aspect ratio
    if (extentWidth >= extentHeight) {
      internal.terrainWidth = terrainProvider.terrainBaseSize;
      internal.terrainDepth = terrainProvider.terrainBaseSize * (extentHeight / extentWidth);
    } else {
      internal.terrainWidth = terrainProvider.terrainBaseSize * (extentWidth / extentHeight);
      internal.terrainDepth = terrainProvider.terrainBaseSize;
    }

    // Calculate height scale for bookmark positioning
    const realHeightScale = terrainProvider.terrainBaseSize / extentSize;
    let visualExaggeration = terrainProvider.calculateVisualExaggeration();

    internal.heightScale = realHeightScale * visualExaggeration;
    internal.initialCameraY = internal.maxHeight * 0.3;

    terrainMesh.heightData = heights;
    positionCameraForTerrain();
    textureGenerator.render();
  }

  function positionCameraForTerrain() {
    const maxDimension = Math.max(internal.terrainWidth, internal.terrainDepth);
    const terrainDiagonal = Math.sqrt(internal.terrainWidth * internal.terrainWidth + internal.terrainDepth * internal.terrainDepth);
    cameraController.distance = terrainDiagonal * 0.8;
    cameraController.pitch = 40;
    cameraController.yaw = 0;
    cameraController.target = Qt.vector3d(0, internal.initialCameraY, 0);
  }

  View3D {
    id: view3d
    anchors.fill: parent

    environment: SceneEnvironment {
      clearColor: "#87CEEB"
      backgroundMode: SceneEnvironment.Color
      antialiasingMode: SceneEnvironment.MSAA
      antialiasingQuality: SceneEnvironment.High

      debugSettings: DebugSettings {
        wireframeEnabled: root.wireframeMode
      }
    }

    PerspectiveCamera {
      id: camera
      clipNear: 1
      clipFar: 50000
      fieldOfView: 60
    }

    DirectionalLight {
      eulerRotation: Qt.vector3d(-45, -45, 0)
      brightness: 1.0
      ambientColor: Qt.rgba(0.3, 0.3, 0.35, 1.0)
    }

    DirectionalLight {
      eulerRotation: Qt.vector3d(-30, 135, 0)
      brightness: 0.3
    }

    TerrainMesh {
      id: terrainMesh
      resolution: terrainProvider.resolution
      terrainWidth: internal.terrainWidth
      terrainDepth: internal.terrainDepth
      satelliteTexture: satelliteTexture
      satelliteTextureReady: textureGenerator.isReady
    }

    // Bookmark markers
    Repeater3D {
      model: root.bookmarkModel

      Node {
        id: bookmarkNode

        required property int index
        required property var model

        property string bmId: model.BookmarkId || ""
        property string bmGroup: model.BookmarkGroup || ""
        property color pinColor: root.getBookmarkColor(bmGroup)

        property var pos3d: {
          if (!root.bookmarkModel || !bmId)
            return null;
          const point = root.bookmarkModel.getBookmarkPoint(bmId);
          if (!point)
            return null;
          return root.geoTo3D(point.x, point.y);
        }

        visible: pos3d !== null
        position: pos3d || Qt.vector3d(0, 0, 0)

        // Invisible hit area (larger sphere for easier clicking)
        Model {
          objectName: "bookmark_" + bookmarkNode.bmId
          source: "#Sphere"
          scale: Qt.vector3d(0.5, 0.5, 0.5)
          position: Qt.vector3d(0, 0.3, 0)
          pickable: true
          opacity: 0

          materials: PrincipledMaterial {
            baseColor: "transparent"
          }
        }

        // Pin head (sphere)
        Model {
          id: pinHead
          source: "#Sphere"
          scale: Qt.vector3d(0.1, 0.1, 0.1)
          position: Qt.vector3d(0, 0.3, 0)

          materials: PrincipledMaterial {
            baseColor: bookmarkNode.pinColor
            metalness: 0.4
            roughness: 0.3
          }
        }

        // Pin needle (cone)
        Model {
          source: "#Cone"
          scale: Qt.vector3d(0.05, 0.2, 0.05)
          position: Qt.vector3d(0, 0.1, 0)
          eulerRotation: Qt.vector3d(180, 0, 0)

          materials: PrincipledMaterial {
            baseColor: Qt.darker(bookmarkNode.pinColor, 1.3)
            metalness: 0.7
            roughness: 0.2
          }
        }
      }
    }
  }

  TouchCameraController {
    id: cameraController
    anchors.fill: parent
    camera: camera

    onSingleTapped: function (x, y) {
      const result = view3d.pick(x, y);
      if (result.objectHit && result.objectHit.objectName && result.objectHit.objectName.startsWith("bookmark_")) {
        const bookmarkId = result.objectHit.objectName.substring(9);

        const point = root.bookmarkModel.getBookmarkPoint(bookmarkId);
        if (point) {
          const pos3d = root.geoTo3D(point.x, point.y);
          if (pos3d) {
            cameraController.lookAtPoint(pos3d, 300);
          }
        }
      }
    }
  }

  // Convert GIS coordinates to 3D scene coordinates
  function geoTo3D(geoX, geoY) {
    const extW = terrainProvider.extent.width;
    const extH = terrainProvider.extent.height;

    if (extW <= 0 || extH <= 0)
      return null;

    // Normalize to 0-1 range within extent
    const nx = (geoX - terrainProvider.extent.xMinimum) / extW;
    const nz = (geoY - terrainProvider.extent.yMinimum) / extH;

    if (nx < 0 || nx > 1 || nz < 0 || nz > 1) {
      return null;
    }

    // Convert to 3D coordinates (centered at origin)
    const x3d = (nx - 0.5) * internal.terrainWidth;
    const z3d = (0.5 - nz) * internal.terrainDepth;

    let y3d = terrainProvider.normalizedHeightAt(geoX, geoY);
    y3d += 15;

    return Qt.vector3d(x3d, y3d, z3d);
  }

  function getBookmarkColor(group) {
    switch (group) {
    case 'red':
      return Theme.bookmarkRed;
    case 'orange':
      return Theme.bookmarkOrange;
    case 'blue':
      return Theme.bookmarkBlue;
    default:
      return Theme.bookmarkDefault;
    }
  }
}
