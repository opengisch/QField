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

  property var tracker: model.tracker

  Component.onCompleted: {
    tracker.rubberbandModel = rubberbandModel;
    tracker.featureModel = featureModel;
  }

  Connections {
    target: positionSource
    enabled: tracker.isActive

    function onPositionInformationChanged() {
      featureModel.positionInformation = positionSource.positionInformation;
      tracker.processPositionInformation(positionSource.positionInformation, positionSource.projectedPosition);
    }
  }

  Connections {
    target: tracker

    function onFeatureCreated() {
      if (tracker.isActive) {
        projectInfo.saveTracker(featureModel.currentLayer);
      }
    }
  }

  RubberbandModel {
    id: rubberbandModel
    frozen: false
    vectorLayer: tracker.vectorLayer
    crs: mapCanvas.mapSettings.destinationCrs
  }

  Rubberband {
    id: rubberband
    visible: tracker.visible
    freeze: tracker.isReplaying

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
      if (!tracker.isActive && !tracker.isReplaying) {
        updateRubberband();
      }
    }

    geometry: Geometry {
      id: featureModelGeometry
      rubberbandModel: rubberbandModel
      vectorLayer: tracker.vectorLayer
    }

    positionLocked: true
    cloudUserInformation: projectInfo.cloudUserInformation
  }
}
