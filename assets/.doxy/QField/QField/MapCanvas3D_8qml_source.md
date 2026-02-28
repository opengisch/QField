

# File MapCanvas3D.qml

[**File List**](files.md) **>** [**3d**](dir_47cbdfc3401e26ec6b0f990982ff02ef.md) **>** [**MapCanvas3D.qml**](MapCanvas3D_8qml.md)

[Go to the documentation of this file](MapCanvas3D_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
import org.qfield
import Theme

Item {
  id: mapArea
  focus: true
  visible: !isLoading

  property alias mapSettings: mapTerrainProvider.mapSettings
  property bool isLoading: mapTerrainProvider.isLoading
  property bool wireframeMode: false

  property bool gnssActive: false
  property var gnssPosition: null
  property real gnssSpeed: -1
  property real gnssDirection: -1

  property TrackingModel trackingModel: null

  signal cameraInteractionDetected

  Quick3DTerrainProvider {
    id: mapTerrainProvider
    project: qgisProject
    mapSettings: mapArea.mapSettings
    forceSquareSize: true

    onTerrainDataReady: {
      if (normalizedData.length === 0) {
        return;
      }

      positionCameraForTerrain();
      mapTextureData.render();

      Qt.callLater(mapArea.playOpeningAnimation);
    }
  }

  Quick3DMapTextureData {
    id: mapTextureData
    mapSettings: mapArea.mapSettings
    extent: mapTerrainProvider.extent
    incrementalRendering: true
    forceDeferredLayersRepaint: mapArea.trackingModel ? mapArea.trackingModel.count > 0 : false
  }

  Texture {
    id: mapTexture
    textureData: mapTextureData
    generateMipmaps: true
    mipFilter: Texture.Linear
  }

  View3D {
    id: view3d
    anchors.fill: parent

    environment: SceneEnvironment {
      clearColor: mapArea.mapSettings ? mapArea.mapSettings.backgroundColor : "#FFFFFF"
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
      mapTerrainGeometry.gridSize: mapTerrainProvider.gridSize
      mapTerrainGeometry.size: mapTerrainProvider.size
      mapTerrainGeometry.heightData: mapTerrainProvider.normalizedData
      texture: mapTexture
      textureReady: mapTextureData.ready
    }

    Node {
      id: gnssMarker
      visible: pos3d !== null

      property var pos3d: {
        if (!mapArea.gnssActive || !mapArea.gnssPosition) {
          return null;
        }
        return mapArea.geoTo3D(mapArea.gnssPosition.x, mapArea.gnssPosition.y);
      }

      position: pos3d || Qt.vector3d(0, 0, 0)
      eulerRotation: mapArea.gnssSpeed > 0 && mapArea.gnssDirection >= 0 ? Qt.vector3d(0, -mapArea.gnssDirection, 0) : Qt.vector3d(0, 0, 0)

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
        visible: mapArea.gnssSpeed > 0 && mapArea.gnssDirection >= 0

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
        visible: !(mapArea.gnssSpeed > 0 && mapArea.gnssDirection >= 0)
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

    Repeater3D {
      id: trackingRubberbands
      model: mapArea.trackingModel

      Rubberband3D {
        required property var modelData
        required property int index
        rubberbandModel: modelData.tracker ? modelData.tracker.rubberbandModel : null
        terrainProvider: mapTerrainProvider
        color: modelData.tracker ? modelData.tracker.color : "#FFFF3232"
        visible: modelData.tracker ? modelData.tracker.visible : false
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
      mapArea.cameraInteractionDetected();
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

  function playOpeningAnimation() {
    const terrainDiagonal = Math.sqrt(Math.pow(mapTerrainProvider.size.width, 2) + Math.pow(mapTerrainProvider.size.height, 2));
    cameraController.distance = terrainDiagonal * 0.8;
    cameraController.pitch = 85;
    cameraController.yaw = mapArea.mapSettings.rotation;
    cameraController.target = Qt.vector3d(0, 0, 0);
    cameraController.updateCameraPosition();
    openingAnimation.start();
  }

  function playClosingAnimation(callback) {
    cameraController.defaultYaw = mapArea.mapSettings.rotation; // Needed so the reset ends on the 2D map canvas rotation
    cameraController.resetView();
    closingAnimation.callback = callback;
    closingAnimation.start();
  }

  function positionCameraForTerrain() {
    const terrainDiagonal = Math.sqrt(Math.pow(mapTerrainProvider.size.width, 2) + Math.pow(mapTerrainProvider.size.height, 2));
    cameraController.distance = terrainDiagonal * 0.8;
    cameraController.pitch = 40;
    cameraController.yaw = mapArea.mapSettings.rotation;
    cameraController.target = Qt.vector3d(0, 0, 0);
  }

  function geoTo3D(geoX, geoY) {
    return mapTerrainProvider.geoTo3D(geoX, geoY, 15);
  }

  function lookAtPoint(pos3d, distance) {
    cameraController.lookAtPoint(pos3d, distance);
  }

  function zoomIn() {
    cameraController.distance = cameraController.clampDistance(cameraController.distance * 0.8);
  }

  function zoomOut() {
    cameraController.distance = cameraController.clampDistance(cameraController.distance * 1.25);
  }
}
```


