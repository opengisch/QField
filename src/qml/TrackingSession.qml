import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

Item {
  id: trackingSession

  property var tracker: model

  Component.onCompleted: {
    tracker.rubberModel = rubberbandModel;
  }

  RubberbandModel {
    id: rubberbandModel
    frozen: false
    vectorLayer: tracker.vectorLayer
    currentCoordinate: positionSource.projectedPosition

    property int measureType: tracker.measureType
    measureValue: {
      switch (measureType) {
      case Tracker.SecondsSinceStart:
        return (positionSource.positionInformation.utcDateTime - tracker.startPositionTimestamp) / 1000;
      case Tracker.Timestamp:
        return positionSource.positionInformation.utcDateTime.getTime();
      case Tracker.GroundSpeed:
        return positionSource.positionInformation.speed;
      case Tracker.Bearing:
        return positionSource.positionInformation.direction;
      case Tracker.HorizontalAccuracy:
        return positionSource.positionInformation.hacc;
      case Tracker.VerticalAccuracy:
        return positionSource.positionInformation.vacc;
      case Tracker.PDOP:
        return positionSource.positionInformation.pdop;
      case Tracker.HDOP:
        return positionSource.positionInformation.hdop;
      case Tracker.VDOP:
        return positionSource.positionInformation.vdop;
      }
      return 0;
    }

    currentPositionTimestamp: positionSource.positionInformation.utcDateTime
    crs: mapCanvas.mapSettings.destinationCrs

    onVertexCountChanged: {
      if (!tracker.isActive || vertexCount == 0) {
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
