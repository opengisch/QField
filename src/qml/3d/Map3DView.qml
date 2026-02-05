import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
import org.qfield
import Theme

Item {
  id: map3DView
  focus: true
  visible: !isLoading

  property alias mapSettings: terrainProvider.mapSettings
  property bool isLoading: terrainProvider.isLoading
  property bool wireframeMode: false

  property bool gnssActive: false
  property var gnssPosition: null
  property real gnssSpeed: -1
  property real gnssDirection: -1

  signal cameraInteractionDetected

  QtObject {
    id: internal
    property real terrainWidth: terrainProvider.size.width
    property real terrainDepth: terrainProvider.size.height
  }

  Quick3DTerrainProvider {
    id: terrainProvider
    project: qgisProject
    mapSettings: map3DView.mapSettings

    onTerrainDataReady: {
      loadTerrain();
      Qt.callLater(map3DView.playOpeningAnimation);
    }
  }

  Quick3DMapTextureData {
    id: textureData
    mapSettings: map3DView.mapSettings
    extent: terrainProvider.extent
  }

  Texture {
    id: satelliteTexture
    textureData: textureData
    generateMipmaps: true
    mipFilter: Texture.Linear
  }

  function loadTerrain() {
    const heights = terrainProvider.normalizedData;
    if (heights.length === 0) {
      return;
    }

    terrainMesh.heightData = heights;
    positionCameraForTerrain();
    textureData.render();
  }

  function positionCameraForTerrain() {
    const terrainDiagonal = Math.sqrt(internal.terrainWidth * internal.terrainWidth + internal.terrainDepth * internal.terrainDepth);
    cameraController.distance = terrainDiagonal * 0.8;
    cameraController.pitch = 40;
    cameraController.yaw = 0;
    cameraController.target = Qt.vector3d(0, 0, 0);
  }

  function playOpeningAnimation() {
    const terrainDiagonal = Math.sqrt(internal.terrainWidth * internal.terrainWidth + internal.terrainDepth * internal.terrainDepth);
    cameraController.distance = terrainDiagonal * 0.8;
    cameraController.pitch = 85;
    cameraController.yaw = 0;
    cameraController.target = Qt.vector3d(0, 0, 0);
    cameraController.updateCameraPosition();
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
      clearColor: map3DView.mapSettings ? map3DView.mapSettings.backgroundColor : "#FFFFFF"
      backgroundMode: SceneEnvironment.Color
      antialiasingMode: SceneEnvironment.MSAA
      antialiasingQuality: SceneEnvironment.High
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
      gridSize: terrainProvider.gridSize
      size: terrainProvider.size
      satelliteTexture: satelliteTexture
      satelliteTextureReady: textureData.ready
    }

    Node {
      id: gnssMarker
      visible: pos3d !== null

      property var pos3d: {
        if (!map3DView.gnssActive || !map3DView.gnssPosition) {
          return null;
        }
        return map3DView.geoTo3D(map3DView.gnssPosition.x, map3DView.gnssPosition.y);
      }

      position: pos3d || Qt.vector3d(0, 0, 0)
      eulerRotation: map3DView.gnssSpeed > 0 && map3DView.gnssDirection >= 0 ? Qt.vector3d(0, -map3DView.gnssDirection, 0) : Qt.vector3d(0, 0, 0)

      Model {
        source: "#Sphere"
        scale: Qt.vector3d(0.3, 0.05, 0.3)
        position: Qt.vector3d(0, 0.02, 0)
        pickable: true

        materials: PrincipledMaterial {
          baseColor: "#4080ff"
          opacity: 0.4
          alphaMode: PrincipledMaterial.Blend
        }

        SequentialAnimation on scale {
          loops: Animation.Infinite
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

      Node {
        visible: map3DView.gnssSpeed > 0 && map3DView.gnssDirection >= 0

        Model {
          source: "#Cone"
          scale: Qt.vector3d(0.2, 0.15, 0.2)
          position: Qt.vector3d(0, 0.15, 0)
          eulerRotation: Qt.vector3d(-90, 0, 0)
          pickable: true

          materials: PrincipledMaterial {
            baseColor: "#2060ff"
            metalness: 0.7
            roughness: 0.1
          }
        }

        Model {
          source: "#Cylinder"
          scale: Qt.vector3d(0.08, 0.12, 0.08)
          position: Qt.vector3d(0, 0.06, 0)
          eulerRotation: Qt.vector3d(-90, 0, 0)
          pickable: true

          materials: PrincipledMaterial {
            baseColor: "#2060ff"
            metalness: 0.6
            roughness: 0.2
          }
        }

        Model {
          source: "#Cone"
          scale: Qt.vector3d(0.12, 0.05, 0.12)
          position: Qt.vector3d(0, 0.0, 0)
          eulerRotation: Qt.vector3d(90, 0, 0)
          pickable: true

          materials: PrincipledMaterial {
            baseColor: "#4080ff"
            metalness: 0.5
            roughness: 0.3
          }
        }
      }

      Model {
        visible: !(map3DView.gnssSpeed > 0 && map3DView.gnssDirection >= 0)
        source: "#Sphere"
        scale: Qt.vector3d(0.15, 0.15, 0.15)
        position: Qt.vector3d(0, 0.08, 0)
        pickable: true

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
    onSingleTapped: function (x, y) {
      const pickResult = view3d.pick(x, y);
      if (pickResult.objectHit) {
        const pos3d = gnssMarker.pos3d;
        if (pos3d) {
          cameraController.lookAtPoint(pos3d, 500);
        }
      }
    }
    onUserInteractionStarted: {
      map3DView.cameraInteractionDetected();
    }
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

  function geoTo3D(geoX, geoY) {
    const extW = terrainProvider.extent.width;
    const extH = terrainProvider.extent.height;

    if (extW <= 0 || extH <= 0)
      return null;

    const nx = (geoX - terrainProvider.extent.xMinimum) / extW;
    const nz = (geoY - terrainProvider.extent.yMinimum) / extH;

    if (nx < 0 || nx > 1 || nz < 0 || nz > 1) {
      return null;
    }

    const x3d = (nx - 0.5) * internal.terrainWidth;
    const z3d = (0.5 - nz) * internal.terrainDepth;

    let y3d = terrainProvider.normalizedHeightAt(geoX, geoY);
    y3d += 15;

    return Qt.vector3d(x3d, y3d, z3d);
  }

  function lookAtPoint(pos3d, distance) {
    cameraController.lookAtPoint(pos3d, distance);
  }
}
