import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core

/**
 * \ingroup qml_gui
 */
Item {
  id: trackingSession

  property var tracker: model.tracker

  property QfPositioning positioning: undefined

  property bool filterAccuracy: false
  property Project project
  property MapSettings mapSettings
  property var cloudUserInformation

  signal featureCreated(var layer)

  Component.onCompleted: {
    tracker.rubberbandModel = rubberbandModel;
    tracker.featureModel = featureModel;
    tracker.filterAccuracy = filterAccuracy;
  }

  onFilterAccuracyChanged: {
    tracker.filterAccuracy = filterAccuracy;
  }

  Connections {
    target: trackingSession.positioning
    enabled: tracker.isActive

    function onPositionInformationChanged() {
      featureModel.appExpressionContextScopesGenerator.positionInformation = trackingSession.positioning.positionInformation;
      tracker.processPositionInformation(trackingSession.positioning.positionInformation, trackingSession.positioning.projectedPosition);
    }
  }

  Connections {
    target: tracker

    function onFeatureCreated() {
      if (tracker.isActive) {
        trackingSession.featureCreated(featureModel.currentLayer);
      }
    }
  }

  QfRubberbandModel {
    id: rubberbandModel
    frozen: false
    vectorLayer: tracker.vectorLayer
    crs: trackingSession.mapSettings.destinationCrs
  }

  QfRubberband {
    id: rubberband
    visible: tracker.visible
    freeze: tracker.isReplaying

    color: tracker.color
    geometryType: Qgis.GeometryType.Line

    mapSettings: trackingSession.mapSettings
    model: rubberbandModel
  }

  QfFeatureModel {
    id: featureModel
    project: trackingSession.project
    currentLayer: tracker.vectorLayer
    feature: tracker.feature

    onFeatureChanged: {
      if (!tracker.isActive && !tracker.isReplaying) {
        updateRubberband();
      }
    }

    geometry: QfGeometry {
      id: featureModelGeometry
      rubberbandModel: rubberbandModel
      vectorLayer: tracker.vectorLayer
    }

    appExpressionContextScopesGenerator: QfAppExpressionContextScopesGenerator {
      positionLocked: true
      cloudUserInformation: trackingSession.cloudUserInformation
    }
  }
}
