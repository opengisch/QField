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

  property var trackerModelItem: model
  property var tracker: model.tracker

  Component.onCompleted: {
    tracker.rubberbandModel = rubberbandModel;
    tracker.featureModel = featureModel;
  }

  Connections {
    target: positionSource

    function onPositionInformationChanged() {
      if (tracker.isActive) {
        tracker.processPositionInformation(positionSource.positionInformation, positionSource.projectedPosition);
      }
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
    vectorLayer: trackerModelItem.vectorLayer
    crs: mapCanvas.mapSettings.destinationCrs
  }

  Rubberband {
    id: rubberband
    visible: trackerModelItem.visible

    color: Qt.rgba(Math.min(0.75, Math.random()), Math.min(0.75, Math.random()), Math.min(0.75, Math.random()), 0.6)
    geometryType: Qgis.GeometryType.Line

    mapSettings: mapCanvas.mapSettings
    model: rubberbandModel
  }

  FeatureModel {
    id: featureModel
    project: qgisProject
    currentLayer: trackerModelItem.vectorLayer
    feature: trackerModelItem.feature

    onFeatureChanged: {
      if (!tracker.isActive) {
        updateRubberband();
      }
    }

    geometry: Geometry {
      id: featureModelGeometry
      rubberbandModel: rubberbandModel
      vectorLayer: trackerModelItem.vectorLayer
    }

    positionInformation: coordinateLocator.positionInformation
    positionLocked: true
    cloudUserInformation: projectInfo.cloudUserInformation
  }
}
