import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
import org.qfield
import Theme

Item {
  id: mapArea
  focus: true
  visible: !mapTerrainProvider.isFirstLoad || !isLoading

  property alias mapSettings: mapTerrainProvider.mapSettings
  property alias terrainExtent: mapTerrainProvider.extent
  property bool isLoading: mapTerrainProvider.isLoading
  property bool isFirstLoad: mapTerrainProvider.isFirstLoad
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

    property bool isFirstLoad: true

    onTerrainDataReady: {
      if (normalizedData.length === 0) {
        return;
      }

      // Build 3x3 metagrid of heights (center block = real DEM, sides = flat)
      const gridW = gridSize.width;
      const gridH = gridSize.height;
      mapArea.metagridWidth = gridW * 3;
      mapArea.metagridHeight = gridH * 3;
      
      const metagrid = [];
      const totalSize = mapArea.metagridWidth * mapArea.metagridHeight;
      
      // Initialize all to 0 (flat gray areas around the edges)
      for (let i = 0; i < totalSize; i++) {
        metagrid[i] = 0;
      }
      
      // Copy only center block with real DEM heights
      for (let z = 0; z < gridH; z++) {
        for (let x = 0; x < gridW; x++) {
          const srcIdx = z * gridW + x;
          const dstX = gridW + x;  // Center block starts at gridW
          const dstZ = gridH + z;  // Center block starts at gridH
          const dstIdx = dstZ * mapArea.metagridWidth + dstX;
          metagrid[dstIdx] = normalizedData[srcIdx];
        }
      }
      
      mapArea.metagridHeights = metagrid;

      // Don't reset pan offsets here — wait until texture is ready
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

    onReadyChanged: {
      if (ready && mapArea.isTransitioning) {
        mapArea.isTransitioning = false;
      }
    }

    onTextureUpdated: {
      if (mapArea.isTransitioning) {
        // Reset pan offsets now that new texture is ready
        mapArea.panOffsetX = 0;
        mapArea.panOffsetZ = 0;
        mapArea.isPanning = false;
        mapArea.isTransitioning = false;
      }
    }
  }

  Texture {
    id: mapTexture
    textureData: mapTextureData
    generateMipmaps: false
    mipFilter: Texture.None
    tilingModeHorizontal: Texture.ClampToEdge
    tilingModeVertical: Texture.ClampToEdge
    pivotU: 0.5
    pivotV: 0.5
    // Texture is 3x3 metagrid; scale 1/3 maps UVs to center block only
    scaleU: (1.0 / 3.0)
    scaleV: (1.0 / 3.0)
    // Shift texture position during pan to show adjacent blocks
    positionU: mapTerrainProvider.size.width > 0 ? -(mapArea.panOffsetX / mapTerrainProvider.size.width) * (1.0 / 3.0) : 0
    positionV: mapTerrainProvider.size.height > 0 ? -(mapArea.panOffsetZ / mapTerrainProvider.size.height) * (1.0 / 3.0) : 0
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

  // Pan/zoom preview state
  property real panOffsetX: 0
  property real panOffsetZ: 0
  property bool isPanning: false
  property bool isTransitioning: false

  // Explicitly update heights when panning state changes
  onIsPanningChanged: {
    if (isPanning) {
      updateTerrainHeights(terrainMesh.mapTerrainGeometry.getShiftedHeights(metagridHeights, metagridWidth, metagridHeight, panOffsetX, panOffsetZ));
    } else {
      updateTerrainHeights(mapTerrainProvider.normalizedData);
    }
  }

  // Metagrid: 3x3 array of heights for smooth panning
  property var metagridHeights: []
  property int metagridWidth: 0
  property int metagridHeight: 0

  // Helper to update terrain heights
  function updateTerrainHeights(newHeights) {
    terrainMesh.mapTerrainGeometry.heightData = newHeights;
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
    onExtentPanMoved: function (sceneX, sceneZ) {
      if (!isPanning) isPanning = true;
      panOffsetX += sceneX;
      panOffsetZ += sceneZ;
      // Update heights immediately as we pan
      updateTerrainHeights(terrainMesh.mapTerrainGeometry.getShiftedHeights(metagridHeights, metagridWidth, metagridHeight, panOffsetX, panOffsetZ));
    }
    onExtentPanFinished: {
      const savedX = panOffsetX;
      const savedZ = panOffsetZ;
      // Texture stays shifted until reload completes; terrain stays flat during drag
      isTransitioning = true;
      applyExtentShift(savedX, savedZ);
    }
    onExtentZoomRequested: function (factor) {
      isTransitioning = true;
      applyExtentZoom(factor);
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

  // Converts scene displacement to a geo offset.
  // Scene +X -> geo +X
  // Scene +Z -> geo -Y
  function applyExtentShift(sceneX, sceneZ) {
    const ext = mapTerrainProvider.extent;
    const sz = mapTerrainProvider.size;
    const geoPerSceneX = ext.width / sz.width;
    const geoPerSceneY = ext.height / sz.height;
    const geoOffsetX = -sceneX * geoPerSceneX;
    const geoOffsetY = sceneZ * geoPerSceneY;
    mapTerrainProvider.setCustomExtent(ext.xMinimum + geoOffsetX, ext.yMinimum + geoOffsetY, ext.xMaximum + geoOffsetX, ext.yMaximum + geoOffsetY);
  }

  // Scales the extent around its center by the given factor, then regenerates.
  // factor < 1.0 means zoom IN (smaller extent), factor > 1.0 means zoom OUT (larger extent)
  function applyExtentZoom(factor) {
    const ext = mapTerrainProvider.extent;
    const cx = (ext.xMinimum + ext.xMaximum) / 2;
    const cy = (ext.yMinimum + ext.yMaximum) / 2;
    let halfW = ext.width / 2 * factor;
    let halfH = ext.height / 2 * factor;

    // Clamp extent size
    const minHalfExtent = 50;
    const maxHalfExtent = 50000;
    halfW = Math.max(minHalfExtent, Math.min(maxHalfExtent, halfW));
    halfH = Math.max(minHalfExtent, Math.min(maxHalfExtent, halfH));

    mapTerrainProvider.setCustomExtent(cx - halfW, cy - halfH, cx + halfW, cy + halfH);
  }
}
