import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
import org.qfield
import Theme

Item {
  id: root
  focus: true

  property var qgisProject
  property bool wireframeMode: false
  property var initialExtent
  property bool isLoading: terrainProvider.isLoading
  property int loadingProgress: terrainProvider.loadingProgress
  property color backgroundColor: "#FFFFFF"

  property var gnssPosition: null  // QgsPointXY
  property bool gnssActive: false

  QtObject {
    id: internal
    property real terrainWidth: terrainProvider.terrainBaseSize
    property real terrainDepth: terrainProvider.terrainBaseSize
    property real initialCameraY: 0

    // Adaptive resolution based on extent size
    function calculateResolution() {
      if (terrainProvider.hasDemLayer) {
        return 64 * 10;
      }

      if (!root.initialExtent)
        return 32;
      const extentSize = Math.max(root.initialExtent.width, root.initialExtent.height);
      if (extentSize < 2000)
        return 64;
      if (extentSize < 10000)
        return 48;
      if (extentSize < 50000)
        return 32;
      return 24;
    }
  }

  Quick3DTerrainProvider {
    id: terrainProvider
    project: root.qgisProject
    extent: root.initialExtent
    resolution: internal.calculateResolution()

    onTerrainDataReady: {
      textureGenerator.extent = root.initialExtent;

      loadTerrain();

      // Play opening animation after terrain is positioned
      Qt.callLater(root.playOpeningAnimation);
    }
  }

  Quick3DMapTextureGenerator {
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

    const extentWidth = terrainProvider.extent.width;
    const extentHeight = terrainProvider.extent.height;
    const extentSize = Math.max(extentWidth, extentHeight);

    if (extentWidth >= extentHeight) {
      internal.terrainWidth = terrainProvider.terrainBaseSize;
      internal.terrainDepth = terrainProvider.terrainBaseSize * (extentHeight / extentWidth);
    } else {
      internal.terrainDepth = terrainProvider.terrainBaseSize;
      internal.terrainWidth = terrainProvider.terrainBaseSize * (extentWidth / extentHeight);
    }
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

  function playOpeningAnimation() {
    const maxDimension = Math.max(internal.terrainWidth, internal.terrainDepth);
    const terrainDiagonal = Math.sqrt(internal.terrainWidth * internal.terrainWidth + internal.terrainDepth * internal.terrainDepth);

    // Start from top view (pitch=85)
    cameraController.distance = terrainDiagonal * 0.8;
    cameraController.pitch = 85;
    cameraController.yaw = 0;
    cameraController.target = Qt.vector3d(0, internal.initialCameraY, 0);
    cameraController.updateCameraPosition();

    // Animate to normal view
    openingAnimation.start();
  }

  function playClosingAnimation(callback) {
    cameraController.resetView();
    closingAnimation.callback = callback;
    closingAnimation.start();
  }

  View3D {
    id: view3d
    anchors.fill: parent

    environment: SceneEnvironment {
      clearColor: root.backgroundColor
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

    Node {
      id: gnssMarker
      visible: root.gnssActive && root.gnssPosition !== null

      property var pos3d: {
        if (!root.gnssPosition)
          return null;
        return root.geoTo3D(root.gnssPosition.x, root.gnssPosition.y);
      }

      position: pos3d || Qt.vector3d(0, 0, 0)

      // Outer pulsing ring
      Model {
        source: "#Sphere"
        scale: Qt.vector3d(0.3, 0.05, 0.3)
        position: Qt.vector3d(0, 0.02, 0)

        materials: PrincipledMaterial {
          baseColor: "#4080ff"
          opacity: 0.4
          alphaMode: PrincipledMaterial.Blend
        }

        SequentialAnimation on scale {
          loops: Animation.Infinite // can we use Infinite? :)
          running: gnssMarker.visible

          Vector3dAnimation {
            from: Qt.vector3d(0.3, 0.05, 0.3)
            to: Qt.vector3d(0.5, 0.05, 0.5)
            duration: 1500
            easing.type: Easing.InOutQuad
          }
          Vector3dAnimation {
            from: Qt.vector3d(0.5, 0.05, 0.5)
            to: Qt.vector3d(0.3, 0.05, 0.3)
            duration: 1500
            easing.type: Easing.InOutQuad
          }
        }
      }

      // Main position sphere
      Model {
        source: "#Sphere"
        scale: Qt.vector3d(0.15, 0.15, 0.15)
        position: Qt.vector3d(0, 0.08, 0)

        materials: PrincipledMaterial {
          baseColor: "#2060ff"
          metalness: 0.6
          roughness: 0.2
        }
      }
    }
  }

  TouchCameraController {
    id: cameraController
    anchors.fill: parent
    camera: camera
  }

  NumberAnimation {
    id: openingAnimation
    target: cameraController
    property: "pitch"
    from: 85
    to: 40
    duration: 1200
    easing.type: Easing.InOutQuad
  }

  SequentialAnimation {
    id: closingAnimation

    property var callback: null

    NumberAnimation {
      target: cameraController
      property: "pitch"
      from: cameraController.pitch
      to: 85
      duration: 800
      easing.type: Easing.InOutQuad
    }

    ScriptAction {
      script: {
        if (closingAnimation.callback) {
          closingAnimation.callback();
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
}
