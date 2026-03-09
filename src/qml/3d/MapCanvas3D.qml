import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
import org.qfield
import Theme

Item {
  id: mapArea
  focus: true
  visible: !isFirstLoad || !isLoading

  property alias mapSettings: mapTerrainProvider.mapSettings
  property alias terrainExtent: mapTerrainProvider.extent
  property alias terrainGeometry: terrainMesh.mapTerrainGeometry

  property bool isLoading: mapTerrainProvider.isLoading
  property bool isFirstLoad: true
  property bool wireframeMode: false
  property alias extentMode: cameraController.extentMode

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

      terrainGeometry.buildMetagridFromProvider(mapTerrainProvider);
      mapTextureData.render();

      if (isFirstLoad) {
        isFirstLoad = false;
        positionCameraForTerrain();
        Qt.callLater(mapArea.playOpeningAnimation);
      }
    }
  }

  Quick3DMapTextureData {
    id: mapTextureData
    mapSettings: mapArea.mapSettings
    extent: mapTerrainProvider.extent
    incrementalRendering: true
    forceDeferredLayersRepaint: mapArea.trackingModel ? mapArea.trackingModel.count > 0 : false

    onTextureUpdated: {
      if (mapTerrainProvider.isTransitioning) {
        // Apply offsets and scale now that new texture is ready
        terrainGeometry.restoreHeightsFromProvider(mapTerrainProvider);
        mapTerrainProvider.endTransition();
      }
    }
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
      mapTerrainGeometry.offsetVector: mapTerrainProvider.offsetVector

      // Texture is 3x3 metagrid; scale 1/3 maps UVs to center block only
      mapTexture.scaleU: (1.0 * mapTerrainProvider.offsetScale / 3.0)
      mapTexture.scaleV: (1.0 * mapTerrainProvider.offsetScale / 3.0)
      mapTexture.positionU: mapTerrainProvider.size.width > 0 ? -(mapTerrainProvider.offsetVector.x / mapTerrainProvider.size.width) * (1.0 / 3.0) : 0
      mapTexture.positionV: mapTerrainProvider.size.height > 0 ? -(mapTerrainProvider.offsetVector.z / mapTerrainProvider.size.height) * (1.0 / 3.0) : 0

      mapTextureData: mapTextureData
    }

    MapToScreen3D {
      id: gnssMapToScreen3D
      terrainProvider: mapTerrainProvider
      heightOffset: 15
    }

    Connections {
      target: mapArea
      function onGnssPositionChanged() {
        if (mapArea.gnssPosition) {
          gnssMapToScreen3D.mapPoint = mapArea.gnssPosition;
        }
      }
    }

    Node {
      id: gnssMarker
      visible: mapArea.gnssActive && mapArea.gnssPosition && !isNaN(gnssMapToScreen3D.scenePoint.x)

      position: gnssMapToScreen3D.scenePoint
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
      if (pickResult.objectHit && gnssMarker.visible) {
        cameraController.lookAtPoint(gnssMapToScreen3D.scenePoint, 500);
      }
    }
    onUserInteractionStarted: {
      mapArea.cameraInteractionDetected();
    }

    onExtentPan: function (sceneX, sceneZ) {
      if (mapTerrainProvider.terrainDataReady) {
        mapTerrainProvider.pan(sceneX, sceneZ);
      }
    }
    onExtentPanFinished: {
      mapTerrainProvider.beginTransition();
    }

    onExtentZoom: function (factor) {
      if (mapTerrainProvider.terrainDataReady) {
        mapTerrainProvider.zoom(factor);
      }
    }
    onExtentZoomFinished: {
      mapTerrainProvider.beginTransition();
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
