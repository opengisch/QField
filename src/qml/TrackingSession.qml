import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Item {
  id: trackingSession

  property var tracker: model
  property var currentPositionInformation: manualPositionInformation !== undefined ? manualPositionInformation : positionSource.positionInformation
  property var currentProjectedPosition: manualProjectedPosition !== undefined ? manualProjectedPosition : positionSource.projectedPosition
  property var manualPositionInformation: undefined
  property var manualProjectedPosition: undefined

  Component.onCompleted: {
    tracker.rubberModel = rubberbandModel;
  }

  Connections {
    target: positionSource

    function onBackgroundModeChanged() {
      if (!positionSource.backgroundMode) {
        // Replay position information collected while in background mode
        const positionInformationList = positionSource.getBackgroundPositionInformation();
        for (const positionInformation of positionInformationList) {
          manualPositionInformation = positionInformation;
          manualProjectedPosition = positionSource.coordinateTransformer.transformPosition(GeometryUtils.point(positionInformation.longitude, positionInformation.latitude, positionInformation.elevation));
        }
      }
    }

    function onPositionInformationChanged() {
      // We need to skip one position information change when returning from background mode
      // to avoid queued signal interference
      if (manualPositionInformation != undefined) {
        manualPositionInformation = undefined;
        manualProjectedPosition = undefined;
      }
    }
  }

  RubberbandModel {
    id: rubberbandModel
    frozen: false
    vectorLayer: tracker.vectorLayer

    property int measureType: tracker.measureType
    measureValue: {
      switch (measureType) {
      case Tracker.SecondsSinceStart:
        return (trackingSession.currentPositionInformation.utcDateTime - tracker.startPositionTimestamp) / 1000;
      case Tracker.Timestamp:
        return trackingSession.currentPositionInformation.utcDateTime.getTime();
      case Tracker.GroundSpeed:
        return trackingSession.currentPositionInformation.speed;
      case Tracker.Bearing:
        return trackingSession.currentPositionInformation.direction;
      case Tracker.HorizontalAccuracy:
        return trackingSession.currentPositionInformation.hacc;
      case Tracker.VerticalAccuracy:
        return trackingSession.currentPositionInformation.vacc;
      case Tracker.PDOP:
        return trackingSession.currentPositionInformation.pdop;
      case Tracker.HDOP:
        return trackingSession.currentPositionInformation.hdop;
      case Tracker.VDOP:
        return trackingSession.currentPositionInformation.vdop;
      }
      return 0;
    }

    currentCoordinate: trackingSession.currentProjectedPosition
    currentPositionTimestamp: trackingSession.currentPositionInformation.utcDateTime
    crs: mapCanvas.mapSettings.destinationCrs

    onVertexCountChanged: {
      if (!tracker.isActive || vertexCount === 0) {
        return;
      }
      if (geometryType === Qgis.GeometryType.Point) {
        featureModel.applyGeometry();
        featureModel.resetFeatureId();
        featureModel.resetAttributes(true);
        featureModel.create();
      } else {
        if ((geometryType === Qgis.GeometryType.Line && vertexCount > 2) || (geometryType === Qgis.GeometryType.Polygon && vertexCount > 3)) {
          featureModel.applyGeometry();
          if ((geometryType === Qgis.GeometryType.Line && vertexCount == 3) || (geometryType === Qgis.GeometryType.Polygon && vertexCount == 4)) {
            // indirect action, no need to check for success and display a toast, the log is enough
            featureModel.create();
            tracker.feature = featureModel.feature;
            projectInfo.saveTracker(featureModel.currentLayer);
          } else {
            // indirect action, no need to check for success and display a toast, the log is enough
            featureModel.save();
          }
        }
      }
    }
  }

  Rubberband {
    id: rubberband
    visible: tracker.visible

    color: Qt.rgba(Math.min(0.75, Math.random()), Math.min(0.75, Math.random()), Math.min(0.75, Math.random()), 0.6)
    geometryType: Qgis.GeometryType.Line

    mapSettings: mapCanvas.mapSettings
    model: rubberbandModel
  }

  FeatureModel {
    id: featureModel
    project: qgisProject
    currentLayer: tracker.vectorLayer
    feature: tracker.feature

    onFeatureChanged: {
      if (!tracker.isActive) {
        updateRubberband();
      }
    }

    geometry: Geometry {
      id: featureModelGeometry
      rubberbandModel: rubberbandModel
      vectorLayer: tracker.vectorLayer
    }

    positionInformation: coordinateLocator.positionInformation
    positionLocked: true
    cloudUserInformation: projectInfo.cloudUserInformation
  }
}
