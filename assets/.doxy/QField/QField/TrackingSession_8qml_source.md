

# File TrackingSession.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**TrackingSession.qml**](TrackingSession_8qml.md)

[Go to the documentation of this file](TrackingSession_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield

Item {
  id: trackingSession

  property var tracker: model.tracker

  Component.onCompleted: {
    tracker.rubberbandModel = rubberbandModel;
    tracker.featureModel = featureModel;
    tracker.filterAccuracy = positioningSettings.accuracyIndicator && positioningSettings.accuracyRequirement;
  }

  Connections {
    target: positioningSettings

    function onAccuracyIndicatorChanged() {
      tracker.filterAccuracy = positioningSettings.accuracyIndicator && positioningSettings.accuracyRequirement;
    }

    function onAccuracyRequirementChanged() {
      tracker.filterAccuracy = positioningSettings.accuracyIndicator && positioningSettings.accuracyRequirement;
    }
  }

  Connections {
    target: positionSource
    enabled: tracker.isActive

    function onPositionInformationChanged() {
      featureModel.appExpressionContextScopesGenerator.positionInformation = positionSource.positionInformation;
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

    color: tracker.color
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

    appExpressionContextScopesGenerator: AppExpressionContextScopesGenerator {
      positionLocked: true
      cloudUserInformation: appScopesGenerator.cloudUserInformation
    }
  }
}
```


